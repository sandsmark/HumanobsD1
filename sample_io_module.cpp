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

#include "sample_io_module.h"
#include <thread>
#include <chrono>
//#include "../Replicode/r_code/image.h"


LOAD_MODULE(SampleIO)

void SampleIO::Sample(SampleIO *_this)  // started upon reception of the ontology map; send samples in sync with the sampling period.
{
    float delta = 0.1;
    std::string object_name = "position";
    uint32_t position_OID = _this->getOID(object_name);
    object_name = "cup";
    uint32_t cup_OID = _this->getOID(object_name);
    object_name = "self_right_hand";
    uint32_t hand_OID = _this->getOID(object_name);
    uint64_t now = Time::Get();
    uint64_t delta_t = (now - _this->reference_time) / 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(delta_t)); // sync with the sampling period.

//bool once=true;
    while (_this->running) {
// Send an update of the positions of 2 entities.
        for (uint32_t i = 0; i < 67; ++i) {
            Sample_Vec3 *s0 = new Sample_Vec3();
            s0->object = cup_OID;
            s0->attribute = position_OID;
            s0->value[0] = 0.1 + delta;
            s0->value[1] = 0.2 + delta;
            s0->value[2] = 0.3 + delta;
            NODE->send(_this, s0, N::PRIMARY);
            Sample_Vec3 *s1 = new Sample_Vec3();
            s1->object = hand_OID;
            s1->attribute = position_OID;
            s1->value[0] = 0.1 + delta;
            s1->value[1] = 0.2 + delta;
            s1->value[2] = 0.3 + delta;
            NODE->send(_this, s1, N::PRIMARY);
            delta += 0.1;
        }

        /*
         if(once){ // dynamic entity.

         once=false;
         Sample_float *s2=new Sample_float();
         s2->object=1001; // new entity created by the I/O module.
         s2->attribute=position_OID;
         s2->value=16;
         NODE->send(_this,s2,N::PRIMARY);
         }*/
        std::this_thread::sleep_for(std::chrono::milliseconds(_this->sampling_period));
    }
}

void SampleIO::initialize(uint64_t reference_time, uint64_t sampling_period)
{
    this->reference_time = reference_time;
    this->sampling_period = sampling_period / 1000; // ms.
    this->running = true;
    this->sample_thread = std::thread(Sample, this);
}

void SampleIO::finalize()
{
    this->running = false;
}

void SampleIO::register_ontology_member(std::string &name, uint32_t OID)
{
    entity_map[name] = OID;
}

uint32_t SampleIO::getOID(std::string &name)
{
    return entity_map[name];
}
