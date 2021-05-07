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

T = TypeVar("T", bound="DtoUeNasPduSessionArgs")


@attr.s(auto_attribs=True)
class DtoUeNasPduSessionArgs:
    """ """

    apn_name: Union[Unset, str] = "internet"
    apn_protocol: Union[Unset, str] = "ipv4"
    apn_user: Union[Unset, str] = "srsuser"
    apn_pass: Union[Unset, str] = "srspass"
    additional_properties: Dict[str, Any] = attr.ib(init=False, factory=dict)

    def to_dict(self) -> Dict[str, Any]:
        apn_name = self.apn_name
        apn_protocol = self.apn_protocol
        apn_user = self.apn_user
        apn_pass = self.apn_pass

        field_dict: Dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if apn_name is not UNSET:
            field_dict["apn_name"] = apn_name
        if apn_protocol is not UNSET:
            field_dict["apn_protocol"] = apn_protocol
        if apn_user is not UNSET:
            field_dict["apn_user"] = apn_user
        if apn_pass is not UNSET:
            field_dict["apn_pass"] = apn_pass

        return field_dict

    @classmethod
    def from_dict(cls: Type[T], src_dict: Dict[str, Any]) -> T:
        d = src_dict.copy()
        apn_name = d.pop("apn_name", UNSET)

        apn_protocol = d.pop("apn_protocol", UNSET)

        apn_user = d.pop("apn_user", UNSET)

        apn_pass = d.pop("apn_pass", UNSET)

        dto_ue_nas_pdu_session_args = cls(
            apn_name=apn_name,
            apn_protocol=apn_protocol,
            apn_user=apn_user,
            apn_pass=apn_pass,
        )

        dto_ue_nas_pdu_session_args.additional_properties = d
        return dto_ue_nas_pdu_session_args

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
