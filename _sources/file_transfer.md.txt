# File Transfer

```{contents}
:depth: 3
```

## Brief

This page describes the file transfer protocol, how to use the API to upload/download
files and the limitations of it.

A file transfer is initiated with either write request (file upload) WRQ or
read request (file download) RRQ. The contents of the file are exchanged using data
packets with block size of 256 bytes. A data packet of less than block size of 256
bytes signals termination of a transfer. Notice that both sides involved in a
transfer are considered senders and receivers. One sends data and receives
acknowledgments, the other sends acknowledgments and receives data.

An error cause termination of the connection. An error is signalled by sending
an error packet. This packet is not acknowledged.

Errors are caused by three types of events: not being able to satisfy the
request (e.g., file not found, access violation), receiving a packet which
cannot be decoded and losing access to a necessary resource (e.g., disk
full or access denied during a transfer).

## File upload (WRQ)

To initiate a transfer from the host device to the remote device the `es_client_file_upload`
function is used.

*Example of starting file upload*

```C
/* esclient core library */
#include <esclient.h>

es_client_file_upload("/file/path/on/the/host/device", "/file/path/on/the/remote/device");
```

The request/response pkts excahnged between the client and the server will look like
this.

```{mermaid}
%%{init: {'theme': 'neutral'}}%%
sequenceDiagram
    participant C as ES Client
    participant S as ES Server

    activate C
    Note left of C: request packet<br/>opcode: ES_TFTP_WRQ<br/>filename: FileName<br/>
    C->>S: WRQ
    deactivate C

    activate S
    Note right of S: ack packet<br/>opcode:ES_TFTP_ACK<br/>block_number:0<br/>
    S->>C: ACK (0)
    deactivate S

    activate C
    Note left of C: DATA packet<br/>opcode:ES_TFTP_DATA<br/>block_number:1<br/>data: 256bytes<br/>
    C->>S: DATA (1)
    deactivate C

    activate S
    Note right of S: ack packet<br/>opcode:ES_TFTP_ACK<br/>block_number:1 <br/>
    S->>C: ACK (1)
    deactivate S

    activate C
    Note left of C: DATA packet<br/>opcode:ES_TFTP_DATA<br/>block_number:2<br/>data: 256bytes<br/>
    C->>S: DATA (2)
    deactivate C

    activate S
    Note right of S: ack packet<br/>opcode:ES_TFTP_ACK<br/>block_number:2<br/>
    S->>C: ACK (2)
    deactivate S

    activate C
    Note left of C: DATA packet<br/>opcode:ES_TFTP_DATA<br/>block_number:N<br/>data: < 512bytes<br/>
    C->>S: DATA (N)
    deactivate C

    activate S
    Note right of S: ack packet<br/>opcode:ES_TFTP_ACK<br/>block_number:N<br/>
    S->>C: ACK (N)
    deactivate S
```

## File download (RRQ)

To initiate a transfer from the server to the host device the `es_client_file_download`
function is used.

*Example of starting file download*

```C
/* esclient core library */
#include <esclient.h>

es_client_file_download("/file/path/on/the/remote/device", "/file/path/on/the/host/device");

```

The request/response pkts excahnged between the client and the server will look like this:

```{mermaid}
%%{init: {'theme': 'neutral'}}%%
sequenceDiagram
    participant C as ES Client
    participant S as ES Server

    activate C
    Note left of C: request packet<br/>opcode:ES_TFTP_RRQ<br/>filename: FileName<br/>
    C->>S: RRQ
    deactivate C

    activate S
    Note right of S: DATA packet<br/>opcode:ES_TFTP_DATA <br/>block_number:1 data: 512bytes<br/>
    S->>C: DATA (1)
    deactivate S

    activate C
    Note left of C: ack packet<br/>opcode:ES_TFTP_ACK<br/>block_number:1<br/>
    C->>S: ACK (1)
    deactivate C

    activate S
    Note right of S: DATA packet<br/>opcode:ES_TFTP_DATA <br/>block_number:2 data: 512bytes<br/>
    S->>C: DATA (2)
    deactivate S

    activate C
    Note left of C: ack packet<br/>opcode:ES_TFTP_ACK<br/>block_number:2<br/>
    C->>S: ACK (2)
    deactivate C

    activate S
    Note right of S: DATA packet<br/>opcode:ES_TFTP_DATA <br/>block_number:N data: < 512bytes<br/>
    S->>C: DATA (N)
    deactivate S

    activate C
    Note left of C: ack packet<br/>opcode:ES_TFTP_ACK<br/>block_number: N<br/>
    C->>S: ACK (N)
    deactivate C
````

## FAQs

*What happens if we want to upload/download a file which already exists on the remote/local
file system?*

The files are always truncated (deleted) in case they already exist on the file system.

## ES TFTP Implementation

### Opcodes

```{eval-rst}
.. autocenumerator:: es_tftp_pkt.h::es_tftp_opcode_t
    :members:
```

### Error codes

```{eval-rst}
.. autocenumerator:: es_tftp_pkt.h::es_tftp_err_code_t
    :members:
```

### Packets

```C
/* ES TFTP Packet structure */
typedef struct __attribute__((__packed__)) _es_tftp_pkt_t {
      uint8_t opcode; /**< Packet type @see es_tftp_opcode_t */
      union {
            es_tftp_request_t request;
            es_tftp_data_t data;
            es_tftp_ack_t ack;
            es_tftp_err_t err;
       };
} es_tftp_pkt_t;
```

```{eval-rst}
.. autocstruct:: es_tftp_pkt.h::es_tftp_request_t
    :members:
.. autocstruct:: es_tftp_pkt.h::es_tftp_data_t
    :members:
.. autocstruct:: es_tftp_pkt.h::es_tftp_ack_t
    :members:
.. autocstruct:: es_tftp_pkt.h::es_tftp_err_t
    :members:
```
