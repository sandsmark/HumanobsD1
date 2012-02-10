//	r_mem_module.cpp
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

#include "r_mem_module.h"


LOAD_MODULE(RMem)


void	RMem::decompile(r_comp::Decompiler	&decompiler,r_comp::Image	*image,uint64	time_offset){

	std::cout<<"\ndecompiling ...\n";
	std::ostringstream	decompiled_code;
	uint32	object_count=decompiler.decompile(image,&decompiled_code,time_offset,true);
	//uint32	object_count=image->code_segment.objects.size();
	std::cout<<"... done\n";
	std::cout<<"\n\nDECOMPILATION\n\n"<<decompiled_code.str()<<std::endl;
	std::cout<<"Image taken at: "<<Time::ToString_year(image->timestamp)<<std::endl<<std::endl;
	std::cout<<object_count<<" objects\n";
}

int32	RMem::initialize(){

	std::string	error;
	if(!r_exec::Compile(source_code_path.c_str(),error)){

		std::cerr<<" <- "<<error<<std::endl;
		return	1;
	}else{

		decompiler.init(&r_exec::Metadata);

		mem=new	DMem(this);

		r_exec::Seed.get_objects(mem,ram_objects);

		mem->init(	base_period,
					reduction_core_count,
					time_core_count,
					mdl_inertia_sr_thr,
					mdl_inertia_cnt_thr,
					tpx_dsr_thr,
					min_sim_time_horizon,
					max_sim_time_horizon,
					sim_time_horizon,
					tpx_time_horizon,
					perf_sampling_period,
					float_tolerance,
					time_tolerance,
					debug,
					ntf_mk_res,
					goal_pred_success_res,
					probe_level);

		uint32	stdin_oid;
		std::string	stdin_symbol("stdin");
		uint32	stdout_oid;
		std::string	stdout_symbol("stdout");
		uint32	self_oid;
		std::string	self_symbol("self");
		UNORDERED_MAP<uint32,std::string>::const_iterator	n;
		for(n=r_exec::Seed.object_names.symbols.begin();n!=r_exec::Seed.object_names.symbols.end();++n){

			if(n->second==stdin_symbol)
				stdin_oid=n->first;
			else	if(n->second==stdout_symbol)
				stdout_oid=n->first;
			else	if(n->second==self_symbol)
				self_oid=n->first;
		}

		if(!mem->load(ram_objects.as_std(),stdin_oid,stdout_oid,self_oid))
			return	2;
	}

	return	0;
}

void	RMem::finalize(){

	delete	mem;
}

void	RMem::send_ontology_map(){

	uint16	ont_opcode=r_exec::GetOpcode("ont");
	UNORDERED_MAP<r_code::Code	*,std::string>	entities;
	for(uint16	i=0;i<ram_objects.size();++i){

		uint16	opcode=ram_objects[i]->code(0).asOpcode();
		if(	opcode==r_exec::Opcodes::Ent	||
			opcode==ont_opcode){

			entities[ram_objects[i]]=r_exec::GetAxiomName(i);
			mem->add_entity_map_entry(ram_objects[i]);
		}
	}

	NODE->send(this,new	OntologyCount(entities.size()),N::PRIMARY);
	//OUTPUT<<"RMem sent ontology member count "<<entities.size()<<std::endl;

	UNORDERED_MAP<r_code::Code	*,std::string>::const_iterator	e;
	for(e=entities.begin();e!=entities.end();++e){

		std::string	name=e->second;
		NODE->send(this,new	OntologyDef(name,e->first->get_oid()),N::PRIMARY);
		OUTPUT<<"RMem sent 1 ontology member: "<<name<<" "<<e->first->get_oid()<<std::endl;
	}
}