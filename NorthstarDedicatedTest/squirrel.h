#pragma once

void InitialiseClientSquirrel(HMODULE baseAddress);
void InitialiseServerSquirrel(HMODULE baseAddress);

// stolen from ttf2sdk: sqvm types
typedef float SQFloat;
typedef long SQInteger;
typedef unsigned long SQUnsignedInteger;
typedef char SQChar;


//ida definitions
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef unsigned int uint;
#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64
#define SLOBYTE(x)   (*((int8*)&(x)))
#define HIDWORD(x)  (*((_DWORD*)&(x)+1))
#define LODWORD(x)  (*((_DWORD*)&(x)))

typedef SQUnsignedInteger SQBool;
typedef SQInteger SQRESULT;

const SQRESULT SQRESULT_ERROR = -1;
const SQRESULT SQRESULT_NULL = 0;
const SQRESULT SQRESULT_NOTNULL = 1;

typedef SQInteger (*SQFunction)(void* sqvm);

float fsqrt(float x);

struct CompileBufferState
{
	const SQChar* buffer;
	const SQChar* bufferPlusLength;
	const SQChar* bufferAgain;

	CompileBufferState(const std::string& code)
	{
		buffer = code.c_str();
		bufferPlusLength = code.c_str() + code.size();
		bufferAgain = code.c_str();
	}
};


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
struct SQInstruction;
struct HSquirrelVM;

/* 127 */
enum SQObjectType : __int32
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
	OT_STRUCT = 0x8200000,
	OT_WEAKREF = 0x8010000,
	OT_TABLE = 0xA000020,
	OT_USERDATA = 0xA000080,
	OT_INSTANCE = 0xA008000,
	OT_ENTITY = 0xA400000,
};

/* 156 */
union __declspec(align(8)) SQObjectValue
{
	SQString* asString;
	SQTable* asTable;
	SQClosure* asClosure;
	SQFunctionProto* asFuncProto;
	SQStructDef* asStructDef;
	__int64 asInteger;
	SQStructInstance* asStructInstance;
	float asFloat;
	SQNativeClosure* asNativeClosure;
	SQArray* asArray;

};

/* 128 */
struct __declspec(align(8)) SQObject
{
	SQObjectType _Type;
	__int32 _structOffset;
	SQObjectValue _VAL;
};

/* 138 */
struct __declspec(align(8)) SQString
{
	__int64* vftable;
	__int32 uiRef;
	__int32 uiRef1;
	SQString* _next_maybe;
	SQSharedState* sharedState;
	__int32 length;
	uint8 gap_24[4];
	char _hash[8];
	char _val[1];
};

/* 137 */
struct __declspec(align(8)) SQTable
{
	__int64* vftable;
	uint8 gap_08[4];
	__int32 uiRef;
	uint8 gap_10[8];
	void* pointer_18;
	void* pointer_20;
	void* _sharedState;
	__int64 field_30;
	void* pointer_38;
	__int32 _numOfNodes;
	__int32 size;
	__int32 field_48;
	__int32 _usedNodes;
	uint8 _gap_50[20];
	__int32 field_64;
	uint8 _gap_68[80];
};

/* 140 */
struct __declspec(align(8)) SQClosure
{
	void* vftable;
	uint8 gap_08[4];
	__int32 uiRef;
	void* pointer_10;
	void* pointer_18;
	void* pointer_20;
	void* sharedState;
	SQObject obj_30;
	SQObject _function;
	SQObject* _outervalues;
	uint8 gap_58[8];
	uint8 gap_60[96];
	SQObject* objectPointer_C0;
	uint8 gap_C8[16];
};

/* 139 */
struct __declspec(align(8)) SQFunctionProto
{
	void* vftable;
	uint8 gap_08[4];
	__int32 uiRef;
	uint8 gap_10[8];
	void* pointer_18;
	void* pointer_20;
	void* sharedState;
	void* pointer_30;
	SQObject fileName;
	SQObject funcName;
	SQObject obj_58;
	uint8 gap_68[64];
	__int32 nParameters;
	uint8 gap_AC[60];
	__int32 nDefaultParams;
	uint8 gap_EC[200];
};

/* 152 */
struct SQStructDef
{
	uint8 gap_0[56];
	SQString* name;
	uint8 gap_[300];
};

/* 150 */
struct SQStructInstance
{
	void* vftable;
	uint8 gap_8[16];
	void* pointer_18;
	uint8 gap_20[8];
	SQSharedState* _sharedState;
	uint8 gap_30[8];
	SQObject data[1];
};

/* 157 */
struct __declspec(align(8)) SQNativeClosure
{
	void* vftable;
	uint8 gap_08[4];
	__int32 uiRef;
	uint8 gap_10[88];
	SQString* _name;
	uint8 gap_0[300];
};

/* 148 */
struct SQSharedState
{
	_BYTE gap_0[72];
	void* unknown;
	_BYTE gap_50[16376];
	StringTable* _stringTable;
	_BYTE gap_4050[88];
	SQObjectType _gobalsStructType;
	SQStructInstance* _globalsStruct;
	_BYTE gap_40B8[16];
	SQObjectType _nativeClosuresType;
	SQTable* _nativeClosures;
	SQObjectType _typedConstantsType;
	SQTable* _typedConstants;
	SQObjectType _untypedConstantsType;
	SQTable* _untypedConstants;
	SQObjectType _globalsMaybeType;
	SQTable* _globals;
	SQObjectType _functionsType;
	SQTable* _functions;
	SQObjectType _structsType;
	SQTable* _structs;
	SQObjectType _typeDefsType;
	SQTable* _typeDefs;
	SQObjectType unknownTableType;
	SQTable* unknownTable;
	SQObjectType _squirrelFilesType;
	SQTable* _squirrelFiles;
	_BYTE gap_4158[80];
	SQObjectType _nativeClosures2Type;
	SQTable* _nativeClosures2;
	SQObjectType _entityTypesMaybeType;
	SQTable* _entityTypesMaybe;
	SQObjectType unknownTable2Type;
	SQTable* unknownTable2;
	_BYTE gap_41D8[72];
	SQObjectType _compilerKeywordsType;
	SQTable* _compilerKeywords;
	HSquirrelVM* _currentThreadMaybe;
	_BYTE gap_4238[8];
	SQObjectType unknownTable3Type;
	SQTable* unknownTable3;
	_BYTE gap_4250[16];
	SQObjectType unknownThreadType;
	SQTable* unknownThread;
	SQObjectType _tableNativeFunctionsType;
	SQTable* _tableNativeFunctions;
	_BYTE gap_4280[232];
	void* logEntityFunction;
	_BYTE gap_4370[15000];
};


/* 149 */
struct StringTable
{
	uint8 gap_0[12];
	int _numofslots;
	uint8 gap_10[200];
};

/* 129 */
struct __declspec(align(8)) HSquirrelVM
{
	void* vftable;
	__int32 uiRef;
	uint8 gap_8[12];
	void* _toString;
	void* _roottable_pointer;
	void* pointer_28;
	CallInfo* ci;
	CallInfo* _callsstack;
	__int32 _callsstacksize;
	__int32 _stackbase;
	SQObject* _stackOfCurrentFunction;
	SQSharedState* sharedState;
	void* pointer_58;
	void* pointer_60;
	__int32 _top;
	SQObject* _stack;
	uint8 gap_78[8];
	SQObject* _vargsstack;
	uint8 gap_88[8];
	SQObject temp_reg;
	uint8 gapA0[8];
	void* pointer_A8;
	uint8 gap_B0[8];
	SQObject _roottable_object;
	SQObject _lasterror;
	SQObject _errorHandler;
	__int64 field_E8;
	__int32 traps;
	uint8 gap_F4[12];
	__int32 _nnativecalls;
	__int32 _suspended;
	__int32 _suspended_root;
	__int32 _callstacksize;
	__int32 _suspended_target;
	__int32 trapAmount;
	__int32 _suspend_varargs;
	SQObject* _object_pointer_120;
};

/* 136 */
struct __declspec(align(8)) CallInfo
{
	SQInstruction* ip;
	SQObject* _literals;
	SQObject obj10;
	SQObject closure;
	__int32 _etraps[4];
	__int32 _root;
	short _vargs_size;
	short _vargs_base;
};

/* 135 */
enum SQOpcode : int
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

/* 141 */
struct __declspec(align(8)) SQStackInfos
{
	char* _name;
	char* _sourceName;
	__int32 _line;
};

/* 151 */
struct __declspec(align(4)) SQInstruction
{
	int op;
	int arg1;
	int output;
	__int16 arg2;
	__int16 arg3;
};

/* 154 */
struct SQLexer
{
	uint8 gap_0[112];
};

/* 153 */
struct SQCompiler
{
	uint8 gap_0[4];
	__int32 _token;
	uint8 gap_8[8];
	SQObject object_10;
	SQLexer lexer;
	uint8 gap_1[768];
};

/* 155 */
struct CSquirrelVM
{
	uint8 gap_0[8];
	HSquirrelVM* sqvm;
};



struct SQVector
{
	SQObjectType _Type;
	float x;
	float y;
	float z;
};


struct SQArray
{
	void* vftable;
	__int32 uiRef;
	uint8 gap_24[36];
	SQObject* _values;
	__int32 _field_38;
	__int32 _allocated;
};


#define INCREMENT_REFERENCECOUNT(val) if((val->_Type & SQOBJECT_REF_COUNTED) != 0) ++val->_VAL.asString->uiRef;


#define DECREMENT_REFERENCECOUNT(val) if((val->_Type & SQOBJECT_REF_COUNTED) != 0){ \
	if (val->_VAL.asString->uiRef-- == 1) {\
        spdlog::info("Deleted SQObject of type {} with address {:X}", sq_getTypeName(val->_Type),val->_VAL.asInteger);\
		(*(void(__fastcall**)(SQString*))(&val->_VAL.asString->vftable[1]))(val->_VAL.asString);} \
	}










struct SQFuncRegistration
{
	const char* squirrelFuncName;
	const char* cppFuncName;
	const char* helpText;
	const char* returnValueType;
	const char* argTypes;
	int16_t somethingThatsZero;
	int16_t padding1;
	int32_t unknown1;
	int64_t unknown2;
	int32_t unknown3;
	int32_t padding2;
	int64_t unknown4;
	int64_t unknown5;
	int64_t unknown6;
	int32_t unknown7;
	int32_t padding3;
	void* funcPtr;

	SQFuncRegistration()
	{
		memset(this, 0, sizeof(SQFuncRegistration));
		this->padding2 = 32;
	}
};

// core sqvm funcs
typedef SQRESULT (*sq_compilebufferType)(void* sqvm, CompileBufferState* compileBuffer, const char* file, int a1, ScriptContext a2);
extern sq_compilebufferType ClientSq_compilebuffer;
extern sq_compilebufferType ServerSq_compilebuffer;

typedef void (*sq_pushroottableType)(void* sqvm);
extern sq_pushroottableType ClientSq_pushroottable;
extern sq_pushroottableType ServerSq_pushroottable;

typedef SQRESULT (*sq_callType)(void* sqvm, SQInteger s1, SQBool a2, SQBool a3);
extern sq_callType ClientSq_call;
extern sq_callType ServerSq_call;

typedef int64_t (*RegisterSquirrelFuncType)(void* sqvm, SQFuncRegistration* funcReg, char unknown);
extern RegisterSquirrelFuncType ClientRegisterSquirrelFunc;
extern RegisterSquirrelFuncType ServerRegisterSquirrelFunc;

// sq stack array funcs
typedef void (*sq_newarrayType)(void* sqvm, SQInteger stackpos);
extern sq_newarrayType ClientSq_newarray;
extern sq_newarrayType ServerSq_newarray;

typedef SQRESULT (*sq_arrayappendType)(void* sqvm, SQInteger stackpos);
extern sq_arrayappendType ClientSq_arrayappend;
extern sq_arrayappendType ServerSq_arrayappend;

// sq stack push funcs
typedef void (*sq_pushstringType)(void* sqvm, const SQChar* str, SQInteger stackpos);
extern sq_pushstringType ClientSq_pushstring;
extern sq_pushstringType ServerSq_pushstring;

// weird how these don't take a stackpos arg?
typedef void (*sq_pushintegerType)(void* sqvm, SQInteger i);
extern sq_pushintegerType ClientSq_pushinteger;
extern sq_pushintegerType ServerSq_pushinteger;

typedef void (*sq_pushfloatType)(void* sqvm, SQFloat f);
extern sq_pushfloatType ClientSq_pushfloat;
extern sq_pushfloatType ServerSq_pushfloat;

typedef void (*sq_pushboolType)(void* sqvm, SQBool b);
extern sq_pushboolType ClientSq_pushbool;
extern sq_pushboolType ServerSq_pushbool;

typedef SQInteger (*sq_pusherrorType)(void* sqvm, const SQChar* error);
extern sq_pusherrorType ClientSq_pusherror;
extern sq_pusherrorType ServerSq_pusherror;

// sq stack get funcs
typedef const SQChar* (*sq_getstringType)(void* sqvm, SQInteger stackpos);
extern sq_getstringType ClientSq_getstring;
extern sq_getstringType ServerSq_getstring;

typedef SQInteger (*sq_getintegerType)(void* sqvm, SQInteger stackpos);
extern sq_getintegerType ClientSq_getinteger;
extern sq_getintegerType ServerSq_getinteger;

typedef SQFloat (*sq_getfloatType)(void*, SQInteger stackpos);
extern sq_getfloatType ClientSq_getfloat;
extern sq_getfloatType ServerSq_getfloat;

typedef SQBool (*sq_getboolType)(void*, SQInteger stackpos);
extern sq_getboolType ClientSq_getbool;
extern sq_getboolType ServerSq_getbool;

typedef SQRESULT (*sq_getType)(void* sqvm, SQInteger idx);
extern sq_getType ServerSq_sq_get;
extern sq_getType ClientSq_sq_get;


//sqvmExecute functions
typedef char (*SQVMStartCallType)(HSquirrelVM* a1, SQClosure* a2, int a3, __int64 nInputParams, unsigned int a5, char a6);
extern SQVMStartCallType SQVMStartCall;
typedef long long (*SQVMCallErrorHandlerType)(HSquirrelVM* sqvm, SQObject* a2);
extern SQVMCallErrorHandlerType SQVMCallErrorHandler;
typedef void (*sq_op_loadcopyType)(SQSharedState* a1, SQObject* a2, SQObject* a3);
extern sq_op_loadcopyType sq_op_loadcopy;
typedef bool (*sq_op_callType)(HSquirrelVM* a1, SQInstruction* a2, SQObject* a3, int* a4, ...);
extern sq_op_callType sq_op_call;
typedef long long (*sq_op_precallType)(HSquirrelVM* a1, SQInstruction* a2, SQObject* key);
extern sq_op_precallType sq_op_precall;
typedef char (*SQVMGetType)(HSquirrelVM* self, SQObject* key, SQObject* dest, SQObject* temp_reg, char a5, char a6);
extern SQVMGetType SQVMGet;
typedef int (*sub_35B80Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3);
extern sub_35B80Type sub_35B80;
typedef char (*SQVMNewSlotType)(HSquirrelVM* sqvm, SQObject* self, SQObject* key, SQObject* val, char bstatic);
extern SQVMNewSlotType SQVMNewSlot;
typedef char (*sub_34CF0Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4);
extern sub_34CF0Type sub_34CF0;
typedef char (*sq_op_setType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4, char a5);
extern sq_op_setType sq_op_set;
typedef bool (*sq_op_eqType)(__int64 a1, SQObject* a2, SQObject* a3);
extern sq_op_eqType sq_op_eq;
typedef char (*sub_2B590Type)(HSquirrelVM* a1, int a2, SQObject* a3, SQObject* a4, SQObject* a5);
extern sub_2B590Type sub_2B590;
typedef long long (*sq_sqvm_ReturnType)(HSquirrelVM* a1, int _arg0, int _arg1, SQObject* retval);
extern sq_sqvm_ReturnType sq_sqvm_Return;
typedef SQObject* (*sq_copyObjectType)(SQObject* dest, SQObject* src);
extern sq_copyObjectType sq_copyObject;
typedef SQTable* (*sq_createTableType)(SQSharedState* sharedState, unsigned int length);
extern sq_createTableType sq_createTable;
typedef SQArray* (*sub_1800Type)(SQSharedState* a1, unsigned int a2);
extern sub_1800Type sub_1800;
typedef long long (*sub_1A20Type)(SQArray* a1, SQObject* a2);
extern sub_1A20Type sub_1A20;
typedef char (*sub_2F740Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3);
extern sub_2F740Type sub_2F740;
typedef long long (*sub_2E670Type)(HSquirrelVM* a1, unsigned int a2, SQObject* a3, SQObject* a4, SQObject* a5);
extern sub_2E670Type sub_2E670;
typedef bool (*sub_2B6C0Type)(HSquirrelVM* a1, __int64 a2, SQObject* a3, SQObject* a4, SQObject* a5);
extern sub_2B6C0Type sub_2B6C0;
typedef char (*sub_2E960Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4, int a5);
extern sub_2E960Type sub_2E960;
typedef char (*sq_op_increment_prefix_localType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, int a4);
extern sq_op_increment_prefix_localType sq_op_increment_prefix_local;
typedef char (*sp_op_increment_prefix_structfieldType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, int a4, int a5);
extern sp_op_increment_prefix_structfieldType sp_op_increment_prefix_structfield;
typedef char (*sub_2EA30Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4, int a5);
extern sub_2EA30Type sub_2EA30;
typedef char (*sub_2E860Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, int a4);
extern sub_2E860Type sub_2E860;
typedef char (*sq_op_increment_postfix_structfieldType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, int a4, int a5);
extern sq_op_increment_postfix_structfieldType sq_op_increment_postfix_structfield;
typedef char (*sub_2D1E0Type)(HSquirrelVM* a1, int a2, SQObject* a3, SQObject* a4, SQObject* dest);
extern sub_2D1E0Type sub_2D1E0;
typedef char (*sub_2CF10Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3);
extern sub_2CF10Type sub_2CF10;
typedef char (*sub_2F0B0Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3);
extern sub_2F0B0Type sub_2F0B0;
typedef char (*sub_2EE20Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4, SQObject* a5, int a6, int a7, int a8, SQInstruction** a9);
extern sub_2EE20Type sub_2EE20;
typedef char (*sub_2EF60Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4);
extern sub_2EF60Type sub_2EF60;
typedef char (*sub_34A30Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3);
extern sub_34A30Type sub_34A30;
typedef long long (*sub_2D950Type)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, __int64 a4);
extern sub_2D950Type sub_2D950;
typedef long long (*sub_35800Type)(__int64* a1, int a2);
extern sub_35800Type sub_35800;
typedef long long (*sub_2F2F0Type)(HSquirrelVM* a1, SQObject* a2, int a3, int a4);
extern sub_2F2F0Type sub_2F2F0;
typedef long long (*sub_3EA70Type)(void* a1, SQObject* a2, SQObject* a3);
extern sub_3EA70Type sub_3EA70;
typedef SQObject* (*SQVMToStringType)(HSquirrelVM* a1, SQObject* object, SQObject* res);
extern SQVMToStringType SQVMToString;
typedef char (*sq_op_addType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4);
extern sq_op_addType sq_op_add;
typedef long long (*sq_dropStackType)(HSquirrelVM* a1, int a2);
extern sq_dropStackType sq_dropStack;
typedef long long (*sub_C6F0Type)(SQObject** a1, int a2);
extern sub_C6F0Type sub_C6F0;
typedef long long (*sub_27390Type)(SQObject* a1, int a2);
extern sub_27390Type sub_27390;
typedef SQStructInstance* (*sub_35B20Type)(HSquirrelVM* a1, SQObject* a2);
extern sub_35B20Type sub_35B20;
typedef char (*sq_op_subType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4);
extern sq_op_subType sq_op_sub;
typedef char (*sq_op_multiplicationType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4);
extern sq_op_multiplicationType sq_op_multiplication;
typedef char (*sq_op_divType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4);
extern sq_op_divType sq_op_div;
typedef char (*sq_op_moduloType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, SQObject* a4);
extern sq_op_moduloType sq_op_modulo;
typedef char (*sq_compareType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, uint32* a4);
extern sq_compareType sq_compare;
typedef SQObject* (*sq_setObjectToFloatType)(SQObject* a1, float a2);
extern sq_setObjectToFloatType sq_setObjectToFloat;
typedef char (*sub_2E100Type)(HSquirrelVM* sqvm, __int64 a2, int a3, int a4, unsigned int a5);
extern sub_2E100Type sub_2E100;
typedef char (*sq_nativecallType)(HSquirrelVM* sqvm, SQNativeClosure* a2, __int64 a3, unsigned int a4, SQObject* a5, uint32* a6, char a7);
extern sq_nativecallType sq_nativecall;
typedef char (*sq_op_typecastType)(HSquirrelVM* a1, SQObject* a2, SQObject* a3, int a4);
extern sq_op_typecastType sq_op_typecast;
typedef char (*sq_op_check_entity_classType)(HSquirrelVM* a1, SQObject* a2, __int64 a3);
extern sq_op_check_entity_classType sq_op_check_entity_class;
typedef long long (*sub_29A40Type)(SQSharedState* sharedState, SQObject* a2, int* a3);
extern sub_29A40Type sub_29A40;
typedef long long (*sub_63E00Type)(SQSharedState* a1, unsigned int a2, uint32* a3);
extern sub_63E00Type sub_63E00;
typedef void (*sub_2EDB0Type)(HSquirrelVM* a1, __int16* a2);
extern sub_2EDB0Type sub_2EDB0;
typedef SQString* (*sqStringTableAddType)(StringTable* a1, char* newString, int length);
extern sqStringTableAddType sqStringTableAdd;
typedef long long (*sub_BEF0Type)(SQObject* a1, unsigned int a2, SQObject* a3);
extern sub_BEF0Type sub_BEF0;
typedef long long (*SQVMRaise_ErrorType)(HSquirrelVM* a1, const char* a2, ...);
extern SQVMRaise_ErrorType SQVMRaise_Error;











template <ScriptContext context> class SquirrelManager
{
  private:
	std::vector<SQFuncRegistration*> m_funcRegistrations;
  public:
	void* sqvm;
	void* sqvm2;
	SQObject* globalClosure;

  public:
	SquirrelManager() : sqvm(nullptr) {}

	void VMCreated(void* newSqvm)
	{
		sqvm = newSqvm;
		sqvm2 = *(void**)((char*)newSqvm +8); // honestly not 100% sure on what this is, but alot of functions take it

		for (SQFuncRegistration* funcReg : m_funcRegistrations)
		{
			spdlog::info("Registering {} function {}", GetContextName(context), funcReg->squirrelFuncName);

			if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
				ClientRegisterSquirrelFunc(sqvm, funcReg, 1);
			else
				ServerRegisterSquirrelFunc(sqvm, funcReg, 1);
		}
	}

	/*void setGlobalClosure(SQObject* closure)
	{
		globalClosure = closure;
	}*/
	
	void VMDestroyed()
	{
		sqvm = nullptr;
	}

	void ExecuteCode(const char* code)
	{
		// ttf2sdk checks ThreadIsInMainThread here, might be good to do that? doesn't seem like an issue rn tho

		if (!sqvm)
		{
			spdlog::error("Cannot execute code, {} squirrel vm is not initialised", GetContextName(context));
			return;
		}

		spdlog::info("Executing {} script code {} ", GetContextName(context), code);

		std::string strCode(code);
		CompileBufferState bufferState = CompileBufferState(strCode);

		SQRESULT compileResult;
		if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
			compileResult = ClientSq_compilebuffer(sqvm2, &bufferState, "console", -1, context);
		else if (context == ScriptContext::SERVER)
			compileResult = ServerSq_compilebuffer(sqvm2, &bufferState, "console", -1, context);

		spdlog::info("sq_compilebuffer returned {}", compileResult);
		if (compileResult >= 0)
		{
			if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
			{
				ClientSq_pushroottable(sqvm2);
				SQRESULT callResult = ClientSq_call(sqvm2, 1, false, false);
				spdlog::info("sq_call returned {}", callResult);
			}
			else if (context == ScriptContext::SERVER)
			{
				ServerSq_pushroottable(sqvm2);
				SQRESULT callResult = ServerSq_call(sqvm2, 1, false, false);
				spdlog::info("sq_call returned {}", callResult);
			}
		}
	}

	int setupfunc(const char* funcname)
	{
		int result = -2;
		if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
		{
			ClientSq_pushroottable(sqvm2);
			ClientSq_pushstring(sqvm2, funcname, -1);
			result = ClientSq_sq_get(sqvm2, -2);
			if (result != SQRESULT_ERROR)
			{
				ClientSq_pushroottable(sqvm2);
			}
		}
		else if (context == ScriptContext::SERVER)
		{
			ServerSq_pushroottable(sqvm2);
			ServerSq_pushstring(sqvm2, funcname, -1);
			result = ServerSq_sq_get(sqvm2, -2);
			if (result != SQRESULT_ERROR)
			{
				ServerSq_pushroottable(sqvm2);
			}
		}
		return result;
	}

	void pusharg(int arg)
	{
		if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
			ClientSq_pushinteger(sqvm2, arg);
		else if (context == ScriptContext::SERVER)
			ServerSq_pushinteger(sqvm2, arg);
	}
	void pusharg(const char* arg)
	{
		if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
			ClientSq_pushstring(sqvm2, arg, -1);
		else if (context == ScriptContext::SERVER)
			ServerSq_pushstring(sqvm2, arg, -1);
	}
	void pusharg(float arg)
	{
		if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
			ClientSq_pushfloat(sqvm2, arg);
		else if (context == ScriptContext::SERVER)
			ServerSq_pushfloat(sqvm2, arg);
	}
	void pusharg(bool arg)
	{
		if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
			ClientSq_pushbool(sqvm2, arg);
		else if (context == ScriptContext::SERVER)
			ServerSq_pushbool(sqvm2, arg);
	}

	int call(int args)
	{
		int result = -2;
		if (context == ScriptContext::CLIENT || context == ScriptContext::UI)
			result = ClientSq_call(sqvm2, args + 1, false, false);
		else if (context == ScriptContext::SERVER)
			result = ServerSq_call(sqvm2, args + 1, false, false);

		return result;
	}

	void AddFuncRegistration(std::string returnType, std::string name, std::string argTypes, std::string helpText, SQFunction func)
	{
		SQFuncRegistration* reg = new SQFuncRegistration;

		reg->squirrelFuncName = new char[name.size() + 1];
		strcpy((char*)reg->squirrelFuncName, name.c_str());
		reg->cppFuncName = reg->squirrelFuncName;

		reg->helpText = new char[helpText.size() + 1];
		strcpy((char*)reg->helpText, helpText.c_str());

		reg->returnValueType = new char[returnType.size() + 1];
		strcpy((char*)reg->returnValueType, returnType.c_str());

		reg->argTypes = new char[argTypes.size() + 1];
		strcpy((char*)reg->argTypes, argTypes.c_str());

		reg->funcPtr = func;

		m_funcRegistrations.push_back(reg);
	}
};







extern SquirrelManager<ScriptContext::CLIENT>* g_ClientSquirrelManager;
extern SquirrelManager<ScriptContext::SERVER>* g_ServerSquirrelManager;
extern SquirrelManager<ScriptContext::UI>* g_UISquirrelManager;
