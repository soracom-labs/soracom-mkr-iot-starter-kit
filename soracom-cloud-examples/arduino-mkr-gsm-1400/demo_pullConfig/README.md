TODO(jon) WIP, README required
TODO() Add a screen grab of HARVEST
TODO() Add a screen grab of METADATA
TODO(jon) make letters into numbers for AT command output

## Soracom Metadata
For the example to work, a configuration must already be installed in Soracom Metadata.

Following are two configuration examples that have been tested with this code.  You will need to copy one of the following JSON configurations into the Soracom Metadata configuration on your User Console.

```json
  {"PTCL":"HTTP",
    "HST":"harvest.soracom.io",
    "PRT":"80"}
```

```json
  {"PTCL": "HTTP",
    "HST": "unified.soracom.io",
    "PRT": "80"}
```
Additional information on Soracom Metadata is available online at [Soracom Metadata](https://developers.soracom.io/en/docs/air/metadata-service/).  Access to the Soracom console is at [Soracom Console](https://console.soracom.io).


```text
17:29:33.187 ->
17:29:33.187 -> Starting Soracom CONFIG test..
17:29:33.187 -> Starting Modem, GSM, GPRS and Soracom Cloud connection
17:29:33.287 -> 	[1/6] GSM_READY * Modem IMEI : 357520078534019 SIM ICCID : 8942310019001627660
17:29:47.281 -> 	[2/6] CONNECTING
17:30:13.285 -> 	[2/6] CONNECTING
17:30:39.307 -> 	[2/6] ERROR!
17:30:59.161 -> 	[2/6] GPRS_READY *
17:31:08.554 -> 	[3/6] Cloud Connection to Soracom metadata, host : metadata.soracom.io, port : 80TRUE *
17:31:10.346 -> .	[4/6] Requesting update from Soracom Metadata
17:31:11.474 -> .	[5/6] Awaiting reply from Soracom metadata
17:31:12.502 -> HTTP/1.1 200 OK
17:31:12.502 -> Content-Type: application/json
17:31:12.502 -> Content-Length: 53
17:31:12.502 -> Date: Fri, 22 Jan 2021 17:31:11 GMT
17:31:12.502 -> Connection: close
17:31:12.502 ->
17:31:12.502 -> {"PTCL":"HTTP","HST":"unified.soracom.io","PRT":"80"}
17:31:12.801 -> Check status : Found 200 OK
17:31:12.801 -> Find JSON string : FOUND JSON BLOCK
17:31:12.801 ->
17:31:12.801 -> Configuration from Soracom Metadata service
17:31:12.801 -> Application Protocol :	HTTP
17:31:12.801 -> Interface Name :	unified.soracom.io
17:31:12.801 -> Interface Port :	80
17:31:12.801 ->
17:31:12.801 -> waitForResponse : +CSQ: 8,4
17:31:12.801 -> waitForResponse : +COPS: 0,0,"3",2
17:31:12.834 -> 	[6/6] Cloud Connection to Soracom cloud, host : unified.soracom.io, port : 80 TRUE *
17:31:14.560 -> POST BUFFER : POST / HTTP/1.1
17:31:14.560 -> Host: unified.soracom.io
17:31:14.560 -> User-Agent: Soracom/1.0
17:31:14.560 -> Accept: application/json
17:31:14.560 -> X-Clacks-Overhead: GNU Terry Pratchett
17:31:14.560 -> Connection: close
17:31:14.560 -> Content-Length: 50
17:31:14.560 ->
17:31:14.560 -> {\"AT+CSQ\":\"8,4\", \"AT+COPS?\":\"0,0,\"3\",2\"}
17:31:14.626 -> .HTTP/1.1 201 Created
17:31:15.653 -> Date: Fri, 22 Jan 2021 17:31:15 GMT
17:31:15.686 -> Connection: close
17:31:15.686 -> Content-Length: 0
17:31:15.686 ->
17:31:15.951 -> client.stop
17:31:15.951 -> gprs.detachGPRS : 1
17:31:16.680 -> gsmAccess.shutdown : 1
```

- `20:23:58.731 -> HTTP/1.1 200 OK`; the reply to our request for metadata from the Soracom service.
- `20:24:00.986 -> POST BUFFER : POST / HTTP/1.1`; our POST to the Soracom Service endpoint provided in the Metadata configuration download.
- `20:24:01.086 -> .HTTP/1.1 201 Created`; the reply to our POST request

