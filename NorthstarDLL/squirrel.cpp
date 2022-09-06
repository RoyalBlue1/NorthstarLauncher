#include "pch.h"
#include "squirrel.h"
#include "concommand.h"
#include "modmanager.h"
#include "dedicated.h"
#include "r2engine.h"
#include "tier0.h"

AUTOHOOK_INIT()
ConVar* CVar_ns_add_line_opcode;

const char* GetContextName(ScriptContext context)
{
	switch (context)
	{
	case ScriptContext::CLIENT:
		return "CLIENT";
	case ScriptContext::SERVER:
		return "SERVER";
	case ScriptContext::UI:
		return "UI";
	default:
		return "UNKNOWN";
	}
}

eSQReturnType SQReturnTypeFromString(const char* pReturnType)
{
	static const std::map<std::string, eSQReturnType> sqReturnTypeNameToString = {
		{"bool", eSQReturnType::Boolean},
		{"float", eSQReturnType::Float},
		{"vector", eSQReturnType::Vector},
		{"int", eSQReturnType::Integer},
		{"entity", eSQReturnType::Entity},
		{"string", eSQReturnType::String},
		{"array", eSQReturnType::Arrays},
		{"asset", eSQReturnType::Asset},
		{"table", eSQReturnType::Table}};

	if (sqReturnTypeNameToString.find(pReturnType) != sqReturnTypeNameToString.end())
		return sqReturnTypeNameToString.at(pReturnType);
	else
		return eSQReturnType::Default; // previous default value
}

const char* sq_OpToString(int op)
{
	switch (op)
	{
	case _OP_LINE:
		return "_OP_LINE";
	case _OP_LOAD:
		return "_OP_LOAD";
	case _OP_LOADCOPY:
		return "_OP_LOADCOPY";
	case _OP_LOADINT:
		return "_OP_LOADINT";
	case _OP_LOADFLOAT:
		return "_OP_LOADFLOAT";
	case _OP_DLOAD:
		return "_OP_DLOAD";
	case _OP_TAILCALL:
		return "_OP_TAILCALL";
	case _OP_CALL:
		return "_OP_CALL";
	case _OP_PREPCALL:
		return "_OP_PREPCALL";
	case _OP_PREPCALLK:
		return "_OP_PREPCALLK";
	case _OP_GETK:
		return "_OP_GETK";
	case _OP_MOVE:
		return "_OP_MOVE";
	case _OP_NEWSLOT:
		return "_OP_NEWSLOT";
	case _OP_DELETE:
		return "_OP_DELETE";
	case _OP_SET:
		return "_OP_SET";
	case _OP_GET:
		return "_OP_GET";
	case _OP_EQ:
		return "_OP_EQ";
	case _OP_NE:
		return "_OP_NE";
	case _OP_ARITH:
		return "_OP_ARITH";
	case _OP_BITW:
		return "_OP_BITW";
	case _OP_RETURN:
		return "_OP_RETURN";
	case _OP_LOADNULLS:
		return "_OP_LOADNULLS";
	case _OP_LOADROOTTABLE:
		return "_OP_LOADROOTTABLE";
	case _OP_LOADBOOL:
		return "_OP_LOADBOOL";
	case _OP_DMOVE:
		return "_OP_DMOVE";
	case _OP_JMP:
		return "_OP_JMP";
	case _OP_JNZ:
		return "_OP_JNZ";
	case _OP_JZ:
		return "_OP_JZ";
	case _OP_LOADFREEVAR:
		return "_OP_LOADFREEVAR";
	case _OP_VARGC:
		return "_OP_VARGC";
	case _OP_GETVARGV:
		return "_OP_GETVARGV";
	case _OP_NEWTABLE:
		return "_OP_NEWTABLE";
	case _OP_NEWARRAY:
		return "_OP_NEWARRAY";
	case _OP_APPENDARRAY:
		return "_OP_APPENDARRAY";
	case _OP_GETPARENT:
		return "_OP_GETPARENT";
	case _OP_COMPOUND_ARITH:
		return "_OP_COMPOUND_ARITH";
	case _OP_COMPOUND_ARITH_LOCAL:
		return "_OP_COMPOUND_ARITH_LOCAL";
	case _OP_INCREMENT_PREFIX:
		return "_OP_INCREMENT_PREFIX";
	case _OP_INCREMENT_PREFIX_LOCAL:
		return "_OP_INCREMENT_PREFIX_LOCAL";
	case _OP_INCREMENT_PREFIX_STRUCTFIELD:
		return "_OP_INCREMENT_PREFIX_STRUCTFIELD";
	case _OP_INCREMENT_POSTFIX:
		return "_OP_INCREMENT_POSTFIX";
	case _OP_INCREMENT_POSTFIX_LOCAL:
		return "_OP_INCREMENT_POSTFIX_LOCAL";
	case _OP_INCREMENT_POSTFIX_STRUCTFIELD:
		return "_OP_INCREMENT_POSTFIX_STRUCTFIELD";
	case _OP_CMP:
		return "_OP_CMP";
	case _OP_EXISTS:
		return "_OP_EXISTS";
	case _OP_INSTANCEOF:
		return "_OP_INSTANCEOF";
	case _OP_NEG:
		return "_OP_NEG";
	case _OP_NOT:
		return "_OP_NOT";
	case _OP_BWNOT:
		return "_OP_BWNOT";
	case _OP_CLOSURE:
		return "_OP_CLOSURE";
	case _OP_FOREACH:
		return "_OP_FOREACH";
	case _OP_FOREACH_STATICARRAY_START:
		return "_OP_FOREACH_STATICARRAY_START";
	case _OP_FOREACH_STATICARRAY_NEXT:
		return "_OP_FOREACH_STATICARRAY_NEXT";
	case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_START:
		return "_OP_FOREACH_STATICARRAY_NESTEDSTRUCT_START";
	case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_NEXT:
		return "_OP_FOREACH_STATICARRAY_NESTEDSTRUCT_NEXT";
	case _OP_DELEGATE:
		return "_OP_DELEGATE";
	case _OP_CLONE:
		return "_OP_CLONE";
	case _OP_TYPEOF:
		return "_OP_TYPEOF";
	case _OP_PUSHTRAP:
		return "_OP_PUSHTRAP";
	case _OP_POPTRAP:
		return "_OP_POPTRAP";
	case _OP_THROW:
		return "_OP_THROW";
	case _OP_CLASS:
		return "_OP_CLASS";
	case _OP_NEWSLOTA:
		return "_OP_NEWSLOTA";
	case _OP_EQ_LITERAL:
		return "_OP_EQ_LITERAL";
	case _OP_NE_LITERAL:
		return "_OP_NE_LITERAL";
	case _OP_FOREACH_SETUP:
		return "_OP_FOREACH_SETUP";
	case _OP_ASSERT_FAILED:
		return "_OP_ASSERT_FAILED";
	case _OP_ADD:
		return "_OP_ADD";
	case _OP_SUB:
		return "_OP_SUB";
	case _OP_MUL:
		return "_OP_MUL";
	case _OP_DIV:
		return "_OP_DIV";
	case _OP_MOD:
		return "_OP_MOD";
	case _OP_PREPCALLK_CALL:
		return "_OP_PREPCALLK_CALL";
	case _OP_PREPCALLK_MOVE_CALL:
		return "_OP_PREPCALLK_MOVE_CALL";
	case _OP_PREPCALLK_LOADINT_CALL:
		return "_OP_PREPCALLK_LOADINT_CALL";
	case _OP_CMP_JZ:
		return "_OP_CMP_JZ";
	case _OP_INCREMENT_LOCAL_DISCARD_JMP:
		return "_OP_INCREMENT_LOCAL_DISCARD_JMP";
	case _OP_JZ_RETURN:
		return "_OP_JZ_RETURN";
	case _OP_JZ_LOADBOOL_RETURN:
		return "_OP_JZ_LOADBOOL_RETURN";
	case _OP_NEWVECTOR:
		return "_OP_NEWVECTOR";
	case _OP_ZEROVECTOR:
		return "_OP_ZEROVECTOR";
	case _OP_GET_VECTOR_COMPONENT:
		return "_OP_GET_VECTOR_COMPONENT";
	case _OP_SET_VECTOR_COMPONENT:
		return "_OP_SET_VECTOR_COMPONENT";
	case _OP_VECTOR_COMPONENT_MINUSEQ:
		return "_OP_VECTOR_COMPONENT_MINUSEQ";
	case _OP_VECTOR_COMPONENT_PLUSEQ:
		return "_OP_VECTOR_COMPONENT_PLUSEQ";
	case _OP_VECTOR_COMPONENT_MULEQ:
		return "_OP_VECTOR_COMPONENT_MULEQ";
	case _OP_VECTOR_COMPONENT_DIVEQ:
		return "_OP_VECTOR_COMPONENT_DIVEQ";
	case _OP_VECTOR_NORMALIZE:
		return "_OP_VECTOR_NORMALIZE";
	case _OP_VECTOR_NORMALIZE_IN_PLACE:
		return "_OP_VECTOR_NORMALIZE_IN_PLACE";
	case _OP_VECTOR_DOT_PRODUCT:
		return "_OP_VECTOR_DOT_PRODUCT";
	case _OP_VECTOR_DOT_PRODUCT2D:
		return "_OP_VECTOR_DOT_PRODUCT2D";
	case _OP_VECTOR_CROSS_PRODUCT:
		return "_OP_VECTOR_CROSS_PRODUCT";
	case _OP_VECTOR_CROSS_PRODUCT2D:
		return "_OP_VECTOR_CROSS_PRODUCT2D";
	case _OP_VECTOR_LENGTH:
		return "_OP_VECTOR_LENGTH";
	case _OP_VECTOR_LENGTHSQR:
		return "_OP_VECTOR_LENGTHSQR";
	case _OP_VECTOR_LENGTH2D:
		return "_OP_VECTOR_LENGTH2D";
	case _OP_VECTOR_LENGTH2DSQR:
		return "_OP_VECTOR_LENGTH2DSQR";
	case _OP_VECTOR_DISTANCE:
		return "_OP_VECTOR_DISTANCE";
	case _OP_VECTOR_DISTANCESQR:
		return "_OP_VECTOR_DISTANCESQR";
	case _OP_VECTOR_DISTANCE2D:
		return "_OP_VECTOR_DISTANCE2D";
	case _OP_VECTOR_DISTANCE2DSQR:
		return "_OP_VECTOR_DISTANCE2DSQR";
	case _OP_INCREMENT_LOCAL_DISCARD:
		return "_OP_INCREMENT_LOCAL_DISCARD";
	case _OP_FASTCALL:
		return "_OP_FASTCALL";
	case _OP_FASTCALL_NATIVE:
		return "_OP_FASTCALL_NATIVE";
	case _OP_FASTCALL_NATIVE_ARGTYPECHECK:
		return "_OP_FASTCALL_NATIVE_ARGTYPECHECK";
	case _OP_FASTCALL_ENV:
		return "_OP_FASTCALL_ENV";
	case _OP_FASTCALL_NATIVE_ENV:
		return "_OP_FASTCALL_NATIVE_ENV";
	case _OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK:
		return "_OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK";
	case _OP_LOADGLOBALARRAY:
		return "_OP_LOADGLOBALARRAY";
	case _OP_GETGLOBAL:
		return "_OP_GETGLOBAL";
	case _OP_SETGLOBAL:
		return "_OP_SETGLOBAL";
	case _OP_COMPOUND_ARITH_GLOBAL:
		return "_OP_COMPOUND_ARITH_GLOBAL";
	case _OP_GETSTRUCTFIELD:
		return "_OP_GETSTRUCTFIELD";
	case _OP_SETSTRUCTFIELD:
		return "_OP_SETSTRUCTFIELD";
	case _OP_COMPOUND_ARITH_STRUCTFIELD:
		return "_OP_COMPOUND_ARITH_STRUCTFIELD";
	case _OP_NEWSTRUCT:
		return "_OP_NEWSTRUCT";
	case _OP_GETSUBSTRUCT:
		return "_OP_GETSUBSTRUCT";
	case _OP_GETSUBSTRUCT_DYNAMIC:
		return "_OP_GETSUBSTRUCT_DYNAMIC";
	case _OP_TYPECAST:
		return "_OP_TYPECAST";
	case _OP_TYPECHECK:
		return "_OP_TYPECHECK";
	case _OP_TYPECHECK_ORNULL:
		return "_OP_TYPECHECK_ORNULL";
	case _OP_TYPECHECK_NOTNULL:
		return "_OP_TYPECHECK_NOTNULL";
	case _OP_CHECK_ENTITY_CLASS:
		return "_OP_CHECK_ENTITY_CLASS";
	case _OP_UNREACHABLE:
		return "_OP_UNREACHABLE";
	case _OP_ARRAY_RESIZE:
		return "_OP_ARRAY_RESIZE";
	default:
		return "UNKNOWN";
	}
}

const char* SQTypeNameFromID(int type)
{
	switch (type)
	{
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
	case OT_ARRAY:
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
	case OT_TABLE:
		return "table";
	case 0xA008000:
		return "instance";
	case OT_ENTITY:
		return "entity";
	}
	return "";
}

// needed to define implementations for squirrelmanager outside of squirrel.h without compiler errors
template class SquirrelManager<ScriptContext::SERVER>;
template class SquirrelManager<ScriptContext::CLIENT>;
template class SquirrelManager<ScriptContext::UI>;

template <ScriptContext context> void SquirrelManager<context>::VMCreated(CSquirrelVM* newSqvm)
{
	m_pSQVM = newSqvm;

	for (SQFuncRegistration* funcReg : m_funcRegistrations)
	{
		spdlog::info("Registering {} function {}", GetContextName(context), funcReg->squirrelFuncName);
		RegisterSquirrelFunc(m_pSQVM, funcReg, 1);
	}

	for (auto& pair : g_pModManager->m_DependencyConstants)
	{
		bool bWasFound = false;
		for (Mod& dependency : g_pModManager->m_LoadedMods)
		{
			if (!dependency.m_bEnabled)
				continue;

			if (dependency.Name == pair.second)
			{
				bWasFound = true;
				break;
			}
		}

		defconst(m_pSQVM, pair.first.c_str(), bWasFound);
	}
}

template <ScriptContext context> void SquirrelManager<context>::VMDestroyed()
{
	m_pSQVM = nullptr;
}

template <ScriptContext context> void SquirrelManager<context>::ExecuteCode(const char* pCode)
{
	if (!m_pSQVM || !m_pSQVM->sqvm)
	{
		spdlog::error("Cannot execute code, {} squirrel vm is not initialised", GetContextName(context));
		return;
	}

	spdlog::info("Executing {} script code {} ", GetContextName(context), pCode);

	std::string strCode(pCode);
	CompileBufferState bufferState = CompileBufferState(strCode);

	SQRESULT compileResult = compilebuffer(&bufferState, "console");
	spdlog::info("sq_compilebuffer returned {}", PrintSQRESULT.at(compileResult));

	if (compileResult != SQRESULT_ERROR)
	{
		pushroottable(m_pSQVM->sqvm);
		SQRESULT callResult = call(m_pSQVM->sqvm, 0);
		spdlog::info("sq_call returned {}", PrintSQRESULT.at(callResult));
	}
}

template <ScriptContext context> void SquirrelManager<context>::AddFuncRegistration(
	std::string returnType, std::string name, std::string argTypes, std::string helpText, SQFunction func)
{
	SQFuncRegistration* reg = new SQFuncRegistration;

	reg->squirrelFuncName = new char[name.size() + 1];
	strcpy((char*)reg->squirrelFuncName, name.c_str());
	reg->cppFuncName = reg->squirrelFuncName;

	reg->helpText = new char[helpText.size() + 1];
	strcpy((char*)reg->helpText, helpText.c_str());

	reg->returnTypeString = new char[returnType.size() + 1];
	strcpy((char*)reg->returnTypeString, returnType.c_str());
	reg->returnType = SQReturnTypeFromString(returnType.c_str());

	reg->argTypes = new char[argTypes.size() + 1];
	strcpy((char*)reg->argTypes, argTypes.c_str());

	reg->funcPtr = func;

	m_funcRegistrations.push_back(reg);
}

template <ScriptContext context> SQRESULT SquirrelManager<context>::setupfunc(const SQChar* funcname)
{
	pushroottable(m_pSQVM->sqvm);
	pushstring(m_pSQVM->sqvm, funcname, -1);

	SQRESULT result = get(m_pSQVM->sqvm, -2);
	if (result != SQRESULT_ERROR)
		pushroottable(m_pSQVM->sqvm);

	return result;
}

template <ScriptContext context> void SquirrelManager<context>::AddFuncOverride(std::string name, SQFunction func)
{
	m_funcOverrides[name] = func;
}

// hooks
bool IsUIVM(ScriptContext context, HSquirrelVM* pSqvm)
{
	return context != ScriptContext::SERVER && g_pSquirrel<ScriptContext::UI>->m_pSQVM &&
		   g_pSquirrel<ScriptContext::UI>->m_pSQVM->sqvm == pSqvm;
}

template <ScriptContext context> void* (*sq_compiler_create)(HSquirrelVM* sqvm, void* a2, void* a3, SQBool bShouldThrowError);
template <ScriptContext context> void* sq_compiler_createHook(HSquirrelVM* sqvm, void* a2, void* a3, SQBool bShouldThrowError)
{
	// store whether errors generated from this compile should be fatal
	if (IsUIVM(context, sqvm))
		g_pSquirrel<ScriptContext::UI>->m_bFatalCompilationErrors = bShouldThrowError;
	else
		g_pSquirrel<context>->m_bFatalCompilationErrors = bShouldThrowError;

	return sq_compiler_create<context>(sqvm, a2, a3, bShouldThrowError);
}

template <ScriptContext context> SQInteger (*SQPrint)(HSquirrelVM* sqvm, const char* fmt);
template <ScriptContext context> SQInteger SQPrintHook(HSquirrelVM* sqvm, const char* fmt, ...)
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

template <ScriptContext context> CSquirrelVM* (*CreateNewVM)(void* a1, ScriptContext realContext);
template <ScriptContext context> CSquirrelVM* CreateNewVMHook(void* a1, ScriptContext realContext)
{
	CSquirrelVM* sqvm = CreateNewVM<context>(a1, realContext);
	if (realContext == ScriptContext::UI)
		g_pSquirrel<ScriptContext::UI>->VMCreated(sqvm);
	else
		g_pSquirrel<context>->VMCreated(sqvm);

	spdlog::info("CreateNewVM {} {}", GetContextName(realContext), (void*)sqvm);
	return sqvm;
}

template <ScriptContext context> void (*DestroyVM)(void* a1, HSquirrelVM* sqvm);
template <ScriptContext context> void DestroyVMHook(void* a1, HSquirrelVM* sqvm)
{
	ScriptContext realContext = context; // ui and client use the same function so we use this for prints
	if (IsUIVM(context, sqvm))
	{
		realContext = ScriptContext::UI;
		g_pSquirrel<ScriptContext::UI>->VMDestroyed();
	}
	else
		DestroyVM<context>(a1, sqvm);

	spdlog::info("DestroyVM {} {}", GetContextName(realContext), (void*)sqvm);
}

template <ScriptContext context> void (*SQCompileError)(HSquirrelVM* sqvm, const char* error, const char* file, int line, int column);
template <ScriptContext context> void ScriptCompileErrorHook(HSquirrelVM* sqvm, const char* error, const char* file, int line, int column)
{
	bool bIsFatalError = g_pSquirrel<context>->m_bFatalCompilationErrors;
	ScriptContext realContext = context; // ui and client use the same function so we use this for prints
	if (IsUIVM(context, sqvm))
	{
		realContext = ScriptContext::UI;
		bIsFatalError = g_pSquirrel<ScriptContext::UI>->m_bFatalCompilationErrors;
	}

	spdlog::error("{} SCRIPT COMPILE ERROR {}", GetContextName(realContext), error);
	spdlog::error("{} line [{}] column [{}]", file, line, column);

	// use disconnect to display an error message for the compile error, but only if the compilation error was fatal
	// todo, we could get this from sqvm itself probably, rather than hooking sq_compiler_create
	if (bIsFatalError)
	{
		// kill dedicated server if we hit this
		if (IsDedicatedServer())
			abort();
		else
		{
			R2::Cbuf_AddText(
				R2::Cbuf_GetCurrentPlayer(),
				fmt::format("disconnect \"Encountered {} script compilation error, see console for details.\"", GetContextName(realContext))
					.c_str(),
				R2::cmd_source_t::kCommandSrcCode);

			// likely temp: show console so user can see any errors, as error message wont display if ui is dead
			// maybe we could disable all mods other than the coremods and try a reload before doing this?
			// could also maybe do some vgui bullshit to show something visually rather than console
			if (realContext == ScriptContext::UI)
				R2::Cbuf_AddText(R2::Cbuf_GetCurrentPlayer(), "showconsole", R2::cmd_source_t::kCommandSrcCode);
		}
	}

	// dont call the original function since it kills game lol
}

struct workingInstruction
{
	int originalPosition;
	int newPosition;
	SQInstruction instruction;
};
#define PRINT_INSTRUCTION_DEBUG
template <ScriptContext context> SQFunctionProto* (*SQFuncState__BuildProto)(SQFuncState* funcState);
template <ScriptContext context> SQFunctionProto* SQFuncState__BuildProtoHook(SQFuncState* funcState)
{
#ifdef PRINT_INSTRUCTION_DEBUG
	const char* debugFunctionName = "GameMode_VerifyModes";
	bool shouldPrintFunc = 
		funcState->funcName._Type == OT_STRING && (strcmp(funcState->funcName._VAL.asString->_val, debugFunctionName) == 0);
	/* for (int i = 0; i < funcState->_instructionsSize; i++)
	{
		if (funcState->_instructions[i].op == _OP_FOREACH_STATICARRAY_NEXT)
		{
			shouldPrintFunc = true;
			break;
		}
	}*/
	if (shouldPrintFunc)
	{
		char* funcName = funcState->funcName._VAL.asString->_val;
		spdlog::info("Function compile name:{}", std::string(funcName));
		for (int i = 0; i < funcState->_instructionsSize; i++)
		{
			workingInstruction inst;
			inst.originalPosition = i;
			inst.newPosition = -1;
			inst.instruction = funcState->_instructions[i];
			spdlog::info(
				"Index {} Instruction {} arg1 {} arg0 {} arg2 {} arg3 {}",
				i,
				std::string(sq_OpToString(inst.instruction.op)),
				inst.instruction.arg1,
				inst.instruction.output,
				inst.instruction.arg2,
				inst.instruction.arg3);
		}
		for (int i = 0; i < funcState->_lineInfoSize; i++)
		{
				spdlog::info("LineInfo Line: {} instruction {}", funcState->_lineInfoData[i].line, funcState->_lineInfoData[i].op);
		}
		for (int i = 0; i < funcState->_localvarinfoSize; i++)
		{
			SQLocalVarInfo* info = &funcState->_localvarinfoData[i];
			if (info->name._Type == OT_STRING)
			{
				spdlog::info(
					"VarInfo name {} qword10 {:X} endOp {} startOp {} dword20 {} dword24 {}",
					std::string(info->name._VAL.asString->_val),
					info->qword10,
					info->_end_op,
					info->_start_op,
					info->dword20,
					info->dword24);
			}
			else
			{
				spdlog::info(
					"VarInfo name UNKNOWN qword10 {:X} endOp {} startOp {} dword20 {} dword24 {}",
					info->qword10,
					info->_end_op,
					info->_start_op,
					info->dword20,
					info->dword24);
			
			}
		}
	}
#endif
	std::vector<workingInstruction> newInstructions;
	for (int i = 0; i < funcState->_instructionsSize; i++)
	{
		workingInstruction inst;
		inst.originalPosition = i;
		inst.newPosition = -1;
		inst.instruction = funcState->_instructions[i];
		for (int j = 0; j < funcState->_lineInfoSize; j++)
		{
			if (funcState->_lineInfoData[j].op == i)
			{
				workingInstruction lineInst;
				lineInst.instruction.op = _OP_LINE;
				lineInst.instruction.arg1 = funcState->_lineInfoData[j].line;
				lineInst.instruction.output = 0;
				lineInst.instruction.arg2 = 0;
				lineInst.instruction.arg3 = 0;
				lineInst.originalPosition = -1;
				lineInst.newPosition = newInstructions.size();
				newInstructions.push_back(lineInst);
			}
		}
		inst.newPosition = newInstructions.size();
		newInstructions.push_back(inst);
	}
	for (auto& inst : newInstructions)
	{
		switch (inst.instruction.op)
		{
		case _OP_JMP:
		case _OP_JZ:
		case _OP_JNZ:
		case _OP_INCREMENT_LOCAL_DISCARD_JMP:
			for (auto& inst2 : newInstructions)
			{
				if (inst2.originalPosition == inst.originalPosition + inst.instruction.arg1)
				{
					inst.instruction.arg1 = inst2.newPosition - inst.newPosition;
					break;
				}
			}
			break;
		case _OP_FOREACH:
			if (inst.instruction.output == 0)
				break;
			for (auto& inst2 : newInstructions)
			{
				if (inst2.originalPosition == inst.originalPosition + inst.instruction.output)
				{
					inst.instruction.output = inst2.newPosition - inst.newPosition;
					inst2.instruction.arg1 = inst.instruction.output*-1;
					break;
				}
			}
			break;
		case _OP_FOREACH_STATICARRAY_NEXT:
		case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_NEXT:
			for (auto& inst2 : newInstructions)
			{
				if (inst2.originalPosition == inst.originalPosition + inst.instruction.output)
				{
					inst.instruction.output = inst2.newPosition - inst.newPosition;
					break;
				}
			}
			break;
		}
	}

	while (funcState->_instructionsAllocatedSize < newInstructions.size())
	{
		g_pSquirrel<context>->reallocinstructionvector(&funcState->_instructions, funcState->_instructionsAllocatedSize * 2);
	}

	for (int i = 0; i < funcState->_lineInfoSize; i++)
	{

		for (auto& inst : newInstructions)
		{
			if (inst.originalPosition == funcState->_lineInfoData[i].op)
			{
				funcState->_lineInfoData[i].op = inst.newPosition-1;
				break;
			}
		}
	}

	for (int i = 0; i < funcState->_localvarinfoSize; i++)
	{
		SQLocalVarInfo* info = &funcState->_localvarinfoData[i];
		for (auto& inst : newInstructions)
		{
			if (inst.originalPosition == info->_start_op)
			{
				info->_start_op = inst.newPosition;
				break;
			}
		}
		for (auto& inst : newInstructions)
		{
			if (inst.originalPosition == info->_end_op)
			{
				info->_end_op = inst.newPosition;
				break;
			}
		}
	}
			

	for (int i = 0; i < newInstructions.size(); i++)
	{
		workingInstruction inst = newInstructions[i];
		funcState->_instructions[i] = inst.instruction;
	}
	funcState->_instructionsSize = newInstructions.size();

	#ifdef PRINT_INSTRUCTION_DEBUG
	if (shouldPrintFunc)
	{
		char* funcName = funcState->funcName._VAL.asString->_val;
		spdlog::info("Function compile name:{}", std::string(funcName));
		for (int i = 0; i < funcState->_instructionsSize; i++)
		{
			workingInstruction inst;
			inst.originalPosition = i;
			inst.newPosition = -1;
			inst.instruction = funcState->_instructions[i];
			spdlog::info(
				"Index {} Instruction {} arg1 {} arg0 {} arg2 {} arg3 {}",
				i,
				std::string(sq_OpToString(inst.instruction.op)),
				inst.instruction.arg1,
				inst.instruction.output,
				inst.instruction.arg2,
				inst.instruction.arg3);
		}
		for (int i = 0; i < funcState->_lineInfoSize; i++)
		{
			spdlog::info("LineInfo Line: {} instruction {}", funcState->_lineInfoData[i].line, funcState->_lineInfoData[i].op);
		}
		for (int i = 0; i < funcState->_localvarinfoSize; i++)
		{
			SQLocalVarInfo* info = &funcState->_localvarinfoData[i];
			if (info->name._Type == OT_STRING)
			{
				spdlog::info(
					"VarInfo name {} qword10 {:X} endOp {} startOp {} dword20 {} dword24 {}",
					std::string(info->name._VAL.asString->_val),
					info->qword10,
					info->_end_op,
					info->_start_op,
					info->dword20,
					info->dword24);
			}
			else
			{
				spdlog::info(
					"VarInfo name UNKNOWN qword10 {:X} endOp {} startOp {} dword20 {} dword24 {}",
					info->qword10,
					info->_end_op,
					info->_start_op,
					info->dword20,
					info->dword24);
			}
		}
	}
#endif

	return SQFuncState__BuildProto<context>(funcState);
}

template <ScriptContext context> int64_t (*RegisterSquirrelFunction)(CSquirrelVM* sqvm, SQFuncRegistration* funcReg, char unknown);
template <ScriptContext context> int64_t RegisterSquirrelFunctionHook(CSquirrelVM* sqvm, SQFuncRegistration* funcReg, char unknown)
{
	if (IsUIVM(context, sqvm->sqvm))
	{
		if (g_pSquirrel<ScriptContext::UI>->m_funcOverrides.count(funcReg->squirrelFuncName))
		{
			g_pSquirrel<ScriptContext::UI>->m_funcOriginals[funcReg->squirrelFuncName] = funcReg->funcPtr;
			funcReg->funcPtr = g_pSquirrel<ScriptContext::UI>->m_funcOverrides[funcReg->squirrelFuncName];
			spdlog::info("Replacing {} in UI", std::string(funcReg->squirrelFuncName));
		}

		return g_pSquirrel<ScriptContext::UI>->RegisterSquirrelFunc(sqvm, funcReg, unknown);
	}

	if (g_pSquirrel<context>->m_funcOverrides.find(funcReg->squirrelFuncName) != g_pSquirrel<context>->m_funcOverrides.end())
	{
		g_pSquirrel<context>->m_funcOriginals[funcReg->squirrelFuncName] = funcReg->funcPtr;
		funcReg->funcPtr = g_pSquirrel<context>->m_funcOverrides[funcReg->squirrelFuncName];
		spdlog::info("Replacing {} in {}", std::string(funcReg->squirrelFuncName),std::string(GetContextName(context)));
	}

	return g_pSquirrel<context>->RegisterSquirrelFunc(sqvm, funcReg, unknown);
}

template <ScriptContext context> bool (*CallScriptInitCallback)(void* sqvm, const char* callback);
template <ScriptContext context> bool CallScriptInitCallbackHook(void* sqvm, const char* callback)
{
	ScriptContext realContext = context;
	bool bShouldCallCustomCallbacks = true;

	if (context == ScriptContext::CLIENT)
	{
		if (!strcmp(callback, "UICodeCallback_UIInit"))
			realContext = ScriptContext::UI;
		else if (strcmp(callback, "ClientCodeCallback_MapSpawn"))
			bShouldCallCustomCallbacks = false;
	}
	else if (context == ScriptContext::SERVER)
		bShouldCallCustomCallbacks = !strcmp(callback, "CodeCallback_MapSpawn");

	if (bShouldCallCustomCallbacks)
	{
		for (Mod mod : g_pModManager->m_LoadedMods)
		{
			if (!mod.m_bEnabled)
				continue;

			for (ModScript script : mod.Scripts)
			{
				for (ModScriptCallback modCallback : script.Callbacks)
				{
					if (modCallback.Context == realContext && modCallback.BeforeCallback.length())
					{
						spdlog::info("Running custom {} script callback \"{}\"", GetContextName(realContext), modCallback.BeforeCallback);
						CallScriptInitCallback<context>(sqvm, modCallback.BeforeCallback.c_str());
					}
				}
			}
		}
	}

	spdlog::info("{} CodeCallback {} called", GetContextName(realContext), callback);
	if (!bShouldCallCustomCallbacks)
		spdlog::info("Not executing custom callbacks for CodeCallback {}", callback);
	bool ret = CallScriptInitCallback<context>(sqvm, callback);

	// run after callbacks
	if (bShouldCallCustomCallbacks)
	{
		for (Mod mod : g_pModManager->m_LoadedMods)
		{
			if (!mod.m_bEnabled)
				continue;

			for (ModScript script : mod.Scripts)
			{
				for (ModScriptCallback modCallback : script.Callbacks)
				{
					if (modCallback.Context == realContext && modCallback.AfterCallback.length())
					{
						spdlog::info("Running custom {} script callback \"{}\"", GetContextName(realContext), modCallback.AfterCallback);
						CallScriptInitCallback<context>(sqvm, modCallback.AfterCallback.c_str());
					}
				}
			}
		}
	}

	return ret;
}

template <ScriptContext context> void ConCommand_script(const CCommand& args)
{
	g_pSquirrel<context>->ExecuteCode(args.ArgS());
}

// literal class type that wraps a constant expression string
template <size_t N> struct TemplateStringLiteral
{
	constexpr TemplateStringLiteral(const char (&str)[N])
	{
		std::copy_n(str, N, value);
	}

	char value[N];
};

template <ScriptContext context, TemplateStringLiteral funcName> SQRESULT SQ_StubbedFunc(HSquirrelVM* sqvm)
{
	spdlog::info("Blocking call to stubbed function {} in {}", funcName.value, GetContextName(context));
	return SQRESULT_NULL;
}

template <ScriptContext context> void StubUnsafeSQFuncs()
{
	if (!Tier0::CommandLine()->CheckParm("-allowunsafesqfuncs"))
	{
		g_pSquirrel<context>->AddFuncOverride("DevTextBufferWrite", SQ_StubbedFunc<context, "DevTextBufferWrite">);
		g_pSquirrel<context>->AddFuncOverride("DevTextBufferClear", SQ_StubbedFunc<context, "DevTextBufferClear">);
		g_pSquirrel<context>->AddFuncOverride("DevTextBufferDumpToFile", SQ_StubbedFunc<context, "DevTextBufferDumpToFile">);
		g_pSquirrel<context>->AddFuncOverride("Dev_CommandLineAddParam", SQ_StubbedFunc<context, "Dev_CommandLineAddParam">);
		g_pSquirrel<context>->AddFuncOverride("DevP4Checkout", SQ_StubbedFunc<context, "DevP4Checkout">);
		g_pSquirrel<context>->AddFuncOverride("DevP4Add", SQ_StubbedFunc<context, "DevP4Add">);
	}
}

ON_DLL_LOAD_RELIESON("client.dll", ClientSquirrel, ConCommand, (CModule module))
{
	AUTOHOOK_DISPATCH_MODULE(client.dll)

	g_pSquirrel<ScriptContext::CLIENT> = new SquirrelManager<ScriptContext::CLIENT>;
	g_pSquirrel<ScriptContext::UI> = new SquirrelManager<ScriptContext::UI>;

	g_pSquirrel<ScriptContext::CLIENT>->__sq_defconst = module.Offset(0x12120).As<sq_defconstType>();
	g_pSquirrel<ScriptContext::UI>->__sq_defconst = g_pSquirrel<ScriptContext::CLIENT>->__sq_defconst;

	g_pSquirrel<ScriptContext::CLIENT>->__sq_compilebuffer = module.Offset(0x3110).As<sq_compilebufferType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_pushroottable = module.Offset(0x5860).As<sq_pushroottableType>();
	g_pSquirrel<ScriptContext::UI>->__sq_compilebuffer = g_pSquirrel<ScriptContext::CLIENT>->__sq_compilebuffer;
	g_pSquirrel<ScriptContext::UI>->__sq_pushroottable = g_pSquirrel<ScriptContext::CLIENT>->__sq_pushroottable;

	g_pSquirrel<ScriptContext::CLIENT>->__sq_call = module.Offset(0x8650).As<sq_callType>();
	g_pSquirrel<ScriptContext::UI>->__sq_call = g_pSquirrel<ScriptContext::CLIENT>->__sq_call;

	g_pSquirrel<ScriptContext::CLIENT>->__sq_newarray = module.Offset(0x39F0).As<sq_newarrayType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_arrayappend = module.Offset(0x3C70).As<sq_arrayappendType>();
	g_pSquirrel<ScriptContext::UI>->__sq_newarray = g_pSquirrel<ScriptContext::CLIENT>->__sq_newarray;
	g_pSquirrel<ScriptContext::UI>->__sq_arrayappend = g_pSquirrel<ScriptContext::CLIENT>->__sq_arrayappend;

	g_pSquirrel<ScriptContext::CLIENT>->__sq_newtable = module.Offset(0x3960).As<sq_newtableType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_newslot = module.Offset(0x70B0).As<sq_newslotType>();
	g_pSquirrel<ScriptContext::UI>->__sq_newtable = g_pSquirrel<ScriptContext::CLIENT>->__sq_newtable;
	g_pSquirrel<ScriptContext::UI>->__sq_newslot = g_pSquirrel<ScriptContext::CLIENT>->__sq_newslot;

	g_pSquirrel<ScriptContext::CLIENT>->__sq_pushstring = module.Offset(0x3440).As<sq_pushstringType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_pushinteger = module.Offset(0x36A0).As<sq_pushintegerType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_pushfloat = module.Offset(0x3800).As<sq_pushfloatType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_pushbool = module.Offset(0x3710).As<sq_pushboolType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_pushasset = module.Offset(0x3560).As<sq_pushassetType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_pushvector = module.Offset(0x3780).As<sq_pushvectorType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_raiseerror = module.Offset(0x8470).As<sq_raiseerrorType>();
	g_pSquirrel<ScriptContext::UI>->__sq_pushstring = g_pSquirrel<ScriptContext::CLIENT>->__sq_pushstring;
	g_pSquirrel<ScriptContext::UI>->__sq_pushinteger = g_pSquirrel<ScriptContext::CLIENT>->__sq_pushinteger;
	g_pSquirrel<ScriptContext::UI>->__sq_pushfloat = g_pSquirrel<ScriptContext::CLIENT>->__sq_pushfloat;
	g_pSquirrel<ScriptContext::UI>->__sq_pushbool = g_pSquirrel<ScriptContext::CLIENT>->__sq_pushbool;
	g_pSquirrel<ScriptContext::UI>->__sq_pushvector = g_pSquirrel<ScriptContext::CLIENT>->__sq_pushvector;
	g_pSquirrel<ScriptContext::UI>->__sq_pushasset = g_pSquirrel<ScriptContext::CLIENT>->__sq_pushasset;
	g_pSquirrel<ScriptContext::UI>->__sq_raiseerror = g_pSquirrel<ScriptContext::CLIENT>->__sq_raiseerror;

	g_pSquirrel<ScriptContext::CLIENT>->__sq_getstring = module.Offset(0x60C0).As<sq_getstringType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_getinteger = module.Offset(0x60E0).As<sq_getintegerType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_getfloat = module.Offset(0x6100).As<sq_getfloatType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_getbool = module.Offset(0x6130).As<sq_getboolType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_get = module.Offset(0x7C30).As<sq_getType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_getasset = module.Offset(0x6010).As<sq_getassetType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_getuserdata = module.Offset(0x63D0).As<sq_getuserdataType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_getvector = module.Offset(0x6140).As<sq_getvectorType>();
	g_pSquirrel<ScriptContext::UI>->__sq_getstring = g_pSquirrel<ScriptContext::CLIENT>->__sq_getstring;
	g_pSquirrel<ScriptContext::UI>->__sq_getinteger = g_pSquirrel<ScriptContext::CLIENT>->__sq_getinteger;
	g_pSquirrel<ScriptContext::UI>->__sq_getfloat = g_pSquirrel<ScriptContext::CLIENT>->__sq_getfloat;
	g_pSquirrel<ScriptContext::UI>->__sq_getbool = g_pSquirrel<ScriptContext::CLIENT>->__sq_getbool;
	g_pSquirrel<ScriptContext::UI>->__sq_get = g_pSquirrel<ScriptContext::CLIENT>->__sq_get;
	g_pSquirrel<ScriptContext::UI>->__sq_getasset = g_pSquirrel<ScriptContext::CLIENT>->__sq_getasset;
	g_pSquirrel<ScriptContext::UI>->__sq_getuserdata = g_pSquirrel<ScriptContext::CLIENT>->__sq_getuserdata;
	g_pSquirrel<ScriptContext::UI>->__sq_getvector = g_pSquirrel<ScriptContext::CLIENT>->__sq_getvector;

	g_pSquirrel<ScriptContext::CLIENT>->__sq_createuserdata = module.Offset(0x38D0).As<sq_createuserdataType>();
	g_pSquirrel<ScriptContext::CLIENT>->__sq_setuserdatatypeid = module.Offset(0x6490).As<sq_setuserdatatypeidType>();
	g_pSquirrel<ScriptContext::UI>->__sq_createuserdata = g_pSquirrel<ScriptContext::CLIENT>->__sq_createuserdata;
	g_pSquirrel<ScriptContext::UI>->__sq_setuserdatatypeid = g_pSquirrel<ScriptContext::CLIENT>->__sq_setuserdatatypeid;

	MAKEHOOK(
		module.Offset(0x108E0),
		&RegisterSquirrelFunctionHook<ScriptContext::CLIENT>,
		&g_pSquirrel<ScriptContext::CLIENT>->RegisterSquirrelFunc);
	g_pSquirrel<ScriptContext::UI>->RegisterSquirrelFunc = g_pSquirrel<ScriptContext::CLIENT>->RegisterSquirrelFunc;

	// uiscript_reset concommand: don't loop forever if compilation fails
	module.Offset(0x3C6E4C).NOP(6);

	MAKEHOOK(module.Offset(0x8AD0), &sq_compiler_createHook<ScriptContext::CLIENT>, &sq_compiler_create<ScriptContext::CLIENT>);

	MAKEHOOK(module.Offset(0x12B00), &SQPrintHook<ScriptContext::CLIENT>, &SQPrint<ScriptContext::CLIENT>);
	MAKEHOOK(module.Offset(0x12BA0), &SQPrintHook<ScriptContext::UI>, &SQPrint<ScriptContext::UI>);

	MAKEHOOK(module.Offset(0x26130), &CreateNewVMHook<ScriptContext::CLIENT>, &CreateNewVM<ScriptContext::CLIENT>);
	MAKEHOOK(module.Offset(0x26E70), &DestroyVMHook<ScriptContext::CLIENT>, &DestroyVM<ScriptContext::CLIENT>);
	MAKEHOOK(module.Offset(0x79A50), &ScriptCompileErrorHook<ScriptContext::CLIENT>, &SQCompileError<ScriptContext::CLIENT>);

	MAKEHOOK(module.Offset(0x10190), &CallScriptInitCallbackHook<ScriptContext::CLIENT>, &CallScriptInitCallback<ScriptContext::CLIENT>);

	RegisterConCommand("script_client", ConCommand_script<ScriptContext::CLIENT>, "Executes script code on the client vm", FCVAR_CLIENTDLL);
	RegisterConCommand("script_ui", ConCommand_script<ScriptContext::UI>, "Executes script code on the ui vm", FCVAR_CLIENTDLL);

	StubUnsafeSQFuncs<ScriptContext::CLIENT>();
	StubUnsafeSQFuncs<ScriptContext::UI>();
}

ON_DLL_LOAD_RELIESON("server.dll", ServerSquirrel, ConCommand, (CModule module))
{
	AUTOHOOK_DISPATCH_MODULE(server.dll)

	g_pSquirrel<ScriptContext::SERVER> = new SquirrelManager<ScriptContext::SERVER>;

	g_pSquirrel<ScriptContext::SERVER>->__sq_defconst = module.Offset(0x1F550).As<sq_defconstType>();

	g_pSquirrel<ScriptContext::SERVER>->__sq_compilebuffer = module.Offset(0x3110).As<sq_compilebufferType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_pushroottable = module.Offset(0x5840).As<sq_pushroottableType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_call = module.Offset(0x8620).As<sq_callType>();

	g_pSquirrel<ScriptContext::SERVER>->__sq_newarray = module.Offset(0x39F0).As<sq_newarrayType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_arrayappend = module.Offset(0x3C70).As<sq_arrayappendType>();

	g_pSquirrel<ScriptContext::SERVER>->__sq_newtable = module.Offset(0x3960).As<sq_newtableType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_newslot = module.Offset(0x7080).As<sq_newslotType>();

	g_pSquirrel<ScriptContext::SERVER>->__sq_pushstring = module.Offset(0x3440).As<sq_pushstringType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_pushinteger = module.Offset(0x36A0).As<sq_pushintegerType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_pushfloat = module.Offset(0x3800).As<sq_pushfloatType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_pushbool = module.Offset(0x3710).As<sq_pushboolType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_pushasset = module.Offset(0x3560).As<sq_pushassetType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_pushvector = module.Offset(0x3780).As<sq_pushvectorType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_raiseerror = module.Offset(0x8440).As<sq_raiseerrorType>();

	g_pSquirrel<ScriptContext::SERVER>->__sq_getstring = module.Offset(0x60A0).As<sq_getstringType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_getinteger = module.Offset(0x60C0).As<sq_getintegerType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_getfloat = module.Offset(0x60E0).As<sq_getfloatType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_getbool = module.Offset(0x6110).As<sq_getboolType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_getasset = module.Offset(0x5FF0).As<sq_getassetType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_getuserdata = module.Offset(0x63B0).As<sq_getuserdataType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_getvector = module.Offset(0x6120).As<sq_getvectorType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_get = module.Offset(0x7C00).As<sq_getType>();

	g_pSquirrel<ScriptContext::SERVER>->__sq_createuserdata = module.Offset(0x38D0).As<sq_createuserdataType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_setuserdatatypeid = module.Offset(0x6470).As<sq_setuserdatatypeidType>();

	g_pSquirrel<ScriptContext::SERVER>->__sq_instructionvectorrealloc = module.Offset(0x69A20).As<sq_instructionvectorreallocType>();

	MAKEHOOK(
		module.Offset(0x1DD10),
		&RegisterSquirrelFunctionHook<ScriptContext::SERVER>,
		&g_pSquirrel<ScriptContext::SERVER>->RegisterSquirrelFunc);

	MAKEHOOK(module.Offset(0x8AA0), &sq_compiler_createHook<ScriptContext::SERVER>, &sq_compiler_create<ScriptContext::SERVER>);

	MAKEHOOK(module.Offset(0x1FE90), &SQPrintHook<ScriptContext::SERVER>, &SQPrint<ScriptContext::SERVER>);
	MAKEHOOK(module.Offset(0x260E0), &CreateNewVMHook<ScriptContext::SERVER>, &CreateNewVM<ScriptContext::SERVER>);
	MAKEHOOK(module.Offset(0x26E20), &DestroyVMHook<ScriptContext::SERVER>, &DestroyVM<ScriptContext::SERVER>);
	MAKEHOOK(module.Offset(0x799E0), &ScriptCompileErrorHook<ScriptContext::SERVER>, &SQCompileError<ScriptContext::SERVER>);
	MAKEHOOK(module.Offset(0x1D5C0), &CallScriptInitCallbackHook<ScriptContext::SERVER>, &CallScriptInitCallback<ScriptContext::SERVER>);
	MAKEHOOK(module.Offset(0x672D0), &SQFuncState__BuildProtoHook<ScriptContext::SERVER>, &SQFuncState__BuildProto<ScriptContext::SERVER>);

	// FCVAR_CHEAT and FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS allows clients to execute this, but since it's unsafe we only allow it when cheats
	// are enabled for script_client and script_ui, we don't use cheats, so clients can execute them on themselves all they want
	RegisterConCommand(
		"script",
		ConCommand_script<ScriptContext::SERVER>,
		"Executes script code on the server vm",
		FCVAR_GAMEDLL | FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS | FCVAR_CHEAT);

	StubUnsafeSQFuncs<ScriptContext::SERVER>();
}


ON_DLL_LOAD_RELIESON("engine.dll", CommonSquirrel, ConVar, (CModule module))
{
	if (Tier0::CommandLine()->CheckParm("-dev"))
		CVar_ns_add_line_opcode = new ConVar("ns_add_line_opcode", "1", FCVAR_NONE, "Whether line opcode gets added for debug call");
	else
		CVar_ns_add_line_opcode = new ConVar("ns_add_line_opcode", "0", FCVAR_NONE, "Whether line opcode gets added for debug call");
}
