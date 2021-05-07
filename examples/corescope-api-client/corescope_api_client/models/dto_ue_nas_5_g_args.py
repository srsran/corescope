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

from ..models.dto_ue_nas_pdu_session_args import DtoUeNasPduSessionArgs
from ..types import UNSET, Unset

T = TypeVar("T", bound="DtoUeNas5gArgs")


@attr.s(auto_attribs=True)
class DtoUeNas5gArgs:
    """ """

    force_imsi_attach: Union[Unset, bool] = False
    eia: Union[Unset, str] = "1, 2"
    eea: Union[Unset, str] = "0, 1, 2"
    pdu_sessions: Union[Unset, List[DtoUeNasPduSessionArgs]] = UNSET
    ia5g: Union[Unset, str] = "1, 2"
    ea5g: Union[Unset, str] = "0, 1, 2"
    additional_properties: Dict[str, Any] = attr.ib(init=False, factory=dict)

    def to_dict(self) -> Dict[str, Any]:
        force_imsi_attach = self.force_imsi_attach
        eia = self.eia
        eea = self.eea
        pdu_sessions: Union[Unset, List[Dict[str, Any]]] = UNSET
        if not isinstance(self.pdu_sessions, Unset):
            pdu_sessions = []
            for pdu_sessions_item_data in self.pdu_sessions:
                pdu_sessions_item = pdu_sessions_item_data.to_dict()

                pdu_sessions.append(pdu_sessions_item)

        ia5g = self.ia5g
        ea5g = self.ea5g

        field_dict: Dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if force_imsi_attach is not UNSET:
            field_dict["force_imsi_attach"] = force_imsi_attach
        if eia is not UNSET:
            field_dict["eia"] = eia
        if eea is not UNSET:
            field_dict["eea"] = eea
        if pdu_sessions is not UNSET:
            field_dict["pdu_sessions"] = pdu_sessions
        if ia5g is not UNSET:
            field_dict["ia5g"] = ia5g
        if ea5g is not UNSET:
            field_dict["ea5g"] = ea5g

        return field_dict

    @classmethod
    def from_dict(cls: Type[T], src_dict: Dict[str, Any]) -> T:
        d = src_dict.copy()
        force_imsi_attach = d.pop("force_imsi_attach", UNSET)

        eia = d.pop("eia", UNSET)

        eea = d.pop("eea", UNSET)

        pdu_sessions = []
        _pdu_sessions = d.pop("pdu_sessions", UNSET)
        for pdu_sessions_item_data in _pdu_sessions or []:
            pdu_sessions_item = DtoUeNasPduSessionArgs.from_dict(pdu_sessions_item_data)

            pdu_sessions.append(pdu_sessions_item)

        ia5g = d.pop("ia5g", UNSET)

        ea5g = d.pop("ea5g", UNSET)

        dto_ue_nas_5_g_args = cls(
            force_imsi_attach=force_imsi_attach,
            eia=eia,
            eea=eea,
            pdu_sessions=pdu_sessions,
            ia5g=ia5g,
            ea5g=ea5g,
        )

        dto_ue_nas_5_g_args.additional_properties = d
        return dto_ue_nas_5_g_args

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
