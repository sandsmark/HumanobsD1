/*
 * HUMANOBS - D1
 *
 * Eric Nivel
 * Center for Analysis and Design of Intelligent Agents
 *   Reykjavik University, Menntavegur 1, 101 Reykjavik, Iceland
 *   http://cadia.ru.is
 * Copyright(c)2012
 *
 * This software was developed by the above copyright holder as part of
 * the HUMANOBS EU research project, in collaboration with the
 * following parties:
 *
 * Autonomous Systems Laboratory
 *   Technical University of Madrid, Spain
 *   http://www.aslab.org/
 *
 * Communicative Machines
 *   Edinburgh, United Kingdom
 *   http://www.cmlabs.com/
 *
 * Istituto Dalle Molle di Studi sull'Intelligenza Artificiale
 *   University of Lugano and SUPSI, Switzerland
 *   http://www.idsia.ch/
 *
 * Institute of Cognitive Sciences and Technologies
 *   Consiglio Nazionale delle Ricerche, Italy
 *   http://www.istc.cnr.it/
 *
 * Dipartimento di Ingegneria Informatica
 *   University of Palermo, Italy
 *   http://roboticslab.dinfo.unipa.it/index.php/Main/HomePage
 *
 *
 * --- HUMANOBS Open-Source BSD License, with CADIA Clause v 1.0 ---
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * and collaboration notice, this list of conditions and the
 * following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * - Neither the name of its copyright holders nor the names of its
 * contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * - CADIA Clause: The license granted in and to the software under this
 * agreement is a limited-use license. The software may not be used in
 * furtherance of:
 * (i) intentionally causing bodily injury or severe emotional distress
 * to any person;
 * (ii) invading the personal privacy or violating the human rights of
 * any person; or
 * (iii) committing or preparing for any act of war.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef r_mem_class_h
#define r_mem_class_h

#include <Core/mBrane.h>
#include "d_mem.h"


using namespace mBrane::sdk;
using namespace mBrane::sdk::payloads;

using namespace r_code;

//// rMem control //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StartMem:
public Message<StartMem,Memory>{
public:
};

class MemReady:
public Message<MemReady,Memory>{
public:
 MemReady():Message<MemReady,Memory>(){}
 MemReady(uint64_t starting_time,uint64_t sampling_period):Message<MemReady,Memory>(),starting_time(starting_time),sampling_period(sampling_period){}
 uint64_t starting_time;
 uint64_t sampling_period;
};

class StopMem:
public Message<StopMem,Memory>{
public:
};

//// I/O -> rMem //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef char String255[255];
typedef float Vec3[3];
typedef float Vec4[4];

// Translates to (mk.val object attribute value).
// Object and attribute are identified by their OID (uint32_t)
// Value is either: uint32_t (an object's ID), uint64_t (a timestamp), bool, float, String255, Vec3, etc.
template<class U> class Sample:
public Message<U,Memory>{
public:
 uint32_t object;
 uint32_t attribute;
 virtual void trace(){ std::cout<<"object: "<<object<<" attr: "<<attribute<<" "; }
};

class Sample_Vec3:
public Sample<Sample_Vec3>{
public:
 Vec3 value;
 Code *get_code(DMem *m);
 void trace(){ Sample<Sample_Vec3>::trace(); std::cout<<"vec3: "<<value[0]<<" "<<value[1]<<" "<<value[2]<<std::endl; }
};

class Sample_uint32:
public Sample<Sample_uint32>{
public:
 uint32_t value;
 Code *get_code(DMem *m);
    void trace(){ Sample<Sample_uint32>::trace(); std::cout<<"val: "<<value<<std::endl; }
};

class Sample_float:
public Sample<Sample_float>{
public:
 float value;
 Code *get_code(DMem *m);
};

class Sample_String255:
public Sample<Sample_String255>{
public:
 Sample_String255(){ memset(value,0,256); }
 String255 value;
 Code *get_code(DMem *m);
 void trace(){ Sample<Sample_String255>::trace(); std::cout<<"val: "<<value<<std::endl; }
};

//// rMem -> I/O //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The rMem notifies the I/O devices of the OIDs for the ontology (ex: position).
// First, send the number of elements, then send each element one by one.
class OntologyCount:
public Message<OntologyCount,Memory>{
public:
 uint32_t count;
 OntologyCount(){}
 OntologyCount(uint32_t count):Message<OntologyCount,Memory>(),count(count){}
};

class OntologyDef:
public Message<OntologyDef,Memory>{
public:
 String255 name;
 uint32_t OID;
 OntologyDef(){}
 OntologyDef(std::string &name,uint32_t OID):Message<OntologyDef,Memory>(),OID(OID){
 memcpy(this->name,name.c_str(),name.length()*sizeof(char));
 }
};

// The rMem sends commands to I/O devices in the form (command arg0 ... argn).
// This translates into one class per command.

template<class U> class Command:
public Message<U,Memory>{
public:
 uint64_t deadline; // o means asap.
};

class Speak:
public Command<Speak>{
public:
 String255 word;
};

class MoveTo:
public Command<MoveTo>{
public:
 uint32_t OID; // of one hand.
 Vec3 target_position;
};

class PointAt:
public Command<PointAt>{
public:
 uint32_t OID; // of one hand.
 Vec3 target_position;
};

class Grab:
public Command<Grab>{
public:
 uint32_t OID; // of one hand.
};

class Release:
public Command<Release>{
public:
 uint32_t OID; // of one hand.
};

class LookAt:
public Command<LookAt>{
public:
 Vec3 target_position;
};

class Bones:
public Command<Bones>{
public:
 Bones() {}
 Vec3 Positions[32];
 Vec4 Orientations[32];
 uint32_t  NumBones;
 uint32_t  Id;
};


#endif
