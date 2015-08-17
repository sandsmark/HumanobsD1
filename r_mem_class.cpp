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

#include "r_mem_class.h"

#include <r_exec/object.h>
#include <r_exec/init.h>
#include <r_comp/class_register.h>


Code *Sample_Vec3::get_code(DMem *m){

 Code *object=new r_exec::LObject(m);
    object->code(0)=Atom::Marker(r_comp::ClassRegister::GetOpcode("mk.val"),4); // Caveat: arity does not include the opcode.
 object->code(1)=Atom::RPointer(0);
 object->code(2)=Atom::RPointer(1);
 object->code(3)=Atom::IPointer(5); // points to the vector.
 object->code(4)=Atom::Float(1); // psln_thr.
    object->code(5)=Atom::Object(r_comp::ClassRegister::GetOpcode("vec3"),3); // Caveat: arity does not include the opcode.
 object->code(6)=Atom::Float(this->value[0]);
 object->code(7)=Atom::Float(this->value[1]);
 object->code(8)=Atom::Float(this->value[2]);

 object->set_reference(0,m->get_object(this->object,this->senderNodeID()));
 object->set_reference(1,m->get_object(this->attribute,this->senderNodeID()));

 return object;
}

Code *Sample_uint32::get_code(DMem *m){

 Code *object=new r_exec::LObject(m);
    object->code(0)=Atom::Marker(r_comp::ClassRegister::GetOpcode("mk.val"),4); // Caveat: arity does not include the opcode.
 object->code(1)=Atom::RPointer(0);
 object->code(2)=Atom::RPointer(1);
 object->code(3)=Atom::RPointer(2);
 object->code(4)=Atom::Float(1); // psln_thr.

 object->set_reference(0,m->get_object(this->object,this->senderNodeID()));
 object->set_reference(1,m->get_object(this->attribute,this->senderNodeID()));
 object->set_reference(2,m->get_object(this->value,this->senderNodeID()));

 return object;
}

Code *Sample_float::get_code(DMem *m){

 Code *object=new r_exec::LObject(m);
    object->code(0)=Atom::Marker(r_comp::ClassRegister::GetOpcode("mk.val"),4); // Caveat: arity does not include the opcode.
 object->code(1)=Atom::RPointer(0);
 object->code(2)=Atom::RPointer(1);
 object->code(3)=Atom::Float(this->value);
 object->code(4)=Atom::Float(1); // psln_thr.

 object->set_reference(0,m->get_object(this->object,this->senderNodeID()));
 object->set_reference(1,m->get_object(this->attribute,this->senderNodeID()));

 return object;
}

Code *Sample_String255::get_code(DMem *m){

 Code *object=new r_exec::LObject(m);
    object->code(0)=Atom::Marker(r_comp::ClassRegister::GetOpcode("mk.val"),4); // Caveat: arity does not include the opcode.
 object->code(1)=Atom::RPointer(0);
 object->code(2)=Atom::RPointer(1);
 object->code(3)=Atom::IPointer(5); // points to the string.
 object->code(4)=Atom::Float(1); // psln_thr.
 std::string text=this->value;
 r_code::Utils::SetString<Code>(object,3,text);
 //object->trace();
 
 object->set_reference(0,m->get_object(this->object,this->senderNodeID()));
 object->set_reference(1,m->get_object(this->attribute,this->senderNodeID()));

 return object;
}
