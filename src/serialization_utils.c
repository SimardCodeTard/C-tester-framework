/* ************************************************************************* */
/*                                                                           */
/*      /$$$$$$  /$$$$$$ /$$      /$$  /$$$$$$  /$$$$$$$  /$$$$$$$           */
/*     /$$__  $$|_  $$_/| $$$    /$$$ /$$__  $$| $$__  $$| $$__  $$          */
/*    | $$  \__/  | $$  | $$$$  /$$$$| $$  \ $$| $$  \ $$| $$  \ $$          */
/*    |  $$$$$$   | $$  | $$ $$/$$ $$| $$$$$$$$| $$$$$$$/| $$  | $$          */
/*     \____  $$  | $$  | $$  $$$| $$| $$__  $$| $$__  $$| $$  | $$          */
/*     /$$  \ $$  | $$  | $$\  $ | $$| $$  | $$| $$  \ $$| $$  | $$          */
/*    |  $$$$$$/ /$$$$$$| $$ \/  | $$| $$  | $$| $$  | $$| $$$$$$$/          */
/*     \______/ |______/|__/     |__/|__/  |__/|__/  |__/|_______/           */
/*                                                                           */
/*    File: serialization_utils.c                                            */
/*     Utils for the serialization and deserialization of test results       */
/*                                                                           */
/* Free Palestine, fuck fascists                                             */
/*                               Made with love and coffee by SimardCodeTard */
/* ************************************************************************* */

#include "../headers/lib.h"

static serialization_result_t	write_exact(uint16_t fd, uint8_t *data,
								uint32_t size)
{
	ssize_t		write_result;
	uint32_t	total_bytes_written;

	if (!data)
		return (SERIALIZE_FAIL);
	total_bytes_written = 0;
	write_result = 1;
	while (write_result > 0 && total_bytes_written < size)
	{
		write_result = write(fd, data + total_bytes_written,
				size - total_bytes_written);
		if (write_result > 0)
			total_bytes_written += write_result;
	}
	if (total_bytes_written != size)
	{
		fprintf(stderr, KYEL SERIALIZATION_FAILED KNRM);
		return (SERIALIZE_FAIL);
	}
	return (SERIALIZE_SUCCESS);
}

static serialization_result_t	serialize_uint16(uint16_t fd, uint16_t data)
{
	uint8_t	bytes[2];

	bytes[1] = data % 256;
	data /= 256;
	bytes[0] = data;
	return (write_exact(fd, bytes, UINT16_SIZE));
}

static serialization_result_t	serialize_string(uint16_t fd, string_t str,
								uint16_t len)
{
	if (!str)
		str = "";
	return (write_exact(fd, (uint8_t *) str, len));
}

static serialization_result_t	serialize_string_field(uint16_t fd,
								string_t field)
{
	uint16_t	field_length;

	field_length = 0;
	if (field)
		field_length = strlen(field);
	if (serialize_uint16(fd, field_length) == SERIALIZE_FAIL)
		return (SERIALIZE_FAIL);
	if ((serialize_string(fd, field, field_length)) == SERIALIZE_FAIL)
		return (SERIALIZE_FAIL);
	return (SERIALIZE_SUCCESS);
}

static serialization_result_t	serialize_byte(uint16_t fd, uint8_t byte)
{
	return (write_exact(fd, &byte, UINT8_SIZE));
}

serialization_result_t	serialize_result(uint16_t fd, test_result_t result)
{
	if (serialize_string_field(fd, result.description) == SERIALIZE_FAIL)
	{
		printf(KYEL "Failed to write description. \n" KNRM);
		return (SERIALIZE_FAIL);
	}
	if (serialize_string_field(fd, result.expected) == SERIALIZE_FAIL)
	{
		printf(KYEL "Failed to write expected. \n" KNRM);
		return (SERIALIZE_FAIL);
	}
	if (serialize_string_field(fd, result.got) == SERIALIZE_FAIL)
	{
		printf(KYEL "Failed to write got. \n" KNRM);
		return (SERIALIZE_FAIL);
	}
	if (serialize_byte(fd, result.success) == SERIALIZE_FAIL)
	{
		printf(KYEL "Failed to write success. \n" KNRM);
		return (SERIALIZE_FAIL);
	}
	return (SERIALIZE_SUCCESS);
}

static serialization_result_t	read_exact(uint16_t fd, uint8_t *buffer,
								uint32_t size)
{
	ssize_t		read_result;
	uint32_t	total_bytes_read;

	if (!buffer)
		return (SERIALIZE_FAIL);
	read_result = 1;
	total_bytes_read = 0;
	while (read_result > 0 && total_bytes_read < size)
	{
		read_result = read(fd, buffer + total_bytes_read,
				size - total_bytes_read);
		if (read_result == -1)
			fprintf(stderr, KRED "ERROR: Read failed.\n" KNRM);
		if (read_result > 0)
			total_bytes_read += read_result;
	}
	if (total_bytes_read < size)
	{
		fprintf(stderr, KRED "Read %d bytes out of %d expected\n" KNRM,
			total_bytes_read, size);
		fprintf(stderr, KYEL DESERIALIZATION_FAILED KNRM);
		return (SERIALIZE_FAIL);
	}
	return (SERIALIZE_SUCCESS);
}

static serialization_result_t	deserialize_uint16(uint16_t fd, uint16_t *buffer)
{
	uint8_t		*bytes;

	bytes = calloc(2, sizeof(uint8_t));
	if (!bytes)
		return (SERIALIZE_FAIL);
	if (read_exact(fd, bytes, UINT16_SIZE) == SERIALIZE_FAIL)
		return (SERIALIZE_FAIL);
	*buffer = (bytes[0] << 8) | bytes[1];
	free(bytes);
	return (SERIALIZE_SUCCESS);
}

static serialization_result_t	deserialize_string(uint16_t fd, string_t *str,
								uint16_t len)
{
	return (read_exact(fd, (uint8_t *) *str, len));
}

static serialization_result_t	deserialize_string_field(uint16_t fd,
								string_t *field)
{
	uint16_t	len;

	if (deserialize_uint16(fd, &len) == SERIALIZE_FAIL)
		return (SERIALIZE_FAIL);
	*field = calloc(len + 1, sizeof(char));
	if (!*field)
	{
		*field = "(memory allocation failed during deserialization)";
		fprintf(stderr, KYEL DESERIALIZATION_MALLOC_FAILED KNRM);
		return (SERIALIZE_FAIL);
	}
	if (deserialize_string(fd, field, len) == SERIALIZE_FAIL)
	{
		free(*field);
		*field = "(test result deserialization failed)";
		return (SERIALIZE_FAIL);
	}
	return (SERIALIZE_SUCCESS);
}

static serialization_result_t	deserialize_byte(uint16_t fd, uint8_t *buffer)
{
	return (read_exact(fd, buffer, UINT8_SIZE));
}

static test_result_t	secure_test_result(test_result_t result)
{
	if (!result.description)
		result.description = "(null)";
	if (!result.expected)
		result.expected = "(null)";
	if (!result.got)
		result.got = "(null)";
	return (result);
}

test_result_t	deserialize_result(uint16_t fd)
{
	test_result_t	result;

	result.description = NULL;
	result.expected = NULL;
	result.got = NULL;
	result.success = false;
	if (deserialize_string_field(fd, &result.description) == SERIALIZE_FAIL)
		return (secure_test_result(result));
	if (deserialize_string_field(fd, &result.expected) == SERIALIZE_FAIL)
		return (secure_test_result(result));
	if (deserialize_string_field(fd, &result.got) == SERIALIZE_FAIL)
		return (secure_test_result(result));
	if (deserialize_byte(fd, &result.success) == SERIALIZE_FAIL)
		return (secure_test_result(result));
	return (secure_test_result(result));
}
