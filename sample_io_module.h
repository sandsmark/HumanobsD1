//	sample_io_module.h
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

#ifndef	sample_io_module_h
#define sample_io_module_h

#include	"integration.h"
#include	"../mBrane/trunk/Core/module_node.h"
//#include	"../Replicode/r_exec/opcodes.h"

#define	N		module::Node
#define	NODE	module::Node::Get()
#define	OUTPUT	NODE->trace(N::APPLICATION)


template<class	U>	class	ThreadedModule:
public	Thread,
public	Module<U>{
};

//	Demonstrates how to interface an I/O module to rMems.
//	As an example, the module samples its environment: it identifies entities in the environment and periodically updates their positions (markers).
//	This is performed in an internal thread controlled by a timer.
//	Notice that entities have a resilience: if the module does not send them repeatedly, they will expire in the rMems,
//	meaning that they will be considered not exisitng anymore.
//	As a general rule, objects are sent to the rMems flat, i.e. they reference objects by their OIDs, instead of using pointers.
MODULE_CLASS_BEGIN(SampleIO,ThreadedModule<SampleIO>)
private:
	static	thread_ret thread_function_call	Sample(void	*args);

	void	initialize();	//	starts the thread.
	void	finalize();		//	kills the thread.

	UNORDERED_MAP<std::string,uint32>	entity_map;	//	stores the OIDs assigned by the module to the objects it controls.
													//	also stores the OIDs assigned by the rMem to ontology members.
	void	register_ontology_member(std::string&name,uint32	OID);
	uint32	getOID(std::string	&name);	//	returns an ontology member's OID.

	uint32	ontology_count;
public:
	void	start(){
		OUTPUT<<"SampleIO "<<"started"<<std::endl;
	}
	void	stop(){
		OUTPUT<<"SampleIO "<<"stopped"<<std::endl;
	}
	template<class	T>	Decision	decide(T	*p){
		return	WAIT;
	}
	template<class	T>	void	react(T	*p){
		OUTPUT<<"SampleIO "<<"got message"<<std::endl;
	}
	void	react(SystemReady	*p){
		OUTPUT<<"SampleIO "<<"got SysReady"<<std::endl;
		NODE->send(this,new	StartMem(),N::PRIMARY);
	}
	void	react(MemReady	*p){
		OUTPUT<<"SampleIO "<<"got MemReady"<<std::endl;
		initialize();
	}
	void	react(OntologyCount	*p){
		OUTPUT<<"got ontology count: "<<p->count<<std::endl;
		ontology_count=p->count;
	}
	void	react(OntologyDef	*p){
		OUTPUT<<"got ontology member: "<<p->name<<std::endl;
		std::string	name=p->name;
		register_ontology_member(name,p->OID);

		if(--ontology_count==0)
			Thread::start(Sample);
	}
	void	react(Speak	*p){
		OUTPUT<<"RMem says: "<<p->word<<std::endl;
	}
MODULE_CLASS_END(SampleIO)


#endif
