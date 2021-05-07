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

T = TypeVar("T", bound="DtoUeLogArgs")


@attr.s(auto_attribs=True)
class DtoUeLogArgs:
    """ """

    filename: Union[Unset, str] = "/tmp/ue_$id.log"
    pdcp_level: Union[Unset, str] = "info"
    pdcp_hex_limit: Union[Unset, int] = 128
    rrc_level: Union[Unset, str] = "info"
    rrc_hex_limit: Union[Unset, int] = 128
    nas_level: Union[Unset, str] = "info"
    nas_hex_limit: Union[Unset, int] = 128
    usim_level: Union[Unset, str] = "info"
    usim_hex_limit: Union[Unset, int] = 128
    additional_properties: Dict[str, Any] = attr.ib(init=False, factory=dict)

    def to_dict(self) -> Dict[str, Any]:
        filename = self.filename
        pdcp_level = self.pdcp_level
        pdcp_hex_limit = self.pdcp_hex_limit
        rrc_level = self.rrc_level
        rrc_hex_limit = self.rrc_hex_limit
        nas_level = self.nas_level
        nas_hex_limit = self.nas_hex_limit
        usim_level = self.usim_level
        usim_hex_limit = self.usim_hex_limit

        field_dict: Dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if filename is not UNSET:
            field_dict["filename"] = filename
        if pdcp_level is not UNSET:
            field_dict["pdcp_level"] = pdcp_level
        if pdcp_hex_limit is not UNSET:
            field_dict["pdcp_hex_limit"] = pdcp_hex_limit
        if rrc_level is not UNSET:
            field_dict["rrc_level"] = rrc_level
        if rrc_hex_limit is not UNSET:
            field_dict["rrc_hex_limit"] = rrc_hex_limit
        if nas_level is not UNSET:
            field_dict["nas_level"] = nas_level
        if nas_hex_limit is not UNSET:
            field_dict["nas_hex_limit"] = nas_hex_limit
        if usim_level is not UNSET:
            field_dict["usim_level"] = usim_level
        if usim_hex_limit is not UNSET:
            field_dict["usim_hex_limit"] = usim_hex_limit

        return field_dict

    @classmethod
    def from_dict(cls: Type[T], src_dict: Dict[str, Any]) -> T:
        d = src_dict.copy()
        filename = d.pop("filename", UNSET)

        pdcp_level = d.pop("pdcp_level", UNSET)

        pdcp_hex_limit = d.pop("pdcp_hex_limit", UNSET)

        rrc_level = d.pop("rrc_level", UNSET)

        rrc_hex_limit = d.pop("rrc_hex_limit", UNSET)

        nas_level = d.pop("nas_level", UNSET)

        nas_hex_limit = d.pop("nas_hex_limit", UNSET)

        usim_level = d.pop("usim_level", UNSET)

        usim_hex_limit = d.pop("usim_hex_limit", UNSET)

        dto_ue_log_args = cls(
            filename=filename,
            pdcp_level=pdcp_level,
            pdcp_hex_limit=pdcp_hex_limit,
            rrc_level=rrc_level,
            rrc_hex_limit=rrc_hex_limit,
            nas_level=nas_level,
            nas_hex_limit=nas_hex_limit,
            usim_level=usim_level,
            usim_hex_limit=usim_hex_limit,
        )

        dto_ue_log_args.additional_properties = d
        return dto_ue_log_args

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
