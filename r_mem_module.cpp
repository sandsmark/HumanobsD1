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

#include "r_mem_module.h"
#include <r_comp/class_register.h>

LOAD_MODULE(RMem)


void RMem::decompile(r_comp::Decompiler &decompiler, r_comp::Image *image, uint64_t time_offset)
{
    std::cout << "\ndecompiling ...\n";
    std::ostringstream decompiled_code;
    uint32_t object_count = decompiler.decompile(image, &decompiled_code, time_offset, true);
//uint32_t object_count=image->code_segment.objects.size();
    std::cout << "... done\n";
    std::cout << "\n\nDECOMPILATION\n\n" << decompiled_code.str() << std::endl;
    //std::cout << "Image taken at: " << Time::ToString_year(image->timestamp) << std::endl << std::endl;
    std::cout << object_count << " objects\n";
}

int32_t RMem::initialize()
{
    if (!image) {
        image = new r_comp::Image;
    }

    if (!metadata) {
        metadata = new r_comp::Metadata;
    }

    std::string error;

    if (!r_exec::Compile(source_code_path.c_str(), error, image, metadata)) {
        std::cerr << " <- " << error << std::endl;
        return 1;
    } else {
        decompiler.init(metadata);
        mem = new DMem(this);
        image->get_objects(mem, ram_objects);
        mem->init(base_period,
                  reduction_core_count,
                  time_core_count,
                  mdl_inertia_sr_thr,
                  mdl_inertia_cnt_thr,
                  tpx_dsr_thr,
                  min_sim_time_horizon,
                  max_sim_time_horizon,
                  sim_time_horizon,
                  tpx_time_horizon,
                  perf_sampling_period,
                  float_tolerance,
                  time_tolerance,
                  primary_thz,
                  secondary_thz,
                  debug,
                  ntf_mk_res,
                  goal_pred_success_res,
                  probe_level,
                  trace_levels);
        uint32_t stdin_oid;
        std::string stdin_symbol("stdin");
        uint32_t stdout_oid;
        std::string stdout_symbol("stdout");
        uint32_t self_oid;
        std::string self_symbol("self");
        UNORDERED_MAP<uint32_t, std::string>::const_iterator n;

        for (n = image->object_names.symbols.begin(); n != image->object_names.symbols.end(); ++n) {
            if (n->second == stdin_symbol) {
                stdin_oid = n->first;
            } else if (n->second == stdout_symbol) {
                stdout_oid = n->first;
            } else if (n->second == self_symbol) {
                self_oid = n->first;
            }
        }

        if (!mem->load(ram_objects.as_std(), stdin_oid, stdout_oid, self_oid)) {
            return 2;
        }
    }

    return 0;
}

void RMem::finalize()
{
    delete mem;
}

void RMem::send_ontology_map()
{
    uint16_t ont_opcode = r_comp::ClassRegister::GetOpcode("ont");
    UNORDERED_MAP<r_code::Code *, std::string> entities;

    for (uint16_t i = 0; i < ram_objects.size(); ++i) {
        uint16_t opcode = ram_objects[i]->code(0).asOpcode();

        if (opcode == r_exec::Opcodes::Ent ||
                opcode == ont_opcode) {
            entities[ram_objects[i]] = metadata->getObjectName(i);
            mem->add_entity_map_entry(ram_objects[i]);
        }
    }

    NODE->send(this, new OntologyCount(entities.size()), N::PRIMARY);
//OUTPUT<<"RMem sent ontology member count "<<entities.size()<<std::endl;
    UNORDERED_MAP<r_code::Code *, std::string>::const_iterator e;

    for (e = entities.begin(); e != entities.end(); ++e) {
        std::string name = e->second;
        NODE->send(this, new OntologyDef(name, e->first->get_oid()), N::PRIMARY);
        OUTPUT << "RMem sent 1 ontology member: " << name << " " << e->first->get_oid() << std::endl;
    }
}
