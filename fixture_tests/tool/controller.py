import serial
from termcolor import colored
from tool.utils import color

DEBUG_SERIAL = False


class Controller:
    def __init__(self, device, baudrate, timeout):
        self._debug = False
        self._serial = serial.Serial(device, baudrate, timeout=timeout)
        self._current_line = None
        self._ignored_lines = set()

    def send_soft_reset(self):
        self._ignored_lines = set()
        self._serial.write(b"\x18")
        self._serial.flush()
        self.clear_line()
        # wait for startup message
        while not self.current_line().startswith("Grbl 3.8"):
            self.clear_line()
        self.clear_line()

    def ignore_line(self, line):
        self._ignored_lines.add(line)

    def current_line(self):
        while self._current_line is None:
            line = self._serial.readline().decode("utf-8").strip()
            if DEBUG_SERIAL:
                print(colored("[c] <- " + line, "light_blue"))

            if line in self._ignored_lines:
                print(color.dark_grey("<- " + line + " (ignored)", dark=True))
                continue

            self._current_line = line
            break
        return self._current_line

    def clear_line(self):
        self._current_line = None

    def next_line(self):
        self.clear_line()
        return self.current_line()

    def send_line(self, line):
        if DEBUG_SERIAL:
            print(colored("[c] -> " + line, "light_blue"))

        self._serial.write(line.encode("utf-8") + b"\n")

    def getc(self, size):
        return self._serial.read(size) or None

    def putc(self, data):
        return self._serial.write(data) or None

    def drain(self, wait_for=0.1):
        timeout = self._serial.timeout
        self._serial.timeout = wait_for
        while self._serial.read(1):
            pass
        self._serial.timeout = timeout

    def close(self):
        self._serial.close()