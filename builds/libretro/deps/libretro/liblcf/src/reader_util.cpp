/*
 * This file is part of liblcf. Copyright (c) 2019 liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#include "lcf_options.h"
#include "scope_guard.h"

#ifdef LCF_SUPPORT_ICU
#   include <unicode/ucsdet.h>
#   include <unicode/ucnv.h>
#   include <unicode/normalizer2.h>
#   include <unicode/unistr.h>
#else
#   ifdef _MSC_VER
#		error MSVC builds require ICU
#	endif
#endif

#ifdef _WIN32
#   include <windows.h>
#else
#   ifndef LCF_SUPPORT_ICU
#       include <iconv.h>
#   endif
#   include <locale>
#endif

#if defined(__MORPHOS__) || defined(__amigaos4__)
#define ICONV_CONST const
#endif
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <vector>

#include "data.h"
#include "inireader.h"
#include "ldb_reader.h"
#include "reader_util.h"

namespace ReaderUtil {
}

std::string ReaderUtil::CodepageToEncoding(int codepage) {
	if (codepage == 0)
		return std::string();

	if (codepage == 932) {
#ifdef LCF_SUPPORT_ICU
		return "ibm-943_P15A-2003";
#else
		return "SHIFT_JIS";
#endif
	}
	if (codepage == 949) {
#ifdef LCF_SUPPORT_ICU
		return "windows-949-2000";
#else
		return "cp949";
#endif
	}
	std::ostringstream out;
#ifdef LCF_SUPPORT_ICU
	out << "windows-" << codepage;
#else
	out << "CP" << codepage;
#endif

	// Looks like a valid codepage
	std::string outs = out.str();
	return outs;
}

std::string ReaderUtil::DetectEncoding(std::istream& filestream) {
	std::vector<std::string> encodings = DetectEncodings(filestream);

	if (encodings.empty()) {
		return "";
	}

	return encodings.front();
}

std::string ReaderUtil::DetectEncoding(std::string const & data) {
	std::vector<std::string> encodings = DetectEncodings(data);

	if (encodings.empty()) {
		return "";
	}

	return encodings.front();
}

std::vector<std::string> ReaderUtil::DetectEncodings(std::istream& filestream) {
#ifdef LCF_SUPPORT_ICU
	std::ostringstream text;

	// Populate Data::terms and Data::system or will empty by default even if load fails
	LDB_Reader::Load(filestream, "");

	text <<
	Data::terms.menu_save <<
	Data::terms.menu_quit <<
	Data::terms.new_game <<
	Data::terms.load_game <<
	Data::terms.exit_game <<
	Data::terms.status <<
	Data::terms.row <<
	Data::terms.order <<
	Data::terms.wait_on <<
	Data::terms.wait_off <<
	Data::terms.level <<
	Data::terms.health_points <<
	Data::terms.spirit_points <<
	Data::terms.normal_status <<
	Data::terms.exp_short <<
	Data::terms.lvl_short <<
	Data::terms.hp_short <<
	Data::terms.sp_short <<
	Data::terms.sp_cost <<
	Data::terms.attack <<
	Data::terms.defense <<
	Data::terms.spirit <<
	Data::terms.agility <<
	Data::terms.weapon <<
	Data::terms.shield <<
	Data::terms.armor <<
	Data::terms.helmet <<
	Data::terms.accessory <<
	Data::terms.save_game_message <<
	Data::terms.load_game_message <<
	Data::terms.file <<
	Data::terms.exit_game_message <<
	Data::terms.yes <<
	Data::terms.no <<
	Data::system.boat_name <<
	Data::system.ship_name <<
	Data::system.airship_name <<
	Data::system.title_name <<
	Data::system.gameover_name <<
	Data::system.system_name <<
	Data::system.system2_name <<
	Data::system.battletest_background <<
	Data::system.frame_name;

	return ReaderUtil::DetectEncodings(text.str());
#else
	return std::vector<std::string>();
#endif
}

std::vector<std::string> ReaderUtil::DetectEncodings(std::string const & data) {
std::vector<std::string> encodings;
#ifdef LCF_SUPPORT_ICU
	if (!data.empty()) {
		UErrorCode status = U_ZERO_ERROR;
		UCharsetDetector* detector = ucsdet_open(&status);

		std::string s = data;
		ucsdet_setText(detector, s.c_str(), s.length(), &status);

		int32_t matches_count;
		const UCharsetMatch** matches = ucsdet_detectAll(detector, &matches_count, &status);

		if (matches != NULL) {
			// Collect all candidates, most confident comes first
			for (int i = 0; i < matches_count; ++i) {
				std::string encoding = ucsdet_getName(matches[i], &status);

				// Fixes to ensure proper Windows encodings
				if (encoding == "Shift_JIS") {
					encodings.push_back("ibm-943_P15A-2003"); // Japanese with \ as backslash
				} else if (encoding == "EUC-KR") {
					encodings.push_back("windows-949-2000"); // Korean with \ as backlash
				} else if (encoding == "GB18030") {
					encodings.push_back("windows-936-2000"); // Simplified Chinese
				} else if (encoding == "ISO-8859-1" || encoding == "windows-1252") {
					encodings.push_back("ibm-5348_P100-1997"); // Occidental with Euro
				} else if (encoding == "ISO-8859-2" || encoding == "windows-1250") {
					encodings.push_back("ibm-5346_P100-1998"); // Central Europe with Euro
				} else if (encoding == "ISO-8859-5" || encoding == "windows-1251") {
					encodings.push_back("ibm-5347_P100-1998"); // Cyrillic with Euro
				} else if (encoding == "ISO-8859-6" || encoding == "windows-1256") {
					encodings.push_back("ibm-9448_X100-2005"); // Arabic with Euro + 8 chars
				} else if (encoding == "ISO-8859-7" || encoding == "windows-1253") {
					encodings.push_back("ibm-5349_P100-1998"); // Greek with Euro
				} else if (encoding == "ISO-8859-8" || encoding == "windows-1255") {
					encodings.push_back("ibm-9447_P100-2002"); // Hebrew with Euro
				} else {
					encodings.push_back(encoding);
				}
			}
		}
		ucsdet_close(detector);
	}
#endif

	return encodings;
}

std::string ReaderUtil::GetEncoding(const std::string& ini_file) {
	INIReader ini(ini_file);
	if (ini.ParseError() != -1) {
		std::string encoding = ini.Get("EasyRPG", "Encoding", std::string());
		if (!encoding.empty()) {
			return ReaderUtil::CodepageToEncoding(atoi(encoding.c_str()));
		}
	}
	return std::string();
}

std::string ReaderUtil::GetEncoding(std::istream& filestream) {
	INIReader ini(filestream);
	if (ini.ParseError() != -1) {
		std::string encoding = ini.Get("EasyRPG", "Encoding", std::string());
		if (!encoding.empty()) {
			return ReaderUtil::CodepageToEncoding(atoi(encoding.c_str()));
		}
	}
	return std::string();
}

std::string ReaderUtil::GetLocaleEncoding() {
#ifdef _WIN32
	int codepage = GetACP();
#elif __ANDROID__
	// No std::locale support in NDK
	// Doesn't really matter because the Android version auto-detects via ICU
	int codepage = 1252;
#else
	int codepage = 1252;

	std::locale loc = std::locale("");
	// Gets the language and culture part only
	std::string loc_full = loc.name().substr(0, loc.name().find_first_of("@."));
	// Gets the language part only
	std::string loc_lang = loc.name().substr(0, loc.name().find_first_of("_"));

	if      (loc_lang == "th")    codepage = 874;
	else if (loc_lang == "ja")    codepage = 932;
	else if (loc_full == "zh_CN" ||
	         loc_full == "zh_SG") codepage = 936;
	else if (loc_lang == "ko")    codepage = 949;
	else if (loc_full == "zh_TW" ||
	         loc_full == "zh_HK") codepage = 950;
	else if (loc_lang == "cs" ||
	         loc_lang == "hu" ||
	         loc_lang == "pl" ||
	         loc_lang == "ro" ||
	         loc_lang == "hr" ||
	         loc_lang == "sk" ||
	         loc_lang == "sl")    codepage = 1250;
	else if (loc_lang == "ru")    codepage = 1251;
	else if (loc_lang == "ca" ||
	         loc_lang == "da" ||
	         loc_lang == "de" ||
	         loc_lang == "en" ||
	         loc_lang == "es" ||
	         loc_lang == "fi" ||
	         loc_lang == "fr" ||
	         loc_lang == "it" ||
	         loc_lang == "nl" ||
	         loc_lang == "nb" ||
	         loc_lang == "pt" ||
	         loc_lang == "sv" ||
	         loc_lang == "eu")    codepage = 1252;
	else if (loc_lang == "el")    codepage = 1253;
	else if (loc_lang == "tr")    codepage = 1254;
	else if (loc_lang == "he")    codepage = 1255;
	else if (loc_lang == "ar")    codepage = 1256;
	else if (loc_lang == "et" ||
	         loc_lang == "lt" ||
	         loc_lang == "lv")    codepage = 1257;
	else if (loc_lang == "vi")    codepage = 1258;
#endif

	return CodepageToEncoding(codepage);
}

std::string ReaderUtil::Recode(const std::string& str_to_encode, const std::string& source_encoding) {
	return ReaderUtil::Recode(str_to_encode, source_encoding, "UTF-8");
}

std::string ReaderUtil::Recode(const std::string& str_to_encode,
                               const std::string& src_enc,
                               const std::string& dst_enc) {

	if (src_enc.empty() || dst_enc.empty() || str_to_encode.empty()) {
		return str_to_encode;
	}

	auto src_cp = atoi(src_enc.c_str());
	const auto& src_enc_str = src_cp > 0
		? ReaderUtil::CodepageToEncoding(src_cp)
		: src_enc;

	auto dst_cp = atoi(dst_enc.c_str());
	const auto& dst_enc_str = dst_cp > 0
		? ReaderUtil::CodepageToEncoding(dst_cp)
		: dst_enc;

#ifdef LCF_SUPPORT_ICU
	auto status = U_ZERO_ERROR;
	auto conv_from = ucnv_open(src_enc_str.c_str(), &status);

	if (status != U_ZERO_ERROR && status != U_AMBIGUOUS_ALIAS_WARNING) {
		fprintf(stderr, "liblcf:  ucnv_open() error for source encoding \"%s\": %s\n", src_enc_str.c_str(), u_errorName(status));
		return std::string();
	}
	status = U_ZERO_ERROR;
	auto conv_from_sg = makeScopeGuard([&]() { ucnv_close(conv_from); });

	auto conv_to = ucnv_open(dst_enc_str.c_str(), &status);

	if (status != U_ZERO_ERROR && status != U_AMBIGUOUS_ALIAS_WARNING) {
		fprintf(stderr, "liblcf:  ucnv_open() error for dest encoding \"%s\": %s\n", dst_enc_str.c_str(), u_errorName(status));
		return std::string();
	}
	auto conv_to_sg = makeScopeGuard([&]() { ucnv_close(conv_to); });
	status = U_ZERO_ERROR;

	std::string result(str_to_encode.size() * 4, '\0');
	auto* src = &str_to_encode.front();
	auto* dst = &result.front();

	ucnv_convertEx(conv_to, conv_from,
			&dst, dst + result.size(),
			&src, src + str_to_encode.size(),
			nullptr, nullptr, nullptr, nullptr,
			true, true,
			&status);

	if (U_FAILURE(status)) {
		fprintf(stderr, "liblcf: ucnv_convertEx() error when encoding \"%s\": %s\n", str_to_encode.c_str(), u_errorName(status));
		return std::string();
	}

	result.resize(dst - result.c_str());
	result.shrink_to_fit();

	return result;
#else
	iconv_t cd = iconv_open(dst_enc_str.c_str(), src_enc_str.c_str());
	if (cd == (iconv_t)-1)
		return str_to_encode;
	char *src = const_cast<char *>(str_to_encode.c_str());
	size_t src_left = str_to_encode.size();
	size_t dst_size = str_to_encode.size() * 5 + 10;
	char *dst = new char[dst_size];
	size_t dst_left = dst_size;
#    ifdef ICONV_CONST
	char ICONV_CONST *p = src;
#    else
	char *p = src;
#    endif
	char *q = dst;
	size_t status = iconv(cd, &p, &src_left, &q, &dst_left);
	iconv_close(cd);
	if (status == (size_t) -1 || src_left > 0) {
		delete[] dst;
		return std::string();
	}
	*q++ = '\0';
	std::string result(dst);
	delete[] dst;
	return result;
#endif
}

std::string ReaderUtil::Normalize(const std::string &str) {
#ifdef LCF_SUPPORT_ICU
	icu::UnicodeString uni = icu::UnicodeString(str.c_str(), "utf-8").toLower();
	UErrorCode err = U_ZERO_ERROR;
	std::string res;
	const icu::Normalizer2* norm = icu::Normalizer2::getNFKCInstance(err);
	if (U_FAILURE(err)) {
		static bool err_reported = false;
		if (!err_reported) {
			fprintf(stderr, "Normalizer2::getNFKCInstance failed (%s). \"nrm\" is probably missing in the ICU data file. Unicode normalization will not work!\n", u_errorName(err));
			err_reported = true;
		}
		uni.toUTF8String(res);
		return res;
	}
	icu::UnicodeString f = norm->normalize(uni, err);
	if (U_FAILURE(err)) {
		uni.toUTF8String(res);
	} else {
		f.toUTF8String(res);
	}
	return res;
#else
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), tolower);
	return result;
#endif
}
