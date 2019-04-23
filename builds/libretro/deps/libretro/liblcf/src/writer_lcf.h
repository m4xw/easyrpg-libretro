/*
 * This file is part of liblcf. Copyright (c) 2018 liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#ifndef LCF_WRITER_LCF_H
#define LCF_WRITER_LCF_H

#include <string>
#include <vector>
#include <iosfwd>
#include <cstring>
#include <memory>
#include <cassert>
#include <stdint.h>
#include "lcf_options.h"
#include "reader_util.h"

/**
 * LcfWriter class.
 */
class LcfWriter {

public:
	/**
	 * Constructs a new File Writer.
	 *
	 * @param filestream already opened filestream.
	 * @param encoding name of the encoding.
	 */
	LcfWriter(std::ostream& filestream, std::string encoding = "");

	/**
	 * Destructor. Closes the opened file.
	 */
	~LcfWriter();

	/**
	 * Writes raw data to the stream (fwrite() wrapper).
	 *
	 * @param ptr pointer to buffer.
	 * @param size size of each element.
	 * @param nmemb number of elements.
	 */
	void Write(const void *ptr, size_t size, size_t nmemb);

	/**
	 * Write a primitive value to the stream.
	 *
	 * @param val the value.
	 */
	template <class T>
	void Write(T val);

	/**
	 * Writes a string to the stream.
	 *
	 * @param str the string.
	 *        Note: the string is converted to the native encoding.
	 */
	void Write(const std::string& str);

	/**
	 * Writes a compressed integer to the stream.
	 *
	 * @param val the integer.
	 */
	void WriteInt(int val);

	/**
	 * Write a vector of primitive values to the stream.
	 *
	 * @param buffer vector to write.
	 */
	template <class T>
	void Write(const std::vector<T>& buffer);

	/**
	 * Returns the current position of the read pointer in
	 * the stream.
	 *
	 * @return current location in the stream.
	 */
	uint32_t Tell();

	/**
	 * Checks if the file is writable and if no error occurred.
	 *
	 * @return true the stream is okay.
	 */
	bool IsOk() const;

	/**
	 * Decodes a string from Utf8 to the set encoding
	 * in the Writer constructor.
	 *
	 * @param str_to_encode UTF-8 string to encode.
	 * @return native version of string.
	 */
	std::string Decode(const std::string& str_to_encode);

private:
	/** Name of the encoding. */
	std::string encoding;
	/** File-stream managed by this Writer. */
	std::ostream& stream;

	/**
	 * Converts a 16bit signed integer to/from little-endian.
	 *
	 * @param us integer to convert.
	 */
	static void SwapByteOrder(int16_t &us);

	/**
	 * Converts a 16bit unsigned integer to/from little-endian.
	 *
	 * @param us integer to convert.
	 */
	static void SwapByteOrder(uint16_t &us);

	/**
	 * Converts a 32bit signed integer to/from little-endian.
	 *
	 * @param us integer to convert.
	 */
	static void SwapByteOrder(int32_t &us);

	/**
	 * Converts a 32bit unsigned integer to/from little-endian.
	 *
	 * @param ui integer to convert.
	 */
	static void SwapByteOrder(uint32_t &ui);

	/**
	 * Converts a double to/from little-endian.
	 *
	 * @param d double to convert.
	 */
	static void SwapByteOrder(double &d);

};

#endif
