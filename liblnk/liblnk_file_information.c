/*
 * File information functions
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "liblnk_debug.h"
#include "liblnk_file_information.h"
#include "liblnk_io_handle.h"
#include "liblnk_libcerror.h"
#include "liblnk_libcnotify.h"
#include "liblnk_libfdatetime.h"
#include "liblnk_libfguid.h"

#include "lnk_file_header.h"

/* Creates file information
 * Make sure the value file_information is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int liblnk_file_information_initialize(
     liblnk_file_information_t **file_information,
     libcerror_error_t **error )
{
	static char *function = "liblnk_file_information_initialize";

	if( file_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		return( -1 );
	}
	if( *file_information != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file information value already set.",
		 function );

		return( -1 );
	}
	*file_information = memory_allocate_structure(
	                     liblnk_file_information_t );

	if( *file_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file information.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_information,
	     0,
	     sizeof( liblnk_file_information_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file information.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_information != NULL )
	{
		memory_free(
		 *file_information );

		*file_information = NULL;
	}
	return( -1 );
}

/* Frees file information
 * Returns 1 if successful or -1 on error
 */
int liblnk_file_information_free(
     liblnk_file_information_t **file_information,
     libcerror_error_t **error )
{
	static char *function = "liblnk_file_information_free";

	if( file_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		return( -1 );
	}
	if( *file_information != NULL )
	{
		memory_free(
		 *file_information );

		*file_information = NULL;
	}
	return( 1 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int liblnk_file_information_read(
     liblnk_file_information_t *file_information,
     libbfio_handle_t *file_io_handle,
     uint8_t *class_identifier,
     size_t class_identifier_size,
     libcerror_error_t **error )
{
	lnk_file_header_t file_header;

	static char *function = "liblnk_file_information_read";
	ssize_t read_count    = 0;

	if( file_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &file_header,
	              sizeof( lnk_file_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( lnk_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		return( -1 );
	}
	if( liblnk_file_information_read_data(
	     file_information,
	     (uint8_t *) &file_header,
	     sizeof( lnk_file_header_t ),
	     class_identifier,
	     class_identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the file header data
 * Returns 1 if successful or -1 on error
 */
int liblnk_file_information_read_data(
     liblnk_file_information_t *file_information,
     const uint8_t *data,
     size_t data_size,
     uint8_t *class_identifier,
     size_t class_identifier_size,
     libcerror_error_t **error )
{
	static char *function = "liblnk_file_information_read_data";
	uint32_t header_size  = 0;

	if( file_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( lnk_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( class_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid class identifier.",
		 function );

		return( -1 );
	}
	if( class_identifier_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: class identifier too small.",
		 function );

		return( -1 );
	}
	if( class_identifier_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: class identifier size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( lnk_file_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (lnk_file_header_t *) data )->header_size,
	 header_size );

	if( header_size != 76 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header size: %" PRIu32 ".",
		 function,
		 header_size );

		return( -1 );
	}
	if( memory_compare(
	     ( (lnk_file_header_t *) data )->class_identifier,
	     lnk_file_class_identifier,
	     16 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file class identifier.",
		 function );

		return( -1 );
	}
/* TODO is a libfguid version of the class identifier needed ? */
	if( memory_copy(
	     class_identifier,
	     ( (lnk_file_header_t *) data )->class_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to copy class identifier.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (lnk_file_header_t *) data )->data_flags,
	 file_information->data_flags );

	byte_stream_copy_to_uint32_little_endian(
	 ( (lnk_file_header_t *) data )->file_attribute_flags,
	 file_information->attribute_flags );

	byte_stream_copy_to_uint64_little_endian(
	 ( (lnk_file_header_t *) data )->creation_time,
	 file_information->creation_time )

	byte_stream_copy_to_uint64_little_endian(
	 ( (lnk_file_header_t *) data )->access_time,
	 file_information->access_time );

	byte_stream_copy_to_uint64_little_endian(
	 ( (lnk_file_header_t *) data )->modification_time,
	 file_information->modification_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (lnk_file_header_t *) data )->file_size,
	 file_information->size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (lnk_file_header_t *) data )->icon_index,
	 file_information->icon_index );

	byte_stream_copy_to_uint32_little_endian(
	 ( (lnk_file_header_t *) data )->show_window_value,
	 file_information->show_window_value );

	byte_stream_copy_to_uint16_little_endian(
	 ( (lnk_file_header_t *) data )->hot_key_value,
	 file_information->hot_key_value );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: header size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 header_size );

		if( liblnk_debug_print_guid_value(
		     function,
		     "class identifier\t\t\t",
		     ( (lnk_file_header_t *) data )->class_identifier,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print GUID value.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "%s: data flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_information->data_flags );
		liblnk_debug_print_data_flags(
		 file_information->data_flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: file attribute flags\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_information->attribute_flags );
		liblnk_debug_print_file_attribute_flags(
		 file_information->attribute_flags );
		libcnotify_printf(
		 "\n" );

		if( liblnk_debug_print_filetime_value(
		     function,
		     "creation time\t\t\t",
		     ( (lnk_file_header_t *) data )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print filetime value.",
			 function );

			return( -1 );
		}
		if( liblnk_debug_print_filetime_value(
		     function,
		     "access time\t\t\t\t",
		     ( (lnk_file_header_t *) data )->access_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print filetime value.",
			 function );

			return( -1 );
		}
		if( liblnk_debug_print_filetime_value(
		     function,
		     "modification time\t\t\t",
		     ( (lnk_file_header_t *) data )->modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print filetime value.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "%s: file size\t\t\t\t: %" PRIu32 " bytes\n",
		 function,
		 file_information->size );

		libcnotify_printf(
		 "%s: icon index\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_information->icon_index );

		libcnotify_printf(
		 "%s: show window value\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_information->show_window_value );

		libcnotify_printf(
		 "%s: hot key value\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 file_information->hot_key_value );

		libcnotify_printf(
		 "%s: reserved:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) ( (lnk_file_header_t *) data )->reserved,
		 10,
		 0 );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

