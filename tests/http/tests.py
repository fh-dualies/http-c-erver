#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# This script makes an HTTP request to a specific server and compares
# responses with user-defined expected responses.
from __future__ import unicode_literals

import sys

from pewpewlaz0rt4nk import (
    Beam,
    Laz0rCannon,
)

__version__ = "2.0.0"

# Set host and port.
# Read the target IP address and port from the command line arguments.
# If there are no command line arguments, use the following default values.
host = "127.0.0.1"
port = 31337

# Overwrite host and port (if specified)
if len(sys.argv) == 3:
    _, host, port = sys.argv
    port = int(port)

# Initialise Laz0rCannon
cannon = Laz0rCannon(host=host, port=port)

#
# Methods
#
cannon += Beam(
    description="No HTTP method specified",
    request=" /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400"],
)
cannon += Beam(
    description="HTTP method with lowercase letters",
    request="get /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 501"],
)
cannon += Beam(
    description="Not implemented HTTP method",
    request="POST /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 501"],
)

#
# HTTP-Version
#
cannon += Beam(
    description="No HTTP-Version specified",
    request="GET /debug\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400"],
)
cannon += Beam(
    description="HTTP-Version with lowercase letters",
    request="GET /debug http/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 505"],
)
cannon += Beam(
    description="HTTP-Version 1.0",
    request="GET /debug HTTP/1.0\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200"],
)
cannon += Beam(
    description="HTTP-Version 1.1",
    request="GET /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200"],
)
cannon += Beam(
    description="HTTP-Version 2.0",
    request="GET /debug HTTP/2.0\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 505"],
)

#
# Ressources
#
cannon += Beam(
    description="No ressource specified",
    request="GET  HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400"],
)
cannon += Beam(
    description="Unicode ressource",
    request="GET /debugß*#+ HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 404"],
)
cannon += Beam(
    description="ressource outside of document root",
    request="GET /../../debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 404"],
)

#
# Host
#

# TODO: Enable these tests once the server correctly handles the Host header
# cannon += Beam(
#    description='No Host specified',
#    request='GET /debug HTTP/1.1\r\n\r\n',
#    response=['HTTP/1.1 400']
# )
# cannon += Beam(
#    description='Host with lowercase letters',
#    request='GET /debug HTTP/1.1\r\nhost: {host}:{port}\r\n\r\n',
#    response=['HTTP/1.1 400']
# )
# TODO: Add more tests for Host

#
# Miscellaneous
#
cannon += Beam(
    description="Request with multiple spaces",
    request="GET /debug  HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400"],
)
cannon += Beam(
    description="Request with unicode characters",
    request="GET /debug HTTP/1.1\r\nHostß*#+: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200"],
)
cannon += Beam(
    description="[OPTIONAL] favicon.ico (depends on htdocs folder)",
    request="GET /favicon.ico HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200"],
)

# Pew pew!
successful = cannon.pewpew()
sys.exit(0 if successful else 1)
