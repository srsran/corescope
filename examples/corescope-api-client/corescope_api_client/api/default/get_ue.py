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

from typing import Any, Dict, Optional

import httpx

from ...client import Client
from ...models.dto_ue_args import DtoUeArgs
from ...types import Response


def _get_kwargs(
    gnb_id: int,
    ue_id: int,
    *,
    client: Client,
) -> Dict[str, Any]:
    url = "{}/corescope/api/gnb/{gnbId}/ue/{ueId}".format(client.base_url, gnbId=gnb_id, ueId=ue_id)

    headers: Dict[str, Any] = client.get_headers()
    cookies: Dict[str, Any] = client.get_cookies()

    return {
        "url": url,
        "headers": headers,
        "cookies": cookies,
        "timeout": client.get_timeout(),
    }


def _parse_response(*, response: httpx.Response) -> Optional[DtoUeArgs]:
    if response.status_code == 200:
        response_200 = DtoUeArgs.from_dict(response.json())

        return response_200
    return None


def _build_response(*, response: httpx.Response) -> Response[DtoUeArgs]:
    return Response(
        status_code=response.status_code,
        content=response.content,
        headers=response.headers,
        parsed=_parse_response(response=response),
    )


def sync_detailed(
    gnb_id: int,
    ue_id: int,
    *,
    client: Client,
) -> Response[DtoUeArgs]:
    kwargs = _get_kwargs(
        gnb_id=gnb_id,
        ue_id=ue_id,
        client=client,
    )

    response = httpx.get(
        **kwargs,
    )

    return _build_response(response=response)


def sync(
    gnb_id: int,
    ue_id: int,
    *,
    client: Client,
) -> Optional[DtoUeArgs]:
    """Only gnbID 0 is allowed yet."""

    return sync_detailed(
        gnb_id=gnb_id,
        ue_id=ue_id,
        client=client,
    ).parsed


async def asyncio_detailed(
    gnb_id: int,
    ue_id: int,
    *,
    client: Client,
) -> Response[DtoUeArgs]:
    kwargs = _get_kwargs(
        gnb_id=gnb_id,
        ue_id=ue_id,
        client=client,
    )

    async with httpx.AsyncClient() as _client:
        response = await _client.get(**kwargs)

    return _build_response(response=response)


async def asyncio(
    gnb_id: int,
    ue_id: int,
    *,
    client: Client,
) -> Optional[DtoUeArgs]:
    """Only gnbID 0 is allowed yet."""

    return (
        await asyncio_detailed(
            gnb_id=gnb_id,
            ue_id=ue_id,
            client=client,
        )
    ).parsed
