/**
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * This file is part of CoreScope.
 *
 * CoreScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * CoreScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#ifndef CORESCOPE_INTERFACES_H
#define CORESCOPE_INTERFACES_H

#include "scope_cfg.h"

#include "srsran/asn1/rrc/uecap.h"
#include "srsran/common/buffer_pool.h"
#include "srsran/interfaces/rrc_interface_types.h"

namespace corescope {

// SCOPE Interface to the REST API
class scope_interface_rest
{
public:
  virtual int get_gnb_status(uint32_t gnb_id, gnb_metrics_t& gnb_metric) = 0;
  virtual int get_gnb_config(uint32_t gnb_id, gnb_args_t& gnb_args)      = 0;
  virtual int add_ue(const ue_args_t& ue_args, uint32_t& ue_id_)         = 0;
  virtual int get_ue_status(uint32_t ue_id, ue_metrics_t& ue_metrics)    = 0;
  virtual int get_ue_config(uint32_t ue_id, ue_args_t& ue_args)          = 0;
  virtual int ue_switch_on(uint32_t gnb_id, uint32_t ue_id)              = 0;
  virtual int ue_switch_off(uint32_t gnb_id, uint32_t ue_id)             = 0;
  virtual int delete_ue(uint32_t ue_id)                                  = 0;
};

class tick_subscriber
{
public:
  virtual void run_tti(uint32_t tti) = 0;
};

class tick_provider
{
public:
  virtual void register_subscriber(std::shared_ptr<tick_subscriber> subscriber)   = 0;
  virtual void deregister_subscriber(std::shared_ptr<tick_subscriber> subscriber) = 0;
};

// UE PDCP interface for UE RRC
class ue_pdcp_interface_ue_rrc
{
public:
  virtual void set_rnti(uint16_t rnti)                           = 0;
  virtual void add_bearer(uint32_t eps_bearer_id, uint32_t lcid) = 0;
  virtual void release_connection()                              = 0;
};

// UE PDCP interface for gNodeB PDCP
class ue_pdcp_interface_gnb
{
public:
  // forwards user-plane data to gateway
  virtual void write_to_gw(uint32_t pdu_session_id, srsran::unique_byte_buffer_t sdu) = 0;
};

class pdcp_gnb_interface_rrc_gnb
{
public:
  virtual int register_ue(uint16_t rnti, ue_pdcp_interface_gnb* ue_pdcp_) = 0;
};

// UE RRC NR interface for GNB RRC
class ue_rrc_interface_gnb
{
public:
  virtual int write_nas(srsran::unique_byte_buffer_t sdu)        = 0;
  virtual int add_bearer(uint32_t pdu_session_id, uint32_t lcid) = 0;
};

class ue_interface_gnb : public ue_rrc_interface_gnb, public ue_pdcp_interface_gnb
{};

// gNodeB PDCP interface for UE PDCP
class gnb_pdcp_interface_ue
{
public:
  virtual void write_to_gtpu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu) = 0;
};

// gNodeB RRC interface to the UE
class gnb_rrc_interface_ue
{
public:
  virtual int         nas_to_ngap(uint16_t rnti, srsran::unique_byte_buffer_t sdu) = 0;
  virtual int         initial_ue(ue_interface_gnb*                ue_,
                                 srsran::nr_establishment_cause_t cause,
                                 srsran::unique_byte_buffer_t     sdu,
                                 uint16_t&                        rnti)                                   = 0;
  virtual cell_info_t get_cell_info()                                              = 0;
};

// GNB RRC interface for UE RRC NR
class gnb_interface_ue : public gnb_rrc_interface_ue, public gnb_pdcp_interface_ue
{};

} // namespace corescope

#endif // CORESCOPE_INTERFACES_H