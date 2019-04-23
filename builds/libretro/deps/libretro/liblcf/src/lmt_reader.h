/*
 * This file is part of liblcf. Copyright (c) 2018 liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#ifndef LCF_LMT_READER_H
#define LCF_LMT_READER_H

#include <string>
#include "reader_lcf.h"
#include "writer_lcf.h"
#include "writer_xml.h"
#include "rpg_treemap.h"
#include "lcf_saveopt.h"

/**
 * LMT Reader namespace.
 */
namespace LMT_Reader {

	/**
	 * Loads Map Tree.
	 */
	bool Load(const std::string& filename, const std::string &encoding);

	/**
	 * Saves Map Tree.
	 */
	bool Save(const std::string& filename, const std::string &encoding, SaveOpt opt = SaveOpt::eNone);

	/**
	 * Saves Map Tree as XML.
	 */
	bool SaveXml(const std::string& filename);

	/**
	 * Loads Map Tree as XML.
	 */
	bool LoadXml(const std::string& filename);

	/**
	 * Loads Map Tree.
	 */
	bool Load(std::istream& filestream, const std::string &encoding);

	/**
	 * Saves Map Tree.
	 */
	bool Save(std::ostream& filestream, const std::string &encoding, SaveOpt opt = SaveOpt::eNone);

	/**
	 * Saves Map Tree as XML.
	 */
	bool SaveXml(std::ostream& filestream);

	/**
	 * Loads Map Tree as XML.
	 */
	bool LoadXml(std::istream& filestream);
}

#endif
