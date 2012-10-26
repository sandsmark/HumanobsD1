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
					primary_thz,
					secondary_thz,
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