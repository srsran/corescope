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
from ...models.dto_ue_id import DtoUeId
from ...types import Response


def _get_kwargs(
    gnb_id: int,
    *,
    client: Client,
    json_body: DtoUeArgs,
) -> Dict[str, Any]:
    url = "{}/corescope/api/gnb/{gnbId}/ue".format(client.base_url, gnbId=gnb_id)

    headers: Dict[str, Any] = client.get_headers()
    cookies: Dict[str, Any] = client.get_cookies()

    json_json_body = json_body.to_dict()

    return {
        "url": url,
        "headers": headers,
        "cookies": cookies,
        "timeout": client.get_timeout(),
        "json": json_json_body,
    }


def _parse_response(*, response: httpx.Response) -> Optional[DtoUeId]:
    if response.status_code == 201:
        response_201 = DtoUeId.from_dict(response.json())

        return response_201
    return None


def _build_response(*, response: httpx.Response) -> Response[DtoUeId]:
    return Response(
        status_code=response.status_code,
        content=response.content,
        headers=response.headers,
        parsed=_parse_response(response=response),
    )


def sync_detailed(
    gnb_id: int,
    *,
    client: Client,
    json_body: DtoUeArgs,
) -> Response[DtoUeId]:
    kwargs = _get_kwargs(
        gnb_id=gnb_id,
        client=client,
        json_body=json_body,
    )

    response = httpx.post(
        **kwargs,
    )

    return _build_response(response=response)


def sync(
    gnb_id: int,
    *,
    client: Client,
    json_body: DtoUeArgs,
) -> Optional[DtoUeId]:
    """Only gnbID 0 is allowed yet. Returns UE ID of the created UE."""

    return sync_detailed(
        gnb_id=gnb_id,
        client=client,
        json_body=json_body,
    ).parsed


async def asyncio_detailed(
    gnb_id: int,
    *,
    client: Client,
    json_body: DtoUeArgs,
) -> Response[DtoUeId]:
    kwargs = _get_kwargs(
        gnb_id=gnb_id,
        client=client,
        json_body=json_body,
    )

    async with httpx.AsyncClient() as _client:
        response = await _client.post(**kwargs)

    return _build_response(response=response)


async def asyncio(
    gnb_id: int,
    *,
    client: Client,
    json_body: DtoUeArgs,
) -> Optional[DtoUeId]:
    """Only gnbID 0 is allowed yet. Returns UE ID of the created UE."""

    return (
        await asyncio_detailed(
            gnb_id=gnb_id,
            client=client,
            json_body=json_body,
        )
    ).parsed
