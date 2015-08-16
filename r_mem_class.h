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

#ifndef	r_mem_class_h
#define	r_mem_class_h

#include	<Core/mBrane.h>
#include	"d_mem.h"


using	namespace	mBrane::sdk;
using	namespace	mBrane::sdk::payloads;

using	namespace	r_code;

////	rMem control	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class	StartMem:
public	Message<StartMem,Memory>{
public:
};

class	MemReady:
public	Message<MemReady,Memory>{
public:
	MemReady():Message<MemReady,Memory>(){}
	MemReady(uint64	starting_time,uint64	sampling_period):Message<MemReady,Memory>(),starting_time(starting_time),sampling_period(sampling_period){}
	uint64	starting_time;
	uint64	sampling_period;
};

class	StopMem:
public	Message<StopMem,Memory>{
public:
};

////	I/O -> rMem		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef	char	String255[255];
typedef	float32	Vec3[3];
typedef	float32	Vec4[4];

//	Translates to (mk.val object attribute value).
//	Object and attribute are identified by their OID (uint32)
//	Value is either: uint32 (an object's ID), uint64 (a timestamp), bool, float32, String255, Vec3, etc.
template<class	U>	class	Sample:
public	Message<U,Memory>{
public:
	uint32	object;
	uint32	attribute;
	virtual	void	trace(){	std::cout<<"object: "<<object<<" attr: "<<attribute<<" ";	}
};

class	Sample_Vec3:
public	Sample<Sample_Vec3>{
public:
	Vec3	value;
	Code	*get_code(DMem	*m);
	void	trace(){	Sample<Sample_Vec3>::trace(); std::cout<<"vec3: "<<value[0]<<" "<<value[1]<<" "<<value[2]<<std::endl;	}
};

class	Sample_uint32:
public	Sample<Sample_uint32>{
public:
	uint32	value;
	Code	*get_code(DMem	*m);
	void	trace(){	Sample<Sample_uint32>::trace(); std::cout<<"val: "<<value<<std::endl;	}
};

class	Sample_float32:
public	Sample<Sample_float32>{
public:
	float32	value;
	Code	*get_code(DMem	*m);
};

class	Sample_String255:
public	Sample<Sample_String255>{
public:
	Sample_String255(){	memset(value,0,256);	}
	String255	value;
	Code		*get_code(DMem	*m);
	void	trace(){	Sample<Sample_String255>::trace(); std::cout<<"val: "<<value<<std::endl;	}
};

////	rMem -> I/O		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	The rMem notifies the I/O devices of the OIDs for the ontology (ex: position).
//	First, send the number of elements, then send each element one by one.
class	OntologyCount:
public	Message<OntologyCount,Memory>{
public:
	uint32	count;
	OntologyCount(){}
	OntologyCount(uint32	count):Message<OntologyCount,Memory>(),count(count){}
};

class	OntologyDef:
public	Message<OntologyDef,Memory>{
public:
	String255	name;
	uint32		OID;
	OntologyDef(){}
	OntologyDef(std::string	&name,uint32	OID):Message<OntologyDef,Memory>(),OID(OID){
		memcpy(this->name,name.c_str(),name.length()*sizeof(char));
	}
};

//	The rMem sends commands to I/O devices in the form (command arg0 ... argn).
//	This translates into one class per command.

template<class	U>	class	Command:
public	Message<U,Memory>{
public:
	uint64	deadline;	//	o means asap.
};

class Speak:
public	Command<Speak>{
public:
	String255	word;
};

class	MoveTo:
public	Command<MoveTo>{
public:
	uint32	OID;	//	of one hand.
	Vec3	target_position;
};

class	PointAt:
public	Command<PointAt>{
public:
	uint32	OID;	//	of one hand.
	Vec3	target_position;
};

class	Grab:
public	Command<Grab>{
public:
	uint32	OID;	//	of one hand.
};

class	Release:
public	Command<Release>{
public:
	uint32	OID;	//	of one hand.
};

class	LookAt:
public	Command<LookAt>{
public:
	Vec3	target_position;
};

class	Bones:
public	Command<Bones>{
public:
	Bones() {}
	Vec3	Positions[32];
	Vec4	Orientations[32];
	uint32  NumBones;
	uint32  Id;
};


#endif
