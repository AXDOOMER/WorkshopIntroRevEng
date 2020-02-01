#include <stdio.h>
#include <sys/ptrace.h>

int main(int argc)
{
	const char * flag = "\0\nBravo, tu as unpacké le payload. Tu as réussi le défi!\nDRAPEAU{d41d8cd98f00b204e9800998ecf8427e}\n";

	// PTRACE: Check also for return value zero?
	if (argc == 1 && !(ptrace(PTRACE_TRACEME, 0, 1, 0) == -1))
	{
		*flag++;
	}

	// We could have XORed the string with a one-time pad that starts with a 0x00 so the flag is not in plain text in the binary.
	puts(flag);
}
