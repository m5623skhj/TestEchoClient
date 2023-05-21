#include "PreCompile.h"
#include <iostream>
#include "EchoClient.h"

int main()
{
	{
		std::wstring optionFile = L"OptionFile/OptionFile.ini";
		EchoClient client(optionFile);

		while (true)
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				break;
			}
		}
	}

	return 0;
}