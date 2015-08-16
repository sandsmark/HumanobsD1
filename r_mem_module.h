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

#ifndef	r_mem_module_h
#define r_mem_module_h

#include	"integration.h"
#include	<Core/module_node.h>
#include	<r_comp/decompiler.h>

#define	N		module::Node
#define	NODE	module::Node::Get()
#define	OUTPUT	NODE->trace(N::APPLICATION)


MODULE_CLASS_BEGIN(RMem,Module<RMem>)
private:
	std::string	source_code_path;

	// Init section.
	uint32	base_period;
	uint32	reduction_core_count;
	uint32	time_core_count;

	// System section.
	float32	mdl_inertia_sr_thr;
	uint32	mdl_inertia_cnt_thr;
	float32	tpx_dsr_thr;
	uint32	min_sim_time_horizon;
	uint32	max_sim_time_horizon;
	float32	sim_time_horizon;
	uint32	tpx_time_horizon;
	uint32	perf_sampling_period;
	float32	float_tolerance;
	uint32	time_tolerance;
	uint32	primary_thz;
	uint32	secondary_thz;

	// Debug section.
	bool	debug;
	uint32	ntf_mk_res;
	uint32	goal_pred_success_res;
	
	// Run section.
	uint32	probe_level;
	
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

		uint32	f=numbers[3];
		mdl_inertia_sr_thr=*reinterpret_cast<float32	*>(&f);
		mdl_inertia_cnt_thr=numbers[4];
		f=numbers[5];
		tpx_dsr_thr=*reinterpret_cast<float32	*>(&f);
		min_sim_time_horizon=numbers[6];
		max_sim_time_horizon=numbers[7];
		f=numbers[8];
		sim_time_horizon=*reinterpret_cast<float32	*>(&f);
		tpx_time_horizon=numbers[9];
		perf_sampling_period=numbers[10];
		f=numbers[11];
		float_tolerance=*reinterpret_cast<float32	*>(&f);
		time_tolerance=numbers[12];
		primary_thz=numbers[13];
		secondary_thz=numbers[14];

		debug=numbers[15]>0?true:false;
		ntf_mk_res=numbers[16];
		goal_pred_success_res=numbers[17];

		probe_level=numbers[18];
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

		//NODE->subscribeMessage(this,Module<RMem>::CID(),0,1,CLASS_ID(StartMem));

		//starting_time=mem->start();

		//Thread::Sleep(1000);

		//image=mem->get_image();

		//OUTPUT<<"\nShutting rMem down...\n";

		//mem->stop();

		//decompile(decompiler,image,starting_time);
		
		delete	image;
	}
	void	react(StartMem	*p){
		OUTPUT<<"RMem "<<"got StartMem"<<std::endl;
		OUTPUT<<"RMem "<<"sending ontology map"<<std::endl;
		send_ontology_map();
		starting_time=mem->start();
		NODE->send(this,new	MemReady(starting_time,100000),N::PRIMARY);	// TODO: find a way to import constants (here: sampling_period defined in std.replicode) from replicode files.
	}
	void	react(StopMem	*p){
		
		//image=mem->get_objects();

		OUTPUT<<"\nShutting rMem down...\n";

		mem->stop();

		//decompile(decompiler,image,starting_time);
		
		//delete	image;
	}

	//	devices -> rMem.

	//void	(*actor_speaks)(_Payload	*p);
	//void	(*actor_points_at)(_Payload	*p);
	//void	(*entity_position)(_Payload	*p);
	//void	(*entity_color)(_Payload	*p);
	//void	(*entity_essence)(_Payload	*p);

	void	react(Sample_uint32	*sample){		
		//sample->trace();
		mem->inject(sample->get_code(mem),sample->senderNodeID());
	}
	void	react(Sample_Vec3	*sample){
		sample->trace();
		mem->inject(sample->get_code(mem),sample->senderNodeID());
	}
	void	react(Sample_String255	*sample){
		//sample->trace();
		mem->inject(sample->get_code(mem),sample->senderNodeID());
	}
	void	react(Sample_float32	*sample){
		//sample->trace();
		//mem->inject(sample->get_code(mem),sample->senderNodeID());
	}
MODULE_CLASS_END(RMem)


#endif
