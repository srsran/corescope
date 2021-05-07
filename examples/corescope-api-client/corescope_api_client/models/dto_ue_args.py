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

from typing import Any, Dict, List, Type, TypeVar, Union

import attr

from ..models.dto_ue_gw_args import DtoUeGwArgs
from ..models.dto_ue_log_args import DtoUeLogArgs
from ..models.dto_ue_nas_5_g_args import DtoUeNas5gArgs
from ..models.dto_ue_pcap_args import DtoUePcapArgs
from ..models.dto_ue_usim_args import DtoUeUsimArgs
from ..types import UNSET, Unset

T = TypeVar("T", bound="DtoUeArgs")


@attr.s(auto_attribs=True)
class DtoUeArgs:
    """ """

    log: Union[Unset, DtoUeLogArgs] = UNSET
    usim: Union[Unset, DtoUeUsimArgs] = UNSET
    nas_5g: Union[Unset, DtoUeNas5gArgs] = UNSET
    gw: Union[Unset, DtoUeGwArgs] = UNSET
    pcap: Union[Unset, DtoUePcapArgs] = UNSET
    additional_properties: Dict[str, Any] = attr.ib(init=False, factory=dict)

    def to_dict(self) -> Dict[str, Any]:
        log: Union[Unset, Dict[str, Any]] = UNSET
        if not isinstance(self.log, Unset):
            log = self.log.to_dict()

        usim: Union[Unset, Dict[str, Any]] = UNSET
        if not isinstance(self.usim, Unset):
            usim = self.usim.to_dict()

        nas_5g: Union[Unset, Dict[str, Any]] = UNSET
        if not isinstance(self.nas_5g, Unset):
            nas_5g = self.nas_5g.to_dict()

        gw: Union[Unset, Dict[str, Any]] = UNSET
        if not isinstance(self.gw, Unset):
            gw = self.gw.to_dict()

        pcap: Union[Unset, Dict[str, Any]] = UNSET
        if not isinstance(self.pcap, Unset):
            pcap = self.pcap.to_dict()

        field_dict: Dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if log is not UNSET:
            field_dict["log"] = log
        if usim is not UNSET:
            field_dict["usim"] = usim
        if nas_5g is not UNSET:
            field_dict["nas_5g"] = nas_5g
        if gw is not UNSET:
            field_dict["gw"] = gw
        if pcap is not UNSET:
            field_dict["pcap"] = pcap

        return field_dict

    @classmethod
    def from_dict(cls: Type[T], src_dict: Dict[str, Any]) -> T:
        d = src_dict.copy()
        _log = d.pop("log", UNSET)
        log: Union[Unset, DtoUeLogArgs]
        if isinstance(_log, Unset):
            log = UNSET
        else:
            log = DtoUeLogArgs.from_dict(_log)

        _usim = d.pop("usim", UNSET)
        usim: Union[Unset, DtoUeUsimArgs]
        if isinstance(_usim, Unset):
            usim = UNSET
        else:
            usim = DtoUeUsimArgs.from_dict(_usim)

        _nas_5g = d.pop("nas_5g", UNSET)
        nas_5g: Union[Unset, DtoUeNas5gArgs]
        if isinstance(_nas_5g, Unset):
            nas_5g = UNSET
        else:
            nas_5g = DtoUeNas5gArgs.from_dict(_nas_5g)

        _gw = d.pop("gw", UNSET)
        gw: Union[Unset, DtoUeGwArgs]
        if isinstance(_gw, Unset):
            gw = UNSET
        else:
            gw = DtoUeGwArgs.from_dict(_gw)

        _pcap = d.pop("pcap", UNSET)
        pcap: Union[Unset, DtoUePcapArgs]
        if isinstance(_pcap, Unset):
            pcap = UNSET
        else:
            pcap = DtoUePcapArgs.from_dict(_pcap)

        dto_ue_args = cls(
            log=log,
            usim=usim,
            nas_5g=nas_5g,
            gw=gw,
            pcap=pcap,
        )

        dto_ue_args.additional_properties = d
        return dto_ue_args

    @property
    def additional_keys(self) -> List[str]:
        return list(self.additional_properties.keys())

    def __getitem__(self, key: str) -> Any:
        return self.additional_properties[key]

    def __setitem__(self, key: str, value: Any) -> None:
        self.additional_properties[key] = value

    def __delitem__(self, key: str) -> None:
        del self.additional_properties[key]

    def __contains__(self, key: str) -> bool:
        return key in self.additional_properties
