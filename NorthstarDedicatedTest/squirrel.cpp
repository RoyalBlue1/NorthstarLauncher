#include "pch.h"
#include "squirrel.h"
#include "hooks.h"
#include "hookutils.h"
#include "sigscanning.h"
#include "concommand.h"
#include "modmanager.h"
#include <iostream>

// hook forward declarations
typedef SQInteger (*SQPrintType)(void* sqvm, char* fmt, ...);
SQPrintType ClientSQPrint;
SQPrintType UISQPrint;
SQPrintType ServerSQPrint;
template <ScriptContext context> SQInteger SQPrintHook(void* sqvm, char* fmt, ...);

typedef void* (*CreateNewVMType)(void* a1, ScriptContext contextArg);
CreateNewVMType ClientCreateNewVM; // only need a client one since ui doesn't have its own func for this
CreateNewVMType ServerCreateNewVM;
template <ScriptContext context> void* CreateNewVMHook(void* a1, ScriptContext contextArg);

typedef void (*DestroyVMType)(void* a1, void* sqvm);
DestroyVMType ClientDestroyVM; // only need a client one since ui doesn't have its own func for this
DestroyVMType ServerDestroyVM;
template <ScriptContext context> void DestroyVMHook(void* a1, void* sqvm);

typedef void (*ScriptCompileError)(void* sqvm, const char* error, const char* file, int line, int column);
ScriptCompileError ClientSQCompileError; // only need a client one since ui doesn't have its own func for this
ScriptCompileError ServerSQCompileError;
template <ScriptContext context> void ScriptCompileErrorHook(void* sqvm, const char* error, const char* file, int line, int column);

typedef char (*CallScriptInitCallbackType)(void* sqvm, const char* callback);
CallScriptInitCallbackType ClientCallScriptInitCallback;
CallScriptInitCallbackType ServerCallScriptInitCallback;
template <ScriptContext context> char CallScriptInitCallbackHook(void* sqvm, const char* callback);


typedef long long (*SQVMExecuteType)(HSquirrelVM* sqvm, SQObject* closure, int paramAmount, int stackbase, SQObject* outres, int throwerror, unsigned int calltype);
SQVMExecuteType ServerSQVMExecute;
template <ScriptContext context> long long SQVMExecuteHook(HSquirrelVM* sqvm, SQObject* closure, int paramAmount, int stackbase, SQObject* outres, int throwerror, unsigned int calltype);
template <ScriptContext context> long long SQVMExecuteHook2(HSquirrelVM* sqvm, SQObject* closure, int paramAmount, int stackbase, SQObject* outres, int throwerror, unsigned int calltype);

//typedef char (*sub_2E100Type)(HSquirrelVM* sqvm, __int64 a2, int a3, int a4, unsigned int a5);
//sub_2E100Type Serversub_2E100;
//template <ScriptContext context> long long sub_2E100Hook(HSquirrelVM* sqvm,__int64 a2, int a3, int a4, unsigned int a5);


// core sqvm funcs
sq_compilebufferType ClientSq_compilebuffer;
sq_compilebufferType ServerSq_compilebuffer;

sq_pushroottableType ClientSq_pushroottable;
sq_pushroottableType ServerSq_pushroottable;

sq_callType ClientSq_call;
sq_callType ServerSq_call;

RegisterSquirrelFuncType ClientRegisterSquirrelFunc;
RegisterSquirrelFuncType ServerRegisterSquirrelFunc;

// sq stack array funcs
sq_newarrayType ClientSq_newarray;
sq_newarrayType ServerSq_newarray;

sq_arrayappendType ClientSq_arrayappend;
sq_arrayappendType ServerSq_arrayappend;

// sq stack push funcs
sq_pushstringType ClientSq_pushstring;
sq_pushstringType ServerSq_pushstring;

sq_pushintegerType ClientSq_pushinteger;
sq_pushintegerType ServerSq_pushinteger;

sq_pushfloatType ClientSq_pushfloat;
sq_pushfloatType ServerSq_pushfloat;

sq_pushboolType ClientSq_pushbool;
sq_pushboolType ServerSq_pushbool;

sq_pusherrorType ClientSq_pusherror;
sq_pusherrorType ServerSq_pusherror;

// sq stack get funcs
sq_getstringType ClientSq_getstring;
sq_getstringType ServerSq_getstring;

sq_getintegerType ClientSq_getinteger;
sq_getintegerType ServerSq_getinteger;

sq_getfloatType ClientSq_getfloat;
sq_getfloatType ServerSq_getfloat;

sq_getboolType ClientSq_getbool;
sq_getboolType ServerSq_getbool;

sq_getType ClientSq_sq_get;
sq_getType ServerSq_sq_get;

const char* sq_OpToString(int op) {
    switch (op) {
    case _OP_LINE: return "_OP_LINE"; //probably stripped
    case _OP_LOAD: return "_OP_LOAD";
    case _OP_LOADCOPY: return "_OP_LOADCOPY";
    case _OP_LOADINT: return "_OP_LOADINT";
    case _OP_LOADFLOAT: return "_OP_LOADFLOAT";
    case _OP_DLOAD: return "_OP_DLOAD";
    case _OP_TAILCALL: return "_OP_TAILCALL";
    case _OP_CALL: return "_OP_CALL";
    case _OP_PREPCALL: return "_OP_PREPCALL";
    case _OP_PREPCALLK: return "_OP_PREPCALLK";
    case _OP_GETK: return "_OP_GETK";
    case _OP_MOVE: return "_OP_MOVE";
    case _OP_NEWSLOT: return "_OP_NEWSLOT";
    case _OP_DELETE: return "_OP_DELETE";
    case _OP_SET: return "_OP_SET";
    case _OP_GET: return "_OP_GET";
    case _OP_EQ: return "_OP_EQ";
    case _OP_NE: return "_OP_NE";
    case _OP_ARITH: return "_OP_ARITH";
    case _OP_BITW: return "_OP_BITW";
    case _OP_RETURN: return "_OP_RETURN";
    case _OP_LOADNULLS: return "_OP_LOADNULLS";
    case _OP_LOADROOTTABLE: return "_OP_LOADROOTTABLE";
    case _OP_LOADBOOL: return "_OP_LOADBOOL";
    case _OP_DMOVE: return "_OP_DMOVE";
    case _OP_JMP: return "_OP_JMP";
    case _OP_JNZ: return "_OP_JNZ";
    case _OP_JZ: return "_OP_JZ";
    case _OP_LOADFREEVAR: return "_OP_LOADFREEVAR";
    case _OP_VARGC: return "_OP_VARGC";
    case _OP_GETVARGV: return "_OP_GETVARGV";
    case _OP_NEWTABLE: return "_OP_NEWTABLE";
    case _OP_NEWARRAY: return "_OP_NEWARRAY";
    case _OP_APPENDARRAY: return "_OP_APPENDARRAY";
    case _OP_GETPARENT: return "_OP_GETPARENT";
    case _OP_COMPOUND_ARITH: return "_OP_COMPOUND_ARITH";
    case _OP_COMPOUND_ARITH_LOCAL: return "_OP_COMPOUND_ARITH_LOCAL";
    case _OP_INCREMENT_PREFIX: return "_OP_INCREMENT_PREFIX";
    case _OP_INCREMENT_PREFIX_LOCAL: return "_OP_INCREMENT_PREFIX_LOCAL";
    case _OP_INCREMENT_PREFIX_STRUCTFIELD: return "_OP_INCREMENT_PREFIX_STRUCTFIELD";
    case _OP_INCREMENT_POSTFIX: return "_OP_INCREMENT_POSTFIX";
    case _OP_INCREMENT_POSTFIX_LOCAL: return "_OP_INCREMENT_POSTFIX_LOCAL";
    case _OP_INCREMENT_POSTFIX_STRUCTFIELD: return "_OP_INCREMENT_POSTFIX_STRUCTFIELD";
    case _OP_CMP: return "_OP_CMP";
    case _OP_EXISTS: return "_OP_EXISTS";
    case _OP_INSTANCEOF: return "_OP_INSTANCEOF";
    case _OP_NEG: return "_OP_NEG";
    case _OP_NOT: return "_OP_NOT";
    case _OP_BWNOT: return "_OP_BWNOT";
    case _OP_CLOSURE: return "_OP_CLOSURE";
    case _OP_FOREACH: return "_OP_FOREACH";
    case _OP_FOREACH_STATICARRAY_START: return "_OP_FOREACH_STATICARRAY_START";
    case _OP_FOREACH_STATICARRAY_NEXT: return "_OP_FOREACH_STATICARRAY_NEXT";
    case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_START: return "_OP_FOREACH_STATICARRAY_NESTEDSTRUCT_START";
    case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_NEXT: return "_OP_FOREACH_STATICARRAY_NESTEDSTRUCT_NEXT";
    case _OP_DELEGATE: return "_OP_DELEGATE";
    case _OP_CLONE: return "_OP_CLONE";
    case _OP_TYPEOF: return "_OP_TYPEOF";
    case _OP_PUSHTRAP: return "_OP_PUSHTRAP";
    case _OP_POPTRAP: return "_OP_POPTRAP";
    case _OP_THROW: return "_OP_THROW";
    case _OP_CLASS: return "_OP_CLASS";
    case _OP_NEWSLOTA: return "_OP_NEWSLOTA";
    case _OP_EQ_LITERAL: return "_OP_EQ_LITERAL";
    case _OP_NE_LITERAL: return "_OP_NE_LITERAL";
    case _OP_FOREACH_SETUP: return "_OP_FOREACH_SETUP";
    case _OP_ASSERT_FAILED: return "_OP_ASSERT_FAILED";
    case _OP_ADD: return "_OP_ADD";
    case _OP_SUB: return "_OP_SUB";
    case _OP_MUL: return "_OP_MUL";
    case _OP_DIV: return "_OP_DIV";
    case _OP_MOD: return "_OP_MOD";
    case _OP_PREPCALLK_CALL: return "_OP_PREPCALLK_CALL";
    case _OP_PREPCALLK_MOVE_CALL: return "_OP_PREPCALLK_MOVE_CALL";
    case _OP_PREPCALLK_LOADINT_CALL: return "_OP_PREPCALLK_LOADINT_CALL";
    case _OP_CMP_JZ: return "_OP_CMP_JZ";
    case _OP_INCREMENT_LOCAL_DISCARD_JMP: return "_OP_INCREMENT_LOCAL_DISCARD_JMP";
    case _OP_JZ_RETURN: return "_OP_JZ_RETURN";
    case _OP_JZ_LOADBOOL_RETURN: return "_OP_JZ_LOADBOOL_RETURN";
    case _OP_NEWVECTOR: return "_OP_NEWVECTOR";
    case _OP_ZEROVECTOR: return "_OP_ZEROVECTOR";
    case _OP_GET_VECTOR_COMPONENT: return "_OP_GET_VECTOR_COMPONENT";
    case _OP_SET_VECTOR_COMPONENT: return "_OP_SET_VECTOR_COMPONENT";
    case _OP_VECTOR_COMPONENT_MINUSEQ: return "_OP_VECTOR_COMPONENT_MINUSEQ";
    case _OP_VECTOR_COMPONENT_PLUSEQ: return "_OP_VECTOR_COMPONENT_PLUSEQ";
    case _OP_VECTOR_COMPONENT_MULEQ: return "_OP_VECTOR_COMPONENT_MULEQ";
    case _OP_VECTOR_COMPONENT_DIVEQ: return "_OP_VECTOR_COMPONENT_DIVEQ";
    case _OP_VECTOR_NORMALIZE: return "_OP_VECTOR_NORMALIZE";
    case _OP_VECTOR_NORMALIZE_IN_PLACE: return "_OP_VECTOR_NORMALIZE_IN_PLACE";
    case _OP_VECTOR_DOT_PRODUCT: return "_OP_VECTOR_DOT_PRODUCT";
    case _OP_VECTOR_DOT_PRODUCT2D: return "_OP_VECTOR_DOT_PRODUCT2D";
    case _OP_VECTOR_CROSS_PRODUCT: return "_OP_VECTOR_CROSS_PRODUCT";
    case _OP_VECTOR_CROSS_PRODUCT2D: return "_OP_VECTOR_CROSS_PRODUCT2D";
    case _OP_VECTOR_LENGTH: return "_OP_VECTOR_LENGTH";
    case _OP_VECTOR_LENGTHSQR: return "_OP_VECTOR_LENGTHSQR";
    case _OP_VECTOR_LENGTH2D: return "_OP_VECTOR_LENGTH2D";
    case _OP_VECTOR_LENGTH2DSQR: return "_OP_VECTOR_LENGTH2DSQR";
    case _OP_VECTOR_DISTANCE: return "_OP_VECTOR_DISTANCE";
    case _OP_VECTOR_DISTANCESQR: return "_OP_VECTOR_DISTANCESQR";
    case _OP_VECTOR_DISTANCE2D: return "_OP_VECTOR_DISTANCE2D";
    case _OP_VECTOR_DISTANCE2DSQR: return "_OP_VECTOR_DISTANCE2DSQR";
    case _OP_INCREMENT_LOCAL_DISCARD: return "_OP_INCREMENT_LOCAL_DISCARD";
    case _OP_FASTCALL: return "_OP_FASTCALL";
    case _OP_FASTCALL_NATIVE: return "_OP_FASTCALL_NATIVE";
    case _OP_FASTCALL_NATIVE_ARGTYPECHECK: return "_OP_FASTCALL_NATIVE_ARGTYPECHECK";
    case _OP_FASTCALL_ENV: return "_OP_FASTCALL_ENV";
    case _OP_FASTCALL_NATIVE_ENV: return "_OP_FASTCALL_NATIVE_ENV";
    case _OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK: return "_OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK";
    case _OP_LOADGLOBALARRAY: return "_OP_LOADGLOBALARRAY";
    case _OP_GETGLOBAL: return "_OP_GETGLOBAL";
    case _OP_SETGLOBAL: return "_OP_SETGLOBAL";
    case _OP_COMPOUND_ARITH_GLOBAL: return "_OP_COMPOUND_ARITH_GLOBAL";
    case _OP_GETSTRUCTFIELD: return "_OP_GETSTRUCTFIELD";
    case _OP_SETSTRUCTFIELD: return "_OP_SETSTRUCTFIELD";
    case _OP_COMPOUND_ARITH_STRUCTFIELD: return "_OP_COMPOUND_ARITH_STRUCTFIELD";
    case _OP_NEWSTRUCT: return "_OP_NEWSTRUCT";
    case _OP_GETSUBSTRUCT: return "_OP_GETSUBSTRUCT";
    case _OP_GETSUBSTRUCT_DYNAMIC: return "_OP_GETSUBSTRUCT_DYNAMIC";
    case _OP_TYPECAST: return "_OP_TYPECAST";
    case _OP_TYPECHECK: return "_OP_TYPECHECK";
    case _OP_TYPECHECK_ORNULL: return "_OP_TYPECHECK_ORNULL";
    case _OP_TYPECHECK_NOTNULL: return "_OP_TYPECHECK_NOTNULL";
    case _OP_CHECK_ENTITY_CLASS: return "_OP_CHECK_ENTITY_CLASS";
    case _OP_UNREACHABLE: return "_OP_UNREACHABLE";
    case _OP_ARRAY_RESIZE: return "_OP_ARRAY_RESIZE";
    default: return "UNKNOWN";
    }
}

//execute functions
SQVMStartCallType SQVMStartCall;
SQVMCallErrorHandlerType SQVMCallErrorHandler;
sq_op_loadcopyType sq_op_loadcopy;
sq_op_callType sq_op_call;
sq_op_precallType sq_op_precall;
SQVMGetType SQVMGet;
sub_35B80Type sub_35B80;
SQVMNewSlotType SQVMNewSlot;
sub_34CF0Type sub_34CF0;
sq_op_setType sq_op_set;
sq_op_eqType sq_op_eq;
sub_2B590Type sub_2B590;
sq_sqvm_ReturnType sq_sqvm_Return;
sq_copyObjectType sq_copyObject;
sq_createTableType sq_createTable;
sub_1800Type sub_1800;
sub_1A20Type sub_1A20;
sub_2F740Type sub_2F740;
sub_2E670Type sub_2E670;
sub_2B6C0Type sub_2B6C0;
sub_2E960Type sub_2E960;
sq_op_increment_prefix_localType sq_op_increment_prefix_local;
sp_op_increment_prefix_structfieldType sp_op_increment_prefix_structfield;
sub_2EA30Type sub_2EA30;
sub_2E860Type sub_2E860;
sq_op_increment_postfix_structfieldType sq_op_increment_postfix_structfield;
sub_2D1E0Type sub_2D1E0;
sub_2CF10Type sub_2CF10;
sub_2F0B0Type sub_2F0B0;
sub_2EE20Type sub_2EE20;
sub_2EF60Type sub_2EF60;
sub_34A30Type sub_34A30;
sub_2D950Type sub_2D950;
sub_35800Type sub_35800;
sub_2F2F0Type sub_2F2F0;
sub_3EA70Type sub_3EA70;
SQVMToStringType SQVMToString;
sq_op_addType sq_op_add;
sq_dropStackType sq_dropStack;
sub_C6F0Type sub_C6F0;
sub_27390Type sub_27390;
sub_35B20Type sub_35B20;
sq_op_subType sq_op_sub;
sq_op_multiplicationType sq_op_multiplication;
sq_op_divType sq_op_div;
sq_op_moduloType sq_op_modulo;
sq_compareType sq_compare;
sq_setObjectToFloatType sq_setObjectToFloat;
sub_2E100Type sub_2E100;
sq_nativecallType sq_nativecall;
sq_op_typecastType sq_op_typecast;
sq_op_check_entity_classType sq_op_check_entity_class;
sub_29A40Type sub_29A40;
sub_63E00Type sub_63E00;
sub_2EDB0Type sub_2EDB0;
sqStringTableAddType sqStringTableAdd;
sub_BEF0Type sub_BEF0;
SQVMRaise_ErrorType SQVMRaise_Error;







template <ScriptContext context> void ExecuteCodeCommand(const CCommand& args);


const char* sq_getTypeName(int type);
int sqObjectToString(char* buf, int bufLength, SQObject* obj);
// inits
SquirrelManager<ScriptContext::CLIENT>* g_ClientSquirrelManager;
SquirrelManager<ScriptContext::SERVER>* g_ServerSquirrelManager;
SquirrelManager<ScriptContext::UI>* g_UISquirrelManager;

SQInteger NSTestFunc(void* sqvm)
{
	return 1;
}

void InitialiseClientSquirrel(HMODULE baseAddress)
{
	HookEnabler hook;

	// client inits
	g_ClientSquirrelManager = new SquirrelManager<ScriptContext::CLIENT>();

	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x12B00,
		&SQPrintHook<ScriptContext::CLIENT>,
		reinterpret_cast<LPVOID*>(&ClientSQPrint)); // client print function
	RegisterConCommand(
		"script_client", ExecuteCodeCommand<ScriptContext::CLIENT>, "Executes script code on the client vm", FCVAR_CLIENTDLL);

	// ui inits
	g_UISquirrelManager = new SquirrelManager<ScriptContext::UI>();

	ENABLER_CREATEHOOK(
		hook, (char*)baseAddress + 0x12BA0, &SQPrintHook<ScriptContext::UI>, reinterpret_cast<LPVOID*>(&UISQPrint)); // ui print function
	RegisterConCommand("script_ui", ExecuteCodeCommand<ScriptContext::UI>, "Executes script code on the ui vm", FCVAR_CLIENTDLL);

	// inits for both client and ui, since they share some functions
	ClientSq_compilebuffer = (sq_compilebufferType)((char*)baseAddress + 0x3110);
	ClientSq_pushroottable = (sq_pushroottableType)((char*)baseAddress + 0x5860);
	ClientSq_call = (sq_callType)((char*)baseAddress + 0x8650);
	ClientRegisterSquirrelFunc = (RegisterSquirrelFuncType)((char*)baseAddress + 0x108E0);

	ClientSq_newarray = (sq_newarrayType)((char*)baseAddress + 0x39F0);
	ClientSq_arrayappend = (sq_arrayappendType)((char*)baseAddress + 0x3C70);

	ClientSq_pushstring = (sq_pushstringType)((char*)baseAddress + 0x3440);
	ClientSq_pushinteger = (sq_pushintegerType)((char*)baseAddress + 0x36A0);
	ClientSq_pushfloat = (sq_pushfloatType)((char*)baseAddress + 0x3800);
	ClientSq_pushbool = (sq_pushboolType)((char*)baseAddress + 0x3710);
	ClientSq_pusherror = (sq_pusherrorType)((char*)baseAddress + 0x8470);

	ClientSq_getstring = (sq_getstringType)((char*)baseAddress + 0x60C0);
	ClientSq_getinteger = (sq_getintegerType)((char*)baseAddress + 0x60E0);
	ClientSq_getfloat = (sq_getfloatType)((char*)baseAddress + 0x6100);
	ClientSq_getbool = (sq_getboolType)((char*)baseAddress + 0x6130);

	ClientSq_sq_get = (sq_getType)((char*)baseAddress + 0x7C30);

	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x26130,
		&CreateNewVMHook<ScriptContext::CLIENT>,
		reinterpret_cast<LPVOID*>(&ClientCreateNewVM)); // client createnewvm function
	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x26E70,
		&DestroyVMHook<ScriptContext::CLIENT>,
		reinterpret_cast<LPVOID*>(&ClientDestroyVM)); // client destroyvm function
	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x79A50,
		&ScriptCompileErrorHook<ScriptContext::CLIENT>,
		reinterpret_cast<LPVOID*>(&ClientSQCompileError)); // client compileerror function
	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x10190,
		&CallScriptInitCallbackHook<ScriptContext::CLIENT>,
		reinterpret_cast<LPVOID*>(&ClientCallScriptInitCallback)); // client callscriptinitcallback function
}

void InitialiseServerSquirrel(HMODULE baseAddress)
{
	g_ServerSquirrelManager = new SquirrelManager<ScriptContext::SERVER>();

	HookEnabler hook;

	ServerSq_compilebuffer = (sq_compilebufferType)((char*)baseAddress + 0x3110);
	ServerSq_pushroottable = (sq_pushroottableType)((char*)baseAddress + 0x5840);
	ServerSq_call = (sq_callType)((char*)baseAddress + 0x8620);
	ServerRegisterSquirrelFunc = (RegisterSquirrelFuncType)((char*)baseAddress + 0x1DD10);

	ServerSq_newarray = (sq_newarrayType)((char*)baseAddress + 0x39F0);
	ServerSq_arrayappend = (sq_arrayappendType)((char*)baseAddress + 0x3C70);

	ServerSq_pushstring = (sq_pushstringType)((char*)baseAddress + 0x3440);
	ServerSq_pushinteger = (sq_pushintegerType)((char*)baseAddress + 0x36A0);
	ServerSq_pushfloat = (sq_pushfloatType)((char*)baseAddress + 0x3800);
	ServerSq_pushbool = (sq_pushboolType)((char*)baseAddress + 0x3710);
	ServerSq_pusherror = (sq_pusherrorType)((char*)baseAddress + 0x8440);

	ServerSq_getstring = (sq_getstringType)((char*)baseAddress + 0x60A0);
	ServerSq_getinteger = (sq_getintegerType)((char*)baseAddress + 0x60C0);
	ServerSq_getfloat = (sq_getfloatType)((char*)baseAddress + 0x60E0);
	ServerSq_getbool = (sq_getboolType)((char*)baseAddress + 0x6110);

	ServerSq_sq_get = (sq_getType)((char*)baseAddress + 0x7C00);

	//execute functions
    SQVMStartCall = (SQVMStartCallType)((char*)baseAddress + 0x2DD20);
    SQVMCallErrorHandler = (SQVMCallErrorHandlerType)((char*)baseAddress + 0x33A70);
    sq_op_loadcopy = (sq_op_loadcopyType)((char*)baseAddress + 0x3d390);
    sq_op_call = (sq_op_callType)((char*)baseAddress + 0x2C440);
    sq_op_precall = (sq_op_precallType)((char*)baseAddress + 0x2C2B0);
    SQVMGet = (SQVMGetType)((char*)baseAddress + 0x343A0);
    sub_35B80 = (sub_35B80Type)((char*)baseAddress + 0x35B80);
    SQVMNewSlot = (SQVMNewSlotType)((char*)baseAddress + 0x34B30);
    sub_34CF0 = (sub_34CF0Type)((char*)baseAddress + 0x34CF0);
    sq_op_set = (sq_op_setType)((char*)baseAddress + 0x34810);
    sq_op_eq = (sq_op_eqType)((char*)baseAddress + 0x2F460);
    sub_2B590 = (sub_2B590Type)((char*)baseAddress + 0x2B590);
    sq_sqvm_Return = (sq_sqvm_ReturnType)((char*)baseAddress + 0x2E490);
    sq_copyObject = (sq_copyObjectType)((char*)baseAddress + 0x1370);
    sq_createTable = (sq_createTableType)((char*)baseAddress + 0x1580);
    sub_1800 = (sub_1800Type)((char*)baseAddress + 0x1800);
    sub_1A20 = (sub_1A20Type)((char*)baseAddress + 0x1A20);
    sub_2F740 = (sub_2F740Type)((char*)baseAddress + 0x2F740);
    sub_2E670 = (sub_2E670Type)((char*)baseAddress + 0x2E670);
    sub_2B6C0 = (sub_2B6C0Type)((char*)baseAddress + 0x2B6C0);
    sub_2E960 = (sub_2E960Type)((char*)baseAddress + 0x2E960);
    sq_op_increment_prefix_local = (sq_op_increment_prefix_localType)((char*)baseAddress + 0x2E780);
    sp_op_increment_prefix_structfield = (sp_op_increment_prefix_structfieldType)((char*)baseAddress + 0x2EB70);
    sub_2EA30 = (sub_2EA30Type)((char*)baseAddress + 0x2EA30);
    sub_2E860 = (sub_2E860Type)((char*)baseAddress + 0x2E860);
    sq_op_increment_postfix_structfield = (sq_op_increment_postfix_structfieldType)((char*)baseAddress + 0x2EC60);
    sub_2D1E0 = (sub_2D1E0Type)((char*)baseAddress + 0x2D1E0);
    sub_2CF10 = (sub_2CF10Type)((char*)baseAddress + 0x2CF10);
    sub_2F0B0 = (sub_2F0B0Type)((char*)baseAddress + 0x2F0B0);
    sub_2EE20 = (sub_2EE20Type)((char*)baseAddress + 0x2EE20);
    sub_2EF60 = (sub_2EF60Type)((char*)baseAddress + 0x2EF60);
    sub_34A30 = (sub_34A30Type)((char*)baseAddress + 0x34A30);
    sub_2D950 = (sub_2D950Type)((char*)baseAddress + 0x2D950);
    sub_35800 = (sub_35800Type)((char*)baseAddress + 0x35800);
    sub_2F2F0 = (sub_2F2F0Type)((char*)baseAddress + 0x2F2F0);
    sub_3EA70 = (sub_3EA70Type)((char*)baseAddress + 0x3EA70);
    SQVMToString = (SQVMToStringType)((char*)baseAddress + 0x2D2D0);
    sq_op_add = (sq_op_addType)((char*)baseAddress + 0x2BAF0);
    sq_dropStack = (sq_dropStackType)((char*)baseAddress + 0x2B500);
    sub_C6F0 = (sub_C6F0Type)((char*)baseAddress + 0xC6F0);
    sub_27390 = (sub_27390Type)((char*)baseAddress + 0x27390);
    sub_35B20 = (sub_35B20Type)((char*)baseAddress + 0x35B20);
    sq_op_sub = (sq_op_subType)((char*)baseAddress + 0x2BC80);
    sq_op_multiplication = (sq_op_multiplicationType)((char*)baseAddress + 0x2BE10);
    sq_op_div = (sq_op_divType)((char*)baseAddress + 0x2C020);
    sq_op_modulo = (sq_op_moduloType)((char*)baseAddress + 0x2C1C0);
    sq_compare = (sq_compareType)((char*)baseAddress + 0x2D080);
    sq_setObjectToFloat = (sq_setObjectToFloatType)((char*)baseAddress + 0x27430);
    sub_2E100 = (sub_2E100Type)((char*)baseAddress + 0x2E100);
    sq_nativecall = (sq_nativecallType)((char*)baseAddress + 0x33E00);
    sq_op_typecast = (sq_op_typecastType)((char*)baseAddress + 0x2F500);
    sq_op_check_entity_class = (sq_op_check_entity_classType)((char*)baseAddress + 0x2F6B0);
    sub_29A40 = (sub_29A40Type)((char*)baseAddress + 0x29A40);
    sub_63E00 = (sub_63E00Type)((char*)baseAddress + 0x63E00);
    sub_2EDB0 = (sub_2EDB0Type)((char*)baseAddress + 0x2EDB0);
    sub_BEF0 = (sub_BEF0Type)((char*)baseAddress + 0xBEF0);
    SQVMRaise_Error = (SQVMRaise_ErrorType)((char*)baseAddress + 0x35A10);
    g_ServerSquirrelManager->globalClosure = (SQObject*)((char*)baseAddress + 0xBCCE30);

	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x1FE90,
		&SQPrintHook<ScriptContext::SERVER>,
		reinterpret_cast<LPVOID*>(&ServerSQPrint)); // server print function
	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x260E0,
		&CreateNewVMHook<ScriptContext::SERVER>,
		reinterpret_cast<LPVOID*>(&ServerCreateNewVM)); // server createnewvm function
	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x26E20,
		&DestroyVMHook<ScriptContext::SERVER>,
		reinterpret_cast<LPVOID*>(&ServerDestroyVM)); // server destroyvm function
	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x799E0,
		&ScriptCompileErrorHook<ScriptContext::SERVER>,
		reinterpret_cast<LPVOID*>(&ServerSQCompileError)); // server compileerror function
	ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x1D5C0,
		&CallScriptInitCallbackHook<ScriptContext::SERVER>,
		reinterpret_cast<LPVOID*>(&ServerCallScriptInitCallback)); // server callscriptinitcallback function
	 ENABLER_CREATEHOOK(
		hook,
		(char*)baseAddress + 0x2F950,
		&SQVMExecuteHook2<ScriptContext::SERVER>,
		reinterpret_cast<LPVOID*>(&ServerSQVMExecute)); // server sqvmexecute function
   /* ENABLER_CREATEHOOK(
        hook,
        (char*)baseAddress + 0x2E100,
        & sub_2E100Hook<ScriptContext::SERVER>,
        reinterpret_cast<LPVOID*>(&Serversub_2E100));*/
	// cheat and clientcmd_can_execute allows clients to execute this, but since it's unsafe we only allow it when cheats are enabled
	// for script_client and script_ui, we don't use cheats, so clients can execute them on themselves all they want
	RegisterConCommand(
		"script",
		ExecuteCodeCommand<ScriptContext::SERVER>,
		"Executes script code on the server vm",
		FCVAR_GAMEDLL | FCVAR_CLIENTCMD_CAN_EXECUTE | FCVAR_CHEAT);
}

// hooks
template <ScriptContext context> SQInteger SQPrintHook(void* sqvm, char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);

	SQChar buf[1024];
	int charsWritten = vsnprintf_s(buf, _TRUNCATE, fmt, va);

	if (charsWritten > 0)
	{
		if (buf[charsWritten - 1] == '\n')
			buf[charsWritten - 1] = '\0';

		spdlog::info("[{} SCRIPT] {}", GetContextName(context), buf);
	}

	va_end(va);
	return 0;
}

template <ScriptContext context> void* CreateNewVMHook(void* a1, ScriptContext realContext)
{
	void* sqvm;

	if (context == ScriptContext::CLIENT)
	{
		sqvm = ClientCreateNewVM(a1, realContext);

		if (realContext == ScriptContext::UI)
			g_UISquirrelManager->VMCreated(sqvm);
		else
			g_ClientSquirrelManager->VMCreated(sqvm);
	}
	else if (context == ScriptContext::SERVER)
	{
		sqvm = ServerCreateNewVM(a1, context);
		g_ServerSquirrelManager->VMCreated(sqvm);
	}

	spdlog::info("CreateNewVM {} {}", GetContextName(realContext), sqvm);
	return sqvm;
}

template <ScriptContext context> void DestroyVMHook(void* a1, void* sqvm)
{
	ScriptContext realContext = context; // ui and client use the same function so we use this for prints

	if (context == ScriptContext::CLIENT)
	{
		if (g_ClientSquirrelManager->sqvm == sqvm)
			g_ClientSquirrelManager->VMDestroyed();
		else if (g_UISquirrelManager->sqvm == sqvm)
		{
			g_UISquirrelManager->VMDestroyed();
			realContext = ScriptContext::UI;
		}

		ClientDestroyVM(a1, sqvm);
	}
	else if (context == ScriptContext::SERVER)
	{
		g_ServerSquirrelManager->VMDestroyed();
		ServerDestroyVM(a1, sqvm);
	}

	spdlog::info("DestroyVM {} {}", GetContextName(realContext), sqvm);
}

template <ScriptContext context> void ScriptCompileErrorHook(void* sqvm, const char* error, const char* file, int line, int column)
{
	ScriptContext realContext = context; // ui and client use the same function so we use this for prints
	if (context == ScriptContext::CLIENT && sqvm == g_UISquirrelManager->sqvm)
		realContext = ScriptContext::UI;

	spdlog::error("{} SCRIPT COMPILE ERROR {}", GetContextName(realContext), error);
	spdlog::error("{} line [{}] column [{}]", file, line, column);

	// dont call the original since it kills game
	// in the future it'd be nice to do an actual error with UICodeCallback_ErrorDialog here, but only if we're compiling level scripts
	// compilestring and stuff shouldn't tho
	// though, that also has potential to be REALLY bad if we're compiling ui scripts lol
}

template <ScriptContext context> char CallScriptInitCallbackHook(void* sqvm, const char* callback)
{
	char ret;

	if (context == ScriptContext::CLIENT)
	{
		ScriptContext realContext = context; // ui and client use the same function so we use this for prints
		bool shouldCallCustomCallbacks = false;

		// since we don't hook arbitrary callbacks yet, make sure we're only doing callbacks on inits
		if (!strcmp(callback, "UICodeCallback_UIInit"))
		{
			realContext = ScriptContext::UI;
			shouldCallCustomCallbacks = true;
		}
		else if (!strcmp(callback, "ClientCodeCallback_MapSpawn"))
			shouldCallCustomCallbacks = true;

		// run before callbacks
		// todo: we need to verify if RunOn is valid for current state before calling callbacks
		if (shouldCallCustomCallbacks)
		{
			for (Mod mod : g_ModManager->m_loadedMods)
			{
				if (!mod.Enabled)
					continue;

				for (ModScript script : mod.Scripts)
				{
					for (ModScriptCallback modCallback : script.Callbacks)
					{
						if (modCallback.Context == realContext && modCallback.BeforeCallback.length())
						{
							spdlog::info(
								"Running custom {} script callback \"{}\"", GetContextName(realContext), modCallback.BeforeCallback);
							ClientCallScriptInitCallback(sqvm, modCallback.BeforeCallback.c_str());
						}
					}
				}
			}
		}

		spdlog::info("{} CodeCallback {} called", GetContextName(realContext), callback);
		if (!shouldCallCustomCallbacks)
			spdlog::info("Not executing custom callbacks for CodeCallback {}", callback);
		ret = ClientCallScriptInitCallback(sqvm, callback);

		// run after callbacks
		if (shouldCallCustomCallbacks)
		{
			for (Mod mod : g_ModManager->m_loadedMods)
			{
				if (!mod.Enabled)
					continue;

				for (ModScript script : mod.Scripts)
				{
					for (ModScriptCallback modCallback : script.Callbacks)
					{
						if (modCallback.Context == realContext && modCallback.AfterCallback.length())
						{
							spdlog::info(
								"Running custom {} script callback \"{}\"", GetContextName(realContext), modCallback.AfterCallback);
							ClientCallScriptInitCallback(sqvm, modCallback.AfterCallback.c_str());
						}
					}
				}
			}
		}
	}
	else if (context == ScriptContext::SERVER)
	{
		// since we don't hook arbitrary callbacks yet, make sure we're only doing callbacks on inits
		bool shouldCallCustomCallbacks = !strcmp(callback, "CodeCallback_MapSpawn");

		// run before callbacks
		// todo: we need to verify if RunOn is valid for current state before calling callbacks
		if (shouldCallCustomCallbacks)
		{
			for (Mod mod : g_ModManager->m_loadedMods)
			{
				if (!mod.Enabled)
					continue;

				for (ModScript script : mod.Scripts)
				{
					for (ModScriptCallback modCallback : script.Callbacks)
					{
						if (modCallback.Context == ScriptContext::SERVER && modCallback.BeforeCallback.length())
						{
							spdlog::info("Running custom {} script callback \"{}\"", GetContextName(context), modCallback.BeforeCallback);
							ServerCallScriptInitCallback(sqvm, modCallback.BeforeCallback.c_str());
						}
					}
				}
			}
		}

		spdlog::info("{} CodeCallback {} called", GetContextName(context), callback);
		if (!shouldCallCustomCallbacks)
			spdlog::info("Not executing custom callbacks for CodeCallback {}", callback);
		ret = ServerCallScriptInitCallback(sqvm, callback);

		// run after callbacks
		if (shouldCallCustomCallbacks)
		{
			for (Mod mod : g_ModManager->m_loadedMods)
			{
				if (!mod.Enabled)
					continue;

				for (ModScript script : mod.Scripts)
				{
					for (ModScriptCallback modCallback : script.Callbacks)
					{
						if (modCallback.Context == ScriptContext::SERVER && modCallback.AfterCallback.length())
						{
							spdlog::info("Running custom {} script callback \"{}\"", GetContextName(context), modCallback.AfterCallback);
							ServerCallScriptInitCallback(sqvm, modCallback.AfterCallback.c_str());
						}
					}
				}
			}
		}
	}

	return ret;
}
bool dumpSQObject(long long basePointer, long long offset)
{
	SQTable* table;
	SQClosure* closure;
	SQFunctionProto* function;
	SQString* string;
	SQNativeClosure* nativeClosure;
	SQVector* vector;
	SQArray* arrays;
	switch (*(long long*)(basePointer + offset))
	{
	case OT_USERPOINTER:
		spdlog::info("Offset {:16X} Found Userpointer", offset);
		return true;
	case OT_VECTOR:
		spdlog::info("Offset {:16X} Found Vector", offset);
		vector = (SQVector*)(basePointer + offset);
		spdlog::info("Offset {:16X} <{},{},{}>",offset+8, vector->x, vector->y, vector->z);
		return true;
	case OT_NULL:
		spdlog::info("Offset {:16X} Found Null", offset);
		return true;
	case OT_BOOL:
		spdlog::info("Offset {:16X} Found Bool", offset);
		spdlog::info("Offset {:16X} {}",offset+8, *((int*)(basePointer + offset + 8)));
		return true;
	case OT_INTEGER:
		spdlog::info("Offset {:16X} Found Integer", offset);
		spdlog::info("Offset {:16X} {}", offset + 8, *((int*)(basePointer + offset + 8)));
		return true;
	case OT_FLOAT:
		spdlog::info("Offset {:16X} Found Float", offset);
		spdlog::info("Offset {:16X} {}", offset + 8, *((float*)(basePointer + offset + 8)));
		return true;
	case OT_STRING:
		spdlog::info("Offset {:16X} Found String", offset);
		string = (SQString*)(basePointer + offset + 8);
		spdlog::info("Offset {:16X} {}",offset+8, string->_val);
		return true;
	case OT_ARRAY:
		spdlog::info("Offset {:16X} Found Array", offset);
		arrays = (SQArray*)(basePointer + offset + 8);
		/* for (int j = 0; j < arrays->_usedSlots; j++)
		{
			spdlog::info("Offset {:16X} {}",offset+8, sq_getTypeName(arrays->_values[j]._Type));
		}*/
		return true;
	case OT_CLOSURE:
		spdlog::info("Offset {:16X} Found Closure", offset);
		closure = (SQClosure*)(basePointer + offset + 8);
		if ((closure!=0)&&(closure->_function._VAL.asInteger!=0)&&(closure->_function._VAL.asFuncProto->funcName._VAL.asInteger!=0))
		    spdlog::info("Offset {:16X} {}",offset+8, closure->_function._VAL.asFuncProto->funcName._VAL.asString->_val);
		else
			spdlog::info("Offset {:16X} unknown", offset + 8);
		return true;
	case OT_NATIVECLOSURE:
		spdlog::info("Offset {:16X} Found NativeClosure", offset);
		nativeClosure = (SQNativeClosure*)(basePointer + offset + 8);
		//if ((nativeClosure!=0)&&(nativeClosure->_name!=0))
		  //  spdlog::info("Offset {:16X} {}",offset+8, nativeClosure->_name->_val);
		//else
			spdlog::info("Offset {:16X} unknown", offset + 8);
		return true;
	case OT_ASSET:
		spdlog::info("Offset {:16X} Found Asset", offset);
		string = (SQString*)(basePointer + offset + 8);
		spdlog::info("Offset {:16X} {}",offset +8, string->_val);
		return true;
	case OT_THREAD:
		spdlog::info("Offset {:16X} Found Thread", offset);
		return true;
	case OT_FUNCPROTO:
		spdlog::info("Offset {:16X} Found FuncProto", offset);
		function = (SQFunctionProto*)(basePointer + offset + 8);
		spdlog::info("Offset {:16X} {}",offset+8, function->funcName._VAL.asString->_val);
		return true;
	case OT_CLAAS:
		spdlog::info("Offset {:X} Found Class", offset);
		return true;
	case OT_STRUCT_DEFINITION:
		spdlog::info("Offset {:16X} Found Struct Definition", offset);
		return true;
	case OT_STRUCT_INSTANCE:
		spdlog::info("Offset {:16X} Found Struct Instance", offset);
		return true;
	case OT_WEAKREF:
		spdlog::info("Offset {:16X} Found WeakRef", offset);
		return true;
	case OT_TABLE:
		spdlog::info("Offset {:16X} Found Table", offset);
		table = (SQTable*)(basePointer + offset + 8);
		/* for (int i = 0; i < table->_numOfNodes; i++)
		{
			SQTableNode* node = &table->_nodes[i];
			if (node->key._Type == OT_STRING)
			{
				SQString* key = node->key._VAL.asString;
				spdlog::info("Offset {:16X} {},{}",offset+8, key->_val, sq_getTypeName(node->val._Type));
			}
		}*/
		return true;
	case OT_USERDATA:
		spdlog::info("Offset {:16X} Found UserData", offset);
		return true;
	case OT_INSTANCE:
		spdlog::info("Offset {:16X} Found Instance", offset);
		return true;
	case OT_ENTITY:
		spdlog::info("Offset {:16X} Found Entity", offset);
		return true;
	}
	spdlog::info("Offset {:16X} 0x{:16X}", offset, *(long long*)(basePointer + offset));
	return false;
}
template <ScriptContext context> void ExecuteCodeCommand(const CCommand& args)
{
	if (context == ScriptContext::CLIENT)
		g_ClientSquirrelManager->ExecuteCode(args.ArgS());
	else if (context == ScriptContext::UI)
		g_UISquirrelManager->ExecuteCode(args.ArgS());
	else if (context == ScriptContext::SERVER)
		g_ServerSquirrelManager->ExecuteCode(args.ArgS());
}

template <ScriptContext context> long long sub_2E100Hook(HSquirrelVM* sqvm, __int64 a2, int a3, int a4, unsigned int a5)
{
    spdlog::info("Stack before native call {}", sqvm->_stackbase);
    //long long res = Serversub_2E100(sqvm, a2, a3, a4, a5);
    spdlog::info("Stack after native call {}", sqvm->_stackbase);
    //return res;
}

template <ScriptContext context> long long SQVMExecuteHook2(
	HSquirrelVM* sqvm, SQObject* closure, int paramAmount, int stackbase, SQObject* outres, int throwerror, unsigned int calltype)
{   
    spdlog::info("Running execute with {}", closure->_VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->_val);
	spdlog::info("paramAmount = {} stackbase = {} throwerror = {} calltype = {}", paramAmount, stackbase, throwerror, calltype);
	for (int i = 0; i < 0x4408; i += 8)
	{
		if (dumpSQObject((long long)sqvm->sharedState, i))
			i += 8;
    }
	long long ret = SQVMExecuteHook<context>(sqvm, closure, paramAmount, stackbase, outres, throwerror, calltype);
	spdlog::info("split Here");
	for (int i = 0; i < 0x4408; i += 8)
	{
		if (dumpSQObject((long long)sqvm->sharedState, i))
			i += 8;
	}
	spdlog::info("Return execute {} with {}", closure->_VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->_val,ret);

	return ret;
    
}

template <ScriptContext context> long long SQVMExecuteHook(HSquirrelVM* sqvm, SQObject* closure, int paramAmount, int stackbase, SQObject* outres, int throwerror, unsigned int calltype)
{
    spdlog::info("Running custom execute with {}",closure->_VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->_val);
	spdlog::info("paramAmount = {} stackbase = {} throwerror = {} calltype = {}", paramAmount, stackbase, throwerror, calltype);
	//only runs on server right now so no check for context

    ++sqvm->_nnativecalls;
    SQSharedState* sharedState = sqvm->sharedState;
    int traps = 0;
    SQInstruction* Instruction; // r14

    HSquirrelVM* savedSharedState = sharedState->_currentThreadMaybe;
    sharedState->_currentThreadMaybe = sqvm;
    if (calltype)
    {
        if ((calltype == 1) || (calltype <= 3))
        {
            traps = sqvm->trapAmount;
            sqvm->ci->_root = sqvm->_suspended_root;
            sqvm->ci->_vargs_base = sqvm->_suspend_varargs;
            sqvm->_suspended = 0;
            if (calltype == 3)
                goto label_errorHandler;
        }
    }
    else
    {
        INCREMENT_REFERENCECOUNT(closure);
        DECREMENT_REFERENCECOUNT((&sqvm->temp_reg));
        sqvm->temp_reg = *closure;
        if (!SQVMStartCall(
            sqvm,
            sqvm->temp_reg._VAL.asClosure,
            sqvm->_top - paramAmount,
            (unsigned int)paramAmount,
            stackbase,
            0))
        {
            if (!sqvm->ci)
                SQVMCallErrorHandler(sqvm, &sqvm->_lasterror);
            sharedState->_currentThreadMaybe = savedSharedState;
            --sqvm->_nnativecalls;
            return 0;
        }
        sqvm->ci->_root = 1;
    }

exeption_restore:
    while (2)
    {
        Instruction = &sqvm->ci->ip[1];
        sqvm->ci->ip = Instruction;


        //spdlog::info("Opcode: {}, stackbase {}", (signed int)opCode, sqvm->_stackbase);
        spdlog::info("OpCode: {}, output: {}, arg1: {}, arg2: {}, arg3: {}", sq_OpToString(Instruction->op),Instruction->output,  Instruction->arg1, Instruction->arg2, Instruction->arg3);
        //spdlog::info("OpCode: {};{:X}, output: {};{:X}, arg1: {};{:X}, arg2: {};{:X}, arg3: {};{:X}", opCode, opCode, Instruction->output, Instruction->output, Instruction->arg1, Instruction->arg1, Instruction->arg2, Instruction->arg2, Instruction->arg3, Instruction->arg3);
        
        switch (Instruction->op)
        {
        case _OP_LOAD: {
            SQObject* target;
            SQObject* source;
            source = &sqvm->ci->_literals[Instruction->arg1];
            target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            *target = *source;
            continue; }
        case _OP_LOADCOPY:
            sq_op_loadcopy(
                sqvm->sharedState,
                &sqvm->ci->_literals[Instruction->arg1],
                &sqvm->_stackOfCurrentFunction[Instruction->output]);
            continue;
        case _OP_LOADINT: {
            int arg1 = Instruction->arg1;
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target)
                target->_Type = OT_INTEGER;
            target->_VAL.asInteger = arg1;
            continue; }
        case _OP_LOADFLOAT: {
            int arg1 = Instruction->arg1;
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target)
                target->_Type = OT_FLOAT;
            target->_VAL.asInteger = arg1;
            continue; }
        case _OP_DLOAD: { //Double load
            SQObject* source = &sqvm->ci->_literals[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            *target = *source;
            source = &sqvm->ci->_literals[(unsigned __int16)Instruction->arg3];
            target = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            *target = *source;
            continue; }
        case _OP_TAILCALL:
            if (!sq_op_call(sqvm, Instruction, outres, &traps, 1))
                goto label_errorHandler;
            if (sqvm->_suspended) {
                sharedState->_currentThreadMaybe = savedSharedState;
                --sqvm->_nnativecalls;
                return 1;
            }

            continue;
        case _OP_CALL:
            if (!sq_op_call(sqvm, Instruction, outres, &traps, 0))
                goto label_errorHandler;
            if (sqvm->_suspended)
            {
                sharedState->_currentThreadMaybe = savedSharedState;
                --sqvm->_nnativecalls;
                return 1;
            }
            continue;
        case _OP_PREPCALL:
            if (!sq_op_precall(sqvm, Instruction, &sqvm->_stackOfCurrentFunction[Instruction->arg1]))
                goto label_errorHandler;
            continue;
        case _OP_PREPCALLK:
            if (!sq_op_precall(sqvm, Instruction, &sqvm->ci->_literals[Instruction->arg1]))
                goto label_errorHandler;
            continue;
        case _OP_GETK: {
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            if (!SQVMGet(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg2], &sqvm->ci->_literals[Instruction->arg1], &res, 0, 1))
            {
                sub_35B80(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg2], &sqvm->ci->_literals[Instruction->arg1]);
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;

            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_MOVE: {

            SQObject* source = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            *target = *source;
            continue; }
        case _OP_NEWSLOT:
            if (!SQVMNewSlot(
                sqvm,
                &sqvm->_stackOfCurrentFunction[Instruction->arg1],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg3],
                0))
                goto label_errorHandler;
            continue;
        case _OP_DELETE: {
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            SQObject* key = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
            SQObject* self = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            INCREMENT_REFERENCECOUNT(self);
            DECREMENT_REFERENCECOUNT(target);
            if (!sub_34CF0(sqvm, self, key, &res))
            {
                DECREMENT_REFERENCECOUNT((&res))
                    goto label_errorHandler;
            }
            *target = res;
            continue; }
        case _OP_SET:
            if (!sq_op_set(
                sqvm,
                &sqvm->_stackOfCurrentFunction[Instruction->arg1],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg3],
                1))
            {
                if (sqvm->_lasterror._Type == OT_NULL)
                    sub_35B80(
                        sqvm,
                        &sqvm->_stackOfCurrentFunction[Instruction->arg1],
                        &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]);
                goto label_errorHandler;
            }
            continue;
        case _OP_GET: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject temp_reg;
            temp_reg._Type = OT_NULL;
            temp_reg._VAL.asInteger = 0i64;
            if (!SQVMGet(sqvm, arg1Obj, arg2Obj, &temp_reg, 0, 1))
            {
                sub_35B80(sqvm, arg1Obj, arg2Obj);
                DECREMENT_REFERENCECOUNT((&temp_reg));
                goto label_errorHandler;
            }

            INCREMENT_REFERENCECOUNT((&temp_reg));
            DECREMENT_REFERENCECOUNT(target);
            *target = temp_reg;
            DECREMENT_REFERENCECOUNT((&temp_reg));
            continue; }
        case _OP_EQ: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;
            target->_VAL.asInteger = sq_op_eq(
                0,
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                &sqvm->_stackOfCurrentFunction[Instruction->arg1]) ? 1 : 0;

            continue; }
        case _OP_NE: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;
            target->_VAL.asInteger = sq_op_eq(
                0,
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                &sqvm->_stackOfCurrentFunction[Instruction->arg1]) ? 0 : 1;
            continue; }
        case _OP_BITW: {
            int operation = (unsigned __int16)Instruction->arg3;
            SQObject* operator1 = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* operator2 = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            if (!sub_2B590(sqvm, operation, &res, operator1, operator2))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = res._Type;
            target->_VAL = res._VAL;
            continue; }
        case _OP_RETURN:
            if (!(unsigned int)sq_sqvm_Return(sqvm, Instruction->output, Instruction->arg1, &sqvm->temp_reg))
                continue;
            sq_copyObject(outres, &sqvm->temp_reg);

            sharedState->_currentThreadMaybe = savedSharedState;
            spdlog::info("Return of {}", closure->_VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->_val);
            --sqvm->_nnativecalls;
            return 1;
        case _OP_LOADNULLS:
            for (int i = 0; i < Instruction->arg1; i++)
            {
                SQObject* target = &sqvm->_stackOfCurrentFunction[i + Instruction->output];
                DECREMENT_REFERENCECOUNT(target)
                    target->_Type = OT_NULL;
                target->_VAL.asInteger = 0;
            }
            continue;
        case _OP_LOADROOTTABLE: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject* roottable = &sqvm->_roottable_object;
            INCREMENT_REFERENCECOUNT(roottable);
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = roottable->_Type;
            target->_VAL = roottable->_VAL;
            continue; }
        case _OP_LOADBOOL: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;
            target->_VAL.asInteger = Instruction->arg1;
            continue; }
        case _OP_DMOVE: {
            SQObject* source = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v94
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; //v95
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = source->_Type;
            target->_VAL = source->_VAL;


            source = &sqvm->_stackOfCurrentFunction[Instruction->arg3];
            target = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = source->_Type;
            target->_VAL = source->_VAL;
            continue; }
        case _OP_JMP:
            sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
            continue;
        case _OP_JNZ: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];

            if ((target->_Type & 0xB) != 0)
            {
                if (target->_VAL.asInteger)
                {
                    sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
                }
            }
            else
            {
                if (target->_Type != OT_FLOAT) {
                    sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
                }
                else if (target->_VAL.asFloat != 0.0)
                {
                    sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
                }
            }
            continue; }
        case _OP_JZ: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if ((target->_Type & 0xB) != 0)
            {
                if (!(target->_VAL.asInteger))
                {
                    sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
                }
            }
            else if (target->_Type == OT_FLOAT && target->_VAL.asFloat == 0.0)
            {
                sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
            }
            continue; }
        case _OP_LOADFREEVAR: {
            SQClosure* cl = sqvm->ci->obj10._VAL.asClosure;
            SQObject* source = &cl->_outervalues[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = source->_Type;
            target->_VAL = source->_VAL;
            continue; }
        case _OP_VARGC: {
            int vargc = (unsigned int)*((__int16*)&sqvm->ci->_root + 2);
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);

            target->_Type = OT_INTEGER;
            target->_VAL.asInteger = vargc;
            continue; }
        case _OP_GETVARGV: {
            SQObject* indexObject = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v100
            if (sqvm->ci->_vargs_size <= 0)
            {
                SQVMRaise_Error(sqvm, "this function doesn't have var args");
                goto label_errorHandler;
            }

            if (!(indexObject->_Type & (1 << 0x1Au)))
            {
                const char* typeName = sq_getTypeName(indexObject->_Type);
                SQVMRaise_Error(sqvm, "indexing 'vargv' with %s", typeName);
                goto label_errorHandler;
            }
            int index; //v14
            if (indexObject->_Type == OT_FLOAT)
                index = (unsigned int)indexObject->_VAL.asFloat;
            else
                index = indexObject->_VAL.asInteger;
            if (index < 0 || index >= sqvm->ci->_vargs_size)
            {
                SQVMRaise_Error(sqvm, "vargv index out of range");
                goto label_errorHandler;
            }
            SQObject* vargv = &sqvm->_vargsstack[(index + sqvm->ci->_vargs_base)];//v103
            INCREMENT_REFERENCECOUNT(vargv);
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; //v104
            DECREMENT_REFERENCECOUNT(target);
            *target = *vargv;
            continue; }
        case _OP_NEWTABLE: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_TABLE;
            target->_VAL.asTable = sq_createTable(sqvm->sharedState, Instruction->arg1);
            INCREMENT_REFERENCECOUNT(target);
            continue; }
        case _OP_NEWARRAY: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_ARRAY;
            target->_VAL.asArray = sub_1800(sqvm->sharedState, 0);
            INCREMENT_REFERENCECOUNT(target);
            spdlog::info("C6F0 in new Array is {:X}", (long long)target->_VAL.asArray->_values);
            sub_C6F0(
                &target->_VAL.asArray->_values,
                Instruction->arg1);
            continue; }
        case _OP_APPENDARRAY: {
            SQObject* data;
            if (Instruction->arg3)
                data = &sqvm->ci->_literals[Instruction->arg1];
            else
                data = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            sub_1A20(
                sqvm->_stackOfCurrentFunction[Instruction->output]._VAL.asArray,
                data);
            continue; }
        case _OP_GETPARENT: {
            SQObject res; //v387 v388
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if (!(unsigned __int8)sub_2F740(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg1], &res))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));

            DECREMENT_REFERENCECOUNT(target);
            *target = res;

            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_COMPOUND_ARITH:
            if (!sub_2E670(
                sqvm,
                (unsigned __int16)Instruction->arg3,
                &sqvm->_stackOfCurrentFunction[HIWORD(Instruction->arg1)],
                &sqvm->_stackOfCurrentFunction[Instruction->arg2],
                &sqvm->_stackOfCurrentFunction[LOWORD(Instruction->arg1)]))
                goto label_errorHandler;
            continue;
        case _OP_COMPOUND_ARITH_LOCAL: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
            if (!sub_2B6C0(sqvm, (unsigned __int16)Instruction->arg3, arg1Obj, arg1Obj, arg2Obj))
                goto label_errorHandler;
            continue;
        }

        case _OP_INCREMENT_PREFIX: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            if (!sub_2E960(sqvm, &res, arg1Obj, arg2Obj, *(uint8*) & Instruction->arg3))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_INCREMENT_PREFIX_LOCAL:
            if (sq_op_increment_prefix_local(
                sqvm,
                &sqvm->_stackOfCurrentFunction[Instruction->output],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                *(uint8*)&Instruction->arg3))
            {
                continue;
            }
            goto label_errorHandler;
        case _OP_INCREMENT_PREFIX_STRUCTFIELD:
            if (!(unsigned __int8)sp_op_increment_prefix_structfield(
                sqvm,
                &sqvm->_stackOfCurrentFunction[Instruction->output],
                &sqvm->_stackOfCurrentFunction[Instruction->arg1],
                (unsigned __int16)Instruction->arg2,
                *(uint8*)&Instruction->arg3))
                goto label_errorHandler;
            continue;
        case _OP_INCREMENT_POSTFIX: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            if (!(unsigned __int8)sub_2EA30(sqvm, &res, arg1Obj, arg2Obj, *(char*)&Instruction->arg3))
            {
                DECREMENT_REFERENCECOUNT((&res))
                    goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_INCREMENT_POSTFIX_LOCAL: {

            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v161
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; //closure

            SQObject res; //v382 v383
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;

            if (!sub_2E860(sqvm, &res, arg2Obj, *(uint8*)&Instruction->arg3))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));

            continue; }
        case _OP_INCREMENT_POSTFIX_STRUCTFIELD:
            if (!sq_op_increment_postfix_structfield(
                sqvm,
                &sqvm->_stackOfCurrentFunction[Instruction->output],
                &sqvm->_stackOfCurrentFunction[Instruction->arg1],
                (unsigned __int16)Instruction->arg2,
                *(uint8*)&Instruction->arg3))
            {
                goto label_errorHandler;
            }
            continue;
        case _OP_CMP: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];//v166
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v167
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; //closure
            SQObject res; //v386
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            if (!sub_2D1E0(sqvm, (unsigned __int16)Instruction->arg3, arg2Obj, arg1Obj, &res))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));

            continue; }
        case _OP_EXISTS: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;
            target->_VAL.asInteger = SQVMGet(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg1], &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2], &sqvm->temp_reg, 1, 0) ? true : false;
            continue; }
        case _OP_INSTANCEOF: {
            SQObject* classObj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* instanceObj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if (((classObj->_Type != OT_CLAAS) || (instanceObj->_Type & 0x408000) == 0))
            {
                const char* classTypeName = sq_getTypeName(classObj->_Type);
                const char* instanceTypeName = sq_getTypeName(instanceObj->_Type);
                SQVMRaise_Error(sqvm, "cannot apply instanceof between a %s and a %s", instanceTypeName, classTypeName);
                goto label_errorHandler;
            }


            long long classOfInstance = *(_QWORD*)(instanceObj->_VAL.asInteger + 56);
            bool isInstance = false;
            while (classOfInstance) {
                if (classOfInstance == classObj->_VAL.asInteger) {
                    isInstance = true;
                    break;
                }
                classOfInstance = *(_QWORD*)(classOfInstance + 56);
            }
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;
            target->_VAL.asInteger = isInstance;


            continue; }
        case _OP_NEG: {
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if (!sub_2CF10(sqvm, &res, &sqvm->_stackOfCurrentFunction[Instruction->arg1]))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));

            continue; }
        case _OP_NOT: {


            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;

            if ((arg1Obj->_Type & 0xB) != 0) {
                target->_VAL.asInteger = arg1Obj->_VAL.asInteger != 0;
            }
            else if (arg1Obj->_Type == OT_FLOAT) {
                target->_VAL.asInteger = arg1Obj->_VAL.asFloat != 0.0;
            }
            else {
                target->_VAL.asInteger = 0;
            }

            continue; }
        case _OP_BWNOT: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //type = v188
            if (arg1Obj->_Type != OT_INTEGER)
            {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                SQVMRaise_Error(sqvm, "attempt to perform a bitwise op on a %s", arg1TypeName);
                goto label_errorHandler;
            }
            sub_27390(&sqvm->_stackOfCurrentFunction[Instruction->output], ~sqvm->_stackOfCurrentFunction[Instruction->arg1]._VAL.asInteger);
            continue; }
        case _OP_CLOSURE: {

            SQObject* arg1Obj = &sqvm->ci->closure._VAL.asClosure->objectPointer_C0[Instruction->arg1];

            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;//v365 v366
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;

            if (!(unsigned __int8)sub_2F0B0(sqvm, &res, arg1Obj))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }

            INCREMENT_REFERENCECOUNT((&res));

            DECREMENT_REFERENCECOUNT(target);
            *target = res;

            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_FOREACH:
            if (!sub_2EE20(
                sqvm,
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg3],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 1],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 2],
                0,
                Instruction->output,
                Instruction->arg1,
                &sqvm->ci->ip))
            {
                goto label_errorHandler;
            }
            continue;
        case _OP_FOREACH_STATICARRAY_START: {
            SQObject* arg1_2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1 + 2]; //v201
            DECREMENT_REFERENCECOUNT(arg1_2Obj);
            arg1_2Obj->_Type = OT_INTEGER;
            arg1_2Obj->_VAL.asInteger = 0;
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v202
            DECREMENT_REFERENCECOUNT(arg1Obj);
            arg1Obj->_Type = OT_INTEGER;
            arg1Obj->_VAL.asInteger = 0;

            //TODO fix struct behaviour
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            SQObject* arg1_1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1 + 1]; //closure
            int structOffset = 2i64 * arg2Obj->_structOffset;
            SQObject* structObj = (SQObject*)(arg2Obj->_VAL.asInteger + 16i64 * arg2Obj->_structOffset + 56);

            INCREMENT_REFERENCECOUNT(structObj);
            DECREMENT_REFERENCECOUNT(arg1_1Obj);
            *arg1_1Obj = *structObj;
            continue; }
        case _OP_FOREACH_STATICARRAY_NEXT: {

            //TODO fix struct behaviour
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2 + 2];
            int arg2Int = ++arg2Obj->_VAL.asInteger;
            if (arg2Int >= (unsigned __int16)Instruction->arg3)
                continue;
            sub_27390(&sqvm->_stackOfCurrentFunction[Instruction->arg2 + 2], arg2Int);
            sq_copyObject(
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 1],
                (SQObject*)(16i64 * arg2Int
                    + sqvm->_stackOfCurrentFunction[Instruction->arg1]._structOffset
                    + 56
                    + sqvm->_stackOfCurrentFunction[Instruction->arg1]._VAL.asInteger));
            sqvm->ci->ip = (SQInstruction*)((long long)sqvm->ci->ip + 16i64 * Instruction->output);
            continue; }
        case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_START: {
            SQObject* arg1_2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1 + 2]; //v201
            DECREMENT_REFERENCECOUNT(arg1_2Obj);
            arg1_2Obj->_Type = OT_INTEGER;
            arg1_2Obj->_VAL.asInteger = 0;
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v202
            DECREMENT_REFERENCECOUNT(arg1Obj);
            arg1Obj->_Type = OT_INTEGER;
            arg1Obj->_VAL.asInteger = 0;



            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];//v10
            SQObject* arg1_1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1 + 1];//closure
            INCREMENT_REFERENCECOUNT(arg2Obj);
            DECREMENT_REFERENCECOUNT(arg1_1Obj);
            *arg1_1Obj = *arg2Obj;
            continue; }
        case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_NEXT: {

            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2 + 2];
            int arg2Int = ++arg2Obj->_VAL.asInteger;
            if (arg2Int >= (unsigned __int16)Instruction->arg3) {
                continue;
            }
            sub_27390(&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2], arg2Int);
            sq_copyObject(
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 1],
                &sqvm->_stackOfCurrentFunction[(unsigned __int8)Instruction->arg1]);
            sqvm->ci->ip = &sqvm->ci->ip[Instruction->output];

            continue; }
        case _OP_DELEGATE: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            if (!sub_2EF60(sqvm, &res, arg1Obj, arg2Obj))
            {
                DECREMENT_REFERENCECOUNT((&res))
                    goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_CLONE: {

            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;

            if (!sub_34A30(sqvm, arg1Obj, &res))
            {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                SQVMRaise_Error(sqvm, "cloning a %s", arg1TypeName);
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_TYPEOF: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            sub_2D950(sqvm, arg1Obj, &res, 0);
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_PUSHTRAP: { //TODO cleanup maybe redo
            long long data1;
            long long data2;

            HIDWORD(data1) = sqvm->_top;
            LODWORD(data1) = sqvm->_stackbase;
            long long offset = 16 * (Instruction->arg1 + (((long long)sqvm->ci->ip - (sqvm->ci->closure._VAL.asInteger + 272)) * 16));
            LODWORD(data2) = Instruction->output;
            long long pointer = sqvm->ci->closure._VAL.asInteger + offset + 272;
            if ((int)*(_DWORD*)sqvm->gap_F4 <= sqvm->traps)
                sub_35800(&sqvm->field_E8, 2 * sqvm->traps);
            long long* trapPointer = (long long*)(sqvm->field_E8 + 24i64 * (_QWORD)sqvm->traps);
            sqvm->traps++;
            if (trapPointer)
            {
                *trapPointer = data1;
                *(trapPointer + 8) = pointer;
                *(trapPointer + 16) = data2;
            }
            ++traps;
            ++sqvm->ci->_etraps[0];
            continue; }
        case _OP_POPTRAP:

            for (int i = 0; i < Instruction->output; i++) {
                --sqvm->traps;
                --traps;
                --sqvm->ci->_etraps[0];
            }
            continue;
        case _OP_THROW:
            sub_35B20(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->output]);
            goto label_errorHandler;
        case _OP_CLASS: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0;

            if (!(unsigned __int8)sub_2F2F0(sqvm, &res, Instruction->arg1, (unsigned __int16)Instruction->arg2))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_NEWSLOTA:
            if (!SQVMNewSlot(
                sqvm,
                &sqvm->_stackOfCurrentFunction[Instruction->arg1],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg3],
                (Instruction->output & 2) != 0))
                goto label_errorHandler;
            if ((Instruction->output & 1) != 0)
                sub_3EA70(
                    (void*)&sqvm->_stackOfCurrentFunction[Instruction->arg1]._VAL.asString->vftable,
                    &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                    &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 - 1]);
            continue;
        case _OP_EQ_LITERAL: {
            SQObject* arg1Obj = &sqvm->ci->_literals[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;
            target->_VAL.asInteger = sq_op_eq(0, arg2Obj, arg1Obj) ? true : false;
            continue; }
        case _OP_NE_LITERAL: {
            SQObject* arg1Obj = &sqvm->ci->_literals[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;
            target->_VAL.asInteger = sq_op_eq(0, arg2Obj, arg1Obj) ? false : true;

            continue; }
        case _OP_ASSERT_FAILED: {
            SQObject testObjectName;
            SQVMToString(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg1], &testObjectName);
            SQVMRaise_Error(sqvm, "Assertion failed: %s", (const char*)(&testObjectName._VAL.asString->_val));
            DECREMENT_REFERENCECOUNT((&testObjectName));
            goto label_errorHandler; }
        case _OP_ADD: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v87
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v88
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
                res._VAL.asInteger = 0;
            if (!sq_op_add(sqvm, &res, arg2Obj, arg1Obj))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_SUB: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v87
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v88
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            if (!sq_op_sub(sqvm, &res, arg2Obj, arg1Obj))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;
            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_MUL: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v87
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v88
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;
            if (!sq_op_multiplication(sqvm, &res, arg2Obj, arg1Obj))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;

            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_DIV: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v87
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v88
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;

            if (!sq_op_div(sqvm, &res, arg2Obj, arg1Obj))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;

            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_MOD: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v87
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v88
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject res;
            res._Type = OT_NULL;
            res._VAL.asInteger = 0i64;

            if (!sq_op_modulo(sqvm, &res, arg2Obj, arg1Obj))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;

            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_CMP_JZ: {
            uint32 intRes;
            if (!sq_compare(
                sqvm,
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2], &sqvm->_stackOfCurrentFunction[Instruction->arg1], &intRes)) {
                goto label_errorHandler;
            }


            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];//v170
            sqvm->ci->ip = &sqvm->ci->ip[1];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_BOOL;
            if (Instruction->arg3)
            {

                if (Instruction->arg3 - 2)
                {

                    if (Instruction->arg3 - 3)
                    {
                        if (Instruction->arg3 == 4 && intRes <= 0)
                        {
                            target->_VAL.asInteger = 1;
                            continue;
                        }
                    }
                    else if (intRes < 0)
                    {
                        target->_VAL.asInteger = 1;
                        continue;
                    }
                }
                else if (intRes >= 0)
                {
                    target->_VAL.asInteger = 1;
                    continue;
                }
            }
            else if (intRes > 0)
            {
                target->_VAL.asInteger = 1;
                continue;
            }
            target->_VAL.asInteger = 0;
			sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
            continue; }
        case _OP_INCREMENT_LOCAL_DISCARD_JMP: {
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v164
            if (arg2Obj->_Type == OT_INTEGER)
            {
                arg2Obj->_VAL.asInteger += *(uint8*)&Instruction->arg3;
                sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
            }
            else if (arg2Obj->_Type == OT_FLOAT)
            {
                arg2Obj->_VAL.asFloat = (float)( * (uint8*)&Instruction->arg3) + arg2Obj->_VAL.asFloat;
				sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
            }
            else {
                const char* arg2TypeName = sq_getTypeName(arg2Obj->_Type);
                SQVMRaise_Error(sqvm, "Cannot increment or decrement %s", arg2TypeName);
                goto label_errorHandler;
            }
            continue; }
        case _OP_NEWVECTOR: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
            SQObject* arg3Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg3];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];

            if (!((arg1Obj->_Type & SQOBJECT_NUMERIC) && (arg2Obj->_Type & SQOBJECT_NUMERIC) && (arg3Obj->_Type & SQOBJECT_NUMERIC)))
            {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                const char* arg2TypeName = sq_getTypeName(arg2Obj->_Type);
                const char* arg3TypeName = sq_getTypeName(arg3Obj->_Type);
                SQVMRaise_Error(sqvm, "Vector component types are <%s, %s, %s>; must be integer or float", arg1TypeName, arg2TypeName, arg3TypeName);
                goto label_errorHandler;
            }
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_VECTOR;
            SQVector* targetVec = (SQVector*)target;
            if (arg1Obj->_Type == OT_INTEGER) {
                targetVec->x = (float)arg1Obj->_VAL.asInteger;
            }
            else {
                targetVec->x = arg1Obj->_VAL.asFloat;
            }
            if (arg2Obj->_Type == OT_INTEGER) {
                targetVec->y = (float)arg2Obj->_VAL.asInteger;
            }
            else {
                targetVec->y = arg2Obj->_VAL.asFloat;
            }
            if (arg3Obj->_Type == OT_INTEGER) {
                targetVec->z = (float)arg3Obj->_VAL.asInteger;
            }
            else {
                targetVec->z = arg3Obj->_VAL.asFloat;
            }
            continue; }
        case _OP_ZEROVECTOR: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            SQVector* targetVec = (SQVector*)target;
            target->_Type = OT_VECTOR;
            targetVec->x = 0.0;
            targetVec->y = 0.0;
            targetVec->z = 0.0;

            continue; }
        case _OP_GET_VECTOR_COMPONENT: {

            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            if (arg1Obj->_Type != 0x40000)
            {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                SQVMRaise_Error(sqvm, "Cannot get vector component (.x/y/z) of %s", arg1TypeName);
                goto label_errorHandler;
            }
            SQVector* arg1Vec = (SQVector*)arg1Obj;
            float output = 0.0;
            switch (Instruction->arg2) {
            case 0:
                output = arg1Vec->x;
                break;
            case 1:
                output = arg1Vec->y;
                break;
            case 2:
                output = arg1Vec->z;
                break;
            default:
                SQVMRaise_Error(sqvm, "Invalid vector component index %d", Instruction->arg2);
                goto label_errorHandler;
            }
            sq_setObjectToFloat(
                &sqvm->_stackOfCurrentFunction[Instruction->output], output);
            continue; }

        case _OP_SET_VECTOR_COMPONENT: {

            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            if (target->_Type != OT_VECTOR) {
                const char* targetTypeName = sq_getTypeName(target->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
                goto label_errorHandler;
            }

            if (!(arg1Obj->_Type & SQOBJECT_NUMERIC)) {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
                goto label_errorHandler;
            }

            float arg1Float;
            if (arg1Obj->_Type == OT_INTEGER)
            {
                arg1Float = (float)arg1Obj->_VAL.asInteger;
            }
            else
            {
                arg1Float = arg1Obj->_VAL.asFloat;
            }
            SQVector* targetVec = (SQVector*)target;
            switch (Instruction->arg2) {
            case 0:
                targetVec->x = arg1Float;
                break;
            case 1:
                targetVec->y = arg1Float;
                break;
            case 2:
                targetVec->z = arg1Float;
                break;

            }

            continue; }
        case _OP_VECTOR_COMPONENT_MINUSEQ: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if (target->_Type != OT_VECTOR) {
                const char* targetTypeName = sq_getTypeName(target->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
                goto label_errorHandler;
            }

            if (!(arg1Obj->_Type & SQOBJECT_NUMERIC)) {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
                goto label_errorHandler;
            }
            float factor;
            if (arg1Obj->_Type == OT_INTEGER) {
                factor = (float)arg1Obj->_VAL.asInteger;
            }
            else {
                factor = arg1Obj->_VAL.asFloat;
            }
            SQVector* targetVec = (SQVector*)target;
            switch (Instruction->arg2) {
            case 0:
                targetVec->x = targetVec->x - factor;
                break;
            case 1:
                targetVec->y = targetVec->y - factor;
                break;
            case 2:
                targetVec->z = targetVec->z - factor;
                break;

            }
            continue; }
        case _OP_VECTOR_COMPONENT_PLUSEQ: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if (target->_Type != OT_VECTOR) {
                const char* targetTypeName = sq_getTypeName(target->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
                goto label_errorHandler;
            }

            if (!(arg1Obj->_Type & SQOBJECT_NUMERIC)) {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
                goto label_errorHandler;
            }
            float factor;
            if (arg1Obj->_Type == OT_INTEGER) {
                factor = (float)arg1Obj->_VAL.asInteger;
            }
            else {
                factor = arg1Obj->_VAL.asFloat;
            }
            SQVector* targetVec = (SQVector*)target;
            switch (Instruction->arg2) {
            case 0:
                targetVec->x = targetVec->x + factor;
                break;
            case 1:
                targetVec->y = targetVec->y + factor;
                break;
            case 2:
                targetVec->z = targetVec->z + factor;
                break;

            }
            continue; }
        case _OP_VECTOR_COMPONENT_MULEQ: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if (target->_Type != OT_VECTOR) {
                const char* targetTypeName = sq_getTypeName(target->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
                goto label_errorHandler;
            }

            if (!(arg1Obj->_Type & SQOBJECT_NUMERIC)) {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
                goto label_errorHandler;
            }
            float factor;
            if (arg1Obj->_Type == OT_INTEGER) {
                factor = (float)arg1Obj->_VAL.asInteger;
            }
            else {
                factor = arg1Obj->_VAL.asFloat;
            }
            SQVector* targetVec = (SQVector*)target;
            switch (Instruction->arg2) {
            case 0:
                targetVec->x = targetVec->x * factor;
                break;
            case 1:
                targetVec->y = targetVec->y * factor;
                break;
            case 2:
                targetVec->z = targetVec->z * factor;
                break;

            }
            continue; }
        case _OP_VECTOR_COMPONENT_DIVEQ: {

            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if (target->_Type != OT_VECTOR) {
                const char* targetTypeName = sq_getTypeName(target->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
                goto label_errorHandler;
            }

            if (!(arg1Obj->_Type & SQOBJECT_NUMERIC)) {
                const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
                SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
                goto label_errorHandler;
            }
            float divisor;
            if (arg1Obj->_Type == OT_INTEGER) {
                divisor = (float)arg1Obj->_VAL.asInteger;
            }
            else {
                divisor = arg1Obj->_VAL.asFloat;
            }
            SQVector* targetVec = (SQVector*)target;
            switch (Instruction->arg2) {
            case 0:
                targetVec->x = targetVec->x / divisor;
                break;
            case 1:
                targetVec->y = targetVec->y / divisor;
                break;
            case 2:
                targetVec->z = targetVec->z / divisor;
                break;

            }
            continue; }
        case _OP_VECTOR_NORMALIZE: {
            SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            if (vec->_Type != OT_VECTOR) {
                const char* typeName = sq_getTypeName(vec->_Type);
                SQVMRaise_Error(sqvm, "Cannot normalize %s", typeName);
                goto label_errorHandler;
            }


            float normalizationFactor = 1.0 / (sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z) + 1.1754944e-38);
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            SQVector* targetVec = (SQVector*)target;
            targetVec->_Type = OT_VECTOR;
            targetVec->x = normalizationFactor * vec->x;
            targetVec->y = normalizationFactor * vec->y;
            targetVec->z = normalizationFactor * vec->z;


            continue; }
        case _OP_VECTOR_NORMALIZE_IN_PLACE: {
            SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            if (vec->_Type != OT_VECTOR)
            {
                const char* typeName = sq_getTypeName(vec->_Type);
                SQVMRaise_Error(sqvm, "Cannot normalize %s", typeName);
                goto label_errorHandler;
            }
            float vecLength = sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
            float normalizationFactor = 1.0 / (vecLength + 1.1754944e-38);
            vec->x *= normalizationFactor;
            vec->y *= normalizationFactor;
            vec->z *= normalizationFactor;

            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], vecLength);

            continue; }
        case _OP_VECTOR_DOT_PRODUCT: {
            SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
            if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
            {
                const char* vec1TypeName = sq_getTypeName(vec1->_Type);
                const char* vec2TypeName = sq_getTypeName(vec2->_Type);
                SQVMRaise_Error(sqvm, "Cannot dot %s with %s", vec1TypeName, vec2TypeName);
                goto label_errorHandler;
            }

            float dotProduct = vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z;
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], dotProduct);
            continue; }
        case _OP_VECTOR_DOT_PRODUCT2D: {
            SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
            if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
            {
                const char* vec1TypeName = sq_getTypeName(vec1->_Type);
                const char* vec2TypeName = sq_getTypeName(vec2->_Type);
                SQVMRaise_Error(sqvm, "Cannot dot %s with %s", vec1TypeName, vec2TypeName);
                goto label_errorHandler;
            }
            float dotProduct = vec1->x * vec2->x + vec1->y * vec2->y;
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], dotProduct);
            continue; }
        case _OP_VECTOR_CROSS_PRODUCT: {
            SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQVector* targetVec = (SQVector*)target;
            if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
            {
                const char* vec1TypeName = sq_getTypeName(vec1->_Type);
                const char* vec2TypeName = sq_getTypeName(vec2->_Type);
                SQVMRaise_Error(sqvm, "Cannot cross %s with %s", vec1TypeName, vec2TypeName);
                goto label_errorHandler;
            }
            DECREMENT_REFERENCECOUNT(target);
            targetVec->_Type = OT_VECTOR;
            targetVec->x = vec1->y * vec2->z - vec1->z * vec2->y;
            targetVec->y = vec1->z * vec2->x - vec1->x * vec2->z;
            targetVec->z = vec1->x * vec2->y - vec1->y * vec2->x;
            continue; }
        case _OP_VECTOR_CROSS_PRODUCT2D: {
            SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
            if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
            {
                const char* vec1TypeName = sq_getTypeName(vec1->_Type);
                const char* vec2TypeName = sq_getTypeName(vec2->_Type);
                SQVMRaise_Error(sqvm, "Cannot cross %s with %s", vec1TypeName, vec2TypeName);
                goto label_errorHandler;
            }
            float crossProduct = vec1->x * vec2->y - vec1->y * vec2->x;
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], crossProduct);
            continue; }

        case _OP_VECTOR_LENGTH: {
            SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            if (vec->_Type != OT_VECTOR)
            {
                const char* typeName = sq_getTypeName(vec->_Type);
                SQVMRaise_Error(sqvm, "Cannot get length of %s", typeName);
                goto label_errorHandler;
            }
            float vecLength = fsqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], vecLength);
            continue; }
        case _OP_VECTOR_LENGTHSQR: {
            SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            if (vec->_Type != OT_VECTOR)
            {
                const char* typeName = sq_getTypeName(vec->_Type);
                SQVMRaise_Error(sqvm, "Cannot get squared length of %s", typeName);
                goto label_errorHandler;
            }
            float vecLength = vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], vecLength);
            continue; }
        case _OP_VECTOR_LENGTH2D: {
            SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            if (vec->_Type != OT_VECTOR)
            {
                const char* typeName = sq_getTypeName(vec->_Type);
                SQVMRaise_Error(sqvm, "Cannot get 2D length of %s", typeName);
                goto label_errorHandler;
            }
            float vecLength = fsqrt(vec->x * vec->x + vec->y * vec->y);
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], vecLength);
            continue; }
        case _OP_VECTOR_LENGTH2DSQR: {
            SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            if (vec->_Type != OT_VECTOR)
            {
                const char* typeName = sq_getTypeName(vec->_Type);
                SQVMRaise_Error(sqvm, "Cannot get 2D squared length of %s", typeName);
                goto label_errorHandler;
            }
            float vecLength = fsqrt(vec->x * vec->x + vec->y * vec->y);
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], vecLength);
            continue; }
        case _OP_VECTOR_DISTANCE: {
            SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
            if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
            {
                const char* vec1TypeName = sq_getTypeName(vec1->_Type);
                const char* vec2TypeName = sq_getTypeName(vec2->_Type);
                SQVMRaise_Error(sqvm, "Cannot get distance between %s and %s", vec1TypeName, vec2TypeName);
                goto label_errorHandler;
            }
            float x = vec2->x - vec1->x;
            float y = vec2->y - vec1->y;
            float z = vec2->z - vec1->z;
            float distance = fsqrt(x * x + y * y + z * z);
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], distance);
            continue; }
        case _OP_VECTOR_DISTANCESQR: {
            SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
            if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
            {
                const char* vec1TypeName = sq_getTypeName(vec1->_Type);
                const char* vec2TypeName = sq_getTypeName(vec2->_Type);
                SQVMRaise_Error(sqvm, "Cannot get squared distance between %s and %s", vec1TypeName, vec2TypeName);
                goto label_errorHandler;
            }
            float x = vec2->x - vec1->x;
            float y = vec2->y - vec1->y;
            float z = vec2->z - vec1->z;
            float distance = x * x + y * y + z * z;
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], distance);
            continue; }
        case _OP_VECTOR_DISTANCE2D: {
            SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
            if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
            {
                const char* vec1TypeName = sq_getTypeName(vec1->_Type);
                const char* vec2TypeName = sq_getTypeName(vec2->_Type);
                SQVMRaise_Error(sqvm, "Cannot get 2D distance between %s and %s", vec1TypeName, vec2TypeName);
                goto label_errorHandler;
            }
            float x = vec2->x - vec1->x;
            float y = vec2->y - vec1->y;
            float distance = fsqrt(x * x + y * y);
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], distance);
            continue; }
        case _OP_VECTOR_DISTANCE2DSQR: {
            SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
            if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
            {
                const char* vec1TypeName = sq_getTypeName(vec1->_Type);
                const char* vec2TypeName = sq_getTypeName(vec2->_Type);
                SQVMRaise_Error(sqvm, "Cannot get 2D squared distance between %s and %s", vec1TypeName, vec2TypeName);
                goto label_errorHandler;
            }
            float x = vec2->x - vec1->x;
            float y = vec2->y - vec1->y;
            float distance = fsqrt(x * x + y * y);
            sq_setObjectToFloat(&sqvm->_stackOfCurrentFunction[Instruction->output], distance);
            continue; }
        case _OP_INCREMENT_LOCAL_DISCARD: {
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];//v14 
            if (arg2Obj->_Type == OT_INTEGER)
            {
                arg2Obj->_VAL.asInteger += *(char*)&Instruction->arg3;
            }
            else
            {
                if (arg2Obj->_Type != OT_FLOAT) {
                    const char* arg2TypeName = sq_getTypeName(arg2Obj->_Type);
                    SQVMRaise_Error(sqvm, "Cannot increment or decrement %s", arg2TypeName);
                    goto label_errorHandler;
                }

                arg2Obj->_VAL.asFloat = (float)(*(char*)&Instruction->arg3) + arg2Obj->_VAL.asFloat;
            }
            continue; }
        case _OP_FASTCALL: {
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; //v36
            INCREMENT_REFERENCECOUNT(sqvm->_stackOfCurrentFunction);
            DECREMENT_REFERENCECOUNT(arg2Obj);
            *arg2Obj = *sqvm->_stackOfCurrentFunction;

            if (!sub_2E100(
                sqvm,
                sqvm->ci->closure._VAL.asClosure->objectPointer_C0[Instruction->arg1]._VAL.asInteger,
                Instruction->output,
                (unsigned __int16)Instruction->arg3,
                sqvm->_stackbase + (unsigned __int16)Instruction->arg2))
            {
                goto label_errorHandler;

            }
            continue; }
        case _OP_FASTCALL_NATIVE:
        case _OP_FASTCALL_NATIVE_ARGTYPECHECK: {
            SQObject* arg1Obj = &sqvm->ci->closure._VAL.asClosure->objectPointer_C0[Instruction->arg1]; //v40
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            INCREMENT_REFERENCECOUNT(sqvm->_stackOfCurrentFunction);
            DECREMENT_REFERENCECOUNT(arg2Obj);
            *arg2Obj = *arg1Obj;
            SQObject res;//src
            res._Type = OT_NULL;
            res._VAL.asInteger = 0;


            int newStackBase = (sqvm->_stackbase + (unsigned __int16)Instruction->arg2);
            bool argCheck = Instruction->op == 104;
            spdlog::info("Calling native with name {} and param3 = {} l2116", arg1Obj->_VAL.asNativeClosure->_name->_val, Instruction->arg3);
            for (int i = 0; i < Instruction->arg3; i++) {
                char buf[256];
                sqObjectToString(buf, 256, &sqvm->_stackOfCurrentFunction[i+1]);
                spdlog::info("{} with total stack offset {}", buf,sqvm->_stackbase +i+2);
            }
            if (!sq_nativecall(sqvm, arg1Obj->_VAL.asNativeClosure, (unsigned __int16)Instruction->arg3, newStackBase, &res, &calltype, argCheck))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            if ((_BYTE)calltype || *(&sqvm->_suspend_varargs + 1) && sqvm->_nnativecalls == 1)
            {

                sqvm->_suspended = 1;
                sqvm->_suspended_target = Instruction->output;
                sqvm->trapAmount = traps;
                sqvm->_suspended_root = sqvm->ci->_root;
                sqvm->_suspend_varargs = *(&sqvm->ci->_root + 1);
                if (*(&sqvm->_suspend_varargs + 1) == 0)
                    sq_copyObject(outres, &res);
                else
                    sq_copyObject(outres, &sqvm->_object_120);
                *(&sqvm->_suspend_varargs + 1) = 0;
                DECREMENT_REFERENCECOUNT((&res));
                sharedState->_currentThreadMaybe = savedSharedState;
                --sqvm->_nnativecalls;
                return 1;
            }

            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; //closure
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;

            DECREMENT_REFERENCECOUNT((&res));
            continue; }
        case _OP_FASTCALL_ENV:
            if (!sub_2E100(
                sqvm,
                sqvm->ci->closure._VAL.asClosure->objectPointer_C0[Instruction->arg1]._VAL.asInteger,
                Instruction->output,
                (unsigned __int16)Instruction->arg3,
                sqvm->_stackbase + (unsigned __int16)Instruction->arg2))
            {
                goto label_errorHandler;
            }
            continue;

        case _OP_FASTCALL_NATIVE_ENV:
        case _OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK: {
            int localCalltype;
            SQObject* arg1Obj = &sqvm->ci->closure._VAL.asClosure->objectPointer_C0[Instruction->arg1];
            int newStackbase = sqvm->_stackbase + (unsigned int)(unsigned __int16)Instruction->arg2;
            bool argTypeCheck = Instruction->op == _OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK;
            SQObject res; //v360
            res._Type = OT_NULL;
            res._VAL.asInteger = 0;
            spdlog::info("Calling native with name {} and param3 = {} l2169", arg1Obj->_VAL.asNativeClosure->_name->_val, Instruction->arg3);
            for (int i = 0; i < Instruction->arg3; i++) {
                char buf[256];
                sqObjectToString(buf, 256, &sqvm->_stackOfCurrentFunction[i + 1]);
                spdlog::info("{} with total stack offset {}", buf, sqvm->_stackbase + i + 2);
            }
            if (!sq_nativecall(sqvm, arg1Obj->_VAL.asNativeClosure, (unsigned __int16)Instruction->arg3, newStackbase, &res, (uint32*) & localCalltype, argTypeCheck))
            {
                DECREMENT_REFERENCECOUNT((&res));
                goto label_errorHandler;
            }
            if (localCalltype || *(&sqvm->_suspend_varargs + 1) && sqvm->_nnativecalls == 1)
            {
                sqvm->_suspended = 1;
                sqvm->_suspended_target = Instruction->output;
                sqvm->trapAmount = traps;
                sqvm->_suspended_root = sqvm->ci->_root;
                sqvm->_suspend_varargs = *(&sqvm->ci->_root + 1);
                if (*(&sqvm->_suspend_varargs + 1) == 0)
                    sq_copyObject(outres, &res);
                else
                    sq_copyObject(outres, &sqvm->_object_120);
                *(&sqvm->_suspend_varargs + 1) = 0;
                DECREMENT_REFERENCECOUNT((&res));
                sharedState->_currentThreadMaybe = savedSharedState;
                --sqvm->_nnativecalls;
                return 1;

            }
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; //closure
            INCREMENT_REFERENCECOUNT((&res));
            DECREMENT_REFERENCECOUNT(target);
            *target = res;

            DECREMENT_REFERENCECOUNT((&res));
            continue; }

        case _OP_LOADGLOBALARRAY: {
            SQObject* source = (SQObject*)&sharedState->_gobalsStructType;//TODO has to be tested if it is array
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = source->_Type;
            target->_VAL = source->_VAL;
            continue; }
        case _OP_GETGLOBAL: {


            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject* arg1Obj = &sharedState->_globalsStruct->data[Instruction->arg1];
            INCREMENT_REFERENCECOUNT(arg1Obj);
            DECREMENT_REFERENCECOUNT(target);
            *target = *arg1Obj;


            continue; }
        case _OP_SETGLOBAL: {
            SQObject* target = &sharedState->_globalsStruct->data[Instruction->arg1];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            INCREMENT_REFERENCECOUNT(arg2Obj);
            DECREMENT_REFERENCECOUNT(target);
            *target = *arg2Obj;
            continue; }
        case _OP_COMPOUND_ARITH_GLOBAL: {
            SQObject* arg1Obj = &sharedState->_globalsStruct->data[Instruction->arg1];
            if (!sub_2B6C0(sqvm, (unsigned __int16)Instruction->arg3, arg1Obj, arg1Obj, &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2])) {
                goto label_errorHandler;
            }
            continue; }

        case _OP_GETSTRUCTFIELD: {
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            int fieldOffset = arg1Obj->_structOffset + (unsigned __int16)Instruction->arg2;
            DECREMENT_REFERENCECOUNT(target);
            SQObject* field = &arg1Obj->_VAL.asStructInstance->data[fieldOffset];
            INCREMENT_REFERENCECOUNT(field);
            *target = *field;
            continue; }
        case _OP_SETSTRUCTFIELD: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            INCREMENT_REFERENCECOUNT(arg2Obj);
            int fieldOffset = Instruction->arg1 + target->_structOffset;
            SQObject* structField = &target->_VAL.asStructInstance->data[fieldOffset];
            DECREMENT_REFERENCECOUNT(structField);
            *structField = *arg2Obj;
            continue; }
        case _OP_COMPOUND_ARITH_STRUCTFIELD: {
            SQObject* target = (SQObject*)(16i64 * (Instruction->arg1 + *((_DWORD*)&sqvm->_stackOfCurrentFunction[Instruction->output]._Type + 1))
                + 56
                + sqvm->_stackOfCurrentFunction[Instruction->output]._VAL.asInteger);
            SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
            if (!sub_2B6C0(sqvm, (unsigned __int16)Instruction->arg3, target, target, arg2Obj))
                goto label_errorHandler;
            continue; }
        case _OP_NEWSTRUCT: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = OT_STRUCT_INSTANCE;
            target->_VAL.asStructInstance = (SQStructInstance*)sub_63E00(
                sqvm->sharedState,
                Instruction->arg1,
                *(_DWORD**)(*(_QWORD*)&sqvm->ci->closure._VAL.asClosure->gap_C8[8] + 8 * (unsigned __int16)Instruction->arg2));
            INCREMENT_REFERENCECOUNT(target);
            continue; }
        case _OP_GETSUBSTRUCT: {
            SQObject* source = &sqvm->_stackOfCurrentFunction[Instruction->arg1];//v10
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            INCREMENT_REFERENCECOUNT(source);
            DECREMENT_REFERENCECOUNT(target);
            target->_Type = source->_Type;
            target->_VAL.asInteger = source->_VAL.asInteger;
            int offset = 16i64 * Instruction->output;
            *(int*)((char*)(&sqvm->_stackOfCurrentFunction) + offset + 4) += (unsigned __int16)Instruction->arg2;
            continue; }
        case _OP_GETSUBSTRUCT_DYNAMIC: {
            int factor = sqvm->_stackOfCurrentFunction[Instruction->arg2]._VAL.asInteger;
            int arg1_0 = Instruction->arg1 & 0xFFFF;
            int arg1_1 = Instruction->arg1 >> 16;
            if (factor >= arg1_1)
            {
                SQVMRaise_Error(sqvm, "Array index %d doesn't index size of %d", factor, arg1_1);
                goto label_errorHandler;
            }
            sq_copyObject(&sqvm->_stackOfCurrentFunction[Instruction->output], &sqvm->_stackOfCurrentFunction[arg1_0]);
            int offset = 16i64 * Instruction->output;
            *(int*)((char*)(&sqvm->_stackOfCurrentFunction) + offset + 4) += factor * (unsigned __int16)Instruction->arg3;
            continue; }
        case _OP_TYPECAST:
            if (!sq_op_typecast(
                sqvm,
                &sqvm->_stackOfCurrentFunction[Instruction->output],
                &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
                Instruction->arg1))
            {
                const char* arg1TypeName = sq_getTypeName(Instruction->arg1);
                const char* arg2TypeName = sq_getTypeName(sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]._Type);
                SQVMRaise_Error(sqvm, "Cannot cast type \"%s\" to \"%s\"", arg2TypeName, arg1TypeName);
                goto label_errorHandler;
            }
            continue;
        case _OP_TYPECHECK: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if ((_DWORD)target->_Type == (unsigned int)Instruction->arg1 || (unsigned int)Instruction->arg1 == OT_ENTITY && target->_Type == OT_NULL) {
                const char* objectTypeName = sq_getTypeName(target->_Type);
                const char* testTypeName = sq_getTypeName((unsigned int)Instruction->arg1);
                SQVMRaise_Error(sqvm, "Expected type \"%s\", found type \"%s\"", testTypeName, objectTypeName);
                goto label_errorHandler;
            }
            continue; }

        case _OP_TYPECHECK_ORNULL: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            if (target->_Type == (unsigned int)Instruction->arg1 || target->_Type == OT_NULL) {
                const char* objectTypeName = sq_getTypeName(target->_Type);
                const char* testedTypeName = sq_getTypeName((unsigned int)Instruction->arg1);
                SQVMRaise_Error(sqvm, "Expected type \"%s ornull\", found type \"%s\"", testedTypeName, objectTypeName);
                goto label_errorHandler;
            }
            continue; }

        case _OP_TYPECHECK_NOTNULL: {
            if (sqvm->_stackOfCurrentFunction[Instruction->output]._Type != OT_NULL) {
                SQVMRaise_Error(sqvm, "Expected non-null value, found \"null\"");
                goto label_errorHandler;
            }
            continue; }

        case _OP_CHECK_ENTITY_CLASS:

            if (!sq_op_check_entity_class(
                sqvm,
                &sqvm->_stackOfCurrentFunction[Instruction->arg1], *(_QWORD*)(*(_QWORD*)(sqvm->ci->closure._VAL.asInteger + 256) + 8i64 * (unsigned __int16)Instruction->arg2))) {
                goto label_errorHandler;
            }

            continue;
        case _OP_UNREACHABLE:
            SQVMRaise_Error(sqvm, "Execution reached script marked as unreachable");
            goto label_errorHandler;
        case _OP_ARRAY_RESIZE: {
            SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
            SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; //v310=val
            SQArray* targetArray = target->_VAL.asArray; //v309

            if (arg1Obj->_VAL.asInteger < 0)
            {
                SQVMRaise_Error(sqvm, "Cannot resize to negative size %d", (unsigned int)arg1Obj->_VAL.asInteger);
                goto label_errorHandler;
            }
            spdlog::info("Array values before BEF0{:X}", (long long)targetArray->_values);
            sub_BEF0(targetArray->_values, arg1Obj->_VAL.asInteger, g_ServerSquirrelManager->globalClosure);//global variable
            if ((targetArray->_usedSlots) <= targetArray->_allocated >> 2)
            {
				if (targetArray->_usedSlots > 4)
                    spdlog::info("Array values before C6F0{:X}", (long long)targetArray->_values);
					sub_C6F0(&targetArray->_values, targetArray->_usedSlots);
            }
            int* value = *(int**)(*(_QWORD*)(&sqvm->ci->closure._VAL.asClosure->gap_C8[8]) + 8i64 * (unsigned __int16)Instruction->arg2); //this is a closure but dont know the field this is a hacky fix
			if (targetArray->_usedSlots >= arg1Obj->_VAL.asInteger)
            {
                continue;
            }
            do
				sub_29A40(sqvm->sharedState, &targetArray->_values[(unsigned int)targetArray->_usedSlots++], value);
			while (targetArray->_usedSlots < arg1Obj->_VAL.asInteger);

            continue; }
        default:
            continue;
        }
    }


label_errorHandler: {
    SQObject currentError;//a2
    currentError._Type = sqvm->_lasterror._Type;//v311
    currentError._VAL = sqvm->_lasterror._VAL; //v312 
    INCREMENT_REFERENCECOUNT((&currentError));


    int lastTop = sqvm->_top; //v315
    int n = 0; //v316
    if (!sqvm->ci) {
        INCREMENT_REFERENCECOUNT((&currentError));
        DECREMENT_REFERENCECOUNT((&sqvm->_lasterror));
        sqvm->_lasterror = currentError;
        DECREMENT_REFERENCECOUNT((&currentError));
        sqvm->sharedState->_currentThreadMaybe = savedSharedState;
        --sqvm->_nnativecalls;
        return 0;
    }
    if (traps)
    {

        while (sqvm->ci->obj10._Type != OT_NATIVECLOSURE || !n)
        {
            if (sqvm->ci->_etraps[0] > 0) {
                sqvm->ci->ip = (SQInstruction*)(sqvm->field_E8 + 24i64 * (unsigned int)(sqvm->traps - 1) + 8);
                sqvm->_top = *(_DWORD*)(sqvm->field_E8 + 24 * (sqvm->traps - 1) + 4);
                sqvm->_stackbase = *(unsigned int*)(sqvm->field_E8 + 24 * (sqvm->traps - 1));//v324
                sqvm->_stackOfCurrentFunction = &sqvm->_stack[sqvm->_stackbase];
                SQObject* newError = (SQObject*)((char*)(&sqvm->_stack[sqvm->_stackbase]) + 16i64 * *(int*)(sqvm->field_E8 + 24 * (sqvm->traps - 1) + 16)); //325
                INCREMENT_REFERENCECOUNT((&currentError));
                DECREMENT_REFERENCECOUNT(newError);
                *newError = currentError;
                --sqvm->traps;
                --traps;
                --sqvm->ci->_etraps[0];
                if (lastTop >= sqvm->_top)
                    sq_dropStack(sqvm, lastTop);

                DECREMENT_REFERENCECOUNT((&currentError))
                    goto exeption_restore; //this continues execution in exeptionHander
            }
            sub_2EDB0(sqvm, (__int16*)&sqvm->ci->_root + 2);

            --sqvm->_callsstacksize;
            DECREMENT_REFERENCECOUNT((&sqvm->ci->obj10));
            sqvm->ci->obj10._Type = OT_NULL;
            sqvm->ci->obj10._VAL.asInteger = 0;

            DECREMENT_REFERENCECOUNT((&sqvm->ci->closure));
            sqvm->ci->closure._Type = OT_NULL;
            sqvm->ci->closure._VAL.asInteger = 0;

            if (sqvm->_callsstacksize)
            {
                sqvm->ci = &sqvm->_callsstack[sqvm->_callsstacksize - 1];
            }
            else
            {
                sqvm->ci = 0i64;
            }
            ++n;
            if (!sqvm->_callsstacksize)
                break;

        }
    }
    else if (throwerror)
        SQVMCallErrorHandler(sqvm, &currentError);
    if (sqvm->ci)
    {
        do
        {
            sqvm->_stackbase -= sqvm->ci->_etraps[1];
            sqvm->_stackOfCurrentFunction = &sqvm->_stack[sqvm->_stackbase];
            sqvm->_top = sqvm->_stackbase + sqvm->ci->_etraps[2];
            sub_2EDB0(sqvm, (__int16*)&sqvm->ci->_root + 2);
            --sqvm->_callsstacksize;
            DECREMENT_REFERENCECOUNT((&sqvm->ci->obj10))
                sqvm->ci->obj10._Type = OT_NULL;
            sqvm->ci->obj10._VAL.asInteger = 0;
            DECREMENT_REFERENCECOUNT((&sqvm->ci->closure))
                sqvm->ci->closure._Type = OT_NULL;
            sqvm->ci->closure._VAL.asInteger = 0;
            if (sqvm->_callsstacksize)
            {
                sqvm->ci = &sqvm->_callsstack[sqvm->_callsstacksize - 1];

                if (sqvm->ci && sqvm->ci->obj10._Type == OT_NATIVECLOSURE)
                    break;
            }
            else
            {
                sqvm->ci = 0i64;
            }
        } while ((sqvm->ci && (!sqvm->ci->_root)) && sqvm->_callsstacksize);
    }
    if (lastTop >= sqvm->_top)
        sq_dropStack(sqvm, lastTop);
    INCREMENT_REFERENCECOUNT((&currentError));
    DECREMENT_REFERENCECOUNT((&sqvm->_lasterror));
    sqvm->_lasterror = currentError;
    DECREMENT_REFERENCECOUNT((&currentError));

    sharedState->_currentThreadMaybe = savedSharedState;
    --sqvm->_nnativecalls;
    return 0;
    }


	
}

float fsqrt(float x)
{
	return sqrtf(x);
}


const char* sq_getTypeName(int type)
{
    switch (type) {
    case OT_ASSET:
        return "asset";
    case OT_INTEGER:
        return "int";
    case OT_BOOL:
        return "bool";
    case SQOBJECT_NUMERIC:
        return "float or int";
    case OT_NULL:
        return "null";
    case OT_VECTOR:
        return "vector";
    case 0:
        return "var";
    case OT_USERDATA:
        return "userdata";
    case OT_FLOAT:
        return "float";
    case OT_STRING:
        return "string";
    case 0x8000040:
        return "array";
    case 0x8000200:
        return "function";
    case 0x8100000:
        return "structdef";
    case OT_THREAD:
        return "thread";
    case OT_FUNCPROTO:
        return "function";
    case OT_CLAAS:
        return "class";
    case OT_WEAKREF:
        return "weakref";
    case 0x8080000:
        return "unimplemented function";
    case 0x8200000:
        return "struct instance";
    case 0xA000020:
        return "table";
    case 0xA008000:
        return "instance";
    case 0xA400000:
        return "entity";

    }
    return 0;
}


int sqObjectToString(char* buf, int bufLength, SQObject* obj) { //returns string length
    const char* typeName = sq_getTypeName(obj->_Type);
    int length;
    switch(obj->_Type) {
    case OT_FLOAT:
        length = sprintf_s(buf, bufLength, "Object of type float with a value of %f", obj->_VAL.asFloat);
        break;
    case OT_CLOSURE:
        length = sprintf_s(buf, bufLength, "Object of type Closure with the name %s", obj->_VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->_val);
        break;
    case OT_NATIVECLOSURE:
        length = sprintf_s(buf, bufLength, "Object of type Nativeclosure with the name %s", obj->_VAL.asNativeClosure->_name->_val);
        break;
    case OT_STRING:
        length = sprintf_s(buf, bufLength, "Object of type String with value %s", obj->_VAL.asString->_val);
        break;
    default:
        const char* typeName = sq_getTypeName(obj->_Type);
        length = sprintf_s(buf, bufLength, "Object of type %s with integer value %p", typeName, (void*)obj->_VAL.asInteger);
    }
    return length;
}