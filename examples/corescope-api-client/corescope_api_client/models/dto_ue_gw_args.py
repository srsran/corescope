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

from ..types import UNSET, Unset

T = TypeVar("T", bound="DtoUeGwArgs")


@attr.s(auto_attribs=True)
class DtoUeGwArgs:
    """ """

    netns: Union[Unset, str] = ""
    tun_dev_name: Union[Unset, str] = "tun_ue_$id"
    tun_dev_netmask: Union[Unset, str] = "255.255.255.0"
    additional_properties: Dict[str, Any] = attr.ib(init=False, factory=dict)

    def to_dict(self) -> Dict[str, Any]:
        netns = self.netns
        tun_dev_name = self.tun_dev_name
        tun_dev_netmask = self.tun_dev_netmask

        field_dict: Dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if netns is not UNSET:
            field_dict["netns"] = netns
        if tun_dev_name is not UNSET:
            field_dict["tun_dev_name"] = tun_dev_name
        if tun_dev_netmask is not UNSET:
            field_dict["tun_dev_netmask"] = tun_dev_netmask

        return field_dict

    @classmethod
    def from_dict(cls: Type[T], src_dict: Dict[str, Any]) -> T:
        d = src_dict.copy()
        netns = d.pop("netns", UNSET)

        tun_dev_name = d.pop("tun_dev_name", UNSET)

        tun_dev_netmask = d.pop("tun_dev_netmask", UNSET)

        dto_ue_gw_args = cls(
            netns=netns,
            tun_dev_name=tun_dev_name,
            tun_dev_netmask=tun_dev_netmask,
        )

        dto_ue_gw_args.additional_properties = d
        return dto_ue_gw_args

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
