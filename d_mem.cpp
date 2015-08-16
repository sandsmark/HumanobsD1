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

#include	"d_mem.h"
#include	"r_mem_class.h"
#include <r_comp/class_register.h>


#define	N		module::Node
#define	NODE	module::Node::Get()
#define	OUTPUT	NODE->trace(N::APPLICATION)

DMem::DMem(mBrane::sdk::module::_Module	*m):r_exec::Mem<r_exec::LObject,r_exec::MemVolatile>(),module(m){
}

void	DMem::eject(View	*view,uint16	nodeID){
}

void	DMem::eject(Code	*command){
	
	uint16	function=(command->code(CMD_FUNCTION).atom>>8)	&	0x000000FF;
    if(function==r_comp::ClassRegister::GetOpcode("speak")){

		Speak	*speak=new	Speak();
		uint16	args_set_index=command->code(CMD_ARGS).asIndex();
		uint16	word_index=args_set_index+1;
		
		std::string	w=r_code::Utils::GetString<Code>(command,word_index);
		memcpy(speak->word,w.c_str(),w.length());
		
		uint16	deadline_index=args_set_index+2;
		speak->deadline=Utils::GetTimestamp<Code>(command,deadline_index);

		NODE->send(module,speak,N::PRIMARY);
    }else	if(function==r_comp::ClassRegister::GetOpcode("move_hand")){

		MoveTo	*move_to=new	MoveTo();
		uint16	args_set_index=command->code(CMD_ARGS).asIndex();

		uint16	hand_index=args_set_index+1;
		move_to->OID=command->get_reference(command->code(hand_index).asIndex())->get_oid();

		uint16	target_position_index=command->code(args_set_index+2).asIndex();
		move_to->target_position[0]=command->code(target_position_index+1).asFloat();
		move_to->target_position[1]=command->code(target_position_index+2).asFloat();
		move_to->target_position[2]=command->code(target_position_index+3).asFloat();

		uint16	deadline_index=args_set_index+3;
		move_to->deadline=Utils::GetTimestamp<Code>(command,deadline_index);

		NODE->send(module,move_to,N::PRIMARY);
    }else	if(function==r_comp::ClassRegister::GetOpcode("grab_hand")){

		Grab	*grab_hand=new	Grab();
		uint16	args_set_index=command->code(CMD_ARGS).asIndex();

		uint16	hand_index=args_set_index+1;
		grab_hand->OID=command->get_reference(command->code(hand_index).asIndex())->get_oid();

		uint16	deadline_index=args_set_index+2;
		grab_hand->deadline=Utils::GetTimestamp<Code>(command,deadline_index);

		NODE->send(module,grab_hand,N::PRIMARY);
    }else	if(function==r_comp::ClassRegister::GetOpcode("release_hand")){

		Release	*release_hand=new	Release();
		uint16	args_set_index=command->code(CMD_ARGS).asIndex();

		uint16	hand_index=args_set_index+1;
		release_hand->OID=command->get_reference(command->code(hand_index).asIndex())->get_oid();

		uint16	deadline_index=args_set_index+2;
		release_hand->deadline=Utils::GetTimestamp<Code>(command,deadline_index);

		NODE->send(module,release_hand,N::PRIMARY);
    }else	if(function==r_comp::ClassRegister::GetOpcode("point_at")){

		PointAt	*point_at=new	PointAt();
		uint16	args_set_index=command->code(CMD_ARGS).asIndex();

		uint16	hand_index=args_set_index+1;
		point_at->OID=command->get_reference(command->code(hand_index).asIndex())->get_oid();

		uint16	target_position_index=command->code(args_set_index+2).asIndex();
		point_at->target_position[0]=command->code(target_position_index+1).asFloat();
		point_at->target_position[1]=command->code(target_position_index+2).asFloat();
		point_at->target_position[2]=command->code(target_position_index+3).asFloat();

		uint16	deadline_index=args_set_index+3;
		point_at->deadline=Utils::GetTimestamp<Code>(command,deadline_index);

		NODE->send(module,point_at,N::PRIMARY);
    }else	if(function==r_comp::ClassRegister::GetOpcode("look_at")){

		LookAt	*look_at=new	LookAt();
		uint16	args_set_index=command->code(CMD_ARGS).asIndex();

		uint16	target_position_index=command->code(args_set_index+1).asIndex();
		look_at->target_position[0]=command->code(target_position_index+1).asFloat();
		look_at->target_position[1]=command->code(target_position_index+2).asFloat();
		look_at->target_position[2]=command->code(target_position_index+3).asFloat();

		uint16	deadline_index=args_set_index+2;
		look_at->deadline=Utils::GetTimestamp<Code>(command,deadline_index);

		NODE->send(module,look_at,N::PRIMARY);
	}
}

void	DMem::inject(Code	*object,uint8	nodeID){	// this is application dependent WRT fact->after and fact->before.

	//object->trace();

	uint64	now=r_exec::Now();

	// Build a fact.
	Code	*fact=new	r_exec::Fact(object,now,now,1,1);	// caveat: use [now, now+sampling_period[ for enabling learning by CTPX.

	// Build a default view for the fact.
	r_exec::View	*view=build_view(now,nodeID);

	// Inject the view.
	view->set_object(fact);
	((_Mem	*)this)->inject(view);
	uint64	_n=r_exec::Now();
	//std::cout<<"time spent for injection: "<<_n-now<<std::endl;
	//std::cout<<"views in stdin: "<<((r_exec::Group	*)get_stdin())->other_views.size()<<std::endl;
}

Code	*DMem::get_object(uint32	OID,uint8	NID){

	UNORDERED_MAP<uint32,P<Code> >::const_iterator	o=entity_map.find(OID);
	if(o!=entity_map.end())
		return	o->second;

	r_exec::LObject	*new_entity=new	r_exec::LObject();	//	OID=0xFFFF. Object not injected, thus not registered. Held only by entity_map and any referring object in mem.
    new_entity->code(0)=Atom::Object(r_comp::ClassRegister::GetOpcode("ent"),1);
	new_entity->code(1)=Atom::Float(1);
	inject(new_entity,NID);
	entity_map[OID]=new_entity;
	return	new_entity;
}

void	DMem::add_entity_map_entry(Code	*entity){

	entity_map[entity->get_oid()]=entity;
}

r_exec::View	*DMem::build_view(uint64	time,uint8	nodeID)	const{	// this is application dependent WRT view->sync.

	r_exec::View	*view=new	r_exec::View();
	const	uint32	arity=VIEW_ARITY;	// reminder: opcode not included in the arity.
	uint16	write_index=0;
	uint16	extent_index=arity+1;

	view->code(VIEW_OPCODE)=Atom::SSet(r_exec::View::ViewOpcode,arity);
	view->code(VIEW_SYNC)=Atom::Float(View::SYNC_ONCE);		// sync on front.
	view->code(VIEW_IJT)=Atom::IPointer(extent_index);		// iptr to injection time.
	view->code(VIEW_SLN)=Atom::Float(1.0);					// sln.
	view->code(VIEW_RES)=Atom::Float(1);					// res is set to 1 upr of the destination group.
	view->code(VIEW_HOST)=Atom::RPointer(0);				// stdin/stdout is the only reference.
	view->code(VIEW_ORG)=Atom::Node(nodeID);				// org.

	Utils::SetTimestamp(&view->code(extent_index),time);

	view->references[0]=get_stdin();

	return	view;
}
