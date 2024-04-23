#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# This script makes a HTTP request to a specific server and compares
# responses with user-defined expected responses.
from __future__ import unicode_literals

import sys

from pewpewlaz0rt4nk import (
    Beam,
    Laz0rCannon,
)

__version__ = '2.0.0'

# Set host and port.
# Read the target IP address and port from the command line arguments.
# If there are no command line arguments, use the following default values.
host = '127.0.0.1'
port = 31337

# Overwrite host and port (if specified)
if len(sys.argv) == 3:
    _, host, port = sys.argv
    port = int(port)

# Initialise Laz0rCannon
cannon = Laz0rCannon(host=host, port=port)

# Laz0r Beams (Tests to fire against the server).
cannon += Beam(
    description='Ungültiges Http-Verb "POST"',
    request='POST /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='Http-Verb in Kleinbuchstaben (get)',
    request='get /index.html HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='Keine Ressource angegeben',
    request='GET  HTTP/1.1\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Keine Http-Version angegeben',
    request='GET /\r\nHost: {host}:{port}\r\n\r\n',
    response=['HTTP/1.1 400']
)

# Pew pew!
successful = cannon.pewpew()
sys.exit(0 if successful else 1)
