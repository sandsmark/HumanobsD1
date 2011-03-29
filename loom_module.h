//	loom_module.h
//
//	Author: Eric Nivel
//
//	BSD license:
//	Copyright (c) 2010, Eric Nivel
//	All rights reserved.
//	Redistribution and use in source and binary forms, with or without
//	modification, are permitted provided that the following conditions are met:
//
//   - Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   - Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   - Neither the name of Eric Nivel nor the
//     names of their contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
//	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//	DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
//	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	loom_module_h
#define loom_module_h

#include	"integration.h"
#include	"../mBrane/trunk/Core/module_node.h"


#define	N		module::Node
#define	NODE	module::Node::Get()
#define	OUTPUT	NODE->trace(N::APPLICATION)

typedef	char* (*LoomOutput)(uint32);
typedef	bool (*LoomInput)(char*);

MODULE_CLASS_BEGIN(Loom,Module<Loom>)
public:
	Thread* thread;
	SharedLibrary* lib;
	LoomOutput waitForEvent;
	LoomInput processEvent;

	std::string	mBrane_weaver_module_path;

	uint32	ontology_count;

	void	loadParameters(const	std::vector<word32>	&numbers,const	std::vector<std::string>	&strings){
		mBrane_weaver_module_path=strings[0];
	}

	void	start(){
		lib = SharedLibrary::New(mBrane_weaver_module_path.c_str());
		if (lib) {
			waitForEvent = lib->getFunction<LoomOutput>("WaitForEvent");
			processEvent = lib->getFunction<LoomInput>("ProcessEvent");
		}
		else {
			waitForEvent = NULL;
			processEvent = NULL;
		}
		thread = NULL;
	}
	void	stop(){
		delete(thread);
		thread = NULL;
		delete(lib);
		lib = NULL;
		waitForEvent = NULL;
		processEvent = NULL;
	}
	template<class	T>	Decision	decide(T	*p){return	WAIT;}
	template<class	T>	void	react(T	*p){}

	void	react(SystemReady	*p){
		OUTPUT<<"Loom "<<"got SysReady"<<std::endl;
		thread = Thread::New<Thread>(run, this);

		ontology_count=0;
		NODE->send(this,new	StartMem(),N::PRIMARY);

		char* data = NULL;

		// Output data to Loom
		if (processEvent)
			processEvent(data);
	}

	void	react(OntologyCount	*p){
		OUTPUT<<"got ontology count: "<<p->count<<std::endl;
		ontology_count=p->count;
	}
	void	react(OntologyDef	*p){
		//OUTPUT<<"got ontology member: "<<p->name<<std::endl;
		//std::string	name=p->name;
		//register_ontology_member(name,p->OID);

		if(--ontology_count==0){
			uint8	u=0;
		}
			
	}
	void	react(Speak	*p){
		OUTPUT<<"RMem says: "<<p->word<<std::endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	static thread_ret thread_function_call run(void	*args) {

		Loom* _this = (Loom*) args;
		if (!_this)
			thread_ret_val(0);

		uint32 type = 0;
		char* data = NULL;
		uint32 dataSize = 0;

		while (_this->waitForEvent) {
			// wait for new input from lib function
			if ( (data = _this->waitForEvent(100)) ) {
				// if there, post output
			}
		}

		thread_ret_val(0);
	}

MODULE_CLASS_END(Loom)


#endif
