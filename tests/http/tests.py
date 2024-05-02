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
# Request line
#
cannon += Beam(
    description="Request line with multiple spaces",
    request="GET  /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line with misplaced line break",
    request="GET \n/debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line with misplaced escape character",
    request="GET \r/debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line with null terminator",
    request="GET \0/debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line with invalid end sequence (missing \r)",
    request="GET /debug HTTP/1.1\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line with invalid end sequence (missing \n)",
    request="GET /debug HTTP/1.1\rHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line with null terminator (switch \r and \n",
    request="GET \0/debug HTTP/1.1\n\rHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line with more than 3 parts",
    request="GET /debug HTTP/1.1 Test\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line with less than 3 parts",
    request="GET /debug\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line leading space",
    request=" GET /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Request line trailing space",
    request="GET /debug HTTP/1.1 \r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)

#
# Headers
#
cannon += Beam(
    description="Header with null terminator",
    request="GET /debug HTTP/1.1\r\nHo\0st: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200 OK", "Content-Type: text/html"],
)
cannon += Beam(
    description="Header with misplaced line break",
    request="GET /debug HTTP/1.1\r\nHos\nt: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200 OK", "Content-Type: text/html"],
)
cannon += Beam(
    description="Header with misplaced escape character",
    request="GET /debug HTTP/1.1\r\nHo\rst: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200 OK", "Content-Type: text/html"],
)
cannon += Beam(
    description="Header with invalid end sequence (missing \r)",
    request="GET /debug HTTP/1.1\r\nHost: {host}:{port}\n\r\n",
    response=["HTTP/1.1 200 OK"],
)
cannon += Beam(
    description="Header with invalid end sequence (missing \n)",
    request="GET /debug HTTP/1.1\r\nHost: {host}:{port}\r\r\n",
    response=["HTTP/1.1 200 OK"],
)
cannon += Beam(
    description="Header with null terminator (switch \r and \n)",
    request="GET /debug HTTP/1.1\r\nHost: {host}:{port}\n\r\r\n",
    response=["HTTP/1.1 200 OK"],
)

#
# Methods
#
cannon += Beam(
    description="No HTTP method specified",
    request=" /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="HTTP method with lowercase letters",
    request="get /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 501 Not Implemented"],
)
cannon += Beam(
    description="Not implemented HTTP method",
    request="POST /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 501 Not Implemented"],
)

#
# HTTP-Version
#
cannon += Beam(
    description="No HTTP-Version specified",
    request="GET /debug\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="HTTP-Version with lowercase letters",
    request="GET /debug http/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 505 HTTP Version Not Supported"],
)
cannon += Beam(
    description="HTTP-Version 1.0",
    request="GET /debug HTTP/1.0\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200 OK"],
)
cannon += Beam(
    description="HTTP-Version 1.1",
    request="GET /debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 200 OK"],
)
cannon += Beam(
    description="HTTP-Version 2.0",
    request="GET /debug HTTP/2.0\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 505 HTTP Version Not Supported"],
)

#
# Resources
#
cannon += Beam(
    description="No ressource specified",
    request="GET  HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 400 Bad Request"],
)
cannon += Beam(
    description="Unicode ressource",
    request="GET /debugß*#+ HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 404 Not Found"],
)
cannon += Beam(
    description="ressource outside of document root",
    request="GET /../../debug HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n",
    response=["HTTP/1.1 404 Not Found"],
)

#
# Host
#
cannon += Beam(
    description='No Host specified',
    request='GET /index.html HTTP/1.1\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
   description='Host with lowercase letters',
   request='GET /index.html HTTP/1.1\r\nhost: {host}:{port}\r\n\r\n',
   response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='Host with uppercase letters',
    request='GET /index.html HTTP/1.1\r\nHOST: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='Host with invalid format',
    request='GET /index.html HTTP/1.1\r\nHOST example.com\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='Host with invalid format',
    request='GET /index.html HTTP/1.1\r\nHOST:example.com\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='Host with no value',
    request='GET /index.html HTTP/1.1\r\nHOST:\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='Host intern - CAPS',
    request='GET /index.html HTTP/1.1\r\nHOST: INTERN\r\n\r\n',
    response=['HTTP/1.1 401 Authentication required']
)
cannon += Beam(
    description='Host intern - lower',
    request='GET /index.html HTTP/1.1\r\nhost: intern\r\n\r\n',
    response=['HTTP/1.1 401 Authentication required']
)
cannon += Beam(
    description='Host intern - mixed',
    request='GET /index.html HTTP/1.1\r\nhost: InTeRn\r\n\r\n',
    response=['HTTP/1.1 401 Authentication required']
)
cannon += Beam(
    description='Host intern - invalid',
    request='GET /index.html HTTP/1.1\r\nhost: intern.com\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='Host intern - with port',
    request='GET /index.html HTTP/1.1\r\nhost: intern:23232\r\n\r\n',
    response=['HTTP/1.1 401 Authentication required']
)
cannon += Beam(
    description='Host extern',
    request='GET /index.html HTTP/1.1\r\nhost: extern\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='Host extern - invalid (requires pse-BeispielWebsite to be served)',
    request='GET /index.html HTTP/1.1\r\nhost: extern.com\r\n\r\n',
    response=[
        'HTTP/1.1 200 OK',
        'Content-Type: text/html', # the order of the headers is not guaranteed
        'Content-Length: 5689', # if another site is served this will be different
    ]
)
cannon += Beam(
    description='Host extern - with port (requires pse-BeispielWebsite to be served)',
    request='GET /index.html HTTP/1.1\r\nhost: extern:23232\r\n\r\n',
    response=[
        'HTTP/1.1 200 OK',
        'Content-Type: text/html', # the order of the headers is not guaranteed
        'Content-Length: 169', # if another site is served this will be different
    ]
)


# Pew pew!
successful = cannon.pewpew()
sys.exit(0 if successful else 1)
