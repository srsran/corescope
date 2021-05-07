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
#include "corescope/hdr/clock.h"
#include "corescope/hdr/scope.h"

#include "srsran/build_info.h"
#include "srsran/common/common_helper.h"
#include "srsran/common/config_file.h"
#include "srsran/common/crash_handler.h"
#include "srsran/srslog/event_trace.h"
#include "srsran/srslog/srslog.h"
#include "srsran/srsran.h"
#include "srsran/support/signal_handler.h"
#include "srsran/version.h"

#include <atomic>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::string;
using std::stringstream;
using namespace corescope;
namespace bpo = boost::program_options;

/**********************************************************************
 *  Program arguments processing
 ***********************************************************************/
string config_file;
static std::atomic<bool> running = {true};

void parse_args(all_args_t* args, int argc, char* argv[])
{
  // temporary helpers for conversion
  string gnb_id{};
  string gnb_cell_id{};
  string gnb_tac{};
  string gnb_mcc{};
  string gnb_mnc{};

  // Command line only options
  bpo::options_description general("General options");

  general.add_options()("help,h", "Produce help message")("version,v", "Print version information and exit");

  // Command line or config file options
  bpo::options_description common("Configuration options");
  // clang-format off
  common.add_options()
    ("api.bind_addr",       bpo::value<string>(&args->rest.bind_addr)->default_value("0.0.0.0"),   "Bind IP address of API Server")
    ("api.bind_port",       bpo::value<uint16_t>(&args->rest.bind_port)->default_value(8000),         "Bind Port of API Server")
    ("api.log_level",       bpo::value<string>(&args->rest.log_level)->default_value("info"),     "API log level")
    ("api.log_hex_limit",   bpo::value<int>(&args->rest.log_hex_limit)->default_value(64),        "API log hex dump limit")
    
    ("gnb.gnb_id",        bpo::value<string>(&gnb_id)->default_value("0x0"),                                "gnb ID")
    ("gnb.name",          bpo::value<string>(&args->gnb.ngap.gnb_name)->default_value("srsenb01"),          "gnb Name")
    ("gnb.cell_id",       bpo::value<string>(&gnb_cell_id)->default_value("0x0"),                               "Cell ID")
    ("gnb.tac",           bpo::value<string>(&gnb_tac)->default_value("0x0"),                                   "Tracking Area Code")
    ("gnb.mcc",           bpo::value<string>(&gnb_mcc)->default_value("001"),                                   "Mobile Country Code")
    ("gnb.mnc",           bpo::value<string>(&gnb_mnc)->default_value("01"),                                    "Mobile Network Code")
    ("gnb.amf_addr",      bpo::value<string>(&args->gnb.ngap.amf_addr)->default_value("127.0.0.1"),         "IP address of MME for S1 connection")
    ("gnb.n1c_bind_addr", bpo::value<string>(&args->gnb.ngap.ngc_bind_addr)->default_value("192.168.3.1"),  "Local IP address to bind for S1AP connection")

    ("gnb.gtp_bind_addr", bpo::value<string>(&args->gnb.gtp.gtp_bind_addr)->default_value("192.168.3.1"),       "Local IP address to bind for GTP connection")

    ("log.all_level",       bpo::value<string>(&args->log.all_level),            "ALL log level")
    ("log.all_hex_limit",   bpo::value<int>(&args->log.all_hex_limit),               "ALL log hex dump limit")

    ("log.main_log",             bpo::value<string>(&args->log.filename)->default_value("/tmp/corescope.log"),  "CoreScope logfile")

    ("log.main_level",       bpo::value<string>(&args->log.main_level),   "CoreScope main application log level")
    ("log.main_hex_limit",   bpo::value<int>(&args->log.main_hex_limit),  "CoreScope main application log hex dump limit")

    ("log.file_max_size",       bpo::value<int>(&args->log.file_max_size),               "Maximum file size (in kilobytes). When passed, multiple files are created. Default -1 (single file)")

    ("log.pdcp_level",          bpo::value<string>(&args->gnb.log.pdcp_level),      "GNB PDCP log level")
    ("log.pdcp_hex_limit",      bpo::value<int>(&args->gnb.log.pdcp_hex_limit),     "GNB PDCP log hex dump limit")
    ("log.rrc_level",           bpo::value<string>(&args->gnb.log.rrc_level),       "GNB RRC  log level")
    ("log.rrc_hex_limit",       bpo::value<int>(&args->gnb.log.rrc_hex_limit),      "GNB RRC  log hex dump limit")
    ("log.gtpu_level",          bpo::value<string>(&args->gnb.log.gtpu_level),      "GNB GTPU log level")
    ("log.gtpu_hex_limit",      bpo::value<int>(&args->gnb.log.gtpu_hex_limit),     "GNB GTPU log hex dump limit")
    ("log.ngap_level",          bpo::value<string>(&args->gnb.log.ngap_level),      "GNB S1AP log level")
    ("log.ngap_hex_limit",      bpo::value<int>(&args->gnb.log.ngap_hex_limit),     "GNB S1AP log hex dump limit");

  // Positional options - config file location
  bpo::options_description position("Positional options");
  position.add_options()
    ("config_file", bpo::value<string>(&config_file), "corescope configuration file");
  // clang-format on

  bpo::positional_options_description p{};
  p.add("config_file", -1);

  // these options are allowed on the command line
  bpo::options_description cmdline_options;
  cmdline_options.add(common).add(position).add(general);

  // parse the command line and store result in vm
  bpo::variables_map vm{};
  try {
    bpo::store(bpo::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
    bpo::notify(vm);
  } catch (bpo::error& e) {
    cerr << e.what() << endl;
    exit(1);
  }

  // help option was given - print usage and exit
  if (vm.count("help")) {
    cout << "Usage: " << argv[0] << " [OPTIONS] config_file" << endl << endl;
    cout << common << endl << general << endl;
    exit(0);
  }

  // print version number and exit
  if (vm.count("version")) {
    cout << "Version " << srsran_get_version_major() << "." << srsran_get_version_minor() << "."
         << srsran_get_version_patch() << endl;
    exit(0);
  }

  // if no config file given, check users home path
  if (!vm.count("config_file")) {
    if (!config_exists(config_file, "corescope.conf")) {
      cout << "Failed to read UE configuration file " << config_file << " - exiting" << endl;
      exit(1);
    }
  }

  cout << "Reading configuration file " << config_file << "..." << endl;
  ifstream conf(config_file.c_str(), ios::in);
  if (conf.fail()) {
    cout << "Failed to read configuration file " << config_file << " - exiting" << endl;
    exit(1);
  }

  // parse config file and handle errors gracefully
  try {
    bpo::store(bpo::parse_config_file(conf, common), vm);
    bpo::notify(vm);
  } catch (const boost::program_options::error& e) {
    cerr << e.what() << endl;
    exit(1);
  }

  // Convert hex strings
  {
    stringstream sstr{};
    sstr << std::hex << vm["gnb.gnb_id"].as<string>();
    sstr >> args->gnb.ngap.gnb_id;
  }
  {
    stringstream sstr{};
    sstr << std::hex << vm["gnb.cell_id"].as<string>();
    uint16_t tmp; // Need intermediate uint16_t as uint8_t is treated as char
    sstr >> tmp;
    args->gnb.ngap.cell_id = tmp;
  }
  {
    stringstream sstr{};
    sstr << std::hex << vm["gnb.tac"].as<string>();
    sstr >> args->gnb.ngap.tac;
  }

  // Convert MCC/MNC strings
  if (!srsran::string_to_mcc(gnb_mcc, &args->gnb.ngap.mcc)) {
    cout << "Error parsing gnb.mcc:" << gnb_mcc << " - must be a 3-digit string." << endl;
  }
  if (!srsran::string_to_mnc(gnb_mnc, &args->gnb.ngap.mnc)) {
    cout << "Error parsing gnb.mnc:" << gnb_mnc << " - must be a 2 or 3-digit string." << endl;
  }

  if (args->gnb.rrc_nr.cell_info.plmn_id.from_string(gnb_mcc + gnb_mnc) != SRSRAN_SUCCESS) {
    cout << "Error converting gnb.mcc and gnb.mnc to PLMN ID." << endl;
  }
  bytes_to_mcc(args->gnb.rrc_nr.cell_info.plmn_id.mcc, &args->gnb.ngap.mcc);
  bytes_to_mnc(
      args->gnb.rrc_nr.cell_info.plmn_id.mnc, &args->gnb.ngap.mnc, args->gnb.rrc_nr.cell_info.plmn_id.nof_mnc_digits);

  args->gnb.rrc_nr.cell_info.tac = args->gnb.ngap.tac;

  // Apply all_level to any unset layers
  if (vm.count("log.all_level")) {
    if (!vm.count("log.main_level")) {
      args->log.main_level = args->log.all_level;
    }
    if (!vm.count("log.pdcp_level")) {
      args->gnb.log.pdcp_level = args->log.all_level;
    }
    if (!vm.count("log.rrc_level")) {
      args->gnb.log.rrc_level = args->log.all_level;
    }
    if (!vm.count("log.gtpu_level")) {
      args->gnb.log.gtpu_level = args->log.all_level;
    }
    if (!vm.count("log.ngap_level")) {
      args->gnb.log.ngap_level = args->log.all_level;
    }
  }

  // Apply all_hex_limit to any unset layers
  if (vm.count("log.all_hex_limit")) {
    if (!vm.count("log.main_hex_limit")) {
      args->log.main_hex_limit = args->log.all_hex_limit;
    }
    if (!vm.count("log.pdcp_hex_limit")) {
      args->gnb.log.pdcp_hex_limit = args->log.all_hex_limit;
    }
    if (!vm.count("log.rrc_hex_limit")) {
      args->gnb.log.rrc_hex_limit = args->log.all_hex_limit;
    }
    if (!vm.count("log.gtpu_hex_limit")) {
      args->gnb.log.gtpu_hex_limit = args->log.all_hex_limit;
    }
    if (!vm.count("log.ngap_hex_limit")) {
      args->gnb.log.ngap_hex_limit = args->log.all_hex_limit;
    }
  }

  args->gnb.gtp.mme_addr       = args->gnb.ngap.amf_addr;
  args->gnb.ngap.gtp_bind_addr = args->gnb.gtp.gtp_bind_addr;
}
static int sigcnt = 0;

void sig_int_handler(int signo)
{
  sigcnt++;
  running = false;
  std::cout << "Stopping srsUE... Press Ctrl+C " << (10 - sigcnt) << " more times to force stop" << endl;
  if (sigcnt >= 10) {
    exit(-1);
  }
}

string get_build_mode()
{
  return string(srsran_get_build_mode());
}

string get_build_info()
{
  if (string(srsran_get_build_info()).find("  ") != string::npos) {
    return string(srsran_get_version());
  }
  return string(srsran_get_build_info());
}

string get_build_string()
{
  stringstream ss;
  ss << "Built in " << get_build_mode() << " mode using " << get_build_info() << "." << std::endl;
  return ss.str();
}

/// Adjusts the input value in args from kbytes to bytes.
static size_t fixup_log_file_maxsize(int x)
{
  return (x < 0) ? 0 : size_t(x) * 1024u;
}

static void signal_handler()
{
  running = false;
}

int main(int argc, char* argv[])
{
  srsran_register_signal_handler(signal_handler);
  cout << "---  Software Radio Systems CoreScope  ---" << endl << endl;
  srsran_debug_handle_crash(argc, argv);

  all_args_t args = {};
  parse_args(&args, argc, argv);

  // Start the log backend.
  srslog::init();

  // Setup the default log sink.
  srslog::set_default_sink(
      (args.log.filename == "stdout")
          ? srslog::fetch_stdout_sink()
          : srslog::fetch_file_sink(args.log.filename, fixup_log_file_maxsize(args.log.file_max_size)));

  oatpp::base::Environment::init();

  // Create clock
  corescope::clock clock(std::chrono::microseconds(1000));
  // Create eNB
  std::shared_ptr<corescope::scope> scope{new corescope::scope(srslog::get_default_sink())};

  clock.register_subscriber(scope);

  if (scope->init(args) != SRSRAN_SUCCESS) {
    scope->stop();
    cout << "Starting CoreScope failed." << endl;
    return SRSRAN_ERROR;
  }

  while (running) {
    sleep(1);
  }
  /* perform detach request for all UEs */
  scope->switch_off();
  scope->stop();
  clock.stop();
  oatpp::base::Environment::destroy();
  cout << "---  exiting  ---" << endl;
  return SRSRAN_SUCCESS;
}