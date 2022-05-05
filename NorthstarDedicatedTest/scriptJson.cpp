#include "pch.h"
#include "scriptJson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "squirrel.h"

SQRESULT sq_DecodeJSON(void* sqvm) {
	const char* json = ServerSq_getstring(sqvm, 1);
	rapidjson::Document doc;
	doc.Parse(json);
	if (doc.HasParseError()) {
		ServerSq_newTable(sqvm);
		return SQRESULT_NOTNULL;
	}
	ServerSq_newTable(sqvm);
	for (rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
		ServerSq_pushstring(sqvm, itr->name.GetString(), -1);
		ServerSq_pushstring(sqvm, itr->value.GetString(), -1);
		ServerSq_newSlot(sqvm, -3, false);
	}
	return SQRESULT_NOTNULL;
}
SQRESULT sq_EncodeJSON(void* sqvm) {
	rapidjson::Document doc;
	doc.SetObject();
	HSquirrelVM* vm = (HSquirrelVM*)sqvm;
	SQTable* table = vm->_stackOfCurrentFunction[1]._VAL.asTable;
	for (int i = 0; i < table->_numOfNodes; i++) {
		tableNode* node = &table->pointer_38[i];
		if (node->key._Type == OT_STRING) {
			doc.AddMember(rapidjson::StringRef(node->key._VAL.asString->_val), rapidjson::StringRef(node->val._VAL.asString->_val), doc.GetAllocator());
		}	
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char* json = buffer.GetString();

	ServerSq_pushstring(sqvm, json, -1);
	return SQRESULT_NOTNULL;
}


void InitialiseSquirrelJson(HMODULE baseAddress) {

	g_ServerSquirrelManager->AddFuncRegistration("table", "DecodeJSON", "string json", "", sq_DecodeJSON);
	g_ServerSquirrelManager->AddFuncRegistration("string", "EncodeJSON", "table data", "", sq_EncodeJSON);

}