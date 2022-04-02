import socket


class Server:
    def __init__(self):
        self._port = 2137
        self._host = "127.0.0.1"
        self._address = (self._host, self._port)
        self._buffer_size = 1024
        self._format = "utf-8"
        self._client = None
        self.on_init()

    def on_init(self):
        self._client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._client.connect(self._address)

    def connection(self):
        data = self._client.recv(self._buffer_size).decode(self._format)
        print(f"Received: {data}")
        msg = "Hello from Python"
        self._client.send(msg.encode(self._format))
        self._client.close()

