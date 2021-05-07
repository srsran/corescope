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

T = TypeVar("T", bound="DtoNgapConfig")


@attr.s(auto_attribs=True)
class DtoNgapConfig:
    """ """

    gnb_id: Union[Unset, int] = UNSET
    cell_id: Union[Unset, int] = UNSET
    tac: Union[Unset, int] = UNSET
    mcc: Union[Unset, str] = UNSET
    mnc: Union[Unset, str] = UNSET
    amf_addr: Union[Unset, str] = UNSET
    gtp_bind_addr: Union[Unset, str] = UNSET
    gtp_advertise_addr: Union[Unset, str] = UNSET
    ngc_bind_addr: Union[Unset, str] = UNSET
    gnb_name: Union[Unset, str] = UNSET
    additional_properties: Dict[str, Any] = attr.ib(init=False, factory=dict)

    def to_dict(self) -> Dict[str, Any]:
        gnb_id = self.gnb_id
        cell_id = self.cell_id
        tac = self.tac
        mcc = self.mcc
        mnc = self.mnc
        amf_addr = self.amf_addr
        gtp_bind_addr = self.gtp_bind_addr
        gtp_advertise_addr = self.gtp_advertise_addr
        ngc_bind_addr = self.ngc_bind_addr
        gnb_name = self.gnb_name

        field_dict: Dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if gnb_id is not UNSET:
            field_dict["gnb_id"] = gnb_id
        if cell_id is not UNSET:
            field_dict["cell_id"] = cell_id
        if tac is not UNSET:
            field_dict["tac"] = tac
        if mcc is not UNSET:
            field_dict["mcc"] = mcc
        if mnc is not UNSET:
            field_dict["mnc"] = mnc
        if amf_addr is not UNSET:
            field_dict["amf_addr"] = amf_addr
        if gtp_bind_addr is not UNSET:
            field_dict["gtp_bind_addr"] = gtp_bind_addr
        if gtp_advertise_addr is not UNSET:
            field_dict["gtp_advertise_addr"] = gtp_advertise_addr
        if ngc_bind_addr is not UNSET:
            field_dict["ngc_bind_addr"] = ngc_bind_addr
        if gnb_name is not UNSET:
            field_dict["gnb_name"] = gnb_name

        return field_dict

    @classmethod
    def from_dict(cls: Type[T], src_dict: Dict[str, Any]) -> T:
        d = src_dict.copy()
        gnb_id = d.pop("gnb_id", UNSET)

        cell_id = d.pop("cell_id", UNSET)

        tac = d.pop("tac", UNSET)

        mcc = d.pop("mcc", UNSET)

        mnc = d.pop("mnc", UNSET)

        amf_addr = d.pop("amf_addr", UNSET)

        gtp_bind_addr = d.pop("gtp_bind_addr", UNSET)

        gtp_advertise_addr = d.pop("gtp_advertise_addr", UNSET)

        ngc_bind_addr = d.pop("ngc_bind_addr", UNSET)

        gnb_name = d.pop("gnb_name", UNSET)

        dto_ngap_config = cls(
            gnb_id=gnb_id,
            cell_id=cell_id,
            tac=tac,
            mcc=mcc,
            mnc=mnc,
            amf_addr=amf_addr,
            gtp_bind_addr=gtp_bind_addr,
            gtp_advertise_addr=gtp_advertise_addr,
            ngc_bind_addr=ngc_bind_addr,
            gnb_name=gnb_name,
        )

        dto_ngap_config.additional_properties = d
        return dto_ngap_config

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
