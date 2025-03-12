
from crc import crc16

PROTOCOL_SOF = 0xa5

def build_stream(data: bytes) -> bytes:
    return bytes([PROTOCOL_SOF]) + data + crc16(data).to_bytes(2, 'little')
