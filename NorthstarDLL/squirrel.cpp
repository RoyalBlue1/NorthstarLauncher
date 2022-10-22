#include "pch.h"
#include "squirrel.h"
#include "concommand.h"
#include "modmanager.h"
#include "dedicated.h"
#include "r2engine.h"
#include "tier0.h"
#include <queue>

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

template <ScriptContext context> CSquirrelVM* (*CreateNewVM)(void* a1, ScriptContext realContext ,float a3);
template <ScriptContext context> CSquirrelVM* CreateNewVMHook(void* a1, ScriptContext realContext,float a3)
{
	CSquirrelVM* sqvm = CreateNewVM<context>(a1, realContext,a3);
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


	PVOID framesToCapture[62];
	int frames = RtlCaptureStackBackTrace(0, 62, framesToCapture, NULL);
	for (int i = 0; i < frames; i++)
	{
		HMODULE backtraceModuleHandle;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, static_cast<LPCSTR>(framesToCapture[i]), &backtraceModuleHandle);

		char backtraceModuleFullName[MAX_PATH];
		GetModuleFileNameExA(GetCurrentProcess(), backtraceModuleHandle, backtraceModuleFullName, MAX_PATH);
		char* backtraceModuleName = strrchr(backtraceModuleFullName, '\\') + 1;

		void* actualAddress = (void*)framesToCapture[i];
		void* relativeAddress = (void*)(uintptr_t(actualAddress) - uintptr_t(backtraceModuleHandle));

		spdlog::error("    {} + {} ({})", backtraceModuleName, relativeAddress, actualAddress);
	}
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
//#define PRINT_INSTRUCTION_DEBUG
template <ScriptContext context> SQFunctionProto* (*SQFuncState__BuildProto)(SQFuncState* funcState);
template <ScriptContext context> SQFunctionProto* SQFuncState__BuildProtoHook(SQFuncState* funcState)
{

#ifdef PRINT_INSTRUCTION_DEBUG
	const char* debugFunctionName = "InitItems";
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
			spdlog::info("{}", *info);
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
					inst2.instruction.arg1 = inst.instruction.output * -1;
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
				funcState->_lineInfoData[i].op = inst.newPosition - 1;
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
			spdlog::info("{}", *info);
		}
	}
#endif

	return SQFuncState__BuildProto<context>(funcState);
}

float fsqrt(float x)
{
	return sqrtf(x);
}

struct debugInstruction
{
	int index;
    int stackbase;
	SQInstruction* inst;
};
static std::queue<debugInstruction> instDebug;


template <ScriptContext context> long long (*SQVMExecute)(
	HSquirrelVM* sqvm, SQObject* closure, int paramAmount, int stackbase, SQObject* outres, int throwerror, unsigned int calltype);
template <ScriptContext context> long long SQVMExecuteHook(
	HSquirrelVM* sqvm, SQObject* closure, int paramAmount, int stackbase, SQObject* outres, int throwerror, unsigned int calltype)
{
	//spdlog::info("Running custom execute with {}", closure->_VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->_val);
	//spdlog::info("paramAmount = {} stackbase = {} throwerror = {} calltype = {}", paramAmount, stackbase, throwerror, calltype);
	// only runs on server right now so no check for context

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
		sqvm->temp_reg = *closure;
		if (!g_pSquirrel<context>->__SQVMStartCall(
				sqvm, sqvm->temp_reg._VAL.asClosure, sqvm->_top - paramAmount, (unsigned int)paramAmount, stackbase, 0))
		{
			if (!sqvm->ci)
				g_pSquirrel<context>->__SQVMCallErrorHandler(sqvm, &sqvm->_lasterror);
			sharedState->_currentThreadMaybe = savedSharedState;
			--sqvm->_nnativecalls;
			return 0;
		}
		sqvm->ci->_root = 1;
	}
	
	exeption_restore:
	while (2)
	{
		sqvm->ci->ip = &sqvm->ci->ip[1];
		Instruction = sqvm->ci->ip;
		debugInstruction deInst;
		deInst.index = Instruction - sqvm->ci->closure._VAL.asFuncProto->instruction;
		deInst.stackbase = sqvm->_stackbase;
		deInst.inst = Instruction;
		//instDebug.emplace(deInst);
		if (instDebug.size() > 300)
			instDebug.pop();

		char instString[512];
		sqInstructionToString(instString, 512, Instruction, sqvm);
		spdlog::info("{}", std::string(instString));
		
		

		switch (Instruction->op)
		{
		case _OP_LINE:
		{
			if (sqvm->sharedState->debugServer)
			{
				const char* name = "UNKNOWN";
				if (sqvm->ci->closure._Type == OT_FUNCPROTO && sqvm->ci->closure._VAL.asFuncProto->fileName._Type == OT_STRING)
					name = sqvm->ci->closure._VAL.asFuncProto->fileName._VAL.asString->_val;

				g_pSquirrel<context>->callLineDebugger(sqvm->sharedState->debugServer, sqvm, Instruction->arg1, name);
			}
			
			continue;
		}
		case _OP_LOAD:
		{
			SQObject* target;
			SQObject* source;
			source = &sqvm->ci->_literals[Instruction->arg1];
			target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = *source;
			continue;
		}
		case _OP_LOADCOPY:
			g_pSquirrel<context>->__sq_op_loadcopy(
				sqvm->sharedState, &sqvm->ci->_literals[Instruction->arg1], &sqvm->_stackOfCurrentFunction[Instruction->output]);
			continue;
		case _OP_LOADINT:
		{
			sqvm->_stackOfCurrentFunction[Instruction->output] = Instruction->arg1;
			continue;
		}
		case _OP_LOADFLOAT:
		{
			sqvm->_stackOfCurrentFunction[Instruction->output] = (float)Instruction->arg1;
			continue;
		}
		case _OP_DLOAD:
		{ // Double load
			SQObject* source = &sqvm->ci->_literals[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = *source;
			source = &sqvm->ci->_literals[(unsigned __int16)Instruction->arg3];
			target = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			*target = *source;
			continue;
		}
		case _OP_TAILCALL:
			if (!g_pSquirrel<context>->__sq_op_call(sqvm, Instruction, outres, &traps, 1))
				goto label_errorHandler;
			if (sqvm->_suspended)
			{
				sharedState->_currentThreadMaybe = savedSharedState;
				--sqvm->_nnativecalls;
				return 1;
			}

			continue;
		case _OP_CALL:
			if (!g_pSquirrel<context>->__sq_op_call(sqvm, Instruction, outres, &traps, 0))
				goto label_errorHandler;
			if (sqvm->_suspended)
			{
				sharedState->_currentThreadMaybe = savedSharedState;
				--sqvm->_nnativecalls;
				return 1;
			}
			continue;
		case _OP_PREPCALL:
			if (!g_pSquirrel<context>->__sq_op_precall(sqvm, Instruction, &sqvm->_stackOfCurrentFunction[Instruction->arg1]))
				goto label_errorHandler;
			continue;
		case _OP_PREPCALLK:
			if (!g_pSquirrel<context>->__sq_op_precall(sqvm, Instruction, &sqvm->ci->_literals[Instruction->arg1]))
				goto label_errorHandler;
			continue;
		case _OP_GETK:
		{
			SQObject res;
			if (!g_pSquirrel<context>->__SQVMGet(
					sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg2], &sqvm->ci->_literals[Instruction->arg1], &res, 0, 1))
			{
				g_pSquirrel<context>->__sub_35B80(
					sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg2], &sqvm->ci->_literals[Instruction->arg1]);
				goto label_errorHandler;
			}
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = res;

			continue;
		}
		case _OP_MOVE:
		{

			SQObject* source = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = *source;
			continue;
		}
		case _OP_NEWSLOT:
			if (!g_pSquirrel<context>->__SQVMNewSlot(
					sqvm,
					&sqvm->_stackOfCurrentFunction[Instruction->arg1],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg3],
					0))
				goto label_errorHandler;
			continue;
		case _OP_DELETE:
		{
			SQObject res;
			SQObject* key = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			SQObject* self = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (!g_pSquirrel<context>->__sub_34CF0(sqvm, self, key, &res))
			{
				goto label_errorHandler;
			}
			*target = res;
			continue;
		}
		case _OP_SET:
			if (!g_pSquirrel<context>->__sq_op_set(
					sqvm,
					&sqvm->_stackOfCurrentFunction[Instruction->arg1],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg3],
					1))
			{
				if (sqvm->_lasterror._Type == OT_NULL)
					g_pSquirrel<context>->__sub_35B80(
						sqvm,
						&sqvm->_stackOfCurrentFunction[Instruction->arg1],
						&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]);
				goto label_errorHandler;
			}
			continue;
		case _OP_GET:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;

			if (!g_pSquirrel<context>->__SQVMGet(sqvm, arg1Obj, arg2Obj, &res, 0, 1))
			{
				g_pSquirrel<context>->__sub_35B80(sqvm, arg1Obj, arg2Obj);
				goto label_errorHandler;
			}

			*target = res;

			continue;
		}
		case _OP_EQ:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target  = g_pSquirrel<context>->__sq_op_eq(
										 0,
										 &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
										 &sqvm->_stackOfCurrentFunction[Instruction->arg1])
										 ? true
										 : false;

			continue;
		}
		case _OP_NE:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = g_pSquirrel<context>->__sq_op_eq(
										 0,
										 &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
										 &sqvm->_stackOfCurrentFunction[Instruction->arg1])
										 ? false
										 : true;
			continue;
		}
		case _OP_BITW:
		{
			int operation = (unsigned __int16)Instruction->arg3;
			SQObject* operator1 = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* operator2 = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;
			if (!g_pSquirrel<context>->__sub_2B590(sqvm, operation, &res, operator1, operator2))
			{
				goto label_errorHandler;
			}
			*target = res;
			continue;
		}
		case _OP_RETURN:
			if (!(unsigned int)g_pSquirrel<context>->__sq_sqvm_Return(sqvm, Instruction->output, Instruction->arg1, &sqvm->temp_reg))
				continue;
			*outres = sqvm->temp_reg;

			sharedState->_currentThreadMaybe = savedSharedState;
			//spdlog::info("Return of {}", closure->_VAL.asClosure->_function._VAL.asFuncProto->funcName._VAL.asString->_val);
			--sqvm->_nnativecalls;
			return 1;
		case _OP_LOADNULLS:
			for (int i = 0; i < Instruction->arg1; i++)
			{
				SQObject* target = &sqvm->_stackOfCurrentFunction[i + Instruction->output];
				target->Null();
			}
			continue;
		case _OP_LOADROOTTABLE:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = sqvm->_roottable_object;
			continue;
		}
		case _OP_LOADBOOL:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target =(bool)Instruction->arg1;
			continue;
		}
		case _OP_DMOVE:
		{
			SQObject* source = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v94
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; // v95
			*target = *source;

			source = &sqvm->_stackOfCurrentFunction[Instruction->arg3];
			target = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			*target = *source;
			continue;
		}
		case _OP_JMP:
			sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
			continue;
		case _OP_JNZ:
		{
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
				if (target->_Type != OT_FLOAT)
				{
					sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
				}
				else if (target->_VAL.asFloat != 0.0)
				{
					sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
				}
			}
			continue;
		}
		case _OP_JZ:
		{
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
			continue;
		}
		case _OP_LOADFREEVAR:
		{
			SQClosure* cl = sqvm->ci->obj10._VAL.asClosure;
			SQObject* source = &cl->_outervalues[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = *source;
			continue;
		}
		case _OP_VARGC:
		{
			int vargc = (unsigned int)*((__int16*)&sqvm->ci->_root + 2);
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];

			*target = vargc;
			continue;
		}
		case _OP_GETVARGV://TODO REDO
		{
			SQObject* indexObject = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v100
			if (sqvm->ci->_vargs_size <= 0)
			{
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "this function doesn't have var args");
				goto label_errorHandler;
			}

			if (!(indexObject->_Type & (1 << 0x1Au)))
			{
				const char* typeName = sq_getTypeName(indexObject->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "indexing 'vargv' with %s", typeName);
				goto label_errorHandler;
			}
			int index; // v14
			if (indexObject->_Type == OT_FLOAT)
				index = (unsigned int)indexObject->_VAL.asFloat;
			else
				index = indexObject->_VAL.asInteger;
			if (index < 0 || index >= sqvm->ci->_vargs_size)
			{
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "vargv index out of range");
				goto label_errorHandler;
			}
			SQObject* vargv = &sqvm->_vargsstack[(index + sqvm->ci->_vargs_base)]; // v103
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; // v104
			*target = *vargv;
			continue;
		}
		case _OP_NEWTABLE:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = g_pSquirrel<context>->__sq_createTable(sqvm->sharedState, Instruction->arg1);
			
			continue;
		}
		case _OP_NEWARRAY:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = g_pSquirrel<context>->__sub_1800(sqvm->sharedState, 0);
			//spdlog::info("C6F0 in new Array is {:X}", (long long)target->_VAL.asArray->_values);
			g_pSquirrel<context>->__sub_C6F0(&target->_VAL.asArray->_values, Instruction->arg1);
			continue; //TODO refcounting
		}
		case _OP_APPENDARRAY:
		{
			SQObject* data;
			if (Instruction->arg3)
				data = &sqvm->ci->_literals[Instruction->arg1];
			else
				data = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			g_pSquirrel<context>->__sub_1A20(sqvm->_stackOfCurrentFunction[Instruction->output]._VAL.asArray, data);
			continue;
		}
		case _OP_GETPARENT:
		{
			SQObject res; // v387 v388

			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (!(unsigned __int8)g_pSquirrel<context>->__sub_2F740(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg1], &res))
			{
				goto label_errorHandler;
			}
			*target = res;

			continue;
		}
		case _OP_COMPOUND_ARITH:
			if (!g_pSquirrel<context>->__sub_2E670(
					sqvm,
					(unsigned __int16)Instruction->arg3,
					&sqvm->_stackOfCurrentFunction[HIWORD(Instruction->arg1)],
					&sqvm->_stackOfCurrentFunction[Instruction->arg2],
					&sqvm->_stackOfCurrentFunction[LOWORD(Instruction->arg1)]))
				goto label_errorHandler;
			continue;
		case _OP_COMPOUND_ARITH_LOCAL:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			if (!g_pSquirrel<context>->__sub_2B6C0(sqvm, (unsigned __int16)Instruction->arg3, arg1Obj, arg1Obj, arg2Obj))
				goto label_errorHandler;
			continue;
		}

		case _OP_INCREMENT_PREFIX:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;

			if (!g_pSquirrel<context>->__sub_2E960(sqvm, &res, arg1Obj, arg2Obj, *(BYTE*)&Instruction->arg3))
			{
				goto label_errorHandler;
			}

			*target = res;

			continue;
		}
		case _OP_INCREMENT_PREFIX_LOCAL:
			if (g_pSquirrel<context>->__sq_op_increment_prefix_local(
					sqvm,
					&sqvm->_stackOfCurrentFunction[Instruction->output],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
					*(BYTE*)&Instruction->arg3))
			{
				continue;
			}
			goto label_errorHandler;
		case _OP_INCREMENT_PREFIX_STRUCTFIELD:
			if (!(unsigned __int8)g_pSquirrel<context>->__sp_op_increment_prefix_structfield(
					sqvm,
					&sqvm->_stackOfCurrentFunction[Instruction->output],
					&sqvm->_stackOfCurrentFunction[Instruction->arg1],
					(unsigned __int16)Instruction->arg2,
					*(BYTE*)&Instruction->arg3))
				goto label_errorHandler;
			continue;
		case _OP_INCREMENT_POSTFIX:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;

			if (!(unsigned __int8)g_pSquirrel<context>->__sub_2EA30(sqvm, &res, arg1Obj, arg2Obj, *(char*)&Instruction->arg3))
			{
				goto label_errorHandler;
			}

			*target = res;

			continue;
		}
		case _OP_INCREMENT_POSTFIX_LOCAL:
		{

			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v161
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; // closure

			SQObject res; // v382 v383


			if (!g_pSquirrel<context>->__sub_2E860(sqvm, &res, arg2Obj, *(BYTE*)&Instruction->arg3))
			{
				goto label_errorHandler;
			}

			*target = res;

			continue;
		}
		case _OP_INCREMENT_POSTFIX_STRUCTFIELD:
			if (!g_pSquirrel<context>->__sq_op_increment_postfix_structfield(
					sqvm,
					&sqvm->_stackOfCurrentFunction[Instruction->output],
					&sqvm->_stackOfCurrentFunction[Instruction->arg1],
					(unsigned __int16)Instruction->arg2,
					*(BYTE*)&Instruction->arg3))
			{
				goto label_errorHandler;
			}
			continue;
		case _OP_CMP:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v166
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v167
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; // closure
			SQObject res; // v386

			if (!g_pSquirrel<context>->__sub_2D1E0(sqvm, (unsigned __int16)Instruction->arg3, arg2Obj, arg1Obj, &res))
			{
				goto label_errorHandler;
			}

			*target = res;

			continue;
		}
		case _OP_EXISTS:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = g_pSquirrel<context>->__SQVMGet(
										 sqvm,
										 &sqvm->_stackOfCurrentFunction[Instruction->arg1],
										 &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
										 &sqvm->temp_reg,
										 1,
										 0)
										 ? true
										 : false;
			continue;
		}
		case _OP_INSTANCEOF:
		{
			SQObject* classObj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* instanceObj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (((classObj->_Type != OT_CLAAS) || (instanceObj->_Type & 0x408000) == 0))
			{
				const char* classTypeName = sq_getTypeName(classObj->_Type);
				const char* instanceTypeName = sq_getTypeName(instanceObj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(
					sqvm, "cannot apply instanceof between a %s and a %s", instanceTypeName, classTypeName);
				goto label_errorHandler;
			}

			long long classOfInstance = *(long long*)(instanceObj->_VAL.as64Integer + 56);
			bool isInstance = false;
			while (classOfInstance)
			{
				if (classOfInstance == classObj->_VAL.as64Integer)
				{
					isInstance = true;
					break;
				}
				classOfInstance = *(long long*)(classOfInstance + 56);
			}
			*target = isInstance;

			continue;
		}
		case _OP_NEG:
		{
			SQObject res;

			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (!g_pSquirrel<context>->__sub_2CF10(sqvm, &res, &sqvm->_stackOfCurrentFunction[Instruction->arg1]))
			{
				goto label_errorHandler;
			}

			*target = res;

			continue;
		}
		case _OP_NOT:
		{

			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			

			if ((arg1Obj->_Type & 0xB) != 0)
			{
				*target = arg1Obj->_VAL.asInteger != 0;
			}
			else if (arg1Obj->_Type == OT_FLOAT)
			{
				*target = arg1Obj->_VAL.asFloat != 0.0;
			}
			else
			{
				*target = false;
				
			}

			continue;
		}
		case _OP_BWNOT:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // type = v188
			if (arg1Obj->_Type != OT_INTEGER)
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "attempt to perform a bitwise op on a %s", arg1TypeName);
				goto label_errorHandler;
			}
			g_pSquirrel<context>->__sub_27390(
				&sqvm->_stackOfCurrentFunction[Instruction->output], ~sqvm->_stackOfCurrentFunction[Instruction->arg1]._VAL.asInteger);
			continue;
		}
		case _OP_CLOSURE:
		{

			SQObject* arg1Obj = &sqvm->ci->closure._VAL.asClosure->fastcallClosureArray[Instruction->arg1];

			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res; // v365 v366


			if (!(unsigned __int8)g_pSquirrel<context>->__sub_2F0B0(sqvm, &res, arg1Obj))
			{
				goto label_errorHandler;
			}

			*target = res;

			continue;
		}
		case _OP_FOREACH:
			if (!g_pSquirrel<context>->__sub_2EE20(
					sqvm,
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg3],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 1],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 2],
					0,
					Instruction->output,
					Instruction->arg1,
					sqvm->ci))
			{
				goto label_errorHandler;
			}
			continue;
		case _OP_FOREACH_STATICARRAY_START:
		{
			SQObject* arg1_2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1 + 2]; // v201
			*arg1_2Obj = 0;
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v202
			*arg1Obj = 0;

			// TODO fix struct behaviour
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			SQObject* arg1_1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1 + 1]; // closure
			SQObject* structObj = &arg2Obj->_VAL.asStructInstance->data[arg2Obj->_structOffset]; // TODO struct

			*arg1_1Obj = *structObj;
			continue;
		}
		case _OP_FOREACH_STATICARRAY_NEXT:
		{

			// TODO fix struct behaviour
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2 + 2];
			int arg2Int = ++arg2Obj->_VAL.asInteger;
			if (arg2Int >= (unsigned __int16)Instruction->arg3)
				continue;
			g_pSquirrel<context>->__sub_27390(&sqvm->_stackOfCurrentFunction[Instruction->arg2 + 2], arg2Int);
			

			sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 1] =
				sqvm->_stackOfCurrentFunction[Instruction->arg1]
					._VAL.asStructInstance->data[sqvm->_stackOfCurrentFunction[Instruction->arg1]._structOffset + arg2Int];
			sqvm->ci->ip = &sqvm->ci->ip[Instruction->output];
			continue;
		}
		case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_START:
		{
			SQObject* arg1_2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1 + 2]; // v201
			*arg1_2Obj = 0;
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v202
			*arg1Obj = 0;

			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v10
			SQObject* arg1_1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1 + 1]; // closure
			*arg1_1Obj = *arg2Obj;
			continue;
		}
		case _OP_FOREACH_STATICARRAY_NESTEDSTRUCT_NEXT:
		{

			SQObject* arg2_2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2 + 2];
			int arg2_2Int = ++arg2_2Obj->_VAL.asInteger;
			if (arg2_2Int >= (unsigned __int16)Instruction->arg3)
			{
				continue;
			}
			g_pSquirrel<context>->__sub_27390(&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2], arg2_2Int);

			sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 1] = sqvm->_stackOfCurrentFunction[(__int8)Instruction->arg1];
			sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 + 1]._structOffset += arg2_2Int*(Instruction->arg1>>8);
			sqvm->ci->ip = &sqvm->ci->ip[Instruction->output];

			continue;
		}
		case _OP_DELEGATE:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;

			if (!g_pSquirrel<context>->__sub_2EF60(sqvm, &res, arg1Obj, arg2Obj))
			{
				goto label_errorHandler;
			}

			*target = res;
			continue;
		}
		case _OP_CLONE:
		{

			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;


			if (!g_pSquirrel<context>->__sub_34A30(sqvm, arg1Obj, &res))
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "cloning a %s", arg1TypeName);
				goto label_errorHandler;
			}
			
			*target = res;
			continue;
		}
		case _OP_TYPEOF:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;

			g_pSquirrel<context>->__sub_2D950(sqvm, arg1Obj, &res, 0);
			
			*target = res;
			continue;
		}
		case _OP_PUSHTRAP:
		{ // TODO cleanup maybe redo
			long long data1 = 0;
			long long data2 = 0;

			HIDWORD(data1) = sqvm->_top;
			LODWORD(data1) = sqvm->_stackbase;
			long long offset = 16 * (Instruction->arg1 + (((long long)sqvm->ci->ip - (sqvm->ci->closure._VAL.as64Integer + 272)) * 16));
			LODWORD(data2) = Instruction->output;
			long long pointer = sqvm->ci->closure._VAL.as64Integer + offset + 272;
			if (*(int*)sqvm->gap_F4 <= sqvm->traps)
				g_pSquirrel<context>->__sub_35800(&sqvm->field_E8, 2 * sqvm->traps);
			long long* trapPointer = (long long*)(sqvm->field_E8 + 24i64 * (long long)sqvm->traps);
			sqvm->traps++;
			if (trapPointer)
			{
				*trapPointer = data1;
				*(trapPointer + 8) = pointer;
				*(trapPointer + 16) = data2;
			}
			++traps;
			++sqvm->ci->_etraps;
			continue;
		}
		case _OP_POPTRAP:

			for (int i = 0; i < Instruction->output; i++)
			{
				--sqvm->traps;
				--traps;
				--sqvm->ci->_etraps;
			}
			continue;
		case _OP_THROW:
			g_pSquirrel<context>->__sub_35B20(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->output]);
			goto label_errorHandler;
		case _OP_CLASS:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;


			if (!(unsigned __int8)g_pSquirrel<context>->__sub_2F2F0(sqvm, &res, Instruction->arg1, (unsigned __int16)Instruction->arg2))
			{
				goto label_errorHandler;
			}
			
			*target = res;
			continue;
		}
		case _OP_NEWSLOTA:
			if (!g_pSquirrel<context>->__SQVMNewSlot(
					sqvm,
					&sqvm->_stackOfCurrentFunction[Instruction->arg1],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg3],
					(Instruction->output & 2) != 0))
				goto label_errorHandler;
			if ((Instruction->output & 1) != 0)
				g_pSquirrel<context>->__sub_3EA70(
					(void*)&sqvm->_stackOfCurrentFunction[Instruction->arg1]._VAL.asString->vtable,
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2 - 1]);
			continue;
		case _OP_EQ_LITERAL:
		{
			SQObject* arg1Obj = &sqvm->ci->_literals[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = g_pSquirrel<context>->__sq_op_eq(0, arg2Obj, arg1Obj) ? true : false;
			continue;
		}
		case _OP_NE_LITERAL:
		{
			SQObject* arg1Obj = &sqvm->ci->_literals[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = g_pSquirrel<context>->__sq_op_eq(0, arg2Obj, arg1Obj) ? false : true;

			continue;
		}
		case _OP_ASSERT_FAILED:
		{
			SQObject testObjectName;
			g_pSquirrel<context>->__SQVMToString(sqvm, &sqvm->_stackOfCurrentFunction[Instruction->arg1], &testObjectName);
			g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Assertion failed: %s", (const char*)(&testObjectName._VAL.asString->_val));
			goto label_errorHandler;
		}
		case _OP_ADD:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v87
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v88
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;

			if (!g_pSquirrel<context>->__sq_op_add(sqvm, &res, arg2Obj, arg1Obj))
			{
				goto label_errorHandler;
			}
			
			*target = res;
			continue;
		}
		case _OP_SUB:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v87
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v88
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;
			
			if (!g_pSquirrel<context>->__sq_op_sub(sqvm, &res, arg2Obj, arg1Obj))
			{
				goto label_errorHandler;
			}
			//spdlog::info("Sub result Type = {} value {:X}", sq_getTypeName(res._Type), res._VAL.as64Integer);
			
			*target = res;
			continue;
		}
		case _OP_MUL:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v87
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v88
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;

			if (!g_pSquirrel<context>->__sq_op_multiplication(sqvm, &res, arg2Obj, arg1Obj))
			{
				goto label_errorHandler;
			}
			
			*target = res;

			continue;
		}
		case _OP_DIV:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v87
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v88
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;


			if (!g_pSquirrel<context>->__sq_op_div(sqvm, &res, arg2Obj, arg1Obj))
			{
				goto label_errorHandler;
			}
			
			*target = res;

			continue;
		}
		case _OP_MOD:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v87
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v88
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject res;


			if (!g_pSquirrel<context>->__sq_op_modulo(sqvm, &res, arg2Obj, arg1Obj))
			{
				goto label_errorHandler;
			}
			
			*target = res;

			continue;
		}
		case _OP_CMP_JZ:
		{
			unsigned int intRes;
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			//spdlog::info("arg1Type {} arg2Type {}", sq_getTypeName(arg1Obj->_Type), sq_getTypeName(arg2Obj->_Type));
			if (!g_pSquirrel<context>->__sq_compare(
					sqvm,
					arg2Obj,
					arg1Obj,
					&intRes))
			{
				goto label_errorHandler;
			}

			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; // v170
			sqvm->ci->ip = &sqvm->ci->ip[1];
			
			
			if ((Instruction->arg3 == 0 && intRes < 0) || (Instruction->arg3 == 2 && intRes <= 0) ||
				(Instruction->arg3 == 3 && intRes > 0) || (Instruction->arg3 == 4 && intRes >= 0))
			{
				*target = true;
				continue;
			}
					
			*target = false;
			sqvm->ci->ip = &sqvm->ci->ip[sqvm->ci->ip->arg1];
			continue;
		}
		case _OP_INCREMENT_LOCAL_DISCARD_JMP:
		{
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v164
			if (arg2Obj->_Type == OT_INTEGER)
			{
				arg2Obj->_VAL.asInteger += *(BYTE*)&Instruction->arg3;
				sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
			}
			else if (arg2Obj->_Type == OT_FLOAT)
			{
				arg2Obj->_VAL.asFloat = (float)(*(BYTE*)&Instruction->arg3) + arg2Obj->_VAL.asFloat;
				sqvm->ci->ip = &sqvm->ci->ip[Instruction->arg1];
			}
			else
			{
				const char* arg2TypeName = sq_getTypeName(arg2Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot increment or decrement %s", arg2TypeName);
				goto label_errorHandler;
			}
			continue;
		}
		case _OP_NEWVECTOR:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg2];
			SQObject* arg3Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg3];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];

			if (!((arg1Obj->_Type & SQOBJECT_NUMERIC) && (arg2Obj->_Type & SQOBJECT_NUMERIC) && (arg3Obj->_Type & SQOBJECT_NUMERIC)))
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				const char* arg2TypeName = sq_getTypeName(arg2Obj->_Type);
				const char* arg3TypeName = sq_getTypeName(arg3Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(
					sqvm, "Vector component types are <%s, %s, %s>; must be integer or float", arg1TypeName, arg2TypeName, arg3TypeName);
				goto label_errorHandler;
			}
			
			SQVector targetVec;
			if (arg1Obj->_Type == OT_INTEGER)
			{
				targetVec.x = (float)arg1Obj->_VAL.asInteger;
			}
			else
			{
				targetVec.x = arg1Obj->_VAL.asFloat;
			}
			if (arg2Obj->_Type == OT_INTEGER)
			{
				targetVec.y = (float)arg2Obj->_VAL.asInteger;
			}
			else
			{
				targetVec.y = arg2Obj->_VAL.asFloat;
			}
			if (arg3Obj->_Type == OT_INTEGER)
			{
				targetVec.z = (float)arg3Obj->_VAL.asInteger;
			}
			else
			{
				targetVec.z = arg3Obj->_VAL.asFloat;
			}
			*target = targetVec;
			continue;
		}
		case _OP_ZEROVECTOR:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQVector targetVec;
			*target = targetVec;
			continue;
		}
		case _OP_GET_VECTOR_COMPONENT:
		{

			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			if (arg1Obj->_Type != 0x40000)
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot get vector component (.x/y/z) of %s", arg1TypeName);
				goto label_errorHandler;
			}
			SQVector* arg1Vec = (SQVector*)arg1Obj;
			float output = 0.0;
			switch (Instruction->arg2)
			{
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
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Invalid vector component index %d", Instruction->arg2);
				goto label_errorHandler;
			}
			sqvm->_stackOfCurrentFunction[Instruction->output]= output;
			continue;
		}

		case _OP_SET_VECTOR_COMPONENT:
		{

			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			if (target->_Type != OT_VECTOR)
			{
				const char* targetTypeName = sq_getTypeName(target->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
				goto label_errorHandler;
			}

			if (!(arg1Obj->_Type & SQOBJECT_NUMERIC))
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
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
			switch (Instruction->arg2)
			{
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

			continue;
		}
		case _OP_VECTOR_COMPONENT_MINUSEQ:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (target->_Type != OT_VECTOR)
			{
				const char* targetTypeName = sq_getTypeName(target->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
				goto label_errorHandler;
			}

			if (!(arg1Obj->_Type & SQOBJECT_NUMERIC))
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
				goto label_errorHandler;
			}
			float factor;
			if (arg1Obj->_Type == OT_INTEGER)
			{
				factor = (float)arg1Obj->_VAL.asInteger;
			}
			else
			{
				factor = arg1Obj->_VAL.asFloat;
			}
			SQVector* targetVec = (SQVector*)target;
			switch (Instruction->arg2)
			{
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
			continue;
		}
		case _OP_VECTOR_COMPONENT_PLUSEQ:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (target->_Type != OT_VECTOR)
			{
				const char* targetTypeName = sq_getTypeName(target->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
				goto label_errorHandler;
			}

			if (!(arg1Obj->_Type & SQOBJECT_NUMERIC))
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
				goto label_errorHandler;
			}
			float factor;
			if (arg1Obj->_Type == OT_INTEGER)
			{
				factor = (float)arg1Obj->_VAL.asInteger;
			}
			else
			{
				factor = arg1Obj->_VAL.asFloat;
			}
			SQVector* targetVec = (SQVector*)target;
			switch (Instruction->arg2)
			{
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
			continue;
		}
		case _OP_VECTOR_COMPONENT_MULEQ:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (target->_Type != OT_VECTOR)
			{
				const char* targetTypeName = sq_getTypeName(target->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
				goto label_errorHandler;
			}

			if (!(arg1Obj->_Type & SQOBJECT_NUMERIC))
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
				goto label_errorHandler;
			}
			float factor;
			if (arg1Obj->_Type == OT_INTEGER)
			{
				factor = (float)arg1Obj->_VAL.asInteger;
			}
			else
			{
				factor = arg1Obj->_VAL.asFloat;
			}
			SQVector* targetVec = (SQVector*)target;
			switch (Instruction->arg2)
			{
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
			continue;
		}
		case _OP_VECTOR_COMPONENT_DIVEQ:
		{

			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (target->_Type != OT_VECTOR)
			{
				const char* targetTypeName = sq_getTypeName(target->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component (.x/y/z) of %s", targetTypeName);
				goto label_errorHandler;
			}

			if (!(arg1Obj->_Type & SQOBJECT_NUMERIC))
			{
				const char* arg1TypeName = sq_getTypeName(arg1Obj->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot set vector component to %s", arg1TypeName);
				goto label_errorHandler;
			}
			float divisor;
			if (arg1Obj->_Type == OT_INTEGER)
			{
				divisor = (float)arg1Obj->_VAL.asInteger;
			}
			else
			{
				divisor = arg1Obj->_VAL.asFloat;
			}
			SQVector* targetVec = (SQVector*)target;
			switch (Instruction->arg2)
			{
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
			continue;
		}
		case _OP_VECTOR_NORMALIZE:
		{
			SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			if (vec->_Type != OT_VECTOR)
			{
				const char* typeName = sq_getTypeName(vec->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot normalize %s", typeName);
				goto label_errorHandler;
			}

			float normalizationFactor = 1.0 / (sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z) + 1.1754944e-38);  //add float min to not devide by 0
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQVector targetVec;
			
			targetVec.x = normalizationFactor * vec->x;
			targetVec.y = normalizationFactor * vec->y;
			targetVec.z = normalizationFactor * vec->z;
			*target = targetVec;
			continue;
		}
		case _OP_VECTOR_NORMALIZE_IN_PLACE:
		{
			SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			if (vec->_Type != OT_VECTOR)
			{
				const char* typeName = sq_getTypeName(vec->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot normalize %s", typeName);
				goto label_errorHandler;
			}
			float vecLength = sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
			float normalizationFactor = 1.0 / (vecLength + 1.1754944e-38); //add float min to not devide by 0
			vec->x *= normalizationFactor;
			vec->y *= normalizationFactor;
			vec->z *= normalizationFactor;

			sqvm->_stackOfCurrentFunction[Instruction->output]=vecLength;

			continue;
		}
		case _OP_VECTOR_DOT_PRODUCT:
		{
			SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
			if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
			{
				const char* vec1TypeName = sq_getTypeName(vec1->_Type);
				const char* vec2TypeName = sq_getTypeName(vec2->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot dot %s with %s", vec1TypeName, vec2TypeName);
				goto label_errorHandler;
			}

			float dotProduct = vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z;
			sqvm->_stackOfCurrentFunction[Instruction->output]=dotProduct;
			continue;
		}
		case _OP_VECTOR_DOT_PRODUCT2D:
		{
			SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
			if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
			{
				const char* vec1TypeName = sq_getTypeName(vec1->_Type);
				const char* vec2TypeName = sq_getTypeName(vec2->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot dot %s with %s", vec1TypeName, vec2TypeName);
				goto label_errorHandler;
			}
			float dotProduct = vec1->x * vec2->x + vec1->y * vec2->y;
			sqvm->_stackOfCurrentFunction[Instruction->output]=dotProduct;
			continue;
		}
		case _OP_VECTOR_CROSS_PRODUCT:
		{
			SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQVector targetVec;
			if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
			{
				const char* vec1TypeName = sq_getTypeName(vec1->_Type);
				const char* vec2TypeName = sq_getTypeName(vec2->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot cross %s with %s", vec1TypeName, vec2TypeName);
				goto label_errorHandler;
			}
			targetVec.x = vec1->y * vec2->z - vec1->z * vec2->y;
			targetVec.y = vec1->z * vec2->x - vec1->x * vec2->z;
			targetVec.z = vec1->x * vec2->y - vec1->y * vec2->x;
			*target = targetVec;
			continue;
		}
		case _OP_VECTOR_CROSS_PRODUCT2D:
		{
			SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
			if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
			{
				const char* vec1TypeName = sq_getTypeName(vec1->_Type);
				const char* vec2TypeName = sq_getTypeName(vec2->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot cross %s with %s", vec1TypeName, vec2TypeName);
				goto label_errorHandler;
			}
			float crossProduct = vec1->x * vec2->y - vec1->y * vec2->x;
			sqvm->_stackOfCurrentFunction[Instruction->output]=crossProduct;
			continue;
		}

		case _OP_VECTOR_LENGTH:
		{
			SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			if (vec->_Type != OT_VECTOR)
			{
				const char* typeName = sq_getTypeName(vec->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot get length of %s", typeName);
				goto label_errorHandler;
			}
			float vecLength = fsqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
			sqvm->_stackOfCurrentFunction[Instruction->output]=vecLength;
			continue;
		}
		case _OP_VECTOR_LENGTHSQR:
		{
			SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			if (vec->_Type != OT_VECTOR)
			{
				const char* typeName = sq_getTypeName(vec->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot get squared length of %s", typeName);
				goto label_errorHandler;
			}
			float vecLength = vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
			sqvm->_stackOfCurrentFunction[Instruction->output]= vecLength;
			continue;
		}
		case _OP_VECTOR_LENGTH2D:
		{
			SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			if (vec->_Type != OT_VECTOR)
			{
				const char* typeName = sq_getTypeName(vec->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot get 2D length of %s", typeName);
				goto label_errorHandler;
			}
			float vecLength = fsqrt(vec->x * vec->x + vec->y * vec->y);
			sqvm->_stackOfCurrentFunction[Instruction->output]= vecLength;
			continue;
		}
		case _OP_VECTOR_LENGTH2DSQR:
		{
			SQVector* vec = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			if (vec->_Type != OT_VECTOR)
			{
				const char* typeName = sq_getTypeName(vec->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot get 2D squared length of %s", typeName);
				goto label_errorHandler;
			}
			float vecLength = fsqrt(vec->x * vec->x + vec->y * vec->y);
			sqvm->_stackOfCurrentFunction[Instruction->output]= vecLength;
			continue;
		}
		case _OP_VECTOR_DISTANCE:
		{
			SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
			if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
			{
				const char* vec1TypeName = sq_getTypeName(vec1->_Type);
				const char* vec2TypeName = sq_getTypeName(vec2->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot get distance between %s and %s", vec1TypeName, vec2TypeName);
				goto label_errorHandler;
			}
			float x = vec2->x - vec1->x;
			float y = vec2->y - vec1->y;
			float z = vec2->z - vec1->z;
			float distance = fsqrt(x * x + y * y + z * z);
			sqvm->_stackOfCurrentFunction[Instruction->output]=distance;
			continue;
		}
		case _OP_VECTOR_DISTANCESQR:
		{
			SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
			if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
			{
				const char* vec1TypeName = sq_getTypeName(vec1->_Type);
				const char* vec2TypeName = sq_getTypeName(vec2->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot get squared distance between %s and %s", vec1TypeName, vec2TypeName);
				goto label_errorHandler;
			}
			float x = vec2->x - vec1->x;
			float y = vec2->y - vec1->y;
			float z = vec2->z - vec1->z;
			float distance = x * x + y * y + z * z;
			sqvm->_stackOfCurrentFunction[Instruction->output]=distance;
			continue;
		}
		case _OP_VECTOR_DISTANCE2D:
		{
			SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
			if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
			{
				const char* vec1TypeName = sq_getTypeName(vec1->_Type);
				const char* vec2TypeName = sq_getTypeName(vec2->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot get 2D distance between %s and %s", vec1TypeName, vec2TypeName);
				goto label_errorHandler;
			}
			float x = vec2->x - vec1->x;
			float y = vec2->y - vec1->y;
			float distance = fsqrt(x * x + y * y);
			sqvm->_stackOfCurrentFunction[Instruction->output]= distance;
			continue;
		}
		case _OP_VECTOR_DISTANCE2DSQR:
		{
			SQVector* vec1 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQVector* vec2 = (SQVector*)&sqvm->_stackOfCurrentFunction[Instruction->arg2];
			if (vec1->_Type != OT_VECTOR || vec2->_Type != OT_VECTOR)
			{
				const char* vec1TypeName = sq_getTypeName(vec1->_Type);
				const char* vec2TypeName = sq_getTypeName(vec2->_Type);
				g_pSquirrel<context>->__SQVMRaise_Error(
					sqvm, "Cannot get 2D squared distance between %s and %s", vec1TypeName, vec2TypeName);
				goto label_errorHandler;
			}
			float x = vec2->x - vec1->x;
			float y = vec2->y - vec1->y;
			float distance = fsqrt(x * x + y * y);
			sqvm->_stackOfCurrentFunction[Instruction->output]= distance;
			continue;
		}
		case _OP_INCREMENT_LOCAL_DISCARD:
		{
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v14
			if (arg2Obj->_Type == OT_INTEGER)
			{
				arg2Obj->_VAL.asInteger += *(char*)&Instruction->arg3;
			}
			else
			{
				if (arg2Obj->_Type != OT_FLOAT)
				{
					const char* arg2TypeName = sq_getTypeName(arg2Obj->_Type);
					g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot increment or decrement %s", arg2TypeName);
					goto label_errorHandler;
				}

				arg2Obj->_VAL.asFloat = (float)(*(char*)&Instruction->arg3) + arg2Obj->_VAL.asFloat;
			}
			continue;
		}
		case _OP_FASTCALL:
		{
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]; // v36
			*arg2Obj = *sqvm->_stackOfCurrentFunction;

			if (!g_pSquirrel<context>->__sub_2E100(
					sqvm,
					sqvm->ci->closure._VAL.asClosure->fastcallClosureArray[Instruction->arg1]._VAL.as64Integer,
					Instruction->output,
					(unsigned __int16)Instruction->arg3,
					sqvm->_stackbase + (unsigned __int16)Instruction->arg2))
			{
				goto label_errorHandler;
			}
			continue;
		}
		case _OP_FASTCALL_NATIVE:
		case _OP_FASTCALL_NATIVE_ARGTYPECHECK:
		{
			SQObject* arg1Obj = &sqvm->ci->closure._VAL.asClosure->fastcallClosureArray[Instruction->arg1]; // v40
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			*arg2Obj = *sqvm->_stackOfCurrentFunction;
			SQObject res; // src
			bool shouldSuspend;

			int newStackBase = (sqvm->_stackbase + (unsigned __int16)Instruction->arg2);
			bool argCheck = Instruction->op == _OP_FASTCALL_NATIVE_ARGTYPECHECK;
			//spdlog::info(
			//	"Calling native with name {} and param3 = {} funcPointer = {:X} l2116",
			//	arg1Obj->_VAL.asNativeClosure->_name->_val,
			//	Instruction->arg3,
			//	arg1Obj->_VAL.asNativeClosure->functionPointer);
			if (!g_pSquirrel<context>->__sq_nativecall(
					sqvm, arg1Obj->_VAL.asNativeClosure, (unsigned __int16)Instruction->arg3, newStackBase, &res, &shouldSuspend, argCheck))
			{
				goto label_errorHandler;
			}
			if (shouldSuspend || sqvm->unknown_field_11C && sqvm->_nnativecalls == 1)
			{

				sqvm->_suspended = 1;
				sqvm->_suspended_target = Instruction->output;
				sqvm->trapAmount = traps;
				sqvm->_suspended_root = sqvm->ci->_root;
				sqvm->_suspend_varargs = *(int*)&sqvm->ci->_vargs_size;
				if (sqvm->unknown_field_11C == 0)
					*outres = res;
				else
					*outres = sqvm->_object_120;
				sqvm->unknown_field_11C = 0;
				sharedState->_currentThreadMaybe = savedSharedState;
				--sqvm->_nnativecalls;
				return 1;
			}

			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; // closure
			*target = res;

			continue;
		}
		case _OP_FASTCALL_ENV:
			if (!g_pSquirrel<context>->__sub_2E100(
					sqvm,
					sqvm->ci->closure._VAL.asClosure->fastcallClosureArray[Instruction->arg1]._VAL.as64Integer,
					Instruction->output,
					(unsigned __int16)Instruction->arg3,
					sqvm->_stackbase + (unsigned __int16)Instruction->arg2))
			{
				goto label_errorHandler;
			}
			continue;

		case _OP_FASTCALL_NATIVE_ENV:
		case _OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK:
		{
			bool localCalltype;
			SQObject* arg1Obj = &sqvm->ci->closure._VAL.asClosure->fastcallClosureArray[Instruction->arg1];
			int newStackbase = sqvm->_stackbase + (unsigned int)(unsigned __int16)Instruction->arg2;
			bool argTypeCheck = Instruction->op == _OP_FASTCALL_NATIVE_ENV_ARGTYPECHECK;
			SQObject res; // v360

			//spdlog::info(
			//	"Calling native with name {} and param3 = {} l2169", arg1Obj->_VAL.asNativeClosure->_name->_val, Instruction->arg3);
			if (!g_pSquirrel<context>->__sq_nativecall(
					sqvm,
					arg1Obj->_VAL.asNativeClosure,
					(unsigned __int16)Instruction->arg3,
					newStackbase,
					&res,
					&localCalltype,
					argTypeCheck))
			{
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
					*outres = res;
				else
					*outres = sqvm->_object_120;
				*(&sqvm->_suspend_varargs + 1) = 0;
				sharedState->_currentThreadMaybe = savedSharedState;
				--sqvm->_nnativecalls;
				return 1;
			}
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]; // closure
			*target = res;

			continue;
		}

		case _OP_LOADGLOBALARRAY:
		{
			
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = sharedState->_globalsArray;
			continue;
		}
		case _OP_GETGLOBAL:
		{

			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject* arg1Obj = &sharedState->_globalsArray._VAL.asStructInstance->data[Instruction->arg1];
			*target = *arg1Obj;

			continue;
		}
		case _OP_SETGLOBAL:
		{
			SQObject* target = &sharedState->_globalsArray._VAL.asStructInstance->data[Instruction->arg1];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			*target = *arg2Obj;
			continue;
		}
		case _OP_COMPOUND_ARITH_GLOBAL:
		{
			SQObject* arg1Obj = &sharedState->_globalsArray._VAL.asStructInstance->data[Instruction->arg1];
			if (!g_pSquirrel<context>->__sub_2B6C0(
					sqvm,
					(unsigned __int16)Instruction->arg3,
					arg1Obj,
					arg1Obj,
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]))
			{
				goto label_errorHandler;
			}
			continue;
		}

		case _OP_GETSTRUCTFIELD:
		{
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1];
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			int fieldOffset = arg1Obj->_structOffset + (unsigned __int16)Instruction->arg2;
			SQObject* field = &arg1Obj->_VAL.asStructInstance->data[fieldOffset];
			*target = *field;
			continue;
		}
		case _OP_SETSTRUCTFIELD:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			int fieldOffset = Instruction->arg1 + target->_structOffset;
			SQObject* structField = &target->_VAL.asStructInstance->data[fieldOffset];
			*structField = *arg2Obj;
			continue;
		}
		case _OP_COMPOUND_ARITH_STRUCTFIELD:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output]._VAL.asStructInstance->data[Instruction->arg1 + sqvm->_stackOfCurrentFunction[Instruction->output]._structOffset];
			SQObject* arg2Obj = &sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2];
			if (!g_pSquirrel<context>->__sub_2B6C0(sqvm, (unsigned __int16)Instruction->arg3, target, target, arg2Obj))
				goto label_errorHandler;
			continue;
		}
		case _OP_NEWSTRUCT:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = (SQStructInstance*)g_pSquirrel<context>->__sub_63E00(
				sqvm->sharedState,
				Instruction->arg1,
				*(unsigned int**)(*(long long*)&sqvm->ci->closure._VAL.asClosure->gap_C8[8] + 8 * (unsigned __int16)Instruction->arg2));
			continue;
		}
		case _OP_GETSUBSTRUCT:
		{
			SQObject* source = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v10
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			*target = *source;
			target->_structOffset += (unsigned __int16)Instruction->arg2;
			continue;
		}
		case _OP_GETSUBSTRUCT_DYNAMIC:
		{
			int factor = sqvm->_stackOfCurrentFunction[Instruction->arg2]._VAL.asInteger;
			int arg1_0 = Instruction->arg1 & 0xFFFF;
			int arg1_1 = Instruction->arg1 >> 16;
			if (factor >= arg1_1)
			{
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Array index %d doesn't index size of %d", factor, arg1_1);
				goto label_errorHandler;
			}

			sqvm->_stackOfCurrentFunction[Instruction->output] = sqvm->_stackOfCurrentFunction[arg1_0];
			sqvm->_stackOfCurrentFunction[Instruction->output]._structOffset += factor * (unsigned __int16)Instruction->arg3;
			continue;
		}
		case _OP_TYPECAST:
			if (!g_pSquirrel<context>->__sq_op_typecast(
					sqvm,
					&sqvm->_stackOfCurrentFunction[Instruction->output],
					&sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2],
					Instruction->arg1))
			{
				const char* arg1TypeName = sq_getTypeName(Instruction->arg1);
				const char* arg2TypeName = sq_getTypeName(sqvm->_stackOfCurrentFunction[(unsigned __int16)Instruction->arg2]._Type);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot cast type \"%s\" to \"%s\"", arg2TypeName, arg1TypeName);
				goto label_errorHandler;
			}
			continue;
		case _OP_TYPECHECK:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (!((int)target->_Type == (unsigned int)Instruction->arg1 ||
				(unsigned int)Instruction->arg1 == OT_ENTITY && target->_Type == OT_NULL))
			{
				const char* objectTypeName = sq_getTypeName(target->_Type);
				const char* testTypeName = sq_getTypeName((unsigned int)Instruction->arg1);
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Expected type \"%s\", found type \"%s\"", testTypeName, objectTypeName);
				goto label_errorHandler;
			}
			continue;
		}

		case _OP_TYPECHECK_ORNULL:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			if (!(target->_Type == (unsigned int)Instruction->arg1 || target->_Type == OT_NULL))
			{
				const char* objectTypeName = sq_getTypeName(target->_Type);
				const char* testedTypeName = sq_getTypeName((unsigned int)Instruction->arg1);
				g_pSquirrel<context>->__SQVMRaise_Error(
					sqvm, "Expected type \"%s ornull\", found type \"%s\"", testedTypeName, objectTypeName);
				goto label_errorHandler;
			}
			continue;
		}

		case _OP_TYPECHECK_NOTNULL:
		{
			if (sqvm->_stackOfCurrentFunction[Instruction->output]._Type != OT_NULL)
			{
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Expected non-null value, found \"null\"");
				goto label_errorHandler;
			}
			continue;
		}

		case _OP_CHECK_ENTITY_CLASS:

			if (!g_pSquirrel<context>->__sq_op_check_entity_class(
					sqvm,
					&sqvm->_stackOfCurrentFunction[Instruction->arg1],
					*(long long*)(*(long long*)(sqvm->ci->closure._VAL.as64Integer + 256) + 8i64 * (unsigned __int16)Instruction->arg2))) //TODO reverse struct
			{
				goto label_errorHandler;
			}

			continue;
		case _OP_UNREACHABLE:
			g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Execution reached script marked as unreachable");
			goto label_errorHandler;
		case _OP_ARRAY_RESIZE:
		{
			SQObject* target = &sqvm->_stackOfCurrentFunction[Instruction->output];
			SQObject* arg1Obj = &sqvm->_stackOfCurrentFunction[Instruction->arg1]; // v310=val
			SQArray* targetArray = target->_VAL.asArray; // v309

			if (arg1Obj->_VAL.asInteger < 0)
			{
				g_pSquirrel<context>->__SQVMRaise_Error(sqvm, "Cannot resize to negative size %d", (unsigned int)arg1Obj->_VAL.asInteger);
				goto label_errorHandler;
			}
			unsigned int oldSize = targetArray->_usedSlots;
			g_pSquirrel<context>->__sub_BEF0(
				&targetArray->_values, arg1Obj->_VAL.asInteger, g_pSquirrel<context>->__globalClosure); // global variable
			if ((targetArray->_usedSlots) <= targetArray->_allocated >> 2)
			{
				if (targetArray->_usedSlots > 4)
				g_pSquirrel<context>->__sub_C6F0(&targetArray->_values, targetArray->_usedSlots);
			}
			int* value = (int*)sqvm->ci->closure._VAL.asFuncProto->unknownArray_D0[Instruction->arg2];
			
			while (oldSize < arg1Obj->_VAL.asInteger)
				g_pSquirrel<context>->__sub_29A40(sqvm->sharedState, &targetArray->_values[oldSize++], value);
			

			continue;
		}
		default:
			continue;
		}
	}

label_errorHandler:
{
	SQObject currentError; // a2
	currentError = sqvm->_lasterror; // v312

	int lastTop = sqvm->_top; // v315
	int n = 0; // v316
	if (!sqvm->ci)
	{
		sqvm->_lasterror = currentError;
		sqvm->sharedState->_currentThreadMaybe = savedSharedState;
		--sqvm->_nnativecalls;
		return 0;
	}
	if (traps)
	{

		while (sqvm->ci->obj10._Type != OT_NATIVECLOSURE || !n)
		{
			if (sqvm->ci->_etraps > 0)
			{
				sqvm->ci->ip = (SQInstruction*)(sqvm->field_E8 + 24i64 * (unsigned int)(sqvm->traps - 1) + 8);
				sqvm->_top = *(int*)(sqvm->field_E8 + 24 * (sqvm->traps - 1) + 4);
				sqvm->_stackbase = *(unsigned int*)(sqvm->field_E8 + 24 * (sqvm->traps - 1)); // v324
				sqvm->_stackOfCurrentFunction = &sqvm->_stack[sqvm->_stackbase];
				SQObject* newError =
					(SQObject*)((char*)(&sqvm->_stack[sqvm->_stackbase]) + 16i64 * *(int*)(sqvm->field_E8 + 24 * (sqvm->traps - 1) + 16)); // 325
				*newError = currentError;
				--sqvm->traps;
				--traps;
				--sqvm->ci->_etraps;
				if (lastTop >= sqvm->_top)
					g_pSquirrel<context>->__sq_dropStack(sqvm, lastTop);

				goto exeption_restore; // this continues execution in exeptionHander
			}
			g_pSquirrel<context>->__sub_2EDB0(sqvm, (__int16*)&sqvm->ci->_root + 2);

			--sqvm->_callsstacksize;
			sqvm->ci->obj10.Null();

			sqvm->ci->closure.Null();

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
	{
		char instString[512];
		while (instDebug.size() > 0)
		{
			debugInstruction inst = instDebug.front();
			sqInstructionToString(instString, 512, inst.inst,inst.index,inst.stackbase);
			spdlog::info("{}", std::string(instString));
			instDebug.pop();
		}

		
		
		std::map<int, std::string> functions;
		int lastBase = sqvm->_stackOfCurrentFunction - sqvm->_stack;
		for (int j = sqvm->_callsstacksize-1; j >=0 ; j--)
		{
			if (sqvm->_callsstack[j].closure._Type!=OT_CLOSURE)
				continue;
			spdlog::info("Function {} with base {}", std::string(sqvm->_callsstack[j].closure._VAL.asFuncProto->funcName._VAL.asString->_val),lastBase);
			functions.emplace(
				lastBase, std::string(sqvm->_callsstack[j].closure._VAL.asFuncProto->funcName._VAL.asString->_val));
			lastBase -= sqvm->_callsstack[j]._stackbaseOffset;
			
		}
			
		//for (int i = 0; &sqvm->_stackOfCurrentFunction[i] < &sqvm->_stack[sqvm->_top]; i++)
		int functionStackIndex = 0;
		std::string funcname = "";
		for (int i = 0; i< sqvm->_top;i++)
		{
			
			spdlog::info("index {} with object {} functionstackIndex {} of function {}",i, sqvm->_stack[i],functionStackIndex,funcname);
			functionStackIndex++;
			if (functions.count(i+1) > 0)
			{
				funcname = functions[i];
				functionStackIndex = 0;
			}
		}
		g_pSquirrel<context>->__SQVMCallErrorHandler(sqvm, &currentError);
	}
		
	if (sqvm->ci)
	{
		do
		{
			sqvm->_stackbase -= sqvm->ci->_stackbaseOffset;
			sqvm->_stackOfCurrentFunction = &sqvm->_stack[sqvm->_stackbase];
			sqvm->_top = sqvm->_stackbase + sqvm->ci->_unknown;
			g_pSquirrel<context>->__sub_2EDB0(sqvm, (__int16*)&sqvm->ci->_root + 2);
			--sqvm->_callsstacksize;
			sqvm->ci->obj10.Null();
			sqvm->ci->closure.Null();
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
		g_pSquirrel<context>->__sq_dropStack(sqvm, lastTop);
	sqvm->_lasterror = currentError;

	sharedState->_currentThreadMaybe = savedSharedState;
	--sqvm->_nnativecalls;
	return 0;
}
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
		spdlog::info("Replacing {} in {}", std::string(funcReg->squirrelFuncName), std::string(GetContextName(context)));
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
struct structdesc
{
	SQObject name;
	long long unknown_10;
	long long unknown_18;
	int unknown_20;
	int unknown_24;
};
SQRESULT dumpScriptDesc(HSquirrelVM* sqvm)
{
	SQSharedState* sharedState = sqvm->sharedState;
	SQTable* table = sharedState->_structs._VAL.asTable;
	int index = g_pSquirrel<ScriptContext::SERVER>->getinteger(sqvm, 1);
	if (index == -1)
	{
		for (int i = 0; i < table->_numOfNodes; i++)
		{
			if (table->_nodes[i].key._Type == OT_STRING)
			{
				spdlog::info("struct description for {} with index {}", std::string(table->_nodes[i].key._VAL.asString->_val),i );
			}
		}
	}
	else if (index < table->_numOfNodes)
	{
		if (table->_nodes[index].val._Type == OT_STRUCT_DEF)
		{
			SQStructDef* structDef = table->_nodes[index].val._VAL.asStructDef;
			spdlog::info("Struct {} contains:", std::string(structDef->_name->_val));
			if (structDef->_variableNamesType == OT_TABLE)
			{
				for (int i = 0; i < structDef->_variableNames->_numOfNodes; i++)
				{
				
					if (structDef->_variableNames->_nodes[i].key._Type == OT_STRING)
					{
						spdlog::info(
							"{} with valueType {}",
							std::string(structDef->_variableNames->_nodes[i].key._VAL.asString->_val),
							structDef->_variableNames->_nodes[i].val._VAL.as64Integer);
					}
				}
			}
			else
			{

			}
		}
		else
		{
			spdlog::info("Index does not contain StructDefinition");
		}
	}
	else
	{
		spdlog::info("Index out of range");
	}
	
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

	g_pSquirrel<ScriptContext::SERVER>->__SQVMStartCall = module.Offset(0x2DD20).As<SQVMStartCallType>();
	g_pSquirrel<ScriptContext::SERVER>->__SQVMCallErrorHandler = module.Offset(0x33A70).As<SQVMCallErrorHandlerType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_loadcopy = module.Offset(0x3d390).As<sq_op_loadcopyType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_call = module.Offset(0x2C440).As<sq_op_callType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_precall = module.Offset(0x2C2B0).As<sq_op_precallType>();
	g_pSquirrel<ScriptContext::SERVER>->__SQVMGet = module.Offset(0x343A0).As<SQVMGetType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_35B80 = module.Offset(0x35B80).As<sub_35B80Type>();
	g_pSquirrel<ScriptContext::SERVER>->__SQVMNewSlot = module.Offset(0x34B30).As<SQVMNewSlotType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_34CF0 = module.Offset(0x34CF0).As<sub_34CF0Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_set = module.Offset(0x34810).As<sq_op_setType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_eq = module.Offset(0x2F460).As<sq_op_eqType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2B590 = module.Offset(0x2B590).As<sub_2B590Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_sqvm_Return = module.Offset(0x2E490).As<sq_sqvm_ReturnType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_copyObject = module.Offset(0x1370).As<sq_copyObjectType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_createTable = module.Offset(0x1580).As<sq_createTableType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_1800 = module.Offset(0x1800).As<sub_1800Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_1A20 = module.Offset(0x1A20).As<sub_1A20Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2F740 = module.Offset(0x2F740).As<sub_2F740Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2E670 = module.Offset(0x2E670).As<sub_2E670Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2B6C0 = module.Offset(0x2B6C0).As<sub_2B6C0Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2E960 = module.Offset(0x2E960).As<sub_2E960Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_increment_prefix_local = module.Offset(0x2E780).As<sq_op_increment_prefix_localType>();
	g_pSquirrel<ScriptContext::SERVER>->__sp_op_increment_prefix_structfield =
		module.Offset(0x2EB70).As<sp_op_increment_prefix_structfieldType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2EA30 = module.Offset(0x2EA30).As<sub_2EA30Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2E860 = module.Offset(0x2E860).As<sub_2E860Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_increment_postfix_structfield =
		module.Offset(0x2EC60).As<sq_op_increment_postfix_structfieldType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2D1E0 = module.Offset(0x2D1E0).As<sub_2D1E0Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2CF10 = module.Offset(0x2CF10).As<sub_2CF10Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2F0B0 = module.Offset(0x2F0B0).As<sub_2F0B0Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2EE20 = module.Offset(0x2EE20).As<sub_2EE20Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2EF60 = module.Offset(0x2EF60).As<sub_2EF60Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_34A30 = module.Offset(0x34A30).As<sub_34A30Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2D950 = module.Offset(0x2D950).As<sub_2D950Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_35800 = module.Offset(0x35800).As<sub_35800Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2F2F0 = module.Offset(0x2F2F0).As<sub_2F2F0Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_3EA70 = module.Offset(0x3EA70).As<sub_3EA70Type>();
	g_pSquirrel<ScriptContext::SERVER>->__SQVMToString = module.Offset(0x2D2D0).As<SQVMToStringType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_add = module.Offset(0x2BAF0).As<sq_op_addType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_dropStack = module.Offset(0x2B500).As<sq_dropStackType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_C6F0 = module.Offset(0xC6F0).As<sub_C6F0Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_27390 = module.Offset(0x27390).As<sub_27390Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_35B20 = module.Offset(0x35B20).As<sub_35B20Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_sub = module.Offset(0x2BC80).As<sq_op_subType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_multiplication = module.Offset(0x2BE10).As<sq_op_multiplicationType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_div = module.Offset(0x2C020).As<sq_op_divType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_modulo = module.Offset(0x2C1C0).As<sq_op_moduloType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_compare = module.Offset(0x2D080).As<sq_compareType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_setObjectToFloat = module.Offset(0x27430).As<sq_setObjectToFloatType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2E100 = module.Offset(0x2E100).As<sub_2E100Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_nativecall = module.Offset(0x33E00).As<sq_nativecallType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_typecast = module.Offset(0x2F500).As<sq_op_typecastType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_op_check_entity_class = module.Offset(0x2F6B0).As<sq_op_check_entity_classType>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_29A40 = module.Offset(0x29A40).As<sub_29A40Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_63E00 = module.Offset(0x63E00).As<sub_63E00Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_2EDB0 = module.Offset(0x2EDB0).As<sub_2EDB0Type>();
	g_pSquirrel<ScriptContext::SERVER>->__sub_BEF0 = module.Offset(0xBEF0).As<sub_BEF0Type>();
	g_pSquirrel<ScriptContext::SERVER>->__SQVMRaise_Error = module.Offset(0x35A10).As<SQVMRaise_ErrorType>();
	g_pSquirrel<ScriptContext::SERVER>->__globalClosure = module.Offset(0xBCCE30).As<SQObject*>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_rdbg_hook = module.Offset(0x6F680).As<sq_rdbg_hookType>();
	g_pSquirrel<ScriptContext::SERVER>->__sq_rdbg_update = module.Offset(0x6C670).As<sq_rdbg_updateType>();

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
	//MAKEHOOK(module.Offset(0x2F950), &SQVMExecuteHook<ScriptContext::SERVER>, &SQVMExecute<ScriptContext::SERVER>);
	
	// FCVAR_CHEAT and FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS allows clients to execute this, but since it's unsafe we only allow it when cheats
	// are enabled for script_client and script_ui, we don't use cheats, so clients can execute them on themselves all they want
	RegisterConCommand(
		"script",
		ConCommand_script<ScriptContext::SERVER>,
		"Executes script code on the server vm",
		FCVAR_GAMEDLL | FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS | FCVAR_CHEAT);

	StubUnsafeSQFuncs<ScriptContext::SERVER>();
	g_pSquirrel<ScriptContext::SERVER>->AddFuncRegistration("void", "dumpSD", "int t", "", dumpScriptDesc);
}

ON_DLL_LOAD_RELIESON("engine.dll", CommonSquirrel, ConVar, (CModule module))
{

	CVar_ns_add_line_opcode = new ConVar(
		"ns_add_line_opcode",
		Tier0::CommandLine()->CheckParm("-dev") ? "1" : "0",
		FCVAR_NONE,
		"Whether line opcode gets added for debug call");
}
/*
AUTOHOOK(InitConsts, client.dll + 0x31AE00, void*, __fastcall, (CSquirrelVM* vm)) {
	HSquirrelVM* sqvm = vm->sqvm;
	void* ret = InitConsts(vm);
	spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->__sq_getConstantsTable(sqvm);");
	for (int i = 0; i < sqvm->sharedState->_typedConstants._VAL.asTable->_numOfNodes; i++)
	{
		tableNode* node = &sqvm->sharedState->_typedConstants._VAL.asTable->_nodes[i];

		if (node->key._Type == OT_STRING)
		{
			switch (node->val._Type)
			{
			case OT_INTEGER:
				//spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->pushstring(sqvm, \"{}\");", node->key._VAL.asString->_val);
				//spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->pushinteger(sqvm, {});", node->val._VAL.asInteger);
				//spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->newslot(sqvm, -3, 0);");
				break;
			case OT_TABLE:
				spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->pushstring(sqvm, \"{}\");", node->key._VAL.asString->_val);
				spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->newtable(sqvm);");
				for (int j = 0; j < node->val._VAL.asTable->_numOfNodes; j++) {
					tableNode* node2 = &node->val._VAL.asTable->_nodes[j];
					if (node2->key._Type == OT_STRING && node2->val._Type == OT_INTEGER) {
						spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->pushstring(sqvm, \"{}\");", node2->key._VAL.asString->_val);
						spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->pushinteger(sqvm, {});", node2->val._VAL.asInteger);
						spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->newslot(sqvm, -3, 0);");
					}
				}
				spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->newslot(sqvm, -3, 0);");
				break;
			}
		}
		
	}
	spdlog::info("g_pSquirrel<ScriptContext::CLIENT>->__sq_removeFromStack(sqvm);");
	return ret;
}*/

