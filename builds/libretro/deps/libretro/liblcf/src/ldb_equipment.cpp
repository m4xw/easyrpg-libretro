/*
 * This file is part of liblcf. Copyright (c) 2018 liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#include "ldb_reader.h"
#include "ldb_chunks.h"
#include "reader_struct.h"

template <>
struct RawStruct<RPG::Equipment> {
	static void ReadLcf(RPG::Equipment& ref, LcfReader& stream, uint32_t length);
	static void WriteLcf(const RPG::Equipment& ref, LcfWriter& stream);
	static int LcfSize(const RPG::Equipment& ref, LcfWriter& stream);
	static void WriteXml(const RPG::Equipment& ref, XmlWriter& stream);
	static void BeginXml(RPG::Equipment& ref, XmlReader& stream);
};

/**
 * Reads Equipment.
 */
void RawStruct<RPG::Equipment>::ReadLcf(RPG::Equipment& ref, LcfReader& stream, uint32_t length) {
	if (length != 10) {
		fprintf(stderr, "Equipment has incorrect size %d (expected 10)\n", length);

		LcfReader::Chunk chunk_info;
		chunk_info.ID = 0x33;
		chunk_info.length = length;

		stream.Skip(chunk_info);

		return;
	}

	stream.Read(ref.weapon_id);
	stream.Read(ref.shield_id);
	stream.Read(ref.armor_id);
	stream.Read(ref.helmet_id);
	stream.Read(ref.accessory_id);
}

void RawStruct<RPG::Equipment>::WriteLcf(const RPG::Equipment& ref, LcfWriter& stream) {
	stream.Write(ref.weapon_id);
	stream.Write(ref.shield_id);
	stream.Write(ref.armor_id);
	stream.Write(ref.helmet_id);
	stream.Write(ref.accessory_id);
}

int RawStruct<RPG::Equipment>::LcfSize(const RPG::Equipment& /* ref */, LcfWriter& /* stream */) {
	return 2 * 5;
}

void RawStruct<RPG::Equipment>::WriteXml(const RPG::Equipment& ref, XmlWriter& stream) {
	stream.BeginElement("Equipment");
	stream.WriteNode<int16_t>("weapon_id", ref.weapon_id);
	stream.WriteNode<int16_t>("shield_id", ref.shield_id);
	stream.WriteNode<int16_t>("armor_id", ref.armor_id);
	stream.WriteNode<int16_t>("helmet_id", ref.helmet_id);
	stream.WriteNode<int16_t>("accessory_id", ref.accessory_id);
	stream.EndElement("Equipment");
}

class EquipmentXmlHandler : public XmlHandler {
private:
	RPG::Equipment& ref;
	int16_t* field;
public:
	EquipmentXmlHandler(RPG::Equipment& ref) : ref(ref), field(NULL) {}
	void StartElement(XmlReader& stream, const char* name, const char** /* atts */) {
		if (strcmp(name, "weapon_id") == 0)
			field = &ref.weapon_id;
		else if (strcmp(name, "shield_id") == 0)
			field = &ref.shield_id;
		else if (strcmp(name, "armor_id") == 0)
			field = &ref.armor_id;
		else if (strcmp(name, "helmet_id") == 0)
			field = &ref.helmet_id;
		else if (strcmp(name, "accessory_id") == 0)
			field = &ref.accessory_id;
		else {
			stream.Error("Unrecognized field '%s'", name);
			field = NULL;
		}
	}
	void EndElement(XmlReader& /* stream */, const char* /* name */) {
		field = NULL;
	}
	void CharacterData(XmlReader& /* stream*/, const std::string& data) {
		if (field != NULL)
			XmlReader::Read(*field, data);
	}
};

void RawStruct<RPG::Equipment>::BeginXml(RPG::Equipment& ref, XmlReader& stream) {
	stream.SetHandler(new WrapperXmlHandler("Equipment", new EquipmentXmlHandler(ref)));
}
