"""
This module provides an HTTP server testing facility.

.. moduleauthor:: Lennart Grahl <lennart.grahl@gmail.com>
.. moduleauthor:: Yves-Noel Weweler <y.weweler@gmail.com>
.. moduleauthor:: Prof. Sebastian Schinzel <schinzel@fh-muenster.de>
"""

from __future__ import (
    print_function,
    unicode_literals,
)

import socket
from builtins import (
    object,
    zip,
)

__version__ = "2.0.1"
__all__ = ("Beam", "Laz0rCannon")


class BeamError(Exception):
    """This exception is for uncategorised beam errors."""

    pass


class BeamRequestError(BeamError):
    """This exception is for HTTP request related errors."""

    pass


class BeamResponseError(BeamError):
    """This exception is for HTTP response related errors."""

    pass


class Response(object):
    """
    Container to store a validated HTTP response line.

    Arguments:
        - `pass_`: Represents a valid or invalid response line.
        - `expected`: The expected response string.
        - `received`: The actual response string.
    """

    def __init__(self, pass_, expected, received):
        """Initialize the container and store the arguments."""
        self.pass_ = pass_
        self.expected = expected
        self.received = received


class Beam(object):
    """
    A beam to be shot by the :class:`Laz0rCannon` (e.g. an HTTP test case).

    Arguments:
        - `request`: The request string that will be sent.
        - `response`: An iterable of strings containing the expected
          response.
        - `description`: The description of the test case.
        - `host`: The hostname of the HTTP server.
        - `port`: The port number where the HTTP server is reachable.
        - `timeout`: The amount of seconds until a timeout occurs while
           reading from/writing to the socket.
        - `buffer_`: The amount of bytes that will be sent at once. See
          :ref:`this section <buffer_>`.
        - `shutdown`: `False` or one of the constants of module
          :mod:`socket`. See :ref:`this section <shutdown>`.
    """

    _sent = _socket = _data = None

    def __init__(
            self,
            request,
            response,
            description=None,
            host=None,
            port=None,
            timeout=None,
            buffer_=4096,
            shutdown=False,
    ):
        """Initialize the beam and store the arguments"""
        # Store arguments
        self.request = request
        self.response = response
        self.description = description
        self.host = host
        self.port = port
        self.timeout = timeout
        self.buffer = buffer_
        self.shutdown = shutdown
        self._format_fields = ["host", "port", "timeout", "buffer"]

        # Reset beam
        self.reset()

    def reset(self):
        """Reset attributes needed to reuse this beam."""
        self._sent = False
        self._socket = None
        self._data = None

    def connect(self):
        """
        Create a :mod:`socket` and establish a connection to the host.
        """
        # Connect to socket and set timeout
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.settimeout(self.timeout)
        self._socket.connect((self.host, self.port))

    def send(self):
        """
        Send the HTTP request via an established connection.

        Return the amount of calls to :meth:`~socket.socket.send`.

        Raises a :class:`BeamRequestError` if the request has already
        been sent.
        """
        # Raise if called twice
        if self._sent:
            raise BeamRequestError("Request already sent.")

        # Format the request
        self.request = self.request.format(
            **{k: self.__dict__[k] for k in self._format_fields}
        )

        # Start sending data
        total, sent = 0, 0
        length = len(self.request)
        count = 0
        while total < length:
            count += 1
            chunk = self.request[total : total + self.buffer].encode(errors="ignore")
            sent = self._socket.send(chunk)
            total += sent
            if sent == 0:
                break

        # Avoid calling send twice and return count
        self._sent = True
        return count

    def receive(self):
        """
        Receive the HTTP response via an established connection.

        Return the amount of calls to :meth:`~socket.socket.recv`.

        Raises:
            - :class:`BeamRequestError`: The HTTP request has not been
              sent, yet.
            - :class:`BeamResponseError`: The HTTP response has already
              been received.
        """
        # Raise if not sent
        if not self._sent:
            raise BeamRequestError("Request has not been sent.")

        # Raise if already received
        if self._data is not None:
            raise BeamResponseError("Response has already been received.")

        count, data = 0, b""
        while True:
            count += 1
            stream = self._socket.recv(self.buffer)
            if not stream:
                break
            data += stream

        # Store data and return count
        self._data = data.decode(errors="ignore")
        return count

    def shutdown_socket(self):
        """Shutdown the :mod:`socket` of an established connection."""
        self._socket.shutdown(self.shutdown)

    # Validate data
    def validate(self):
        """
        Validate an HTTP response.

        Return a list of :class:`Response` instances.

        Raises:
            - :class:`BeamRequestError`: The HTTP request has not been
              sent, yet.
            - :class:`BeamResponseError`: The HTTP response not been
              received, yet.
        """
        # Raise if not sent
        if not self._sent:
            raise BeamRequestError("Request has not been sent.")

        # Raise if not received
        if self._data is None:
            raise BeamResponseError("Response has not been received.")

        response = []
        # Iterate over lines
        for expected, received in zip(self.response, self._data.splitlines()):
            # Store result
            response.append(
                Response(self._line_valid(expected, received), expected, received)
            )

        return response

    def terminate(self):
        """Terminate an established connection to the :mod:`socket`."""
        try:
            self._socket.shutdown(socket.SHUT_RDWR)
            self._socket.close()
        except socket.error:
            pass

    @staticmethod
    def _line_valid(expected, received):
        """
        Compare an actual response line with an expected response line.

        Arguments:
            - `expected`: The expected response string.
            - `received`: The actual response string.

        Return a boolean representing a valid or invalid response.
        """
        return received.startswith(expected)


class Laz0rCannon(object):
    """
    A laz0r cannon that shoots laz0r beams at HTTP servers.

    Arguments:
        - `host`: The hostname of the HTTP server.
        - `port`: The port number where the HTTP server is reachable.
        - `timeout`: The amount of seconds until a timeout occurs while
          reading from/writing to the socket.
        - `use_colors`: Turn colored output on the console on or off.
    """

    _result = None

    def __init__(self, host=None, port=None, timeout=3, use_colors=True):
        """Initialize the laz0r cannon and store the arguments."""
        # Predefined colors
        colors_enabled = {
            "fail": "\33[31m",
            "pass": "\33[32m",
            "warning": "\33[33m",
            "reset": "\33[0m",
        }
        colors_disabled = {"fail": "", "pass": "", "warning": "", "reset": ""}

        # Store arguments
        self._host = host
        self._port = port
        self._timeout = timeout
        self._beams = []
        self._colors = colors_enabled if use_colors else colors_disabled

        # Reset cannon
        self.reset()

    def reset(self):
        """Reset statistics of passed and failed HTTP responses."""
        self._result = {"pass": 0, "fail": 0}

        # Call reset on each beam
        for beam in self._beams:
            beam.reset()

    def add(self, beam):
        """
        Add a beam to the cannon.

        Arguments:
            - `beam`: A :class:`Beam` instance.

        The attributes `host`, `port` and `timeout` of a :class:`Beam`
        will be overwritten by this class's attributes if they haven't
        been defined explicitly.
        """
        # Host set?
        if beam.host is None:
            beam.host = self._host

        # Port set?
        if beam.port is None:
            beam.port = self._port

        # Timeout set in this beam?
        if beam.timeout is None:
            beam.timeout = self._timeout

        # Append the beam
        self._beams.append(beam)

    def __iadd__(self, beam):
        """Add a beam to the cannon with style."""
        self.add(beam)
        return self

    def _fail(self, msg=None):
        """Increase the fail counter and print a message."""
        self._result["fail"] += 1
        if msg is not None:
            self._print_fail(msg)

    def _pass(self, msg=None):
        """Increase the pass counter and print a message."""
        self._result["pass"] += 1
        if msg is not None:
            self._print_pass(msg)

    def _print(self, type_, msg):
        """
        Print a colorized message. The type_ determines the ANSI color
        used.
        """
        print(" " * 4, self._colors[type_], msg, self._colors["reset"], sep="")

    def _print_fail(self, msg):
        """Print a failure."""
        self._print("fail", msg)

    def _print_warning(self, msg):
        """Print a warning."""
        self._print("warning", msg)

    def _print_pass(self, msg):
        """Print a pass."""
        self._print("pass", msg)

    def pewpew(self, settings=True):
        """
        Align the laz0r cannon and `PEWPEWs` each laz0r :class:`Beam`
        towards the HTTP server.

        Arguments:
            - `settings`: If set to `True` setting informations will be
              printed.
        """
        # Beams?
        if not self._beams:
            print("There are no beams to pewpew!")
            return

        # Show settings?
        if settings:
            print("* Settings")
            print("{}Host: {}".format(" " * 4, self._host))
            print("{}Port: {}".format(" " * 4, self._port))
            print()

        # Loop through beam list
        for i, beam in enumerate(self._beams, 1):
            description = ": " + beam.description if beam.description else ""
            print("* Beam #{:0>3}{}".format(i, description))

            # Create socket and connect to host
            try:
                beam.connect()
            except socket.error as exc:
                self._fail("Connecting failed: {}".format(exc))
                print()
                continue

            # Send request
            try:
                count_sent = beam.send()
            except socket.error as exc:
                self._fail("Sending failed: {}".format(exc))
                print()
                continue

            # Close socket so the server does know we're not sending anything anymore
            if beam.shutdown is not False:
                try:
                    beam.shutdown_socket()
                except socket.error as exc:
                    self._print_warning("Closing socket failed: {}".format(exc))

            # Receive answer
            try:
                count_read = beam.receive()
            except socket.error as exc:
                self._fail("Receiving failed: {}".format(exc))
                print()
                continue

            # Read/Write statistic
            print("{}Sent: {}x".format(" " * 4, count_sent))
            print("{}Read: {}x".format(" " * 4, count_read))

            # Check response
            pass_ = True
            for j, response in enumerate(beam.validate(), 1):
                # Data is as expected
                if response.pass_:
                    self._print_pass("[{}] Ok".format(j))

                # Invalid data
                else:
                    pass_ = False
                    formatter = "[{0}] Failed\n{3}Expected: {1}\n{3}Received: {2}"
                    self._print_fail(
                        formatter.format(
                            j, response.expected, response.received, " " * 8
                        )
                    )

            # Close socket
            beam.terminate()
            self._pass() if pass_ else self._fail()
            print()

        # Print statistics
        print("* Summary")
        print("{}Successful: {}".format(" " * 4, self._result["pass"]))
        print("{}Failures:   {}".format(" " * 4, self._result["fail"]))
        print()

        return self._result["fail"] == 0  # Return True if no failures occurred
