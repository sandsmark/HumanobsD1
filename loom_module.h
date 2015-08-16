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

#ifndef	loom_module_h
#define loom_module_h

#include	"integration.h"
#include	<Core/module_node.h>


#define	N		module::Node
#define	NODE	module::Node::Get()
#define	OUTPUT	NODE->trace(N::APPLICATION)

MODULE_CLASS_BEGIN(Loom_module,Module<Loom_module>)
public:
	SharedLibrary	*lib;
	
	typedef	void	(*OutputToDevices)(_Payload	*p);
	OutputToDevices	output_to_devices;

	typedef	void	(*SetModule)( Loom_module *iModule );
	SetModule	set_module;

	std::string	device_hub_path;

	void	loadParameters(const	std::vector<word32>	&numbers,const	std::vector<std::string>	&strings){
		device_hub_path=strings[0];
	}

	void	start(){
		output_to_devices=NULL;
		lib=SharedLibrary::New(device_hub_path.c_str());
		if(lib)
		{
			output_to_devices = lib->getFunction<OutputToDevices>( "ProcessPayload" );
			set_module        = lib->getFunction<SetModule>( "SetModule" );
		}
	}
	void	stop(){
		NODE->send(this,new	StopMem(),N::PRIMARY);
		if(lib)
			delete	lib;
	}
	template<class	T>	Decision	decide(T	*p){return	WAIT;}
	template<class	T>	void	react(T	*p){}

	void	react(SystemReady	*p){
		OUTPUT<<"Loom "<<"got SysReady"<<std::endl;

		if ( set_module )
			set_module( this );

		if(output_to_devices)
			output_to_devices(p);

		uint16 vId = StartMem::CID();
		NODE->send(this,new	StartMem(),N::PRIMARY);

		//	for testing.
		//Sleep(1000);
		//NODE->send(this,new	StopMem(),N::PRIMARY);
	}

	//	rMem -> devices -> Loom.
	/*
	void	(*mem_ready)(_Payload	*);
	void	(*set_ontology_count)(_Payload	*);
	void	(*set_ontology_member)(_Payload	*);
	void	(*speak)(_Payload	*);
	void	(*move_hand)(_Payload	*);
	void	(*grab_hand)(_Payload	*);
	void	(*release_hand)(_Payload	*);
	void	(*point_at)(_Payload	*);
	void	(*look_at)(_Payload	*);
	*/
	void	react(MemReady	*p){
		
		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(OntologyCount	*p){
		//OUTPUT<<"got ontology count: "<<p->count<<std::endl;
		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(OntologyDef	*p){
		//OUTPUT<<"got ontology member: "<<p->name<<std::endl;
		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(Speak	*p){
		OUTPUT<<"RMem says: "<<p->word<<std::endl;

		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(MoveTo	*p){
		
		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(PointAt	*p){
		
		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(Grab	*p){
		
		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(Release	*p){
		
		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(LookAt	*p){
		
		if(output_to_devices)
			output_to_devices(p);
	}
	void	react(Sample_Vec3	*p){

		if(output_to_devices)
			output_to_devices(p);
	}

	void	react(Bones	*p){

		if(output_to_devices)
			output_to_devices(p);
	}

	void	react(Sample_String255	*sample){
		int a = 0;
	}

	void on_heard( const char *iText )
	{
		Sample_String255 *vText = new Sample_String255;
		strncpy_s( vText->value, 255, iText, 255 );
		NODE->send( this, vText, N::PRIMARY );
	}

MODULE_CLASS_END(Loom_module)

#endif
