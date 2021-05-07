#!/bin/python3
# Copyright 2013-2021 Software Radio Systems Limited
#
# This file is part of CoreScope.
#
# CoreScope is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# CoreScope is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# A copy of the GNU Affero General Public License can be found in
# the LICENSE file in the top-level directory of this distribution
# and at http://www.gnu.org/licenses/.
#

from subprocess import Popen
import time
import netifaces

from corescope_api_client import Client
from corescope_api_client.types import Response
from corescope_api_client.api.default import update_ue_state
from corescope_api_client.models import dto_ue_status_update, dto_ue_id
from corescope_api_client.api.default import create_ue
from corescope_api_client.models import dto_ue_args, dto_ue_nas_pdu_session_args
from corescope_api_client.api.default import get_ue_status
from corescope_api_client.models import dto_ue_status


class coreScopeTester:
    def __init__(self, ip, gnb_id):
        self.client = Client(base_url="http://{}:8000".format(ip), timeout=5)
        self.gnb_id = gnb_id

    def switch_on(self, ue_id):
        ue_status_update_args = dto_ue_status_update.DtoUeStatusUpdate(switch_on=True)

        print("Switching UE on")
        toggle_state_ue_data: Response[dto_ue_status_update.T] = update_ue_state.sync_detailed(
            client=self.client, gnb_id=0, ue_id=ue_id, json_body=ue_status_update_args)
        return str(toggle_state_ue_data.status_code).startswith("2")

    def switch_off(self, ue_id):
        ue_status_update_args = dto_ue_status_update.DtoUeStatusUpdate(switch_on=False)

        print("Switching UE off")
        toggle_state_ue_data: Response[dto_ue_status_update.T] = update_ue_state.sync_detailed(
            client=self.client, gnb_id=0, ue_id=ue_id, json_body=ue_status_update_args)
        return str(toggle_state_ue_data.status_code).startswith("2")

    def add_ue(self):
        ue_args = dto_ue_args.DtoUeArgs()

        # usim args
        ue_args.usim = dto_ue_args.DtoUeUsimArgs()

        # nas args
        ue_args.nas_5g = dto_ue_args.DtoUeNas5gArgs()

        # pdu sessions (APNs)
        ue_args.nas_5g.pdu_sessions = list()
        pdu_session_1 = dto_ue_nas_pdu_session_args.DtoUeNasPduSessionArgs(
            apn_name="internet")
        ue_args.nas_5g.pdu_sessions.append(pdu_session_1)

        # gw args
        ue_args.gw = dto_ue_args.DtoUeGwArgs()

        # pcap args
        ue_args.pcap = dto_ue_args.DtoUePcapArgs()

        # log file args
        ue_args.log = dto_ue_args.DtoUeLogArgs(
            filename="/tmp/corescope.log")

        print("Adding UE")
        add_ue_data: Response[dto_ue_id.DtoUeId] = create_ue.sync_detailed(client=self.client, gnb_id=0, json_body=ue_args)

        if str(add_ue_data.status_code).startswith("2"):
            ue_id = add_ue_data.parsed.ue_id
            return ue_id
        else:
            return -1

    def is_connected(self, ue_id):
        ue_status_data: Response[dto_ue_status.T] = get_ue_status.sync_detailed(client=self.client, gnb_id=0, ue_id=ue_id)
        if not str(ue_status_data.status_code).startswith("2"):
            print("Getting UE status failed. Exiting... (%s)" % ue_status_data.status_code)
            exit(1)
        else:
            return ue_status_data.parsed.active and ue_status_data.parsed.initiated

    def is_interface_up(self, interface):
        return interface in netifaces.interfaces() 

# Configure the following arguments
corescope_ip = "127.0.0.1"
corenetwork_ip = "10.45.0.1"
gnb_id = 0
corescope_tester = coreScopeTester(corescope_ip, gnb_id)

###
# add one UE
###
ue_id = corescope_tester.add_ue()
if ue_id >= 0:
    print("Added UE with ID: {}".format(ue_id))
else:
    print("Adding UE failed. Exiting...")
    exit(1)

###
# switch UE on
###
if not corescope_tester.switch_on(ue_id):
    print("Switching UE on failed. Exiting...")
    exit(1)
time.sleep(2)  # wait until interface is up

###
# check status of UE and local interface
###
if not corescope_tester.is_interface_up("tun_ue_{}".format(ue_id)):
    print("UE interface does not exist. Exiting...")
    exit(1)

if not corescope_tester.is_connected(ue_id):
    print("UE did not connect to core network, trying again in 2s...")
    time.sleep(2)
    if not corescope_tester.is_connected(ue_id):
        print("UE did not connect again. Exiting...")
        exit(1)

###
# send some data
###
print("Sending ping over tun interface...")
ping_proc = Popen(["ping", corenetwork_ip, "-I", "tun_ue_{}".format(ue_id), "-c", "3"])
stdout, stderr = ping_proc.communicate()

print("Sleeping for a while")
time.sleep(4)
# trigger the inactivity timer by not sending any data (add 5 sec to be sure)

if not corescope_tester.switch_off(ue_id):
    print("Switching UE off failed. Exiting...")
    exit(1)
