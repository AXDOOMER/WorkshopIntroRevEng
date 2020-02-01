key = 0x391833a7

file_content = None

with open('payload', 'rb') as f:
	file_content = f.read()

obfuscated = [None] * len(file_content)

i = len(file_content) - 1
while i >= 0:
	obfuscated[i] = ord(file_content[i]) ^ (key & 0xFF)
	key = (key << 1) | (key >> 31)
	i = i - 1

with open('payload2', 'wb') as f:
	newFileByteArray = bytearray(obfuscated)
	f.write(newFileByteArray)
