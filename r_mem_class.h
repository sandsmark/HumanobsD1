//	r_mem_class.h
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

#ifndef	r_mem_class_h
#define	r_mem_class_h

//#include	"../mBrane/trunk/Core/mbrane.h"
#include	<mBrane/trunk/Core/mbrane.h>
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