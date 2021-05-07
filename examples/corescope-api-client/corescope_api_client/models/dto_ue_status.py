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

from ..models.dto_ue_nas_5_g_status import DtoUeNas5gStatus
from ..types import UNSET, Unset

T = TypeVar("T", bound="DtoUeStatus")


@attr.s(auto_attribs=True)
class DtoUeStatus:
    """ """

    active: Union[Unset, bool] = UNSET
    initiated: Union[Unset, bool] = UNSET
    nas_5g: Union[Unset, DtoUeNas5gStatus] = UNSET
    additional_properties: Dict[str, Any] = attr.ib(init=False, factory=dict)

    def to_dict(self) -> Dict[str, Any]:
        active = self.active
        initiated = self.initiated
        nas_5g: Union[Unset, Dict[str, Any]] = UNSET
        if not isinstance(self.nas_5g, Unset):
            nas_5g = self.nas_5g.to_dict()

        field_dict: Dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if active is not UNSET:
            field_dict["active"] = active
        if initiated is not UNSET:
            field_dict["initiated"] = initiated
        if nas_5g is not UNSET:
            field_dict["nas_5g"] = nas_5g

        return field_dict

    @classmethod
    def from_dict(cls: Type[T], src_dict: Dict[str, Any]) -> T:
        d = src_dict.copy()
        active = d.pop("active", UNSET)

        initiated = d.pop("initiated", UNSET)

        _nas_5g = d.pop("nas_5g", UNSET)
        nas_5g: Union[Unset, DtoUeNas5gStatus]
        if isinstance(_nas_5g, Unset):
            nas_5g = UNSET
        else:
            nas_5g = DtoUeNas5gStatus.from_dict(_nas_5g)

        dto_ue_status = cls(
            active=active,
            initiated=initiated,
            nas_5g=nas_5g,
        )

        dto_ue_status.additional_properties = d
        return dto_ue_status

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
