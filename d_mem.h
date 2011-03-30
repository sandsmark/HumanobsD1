//	d_mem.h
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

#ifndef	d_mem_h
#define	d_mem_h

#include	"../mBrane/trunk/Core/module_node.h"

#include	"../Replicode/r_exec/object.h"
#include	"../Replicode/r_exec/mem.h"


class	RMem;

//	Distributed rMem.
class	DMem:
public	r_exec::Mem<r_exec::LObject>{
private:
	mBrane::sdk::module::_Module	*module;

	UNORDERED_MAP<uint32,P<Code> >	entity_map;	//	first n objects: left side: OID assigned by rMem, right side: object (ent and ont). n=number of ent and ont objects.
												//	from n+1 on: left side: OID assigned by I/O devices (new entites in the environment), right side: the corresponding replicode object.

	r_exec::View	*build_view(uint64	time,uint8	nodeID)	const;
public:
	DMem(mBrane::sdk::module::_Module	*m);

	void	eject(View	*view,uint16	nodeID);	//	RMem to RMem.
	void	eject(Code	*command);	//	RMem to I/O device.
	void	inject(Code	*object,uint8	nodeID);

	void	add_entity_map_entry(Code	*entity);	//	called for each axiomatic entity.

	Code	*get_object(uint32	OID,uint8	NID);	//	creates a new entity if the OID is not found.
};


#endif