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

T = TypeVar("T", bound="DtoUeUsimArgs")


@attr.s(auto_attribs=True)
class DtoUeUsimArgs:
    """ """

    mode: Union[Unset, str] = "soft"
    algo: Union[Unset, str] = "mil"
    using_op: Union[Unset, bool] = False
    op: Union[Unset, str] = ""
    opc: Union[Unset, str] = "63bfa50ee6523365ff14c1f45f88737d"
    imsi: Union[Unset, str] = "001010123456780"
    imei: Union[Unset, str] = "353490069873310"
    k: Union[Unset, str] = "00112233445566778899aabbccddeeff"
    pin: Union[Unset, str] = ""
    reader: Union[Unset, str] = ""
    additional_properties: Dict[str, Any] = attr.ib(init=False, factory=dict)

    def to_dict(self) -> Dict[str, Any]:
        mode = self.mode
        algo = self.algo
        using_op = self.using_op
        op = self.op
        opc = self.opc
        imsi = self.imsi
        imei = self.imei
        k = self.k
        pin = self.pin
        reader = self.reader

        field_dict: Dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if mode is not UNSET:
            field_dict["mode"] = mode
        if algo is not UNSET:
            field_dict["algo"] = algo
        if using_op is not UNSET:
            field_dict["using_op"] = using_op
        if op is not UNSET:
            field_dict["op"] = op
        if opc is not UNSET:
            field_dict["opc"] = opc
        if imsi is not UNSET:
            field_dict["imsi"] = imsi
        if imei is not UNSET:
            field_dict["imei"] = imei
        if k is not UNSET:
            field_dict["k"] = k
        if pin is not UNSET:
            field_dict["pin"] = pin
        if reader is not UNSET:
            field_dict["reader"] = reader

        return field_dict

    @classmethod
    def from_dict(cls: Type[T], src_dict: Dict[str, Any]) -> T:
        d = src_dict.copy()
        mode = d.pop("mode", UNSET)

        algo = d.pop("algo", UNSET)

        using_op = d.pop("using_op", UNSET)

        op = d.pop("op", UNSET)

        opc = d.pop("opc", UNSET)

        imsi = d.pop("imsi", UNSET)

        imei = d.pop("imei", UNSET)

        k = d.pop("k", UNSET)

        pin = d.pop("pin", UNSET)

        reader = d.pop("reader", UNSET)

        dto_ue_usim_args = cls(
            mode=mode,
            algo=algo,
            using_op=using_op,
            op=op,
            opc=opc,
            imsi=imsi,
            imei=imei,
            k=k,
            pin=pin,
            reader=reader,
        )

        dto_ue_usim_args.additional_properties = d
        return dto_ue_usim_args

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
