//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/ HUMANOBS - D1
//_/_/
//_/_/ Eric Nivel
//_/_/ Center for Analysis and Design of Intelligent Agents
//_/_/   Reykjavik University, Menntavegur 1, 101 Reykjavik, Iceland
//_/_/   http://cadia.ru.is
//_/_/ Copyright(c)2012
//_/_/
//_/_/ This software was developed by the above copyright holder as part of 
//_/_/ the HUMANOBS EU research project, in collaboration with the 
//_/_/ following parties:
//_/_/ 
//_/_/ Autonomous Systems Laboratory
//_/_/   Technical University of Madrid, Spain
//_/_/   http://www.aslab.org/
//_/_/
//_/_/ Communicative Machines
//_/_/   Edinburgh, United Kingdom
//_/_/   http://www.cmlabs.com/
//_/_/
//_/_/ Istituto Dalle Molle di Studi sull'Intelligenza Artificiale
//_/_/   University of Lugano and SUPSI, Switzerland
//_/_/   http://www.idsia.ch/
//_/_/
//_/_/ Institute of Cognitive Sciences and Technologies
//_/_/   Consiglio Nazionale delle Ricerche, Italy
//_/_/   http://www.istc.cnr.it/
//_/_/
//_/_/ Dipartimento di Ingegneria Informatica
//_/_/   University of Palermo, Italy
//_/_/   http://roboticslab.dinfo.unipa.it/index.php/Main/HomePage
//_/_/
//_/_/
//_/_/ --- HUMANOBS Open-Source BSD License, with CADIA Clause v 1.0 ---
//_/_/
//_/_/ Redistribution and use in source and binary forms, with or without 
//_/_/ modification, is permitted provided that the following conditions 
//_/_/ are met:
//_/_/
//_/_/ - Redistributions of source code must retain the above copyright 
//_/_/ and collaboration notice, this list of conditions and the 
//_/_/ following disclaimer.
//_/_/
//_/_/ - Redistributions in binary form must reproduce the above copyright 
//_/_/ notice, this list of conditions and the following
//_/_/ disclaimer in the documentation and/or other materials provided 
//_/_/ with the distribution.
//_/_/
//_/_/ - Neither the name of its copyright holders nor the names of its 
//_/_/ contributors may be used to endorse or promote products 
//_/_/ derived from this software without specific prior written permission.
//_/_/
//_/_/ - CADIA Clause: The license granted in and to the software under this 
//_/_/ agreement is a limited-use license. The software may not be used in 
//_/_/ furtherance of: 
//_/_/ (i) intentionally causing bodily injury or severe emotional distress 
//_/_/ to any person; 
//_/_/ (ii) invading the personal privacy or violating the human rights of 
//_/_/ any person; or 
//_/_/ (iii) committing or preparing for any act of war.
//_/_/
//_/_/ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//_/_/ "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//_/_/ LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
//_/_/ A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//_/_/ OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//_/_/ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//_/_/ LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
//_/_/ DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
//_/_/ THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//_/_/ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//_/_/ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#ifndef	sample_io_module_h
#define sample_io_module_h

#include	"integration.h"
#include	<Core/module_node.h>
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

	void	initialize(uint64_t	reference_time,uint64_t	sampling_period);
	void	finalize();		//	kills the thread.

	UNORDERED_MAP<std::string,uint32_t>	entity_map;	//	stores the OIDs assigned by the module to the objects it controls.
													//	also stores the OIDs assigned by the rMem to ontology members.
	void	register_ontology_member(std::string&name,uint32_t	OID);
	uint32_t	getOID(std::string	&name);	//	returns an ontology member's OID.

	uint64_t	reference_time;
	uint64_t	sampling_period;
	uint32_t	ontology_count;
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
		initialize(p->starting_time,p->sampling_period);
		Thread::start(Sample);
	}
	void	react(OntologyCount	*p){
		OUTPUT<<"got ontology count: "<<p->count<<std::endl;
		ontology_count=p->count;
	}
	void	react(OntologyDef	*p){
		OUTPUT<<"got ontology member: "<<p->name<<std::endl;
		std::string	name=p->name;
		register_ontology_member(name,p->OID);
	}
	void	react(Speak	*p){
		OUTPUT<<"RMem says: "<<p->word<<std::endl;
	}
MODULE_CLASS_END(SampleIO)


#endif
