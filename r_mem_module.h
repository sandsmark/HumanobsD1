//	r_mem_module.h
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

#ifndef	r_mem_module_h
#define r_mem_module_h

#include	"integration.h"
#include	"../mBrane/trunk/Core/module_node.h"
#include	"../Replicode/r_exec/mem.h"
#include	"../Replicode/r_comp/decompiler.h"

#define	N		module::Node
#define	NODE	module::Node::Get()
#define	OUTPUT	NODE->trace(N::APPLICATION)


MODULE_CLASS_BEGIN(RMem,Module<RMem>)
private:
	std::string	source_code_path;

	uint32	base_period;
	uint16	reduction_core_count;
	uint16	time_core_count;
	uint32	probe_level;
	uint16	notification_resilience;
	uint32	goal_resilience;
	uint32	asmp_resilience;
	uint32	sim_resilience;
	float32	float_tolerance;
	float32	time_tolerance;
	
	r_comp::Decompiler	decompiler;
	DMem				*mem;

	uint64							starting_time;
	r_comp::Image					*image;
	r_code::vector<r_code::Code	*>	ram_objects;

	int32	initialize();
	void	finalize();

	void	decompile(r_comp::Decompiler	&decompiler,r_comp::Image	*image,uint64	time_offset);

	void	send_ontology_map();
public:
	void	loadParameters(const	std::vector<word32>	&numbers,const	std::vector<std::string>	&strings){
		source_code_path=strings[0];
		base_period=numbers[0];
		reduction_core_count=numbers[1];
		time_core_count=numbers[2];
		probe_level=numbers[3];
		notification_resilience=numbers[4];
		goal_resilience=numbers[5];
		asmp_resilience=numbers[6];
		sim_resilience=numbers[7];
		uint32	f=numbers[8];
		float_tolerance=*reinterpret_cast<float32	*>(&f);
		f=numbers[9];
		time_tolerance=*reinterpret_cast<float32	*>(&f);
	}
	void	start(){
		int32	err=initialize();
		OUTPUT<<"RMem "<<"got started"<<std::endl;
		if(err)
			OUTPUT<<"Error in rMem::initialize()\n";
	}
	void	stop(){
		finalize();
		OUTPUT<<"RMem "<<"stopped"<<std::endl;
	}
	template<class	T>	Decision	decide(T	*p){
		return	WAIT;
	}
	template<class	T>	void	react(T	*p){
		OUTPUT<<"RMem "<<"got message"<<std::endl;
	}
	void	react(SystemReady	*p){
		OUTPUT<<"RMem "<<"got SysReady"<<std::endl;
	}
	void	react(StartMem	*p){
		send_ontology_map();
		starting_time=mem->start();
		NODE->send(this,new	MemReady(),N::PRIMARY);
	}
	void	react(StopMem	*p){
		mem->suspend();
		image=mem->get_image();
		mem->resume();

		OUTPUT<<"\nShutting rMem down...\n";
		mem->stop();

		decompile(decompiler,image,starting_time);
		
		delete	image;
	}

	//	devices -> rMem.
	/*
	void	(*actor_speaks)(_Payload	*p);
	void	(*actor_points_at)(_Payload	*p);
	void	(*entity_position)(_Payload	*p);
	void	(*entity_color)(_Payload	*p);
	void	(*entity_essence)(_Payload	*p);
	*/

	void	react(Sample_uint32	*sample){		
		mem->inject(sample->get_code(mem),sample->senderNodeID());
	}
	void	react(Sample_Vec3	*sample){
		mem->inject(sample->get_code(mem),sample->senderNodeID());
	}
	void	react(Sample_String255	*sample){
		mem->inject(sample->get_code(mem),sample->senderNodeID());
	}
MODULE_CLASS_END(RMem)


#endif
