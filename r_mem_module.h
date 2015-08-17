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

#ifndef r_mem_module_h
#define r_mem_module_h

#include "integration.h"
#include <Core/module_node.h>
#include <r_comp/decompiler.h>

#define N module::Node
#define NODE module::Node::Get()
#define OUTPUT NODE->trace(N::APPLICATION)


MODULE_CLASS_BEGIN(RMem, Module<RMem>)
private:
std::string source_code_path;

// Init section.
uint32_t base_period;
uint32_t reduction_core_count;
uint32_t time_core_count;

// System section.
float mdl_inertia_sr_thr;
uint32_t mdl_inertia_cnt_thr;
float tpx_dsr_thr;
uint32_t min_sim_time_horizon;
uint32_t max_sim_time_horizon;
float sim_time_horizon;
uint32_t tpx_time_horizon;
uint32_t perf_sampling_period;
float float_tolerance;
uint32_t time_tolerance;
uint32_t primary_thz;
uint32_t secondary_thz;
uint32_t trace_levels;

// Debug section.
bool debug;
uint32_t ntf_mk_res;
uint32_t goal_pred_success_res;

// Run section.
uint32_t probe_level;

r_comp::Decompiler decompiler;
DMem *mem;

uint64_t starting_time;
r_comp::Image *image;
r_comp::Metadata                *metadata;
r_code::vector<r_code::Code *> ram_objects;

int32_t initialize();
void finalize();

void decompile(r_comp::Decompiler &decompiler, r_comp::Image *image, uint64_t time_offset);

void send_ontology_map();
public:
void loadParameters(const std::vector<int32_t> &numbers, const std::vector<std::string> &strings)
{
    source_code_path = strings[0];
    base_period = numbers[0];
    reduction_core_count = numbers[1];
    time_core_count = numbers[2];
    uint32_t f = numbers[3];
    mdl_inertia_sr_thr = *reinterpret_cast<float *>(&f);
    mdl_inertia_cnt_thr = numbers[4];
    f = numbers[5];
    tpx_dsr_thr = *reinterpret_cast<float *>(&f);
    min_sim_time_horizon = numbers[6];
    max_sim_time_horizon = numbers[7];
    f = numbers[8];
    sim_time_horizon = *reinterpret_cast<float *>(&f);
    tpx_time_horizon = numbers[9];
    perf_sampling_period = numbers[10];
    f = numbers[11];
    float_tolerance = *reinterpret_cast<float *>(&f);
    time_tolerance = numbers[12];
    primary_thz = numbers[13];
    secondary_thz = numbers[14];
    debug = numbers[15] > 0 ? true : false;
    ntf_mk_res = numbers[16];
    goal_pred_success_res = numbers[17];
    probe_level = numbers[18];
    trace_levels = 0;
}
void start()
{
    int32_t err = initialize();
    OUTPUT << "RMem " << "got started" << std::endl;

    if (err) {
        OUTPUT << "Error in rMem::initialize()\n";
    }
}
void stop()
{
    finalize();
    OUTPUT << "RMem " << "stopped" << std::endl;
}
template<class T> Decision decide(T *p)
{
    return WAIT;
}
template<class T> void react(T *p)
{
    OUTPUT << "RMem " << "got message" << std::endl;
}
void react(SystemReady *p)
{
    OUTPUT << "RMem " << "got SysReady" << std::endl;
//NODE->subscribeMessage(this,Module<RMem>::CID(),0,1,CLASS_ID(StartMem));
//starting_time=mem->start();
//Thread::Sleep(1000);
//image=mem->get_image();
//OUTPUT<<"\nShutting rMem down...\n";
//mem->stop();
//decompile(decompiler,image,starting_time);
    delete metadata;
    delete image;
}
void react(StartMem *p)
{
    OUTPUT << "RMem " << "got StartMem" << std::endl;
    OUTPUT << "RMem " << "sending ontology map" << std::endl;
    send_ontology_map();
    starting_time = mem->start();
    NODE->send(this, new MemReady(starting_time, 100000), N::PRIMARY); // TODO: find a way to import constants (here: sampling_period defined in std.replicode) from replicode files.
}
void react(StopMem *p)
{
//image=mem->get_objects();
    OUTPUT << "\nShutting rMem down...\n";
    mem->stop();
//decompile(decompiler,image,starting_time);
//delete image;
}

// devices -> rMem.

//void (*actor_speaks)(_Payload *p);
//void (*actor_points_at)(_Payload *p);
//void (*entity_position)(_Payload *p);
//void (*entity_color)(_Payload *p);
//void (*entity_essence)(_Payload *p);

void react(Sample_uint32 *sample)
{
//sample->trace();
    mem->inject(sample->get_code(mem), sample->senderNodeID());
}
void react(Sample_Vec3 *sample)
{
    sample->trace();
    mem->inject(sample->get_code(mem), sample->senderNodeID());
}
void react(Sample_String255 *sample)
{
//sample->trace();
    mem->inject(sample->get_code(mem), sample->senderNodeID());
}
void react(Sample_float *sample)
{
//sample->trace();
//mem->inject(sample->get_code(mem),sample->senderNodeID());
}
MODULE_CLASS_END(RMem)


#endif
