#pragma once
/*
   This file has been generated by IDA.
   It contains local type definitions from
   the type library 'server.dll'
*/

#include <spdlog/fmt/bundled/format.h>



struct HSquirrelVM;
struct CallInfo;
struct SQTable;
struct SQString;
struct SQFunctionProto;
struct SQClosure;
struct SQSharedState;
struct StringTable;
struct SQStructInstance;
struct SQStructDef;
struct SQNativeClosure;
struct SQArray;
struct tableNode;
struct SQUserData;
struct SQInstruction;
struct SQRefCounted;
struct SQLineInfo;
struct SQLocalVarInfo;
struct SQDbgServer;

const char* sq_getTypeName(int type);
const char* sq_OpToString(int op);
int sqInstructionToString(char* outBuffer, int outBufferLength, SQInstruction* instruction, HSquirrelVM* sqvm);
int sqInstructionToString(char* outBuffer, int outBufferLength, SQInstruction* instruction, int index,int stackbase);
typedef void (*releasehookType)(void* val, int size);

#define HIDWORD(x) (*((int*)&(x) + 1)) //TODO REMOVE THESE
#define LODWORD(x) (*((int*)&(x))) //TODO REMOVE THESE


/* 127 */
enum SQObjectType : int
{
	_RT_NULL = 0x1,
	_RT_INTEGER = 0x2,
	_RT_FLOAT = 0x4,
	_RT_BOOL = 0x8,
	_RT_STRING = 0x10,
	_RT_TABLE = 0x20,
	_RT_ARRAY = 0x40,
	_RT_USERDATA = 0x80,
	_RT_CLOSURE = 0x100,
	_RT_NATIVECLOSURE = 0x200,
	_RT_GENERATOR = 0x400,
	OT_USERPOINTER = 0x800,
	_RT_USERPOINTER = 0x800,
	_RT_THREAD = 0x1000,
	_RT_FUNCPROTO = 0x2000,
	_RT_CLASS = 0x4000,
	_RT_INSTANCE = 0x8000,
	_RT_WEAKREF = 0x10000,
	OT_VECTOR = 0x40000,
	SQOBJECT_CANBEFALSE = 0x1000000,
	OT_NULL = 0x1000001,
	OT_BOOL = 0x1000008,
	SQOBJECT_DELEGABLE = 0x2000000,
	SQOBJECT_NUMERIC = 0x4000000,
	OT_INTEGER = 0x5000002,
	OT_FLOAT = 0x5000004,
	SQOBJECT_REF_COUNTED = 0x8000000,
	OT_STRING = 0x8000010,
	OT_ARRAY = 0x8000040,
	OT_CLOSURE = 0x8000100,
	OT_NATIVECLOSURE = 0x8000200,
	OT_ASSET = 0x8000400,
	OT_THREAD = 0x8001000,
	OT_FUNCPROTO = 0x8002000,
	OT_CLAAS = 0x8004000,
	OT_STRUCT_DEF = 0x8100000,
	OT_STRUCT_INSTANCE = 0x8200000,
	OT_WEAKREF = 0x8010000,
	OT_TABLE = 0xA000020,
	OT_USERDATA = 0xA000080,
	OT_INSTANCE = 0xA008000,
	OT_ENTITY = 0xA400000,
	OT_UNIMPLEMENTED_FUNCTION = 0x8080000,
};

/* 156 */


struct SQRefCountedVTable
{
	void* constructorMaybe;
	void (*destructor)(SQRefCounted*);
};

struct SQRefCounted
{
	SQRefCountedVTable* vtable;
	int uiRef;
	int padding;

};

struct othervarInfo
{
	__int64 unknown_0;
	__int64 unknown_8;
	SQString* name;
	__int64 unknown_18;
	__int64 unknown_20;
	__int64 stackposition;
};

union SQObjectValue
{
	SQString* asString;
	SQTable* asTable;
	SQClosure* asClosure;
	SQFunctionProto* asFuncProto;
	SQStructDef* asStructDef;
	long long as64Integer;
	SQNativeClosure* asNativeClosure;
	SQArray* asArray;
	HSquirrelVM* asThread;
	float asFloat;
	int asInteger;
	SQUserData* asUserdata;
	SQStructInstance* asStructInstance;
	SQRefCounted* asRefCounted;
	bool asBoolean;
};

/* 160 */
struct SQVector
{
	SQObjectType _Type;
	float x;
	float y;
	float z;
	SQVector()
	{
		this->_Type = OT_VECTOR;
		this->x = 0.0;
		this->y = 0.0;
		this->z = 0.0;
	}
};

/* 128 */
struct SQObject
{
	SQObjectType _Type;
	int _structOffset;
	SQObjectValue _VAL;

	SQObject& operator=(const SQObject& other) {
		if (this == &other)
			return *this;
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			if (this->_VAL.asRefCounted->uiRef-- == 1)
				//if (this->_VAL.asRefCounted->vtable && this->_VAL.asRefCounted->vtable->destructor)
					this->_VAL.asRefCounted->vtable->destructor(this->_VAL.asRefCounted);
		}
		if (other._Type & SQOBJECT_REF_COUNTED)
		{
			other._VAL.asRefCounted->uiRef++;
		}

		this->_Type = other._Type;
		this->_structOffset = other._structOffset;
		this->_VAL = other._VAL;
		return *this;
	}
	SQObject& operator=(const float& other)
	{
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			if (this->_VAL.asRefCounted->uiRef-- == 1)
				//if (this->_VAL.asRefCounted->vtable && this->_VAL.asRefCounted->vtable->destructor)
					this->_VAL.asRefCounted->vtable->destructor(this->_VAL.asRefCounted);
		}

		this->_Type = OT_FLOAT;
		this->_structOffset = 0;
		this->_VAL.as64Integer = 0i64;
		this->_VAL.asFloat = other;
		return *this;
	}
	SQObject& operator=(const int& other)
	{
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			if (this->_VAL.asRefCounted->uiRef-- == 1)
				//if (this->_VAL.asRefCounted->vtable && this->_VAL.asRefCounted->vtable->destructor)
					this->_VAL.asRefCounted->vtable->destructor(this->_VAL.asRefCounted);
		}

		this->_Type = OT_INTEGER;
		this->_structOffset = 0;
		this->_VAL.as64Integer = 0i64;
		this->_VAL.asInteger = other;
		return *this;
	}
	SQObject& operator=(const bool& other)
	{
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			if (this->_VAL.asRefCounted->uiRef-- == 1)
				//if (this->_VAL.asRefCounted->vtable && this->_VAL.asRefCounted->vtable->destructor)
					this->_VAL.asRefCounted->vtable->destructor(this->_VAL.asRefCounted);
		}

		this->_Type = OT_BOOL;
		this->_structOffset = 0;
		this->_VAL.as64Integer = 0i64;
		this->_VAL.asInteger =(int) other;
		return *this;
	}
	SQObject& operator=(const SQVector& other)
	{
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			if (this->_VAL.asRefCounted->uiRef-- == 1)
				//if (this->_VAL.asRefCounted->vtable && this->_VAL.asRefCounted->vtable->destructor)
					this->_VAL.asRefCounted->vtable->destructor(this->_VAL.asRefCounted);
		}

		this->_Type = OT_VECTOR;
		SQVector* thisVec = (SQVector*)this;
		thisVec->x = other.x;
		thisVec->y = other.y;
		thisVec->z = other.z;

		return *this;
	}
	SQObject& operator=(SQTable* other);
	SQObject& operator=(SQArray* other);
	SQObject& operator=(SQStructInstance* other);
	SQObject& operator=(SQString* other);

	~SQObject()
	{
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			if (this->_VAL.asRefCounted->uiRef-- == 1)
				//if (this->_VAL.asRefCounted->vtable && this->_VAL.asRefCounted->vtable->destructor)
					this->_VAL.asRefCounted->vtable->destructor(this->_VAL.asRefCounted);
		}
	}
	SQObject()
	{
		this->_Type = OT_NULL;
		this->_structOffset = 0;
		this->_VAL.as64Integer = 0i64;
	}
	void Null()
	{
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			if (this->_VAL.asRefCounted->uiRef-- == 1)
				//if (this->_VAL.asRefCounted->vtable && this->_VAL.asRefCounted->vtable->destructor)
					this->_VAL.asRefCounted->vtable->destructor(this->_VAL.asRefCounted);
		}
		this->_Type = OT_NULL;
		this->_structOffset = 0;
		this->_VAL.as64Integer = 0i64;
	}
	void IncrementReference() {
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			this->_VAL.asRefCounted->uiRef++;
		}
	}
	void DecrementReference() {
		if (this->_Type & SQOBJECT_REF_COUNTED)
		{
			if (this->_VAL.asRefCounted->uiRef-- == 1)
				//if (this->_VAL.asRefCounted->vtable && this->_VAL.asRefCounted->vtable->destructor)
					this->_VAL.asRefCounted->vtable->destructor(this->_VAL.asRefCounted);
		}
	}
};

struct alignas(4) SQInstruction
{
	int op;
	int arg1;
	int output;
	short arg2;
	short arg3;
};

/* 138 */
struct alignas(8) SQString
{
	void* vtable;
	int uiRef;
	int padding;
	SQString* _next_maybe;
	SQSharedState* sharedState;
	int length;
	unsigned char gap_24[4]; // prob padding
	long long _hash;
	char _val[1];
};

/* 137 */
struct alignas(8) SQTable
{
	void* vtable;
	int uiRef;
	unsigned char gap_0C[4];
	unsigned char gap_10[8];
	void* pointer_18;
	void* pointer_20;
	void* _sharedState;
	long long field_30;
	tableNode* _nodes;
	int _numOfNodes;
	int size;
	int field_48;
	int _usedNodes;
	unsigned char _gap_50[20];
	int field_64;
	unsigned char _gap_68[80];
};

/* 140 */
struct alignas(8) SQClosure
{
	void* vtable;
	int uiRef;
	unsigned char gap_0C[4];
	void* pointer_10;
	void* pointer_18;
	void* pointer_20;
	SQSharedState* sharedState;
	SQObject obj_30;
	SQObject _function;
	SQObject* _outervalues;
	unsigned char gap_58[8];
	unsigned char gap_60[96];
	SQObject* fastcallClosureArray;
	unsigned char gap_C8[16];
};

/* 139 */
struct alignas(8) SQFunctionProto
{
	void* vftable;
	int uiRef;
	BYTE gap_0C[12];
	void* pointer_18;
	void* pointer_20;
	void* sharedState;
	void* pointer_30;
	SQObject fileName;
	SQObject funcName;
	SQObject obj_58;
	BYTE gap_68[8];
	int unknown_70;
	int _stacksize;
	BYTE gap_78[4];
	int localVarInfoSize;
	SQLocalVarInfo* localVarInfos;
	int lineInfoSize;
	BYTE gap_8C[4];
	SQLineInfo* lineInfos;
	int literalsSize;
	BYTE gap_9C[4];
	SQObject* literals;
	__int32 nParameters;
	BYTE gap_AC[4];
	SQObject* _parameters;
	int unknown_B8;
	BYTE gap_BC[4];
	SQObject* unknownPointer;
	int unknown_C8;
	BYTE gap_CC[4];
	long long* unknownArray_D0;
	int otherVarInfoSize;
	BYTE gap_DC[4];
	othervarInfo* _otherVarInfo;
	int nDefaultParams;
	BYTE gap_EC[4];
	SQObject* objectArray_F0;
	int unknown_F8;
	BYTE gap_FC[4];
	SQInstruction skippedInstruction;
	SQInstruction instruction[1];
};


/* 152 */
struct SQStructDef
{
	void* vtable;
	int uiRef;
	unsigned char padding_C[4];
	unsigned char unknown[24];
	SQSharedState* sharedState;
	SQObjectType _nameType;
	SQString* _name;
	unsigned char gap_38[16];
	SQObjectType _variableNamesType;
	SQTable* _variableNames;
	unsigned char gap_[32];
};

/* 157 */
struct alignas(8) SQNativeClosure
{
	void* vtable;
	int uiRef;
	unsigned char gap_C[4];
	long long value_10;
	long long value_18;
	long long value_20;
	SQSharedState* sharedState;
	char unknown_30;
	unsigned char padding_34[7];
	long long value_38;
	long long value_40;
	long long value_48;
	long long value_50;
	long long functionPointer;
	SQObjectType _nameType;
	SQString* _name;
	long long value_70;
	long long value_78;
	unsigned char justInCaseGap_80[300];
};

/* 162 */
struct SQArray
{
	void* vtable;
	int uiRef;
	unsigned char gap_24[36];
	SQObject* _values;
	int _usedSlots;
	int _allocated;
};

/* 129 */
struct alignas(8) HSquirrelVM
{
	void* vtable;
	int uiRef;
	unsigned char gap_C[12];
	void* _toString;
	void* _roottable_pointer;
	void* pointer_28;
	CallInfo* ci;
	CallInfo* _callsstack;
	int _callsstacksize;
	int _stackbase;
	SQObject* _stackOfCurrentFunction;
	SQSharedState* sharedState;
	void* pointer_58;
	void* pointer_60;
	int _top;
	SQObject* _stack;
	unsigned char gap_78[8];
	SQObject* _vargsstack;
	unsigned char gap_88[8];
	SQObject temp_reg;
	unsigned char gapA0[8];
	void* pointer_A8;
	unsigned char gap_B0[8];
	SQObject _roottable_object;
	SQObject _lasterror;
	SQObject _errorHandler;
	long long field_E8;
	int traps;
	unsigned char gap_F4[12];
	int _nnativecalls;
	int _suspended;
	int _suspended_root;
	int _callstacksize;
	int _suspended_target;
	int trapAmount;
	int _suspend_varargs;
	int unknown_field_11C;
	SQObject _object_120;
};

/* 150 */
struct SQStructInstance
{
	void* vtable;
	unsigned int uiRef;
	unsigned char gap_C[12];
	void* pointer_18;
	unsigned char gap_20[8];
	SQSharedState* _sharedState;
	unsigned char gap[8];
	SQObject data[20];
};

/* 148 */
struct alignas(8) SQSharedState
{
	BYTE gap_0[72];
	void* unknown;
	BYTE gap_50[16308];
	__int8 unknown_4004;
	BYTE gap_4005[3];
	__int32 unknown_4008;
	BYTE gap_400C[12];
	__int64 unknown_4018;
	BYTE gap_4020[8];
	SQObjectType _unknownTableType00;
	__int64 _unknownTableValue00;
	BYTE gap_4038[16];
	StringTable* _stringTable;
	BYTE gap_4050[32];
	SQObjectType _unknownTableType0;
	__int64 _unknownTableValue0;
	SQObjectType _unknownObjectType1;
	__int64 _unknownObjectValue1;
	BYTE gap_4090[8];
	SQObjectType _unknownArrayType2;
	__int64 _unknownArrayValue2;
	SQObject _globalsArray;
	BYTE gap_40B8[16];
	SQObject _nativeClosures;
	SQObject _typedConstants;
	SQObject _untypedConstants;
	SQObject _globals;
	SQObject _functions;
	SQObject _structs;
	SQObject _typeDefs;
	SQObject unknownTable;
	SQObject _squirrelFiles;
	BYTE gap_4158[24];
	SQObject fileVariables;
	BYTE gap_4180[16];
	SQTable* unknownTable_4190;
	BYTE gap_4198[8];
	SQTable* unknownTable_41A0;
	SQObject _nativeClosures2;
	SQObject _entityTypesMaybe;
	SQObject unknownTable2;
	char* unknownPointer_41D8;
	__int32 unknown_41E0;
	BYTE gap_41E4[4];
	char* unknownPointer_41E8;
	int usedBufferBits;
	unsigned int unknown_41F4;
	BYTE gap_41F8[8];
	__int64 unknown_4200;
	__int64 unknown_4208;
	BYTE gap_4210[8];
	void* compiler;
	SQObject _compilerKeywordTable;
	HSquirrelVM* _currentThreadMaybe;
	SQDbgServer* debugServer;
	SQObject unknownTable3;
	void* lastAddedChainableMaybe;
	BYTE gap_4258[8];
	SQObject unknownThread;
	SQObject _tableNativeFunctions;
	SQObject _unknownTable4;
	SQObject _unknownObject5;
	SQObject _unknownObject6;
	SQObject _unknownObject7;
	SQObject _unknownObject8;
	SQObject _unknownObject9;
	SQObject _unknownObject10;
	SQObject _unknownObject11;
	SQObject _unknownObject12;
	SQObject _unknownObject13;
	SQObject _unknownObject14;
	SQObject _unknownObject15;
	BYTE gap_4340[8];
	void* compileErrorFunction;
	void* printFunction;
	BYTE gap_4358[16];
	void* logEntityFunction;
	BYTE gap_4370[40];
	SQObject _waitStringValue;
	SQObject _SpinOffAndWaitForStringValue;
	SQObject _SpinOffAndWaitForSoloStringValue;
	SQObject _SpinOffStringValue;
	SQObject _SpinOffDelayedStringValue;
	BYTE gap_43E8[8];
	bool enableDebugInfo;
	BYTE gap_43F1[23];
};


/* 165 */
struct tableNode
{
	SQObject val;
	SQObject key;
	tableNode* next;
};

/* 136 */
struct alignas(8) CallInfo
{
	SQInstruction* ip;
	SQObject* _literals;
	SQObject obj10;
	SQObject closure;
	int _etraps;
	int _stackbaseOffset;
	int _unknown;
	int _unknown1;
	int _root;
	short _vargs_size;
	short _vargs_base;
};

/* 149 */
struct StringTable
{
	unsigned char gap_0[12];
	int _numofslots;
	unsigned char gap_10[200];
};

/* 141 */
struct alignas(8) SQStackInfos
{
	char* _name;
	char* _sourceName;
	int _line;
};

/* 151 */


/* 154 */
struct SQLexer
{
	unsigned char gap_0[112];
};

/* 153 */
struct SQCompiler
{
	unsigned char gap_0[4];
	int _token;
	unsigned char gap_8[8];
	SQObject object_10;
	SQLexer lexer;
	unsigned char gap_90[752];
	HSquirrelVM* sqvm;
	unsigned char gap_288[8];
};

/* 155 */
struct CSquirrelVM
{
	unsigned char gap_0[8];
	HSquirrelVM* sqvm;
	unsigned char gap_10[44];
	int loadEnumFromFileMaybe;
	unsigned char gap_40[200];
};

struct SQUserData
{
	void* vtable;
	int uiRef;
	char gap_12[4];
	long long unknown_10;
	long long unknown_18;
	long long unknown_20;
	long long sharedState;
	long long unknown_30;
	int size;
	char padding1[4];
	releasehookType releaseHook;
	long long typeId;
	char data[1];
};

struct SQLocalVarInfo
{
	SQObject name;
	long long qword10;
	int _start_op;
	int _end_op;
	int stackpos;
	int dword24;
};

struct SQLineInfo
{
	int line;
	int op;
};

struct alignas(8) SQFuncState
{
	BYTE gap_0[1536];
	int unknown_600;
	BYTE gap_604[256];
	int lexicalScopesSizeMaybe;
	BYTE gap_708[8200];
	SQLocalVarInfo constInfo[32];
	int _constSize;
	BYTE gap_2C14[1292];
	SQLocalVarInfo* _vlocalsData;
	unsigned int _vlocalsSize;
	unsigned int _vlocalsAllocatedSize;
	int* unknownPointer_3130;
	unsigned int unknown_3138;
	BYTE gap_313C[20];
	unsigned int _stacksize;
	BYTE gap_3154[36];
	SQObject unknownObject_3178;
	BYTE gap_3188[8];
	unsigned int unknown_3190;
	BYTE gap_3194[12];
	unsigned int _parametersSize;
	BYTE gap_31A4[4];
	char unkknown_31A8;
	BYTE gap_31A9[7];
	unsigned int unknown_31B0;
	BYTE gap_31B4[4];
	SQInstruction* _instructions;
	unsigned int _instructionsSize;
	unsigned int _instructionsAllocatedSize;
	BYTE gap_31C8[8];
	unsigned int unknown_31D0;
	BYTE gap_31D4[4];
	int unknown_31D8;
	BYTE gap_31DC[4];
	SQLocalVarInfo* _localvarinfoData;
	unsigned int _localvarinfoSize;
	unsigned int _localvarinfoAllocadedSize;
	BYTE gap_31F0[16];
	SQObject unknownSqTable_3200;
	SQObject funcName;
	SQObject fileName;
	BYTE gap_3230[4];
	unsigned int _nliterralsMaybe;
	unsigned int unknown_3238;
	BYTE gap_323C[4];
	SQLineInfo* _lineInfoData;
	unsigned int _lineInfoSize;
	int _lineInfoAllocatedSize;
	__int64 unknown_3250;
	BYTE gap_3258[32];
	void* _defaultParamData;
	unsigned int _defaultParamSize;
	BYTE gap_3284[4];
	int lastLineInfoLine;
	BYTE gap_328C[4];
	char unknown_3290;
	BYTE gap_3291[7];
	SQSharedState* sharedState;
	BYTE gap_32A0[16];
	__int64 unknown_32B0;
	unsigned int unknown_32B8;
	BYTE gap_32BC[4];
	void* unknownPointer_32C0;
	__int64 unknown_32C8;
	void* error_func;
	__int64 error_func_param;
};
struct SQ_XMLElementState
{
	char name[256];
	bool haschildren;
};

struct alignas(8) SQDbgServer
{
	SQ_XMLElementState xmlState[10];
	BYTE gap_00[2];
	int xmlCurrentElement;
	HSquirrelVM* sqvm;
	SQObject unknownObject_A18;
	__int32 _state;
	BYTE gap_A2C[4];
	SOCKET unusedHostSocket;
	SOCKET socketToDebugger;
	SOCKET socketToDebuggerCopy;
	__int64 unknown_A48;
	__int64 unkonwn_A50;
	char* functionpointer_A58;
	__int64 unknown_A60;
	void* functionpointer_A68;
	__int64 unknown_A70;
	void* functionpointer_A78;
	__int64 unknown_A80;
	BYTE gap_A88[2];
	bool _autoupdate;
	BYTE gap_A8C[5];
	SQObject obj;
	char* _scratchString;
	__int64 unknown_AA8;
	__int64 unknown_AB0;
	SQObject unknownObject_AB8;
	BYTE gap_AC8[8];
	__int8 unknown_AD0;
	BYTE gap_AD1[15];
	__int64 unknown_AE0;
	__int64 unknown_AE8;
	BYTE gap_AF0[16];
	__int64 unknown_B00;
	__int64 unknown_B08;
	__int64 unknown_B10;
	__int64 unknown_B18;
	char sendbuffer[1024];
	unsigned int sendBufferLength;
	BYTE gap_F24[4];
	BYTE endat_F28;
};


enum SQOpCodes
{
	_OP_LINE = 0,
	_OP_LOAD = 1,
	_OP_LOADCOPY = 2,
	_OP_LOADINT = 3,
	_OP_LOADFLOAT = 4,
	_OP_DLOAD = 5,
	_OP_TAILCALL = 6,
	_OP_CALL = 7,
	_OP_PREPCALL = 8,
	_OP_PREPCALLK = 9,
	_OP_GETK = 10,
	_OP_MOVE = 11,
	_OP_NEWSLOT = 12,
	_OP_DELETE = 13,
	_OP_SET = 14,
	_OP_GET = 15,
	_OP_EQ = 16,
	_OP_NE = 17,
	_OP_ARITH = 18,
	_OP_BITW = 19,
	_OP_RETURN = 20,
	_OP_LOADNULLS = 21,
	_OP_LOADROOTTABLE = 22,
	_OP_LOADBOOL = 23,
	_OP_DMOVE = 24,
	_OP_JMP = 25,
	_OP_JNZ = 26,
	_OP_JZ = 27,
	_OP_LOADFREEVAR = 28,
	_OP_VARGC = 29,
	_OP_GETVARGV = 30,
	_OP_NEWTABLE = 31,
	_OP_NEWARRAY = 32,
	_OP_APPENDARRAY = 33,
	_OP_GETPARENT = 34,
	_OP_COMPOUND_ARITH = 35,
	_OP_COMPOUND_ARITH_LOCAL = 36,
	_OP_INCREMENT_PREFIX = 37,
	_OP_INCREMENT_PREFIX_LOCAL = 38,
	_OP_INCREMENT_PREFIX_STRUCTFIELD = 39,
	_OP_INCREMENT_POSTFIX = 40,
	_OP_INCREMENT_POSTFIX_LOCAL = 41,
	_OP_INCREMENT_POSTFIX_STRUCTFIELD = 42,
	_OP_CMP = 43,
	_OP_EXISTS = 44,
	_OP_INSTANCEOF = 45,
	_OP_NEG = 46,
	_OP_NOT = 47,
	_OP_BWNOT = 48,
	_OP_CLOSURE = 49,
	_OP_FOREACH = 50,
	_OP_FOREACH_STATICARRAY_START = 51,
	_OP_FOREACH_STATICARRAY_NEXT = 52,
	_OP_FOREACH_STATICARRAY_NESTEDSTRUCT_START = 53,
	_OP_FOREACH_STATICARRAY_NESTEDSTRUCT_NEXT = 54,
	_OP_DELEGATE = 55,
	_OP_CLONE = 56,
	_OP_TYPEOF = 57,
	_OP_PUSHTRAP = 58,
	_OP_POPTRAP = 59,
	_OP_THROW = 60,
	_OP_CLASS = 61,
	_OP_NEWSLOTA = 62,
	_OP_EQ_LITERAL = 63,
	_OP_NE_LITERAL = 64,
	_OP_FOREACH_SETUP = 65,
	_OP_ASSERT_FAILED = 66,
	_OP_ADD = 67,
	_OP_SUB = 68,
	_OP_MUL = 69,
	_OP_DIV = 70,
	_OP_MOD = 71,
	_OP_PREPCALLK_CALL = 72,
	_OP_PREPCALLK_MOVE_CALL = 73,
	_OP_PREPCALLK_LOADINT_CALL = 74,
	_OP_CMP_JZ = 75,
	_OP_INCREMENT_LOCAL_DISCARD_JMP = 76,
	_OP_JZ_RETURN = 77,
	_OP_JZ_LOADBOOL_RETURN = 78,
	_OP_NEWVECTOR = 79,
	_OP_ZEROVECTOR = 80,
	_OP_GET_VECTOR_COMPONENT = 81,
	_OP_SET_VECTOR_COMPONENT = 82,
	_OP_VECTOR_COMPONENT_MINUSEQ = 83,
	_OP_VECTOR_COMPONENT_PLUSEQ = 84,
	_OP_VECTOR_COMPONENT_MULEQ = 85,
	_OP_VECTOR_COMPONENT_DIVEQ = 86,
	_OP_VECTOR_NORMALIZE = 87,
	_OP_VECTOR_NORMALIZE_IN_PLACE = 88,
	_OP_VECTOR_DOT_PRODUCT = 89,
	_OP_VECTOR_DOT_PRODUCT2D = 90,
	_OP_VECTOR_CROSS_PRODUCT = 91,
	_OP_VECTOR_CROSS_PRODUCT2D = 92,
	_OP_VECTOR_LENGTH = 93,
	_OP_VECTOR_LENGTHSQR = 94,
	_OP_VECTOR_LENGTH2D = 95,
	_OP_VECTOR_LENGTH2DSQR = 96,
	_OP_VECTOR_DISTANCE = 97,
	_OP_VECTOR_DISTANCESQR = 98,
	_OP_VECTOR_DISTANCE2D = 99,
	_OP_VECTOR_DISTANCE2DSQR = 100,
	_OP_INCREMENT_LOCAL_DISCARD = 101,
	_OP_FASTCALL = 102,
	_OP_FASTCALL_NATIVE = 103,
	_OP_FASTCALL_NATIVE_ARGTYPECHECK = 104,
	_OP_FASTCALL_ENV = 105,
	_OP_FASTCALL_NATIVE_ENV = 106,
	_OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK = 107,
	_OP_LOADGLOBALARRAY = 108,
	_OP_GETGLOBAL = 109,
	_OP_SETGLOBAL = 110,
	_OP_COMPOUND_ARITH_GLOBAL = 111,
	_OP_GETSTRUCTFIELD = 112,
	_OP_SETSTRUCTFIELD = 113,
	_OP_COMPOUND_ARITH_STRUCTFIELD = 114,
	_OP_NEWSTRUCT = 115,
	_OP_GETSUBSTRUCT = 116,
	_OP_GETSUBSTRUCT_DYNAMIC = 117,
	_OP_TYPECAST = 118,
	_OP_TYPECHECK = 119,
	_OP_TYPECHECK_ORNULL = 120,
	_OP_TYPECHECK_NOTNULL = 121,
	_OP_CHECK_ENTITY_CLASS = 122,
	_OP_UNREACHABLE = 123,
	_OP_ARRAY_RESIZE = 124,
};



template <> struct fmt::formatter<SQString>
{
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
	{
		return ctx.end();
	}

	template <typename FormatContext> auto format(const SQString& input, FormatContext& ctx) -> decltype(ctx.out())
	{
		return format_to(ctx.out(), "{}", std::string(input._val,input.length));
	}
};

template <> struct fmt::formatter<SQObject>
{
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
	{
		return ctx.end();
	}

	template <typename FormatContext> auto format(const SQObject& input, FormatContext& ctx) -> decltype(ctx.out())
	{
		switch (input._Type)
		{
		case OT_INTEGER:
			return format_to(ctx.out(), "INTEGER({})", input._VAL.asInteger);
		case OT_FLOAT:
			return format_to(ctx.out(), "FLOAT({})", input._VAL.asFloat);
		case OT_STRING:
			return format_to(ctx.out(), "STRING(\"{}\")", std::string(input._VAL.asString->_val, input._VAL.asString->length));
		case OT_ASSET:
			return format_to(ctx.out(), "ASSET(\"{}\")", std::string(input._VAL.asString->_val, input._VAL.asString->length));
		case OT_FUNCPROTO:
			return format_to(
				ctx.out(),
				"FUNCPROTO(\"{}\")",
				std::string(input._VAL.asFuncProto->funcName._VAL.asString->_val, input._VAL.asFuncProto->funcName._VAL.asString->length));
		case OT_NATIVECLOSURE:
			return format_to(
				ctx.out(),
				"NATIVECLOSURE(\"{}\")",
				std::string(input._VAL.asNativeClosure->_name->_val, input._VAL.asNativeClosure->_name->length));
		case OT_BOOL:
			return format_to(ctx.out(), "BOOL({})", input._VAL.asBoolean);
		case OT_ARRAY:
			
			return format_to(ctx.out(), "ARRAY");
		case OT_TABLE:
			return format_to(ctx.out(), "TABLE");
		case OT_ENTITY:
			return format_to(ctx.out(), "ENTITY");
		case OT_CLAAS:
			return format_to(ctx.out(), "CLASS");
		case OT_CLOSURE:
			return format_to(
				ctx.out(),
				"CLOSURE(\"{}\")",
				std::string(
					input._VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->_val,
					input._VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->length));
		case OT_NULL:
			return format_to(ctx.out(), "NULL");
		case OT_STRUCT_DEF:
			if (input._VAL.asStructDef->_nameType==OT_STRING)
				return format_to(ctx.out(), "STRUCT_DEF({})",input._VAL.asStructDef->_name->_val);
			return format_to(ctx.out(), "STRUCT_DEF");
		case OT_STRUCT_INSTANCE:
			return format_to(ctx.out(), "STRUCT_INSTANCE");
		case OT_INSTANCE:
			return format_to(ctx.out(), "CLASS_INSTANCE");
		case OT_USERDATA:
			return format_to(ctx.out(), "USERDATA");
		default:
			return format_to(ctx.out(), "NEED_TO_IMPLEMENT({})", std::string(sq_getTypeName(input._Type)));
		}
	}
};

template <> struct fmt::formatter<SQLocalVarInfo>
{
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
	{
		return ctx.end();
	}

	template <typename FormatContext> auto format(const SQLocalVarInfo& input, FormatContext& ctx) -> decltype(ctx.out())
	{

		if (input.name._Type == OT_STRING)
			return format_to(ctx.out(), "Var \"{}\" startOP {} endOP {} stackpos {} qword10 {:X} dword24 {:X}", std::string(input.name._VAL.asString->_val),input._start_op,input._end_op,input.stackpos,input.qword10,input.dword24);
		else
			return format_to(
				ctx.out(),
				"Var \"UNKNOWN\" startOP {} endOP {} stackpos {} qword10 {:X} dword24 {:X}",
				input._start_op,
				input._end_op,
				input.stackpos,
				input.qword10,
				input.dword24);
	
	}
};

