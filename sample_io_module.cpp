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

#include "sample_io_module.h"

//#include	"../Replicode/r_code/image.h"


LOAD_MODULE(SampleIO)

thread_ret thread_function_call	SampleIO::Sample(void	*args){	// started upon reception of the ontology map; send samples in sync with the sampling period.

	SampleIO	*_this=(SampleIO	*)args;

	float32	delta=0.1;

	std::string	object_name="position";
	uint32	position_OID=_this->getOID(object_name);

	object_name="cup";
	uint32	cup_OID=_this->getOID(object_name);

	object_name="self_right_hand";
	uint32	hand_OID=_this->getOID(object_name);

	uint64	now=Time::Get();
	uint64	delta_t=(now-_this->reference_time)/1000;
	Thread::Sleep(delta_t);	// sync with the sampling period.

	//bool	once=true;
	while(1){

		// Send an update of the positions of 2 entities.
		for(uint32	i=0;i<67;++i){
		Sample_Vec3	*s0=new	Sample_Vec3();
		s0->object=cup_OID;
		s0->attribute=position_OID;
		s0->value[0]=0.1+delta;
		s0->value[1]=0.2+delta;
		s0->value[2]=0.3+delta;
		NODE->send(_this,s0,N::PRIMARY);

		Sample_Vec3	*s1=new	Sample_Vec3();
		s1->object=hand_OID;
		s1->attribute=position_OID;
		s1->value[0]=0.1+delta;
		s1->value[1]=0.2+delta;
		s1->value[2]=0.3+delta;
		NODE->send(_this,s1,N::PRIMARY);

		delta+=0.1;
		}
/*
		if(once){	//	dynamic entity.

			once=false;
			Sample_float32	*s2=new	Sample_float32();
			s2->object=1001;	//	new entity created by the I/O module.
			s2->attribute=position_OID;
			s2->value=16;
			NODE->send(_this,s2,N::PRIMARY);
		}*/
		Thread::Sleep(_this->sampling_period);
	}

	thread_ret_val(0);
}

void	SampleIO::initialize(uint64	reference_time,uint64	sampling_period){

	this->reference_time=reference_time;
	this->sampling_period=sampling_period/1000;	// ms.
}

void	SampleIO::finalize(){

	Thread::TerminateAndWait(this);
}

void	SampleIO::register_ontology_member(std::string&name,uint32	OID){

	entity_map[name]=OID;
}

uint32	SampleIO::getOID(std::string	&name){

	return	entity_map[name];
}