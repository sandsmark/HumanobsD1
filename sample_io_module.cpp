//	sample_io_module.cpp
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

#include "sample_io_module.h"

#include	"../Replicode/r_code/image.h"


LOAD_MODULE(SampleIO)

thread_ret thread_function_call	SampleIO::Sample(void	*args){

	SampleIO	*_this=(SampleIO	*)args;

	float32	delta=0.1;

	std::string	object_name="position";
	uint32	position_OID=_this->getOID(object_name);

	object_name="cup";
	uint32	cup_OID=_this->getOID(object_name);

	object_name="self_hand";
	uint32	hand_OID=_this->getOID(object_name);

	bool	once=true;
	while(1){

		Thread::Sleep(10);

		//	Send an update of the positions of 2 entities.

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

		if(once){	//	dynamic entity.

			once=false;
			Sample_float32	*s2=new	Sample_float32();
			s2->object=1001;	//	new entity created by the I/O module.
			s2->attribute=position_OID;
			s2->value=16;
			NODE->send(_this,s2,N::PRIMARY);
		}
	}

	thread_ret_val(0);
}

void	SampleIO::initialize(){
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