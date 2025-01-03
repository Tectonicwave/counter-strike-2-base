#include "../../manager.h"
#pragma

namespace utils
{
	class CUtlBuffer
	{
	public:
		Memory_pad(0x80);

		CUtlBuffer(int a1, int nSize, int a3)
		{
			if (!manager->function.utl_buffer_init)
				return;

			manager->function.utl_buffer_init(this, a1, nSize, a3);
		}

		void PutString(const char* szString)
		{
			if (!manager->function.utl_buffer_put_string)
				return;

			manager->function.utl_buffer_put_string(this, szString);
		}

		void EnsureCapacity(int nSize)
		{
			if (!manager->function.utl_buffer_ensure_capacity)
				return;

			manager->function.utl_buffer_ensure_capacity(this, nSize);
		}
	};

}