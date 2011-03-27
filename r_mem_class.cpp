#include	"r_mem_class.h"

#include	"../Replicode/r_exec/object.h"
#include	"../Replicode/r_exec/init.h"


Code	*Sample_Vec3::get_code(DMem *m){

	Code	*object=new	r_exec::LObject(m);
	object->code(0)=Atom::Marker(r_exec::GetOpcode("mk.val"),4);	//	Caveat: arity does not include the opcode.
	object->code(1)=Atom::RPointer(0);
	object->code(2)=Atom::RPointer(1);
	object->code(3)=Atom::IPointer(5);	//	points to the vector.
	object->code(4)=Atom::Float(1);		//	psln_thr.
	object->code(5)=Atom::Object(r_exec::GetOpcode("vec3"),3);	//	Caveat: arity does not include the opcode.
	object->code(6)=Atom::Float(this->value[0]);
	object->code(7)=Atom::Float(this->value[1]);
	object->code(8)=Atom::Float(this->value[2]);

	object->set_reference(0,m->get_object(this->object,this->senderNodeID()));
	object->set_reference(1,m->get_object(this->attribute,this->senderNodeID()));

	return	object;
}

Code	*Sample_uint32::get_code(DMem *m){

	Code	*object=new	r_exec::LObject(m);
	object->code(0)=Atom::Marker(r_exec::GetOpcode("mk.val"),4);	//	Caveat: arity does not include the opcode.
	object->code(1)=Atom::RPointer(0);
	object->code(2)=Atom::RPointer(1);
	object->code(3)=Atom::RPointer(2);
	object->code(4)=Atom::Float(1);		//	psln_thr.

	object->set_reference(0,m->get_object(this->object,this->senderNodeID()));
	object->set_reference(1,m->get_object(this->attribute,this->senderNodeID()));
	object->set_reference(2,m->get_object(this->value,this->senderNodeID()));

	return	object;
}

Code	*Sample_float32::get_code(DMem *m){

	Code	*object=new	r_exec::LObject(m);
	object->code(0)=Atom::Marker(r_exec::GetOpcode("mk.val"),4);	//	Caveat: arity does not include the opcode.
	object->code(1)=Atom::RPointer(0);
	object->code(2)=Atom::RPointer(1);
	object->code(3)=Atom::Float(this->value);
	object->code(4)=Atom::Float(1);		//	psln_thr.

	object->set_reference(0,m->get_object(this->object,this->senderNodeID()));
	object->set_reference(1,m->get_object(this->attribute,this->senderNodeID()));

	return	object;
}

Code	*Sample_String255::get_code(DMem *m){

	Code	*object=new	r_exec::LObject(m);
	object->code(0)=Atom::Marker(r_exec::GetOpcode("mk.val"),4);	//	Caveat: arity does not include the opcode.
	object->code(1)=Atom::RPointer(0);
	object->code(2)=Atom::RPointer(1);
	object->code(3)=Atom::IPointer(5);	//	points to the string.
	object->code(4)=Atom::Float(1);		//	psln_thr.
	std::string	text=this->value;
	r_code::Utils::SetString<Code>(object,3,text);
	
	object->set_reference(0,m->get_object(this->object,this->senderNodeID()));
	object->set_reference(1,m->get_object(this->attribute,this->senderNodeID()));

	return	object;
}