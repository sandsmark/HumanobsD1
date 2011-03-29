//	loom_module.h
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

#ifndef	loom_module_h
#define loom_module_h

#include	"integration.h"
#include	"../mBrane/trunk/Core/module_node.h"


#define	N		module::Node
#define	NODE	module::Node::Get()
#define	OUTPUT	NODE->trace(N::APPLICATION)

MODULE_CLASS_BEGIN(Loom,Module<Loom>)
public:
	SharedLibrary	*lib;

	//	rMem -> devices.
	/*
	void	(*set_ontology_count)(Payload	*);
	void	(*set_ontology_member)(Payload	*);
	void	(*speak)(Payload	*);
	void	(*move_hand)(Payload	*);
	void	(*grab_hand)(Payload	*);
	void	(*release_hand)(Payload	*);
	void	(*point_at)(Payload	*);
	void	(*look_at)(Payload	*);
	*/
	typedef	void	(*OutputToDevices)(_Payload	*p);
	OutputToDevices	output_to_devices;

	//	devices -> rMem.
	/*
	void	(*actor_speaks)(Payload	*p);
	void	(*actor_points_at)(Payload	*p);
	void	(*entity_position)(Payload	*p);
	void	(*entity_color)(Payload	*p);
	void	(*entity_essence)(Payload	*p);
	*/

	std::string	device_hub_path;

	uint32	ontology_count;

	void	loadParameters(const	std::vector<word32>	&numbers,const	std::vector<std::string>	&strings){
		device_hub_path=strings[0];
	}

	void	start(){
		lib = SharedLibrary::New(device_hub_path.c_str());
		if(lib)
			output_to_devices=lib->getFunction<OutputToDevices>("output_to_devices");
	}
	void	stop(){
		delete	lib;
	}
	template<class	T>	Decision	decide(T	*p){return	WAIT;}
	template<class	T>	void	react(T	*p){}

	void	react(SystemReady	*p){
		OUTPUT<<"Loom "<<"got SysReady"<<std::endl;
		ontology_count=0;

		if(lib)
			output_to_devices(p);

		NODE->send(this,new	StartMem(),N::PRIMARY);

		//	for testing.
		Sleep(1000);
		NODE->send(this,new	StopMem(),N::PRIMARY);
	}

	void	react(OntologyCount	*p){
		//OUTPUT<<"got ontology count: "<<p->count<<std::endl;
		if(lib)
			output_to_devices(p);
	}
	void	react(OntologyDef	*p){
		//OUTPUT<<"got ontology member: "<<p->name<<std::endl;
		if(lib)
			output_to_devices(p);
	}
	void	react(Speak	*p){
		OUTPUT<<"RMem says: "<<p->word<<std::endl;

		if(lib)
			output_to_devices(p);
	}
	void	react(MoveTo	*p){
		
		if(lib)
			output_to_devices(p);
	}
	void	react(PointAt	*p){
		
		if(lib)
			output_to_devices(p);
	}
	void	react(Grab	*p){
		
		if(lib)
			output_to_devices(p);
	}
	void	react(Release	*p){
		
		if(lib)
			output_to_devices(p);
	}
	void	react(LookAt	*p){
		
		if(lib)
			output_to_devices(p);
	}
MODULE_CLASS_END(Loom)


#endif
