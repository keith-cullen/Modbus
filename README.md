Modbus Protocol
===============

Copyright (c) 2016 - 2017 Keith Cullen


To test the PDU library
-----------------------

$ cd test_mb_pdu

$ make

$ ./test_mb_pdu

To test the RTU ADU library
---------------------------

$ cd test_mb_rtu_adu

$ make

$ ./test_mb_rtu_adu

To test the TCP ADU library
---------------------------

$ cd test_mb_tcp_adu

$ make

$ ./test_mb_tcp_adu

To test the IP authentication library
-------------------------------------

$ cd test_mb_ip_auth

$ make

$ ./test_mb_ip_auth

To test the RTU master/slave
----------------------------

$ socat -d -d pty,raw,echo=0 pty,raw,echo=0

(In a different terminal)

$ cd test_mb_rtu_slave

$ make

$ ./test_mb_rtu_slave /dev/pts/2

(In a different terminal)

$ cd test_mb_rtu_master

$ make

$ ./test_mb_rtu_master /dev/pts/3

To test the TCP client/server
-----------------------------

$ cd test_mb_tcp_server

$ make

$ ./test_mb_tcp_server

(In a different terminal)

$ cd test_mb_tcp_client

$ make

$ ./test_mb_tcp_client


Supported Protocol Versions
---------------------------

| Protocol Version                                        | Supported |
|---------------------------------------------------------|-----------|
| Modbus TCP                                              | yes       |
| Modbus RTU                                              | yes       |
| Modbus ASCII                                            | no        |


Supported Function Codes
------------------------

| Function Code                                           | Supported |
|---------------------------------------------------------|-----------|
| (0x01) Read Coils                                       | yes       |
| (0x02) Read Discrete Inputs                             | yes       |
| (0x03) Read Holding Registers                           | yes       |
| (0x04) Read Input Registers                             | yes       |
| (0x05) Write Single Coil                                | yes       |
| (0x06) Write Single Register                            | yes       |
| (0x07) Read Exception Status (Serial Line Only)         | yes       |
| (0x08) Diagnostics (Serial Line Only)                   | yes       |
| (0x0b) Get Comm Event Counter (Serial Line Only)        | yes       |
| (0x0c) Get Comm Event Log (Serial Line Only)            | yes       |
| (0x0f) Write Multiple Coils                             | yes       |
| (0x10) Write Multiple Registers                         | yes       |
| (0x11) Report Server ID (Serial Line Only)              | yes       |
| (0x14) Read File Record                                 | yes       |
| (0x15) Write File Record                                | yes       |
| (0x16) Mask Write Register                              | yes       |
| (0x17) Read/Write Multiple Registers                    | yes       |
| (0x18) Read FIFO Queue                                  | yes       |
| (0x2b) Encapsulated Interface Transport                 | yes       |
