/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "vm_local.h"
#ifdef __cplusplus
}
#endif

#include "vm_llvm.h"

#include <llvm/Module.h>
#include <llvm/LLVMContext.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/PassManager.h>
//#include <llvm/Target/TargetSelect.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Support/PassManagerBuilder.h>

using namespace llvm;

#ifdef __cplusplus
extern "C" {
#endif

static ExecutionEngine *engine = NULL;

/*
==============
VM_LoadLLVM
==============
*/
void *VM_LoadLLVM( vm_t *vm, intptr_t (*systemcalls)(intptr_t, ...) ) {
	char	name[MAX_QPATH];
	char	filename[MAX_QPATH];
	char	*bytes;
	int		len;
	std::string error;

	COM_StripExtension(vm->name, name, sizeof(name));
	Com_sprintf(filename, sizeof(filename), "%sllvm.bc", name);
	len = FS_ReadFile(filename, (void **)&bytes);

	if (!bytes) {
		Com_Printf("Couldn't load LLVM file: %s\n", filename);
		return NULL;
	}

	MemoryBuffer *buffer = MemoryBuffer::getMemBuffer(StringRef(bytes, len));
	Module *module = ParseBitcodeFile(buffer, getGlobalContext(), &error);
	delete buffer;
	FS_FreeFile(bytes);

	if (!module) {
		Com_Printf("Couldn't parse LLVM file: %s: %s\n", filename, error.c_str());
		return NULL;
	}

	PassManagerBuilder pmb;
	PassManager p;
	llvm::Pass *InliningPass = createFunctionInliningPass(275);
	p.add(new TargetData(module));
	createStandardModulePasses(&p, 3, false, true, true, true, false, InliningPass);
	p.run(*module);

	if (!engine) {
		InitializeNativeTarget();

		std::string str;
		/* LLVM has gone through some churn since the initial version of the patch was
		 * written in 2009. For some unknown reason, the last argument of create() has
		 * to be false, otherwise LLVM will seg fault when JIT compiling vmMain in the
		 * call to engine->getPointerToFunction below. This is OK though, since the
		 * parameter is only there for backwards compatibility and they plan to reverse
		 * its default to false at some point in the future.  */
		engine = ExecutionEngine::create( module, false, &str, CodeGenOpt::Default, false );
		if (!engine) {
			Com_Printf("Couldn't create ExecutionEngine: %s\n", str.c_str());
			return NULL;
		}		
	} else {
		engine->addModule( module );
	}

	Function *dllEntry_ptr = module->getFunction(std::string("dllEntry"));
	if (!dllEntry_ptr) {
		Com_Printf("Couldn't get function address of dllEntry");
		return NULL;
	}
	void  (*dllEntry)( intptr_t (*syscallptr)(intptr_t, ...) ) = (void (*)(intptr_t (*syscallptr)(intptr_t, ...)))engine->getPointerToFunction( dllEntry_ptr );
	dllEntry(systemcalls);

	Function *vmMain_ptr = module->getFunction(std::string("vmMain"));
	if (!vmMain_ptr) {
		Com_Printf("Couldn't get function address of vmMain");
		return NULL;
	}
	intptr_t(*fp)(int, ...) = (intptr_t(*)(int, ...))engine->getPointerToFunction( vmMain_ptr );

	vm->entryPoint = fp;

	return module;
}

/*
==============
VM_UnloadLLVM
==============
*/
void VM_UnloadLLVM( void *llvmModule ) {
	if (!llvmModule) {
		Com_Printf("VM_UnloadLLVM called with NULL pointer");
		return;
	}

	if (!engine->removeModule((Module *)llvmModule)) {
		Com_Printf("Couldn't remove LLVM module");
		return;
	}

	delete (Module *)llvmModule;
}

#ifdef __cplusplus
}
#endif

