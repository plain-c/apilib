#include "../cfg.h"

# if(APILIB_OS_WINNT != 0)
# include <windows.h>
# endif

#include "spliceimport.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

bool apilib::spliceimport_get_current_tid(spliceimport_tid_t* tid)
{
  bool r = false;

# if(APILIB_OS_WINNT != 0)
  if(tid != NULL)
  {
    *tid = ::GetCurrentThreadId();
    r = true;
  }
# endif

  return r;
}

bool apilib::spliceimport_get_current_pid(spliceimport_pid_t* pid)
{
  bool r = false;

# if(APILIB_OS_WINNT != 0)
  if(pid != NULL)
  {
    *pid = ::GetCurrentProcessId();
    r = true;
  }
# endif

  return r;
}

bool apilib::spliceimport_cs_create(spliceimport_cs_t* cs)
{
  bool r = false;

# if(APILIB_OS_WINNT != 0)
  if(cs != NULL && ::RtlInitializeCriticalSection(cs) == STATUS_SUCCESS)
  {
    r = true;
  }
# endif

  return r;
}

bool apilib::spliceimport_cs_destroy(spliceimport_cs_t* cs)
{
  bool r = false;

# if(APILIB_OS_WINNT != 0)
  if(cs != NULL && ::RtlDeleteCriticalSection(cs) == STATUS_SUCCESS)
  {
    r = true;
  }
# endif

  return r;
}

bool apilib::spliceimport_cs_enter(spliceimport_cs_t* cs)
{
  bool r = false;

# if(APILIB_OS_WINNT != 0)
  if(cs != NULL && ::RtlEnterCriticalSection(cs) == STATUS_SUCCESS)
  {
    r = true;
  }
# endif

  return r;
}

bool apilib::spliceimport_cs_leave(spliceimport_cs_t* cs)
{
  bool r = false;

# if(APILIB_OS_WINNT != 0)
  if(cs != NULL && ::RtlLeaveCriticalSection(cs) == STATUS_SUCCESS)
  {
    r = true;
  }
# endif

  return r;
}

void* apilib::spliceimport_memalloc(size_t size)
{
  void* r = NULL;

  if(size != 0)
  {
#   if(APILIB_OS_WINNT != 0)
    {
      ::PVOID BaseAddress = NULL;
      ::SIZE_T RegionSize = size;
      if(::NtAllocateVirtualMemory((::HANDLE)-1, &BaseAddress, 0, &RegionSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE) == STATUS_SUCCESS && BaseAddress != NULL && RegionSize >= size)
      {
        r = BaseAddress;
      }
    }
#   endif
  }

  return r;
}

void* apilib::spliceimport_memalloc_address(void* address, size_t size)
{
  void* r = NULL;

  if(address != NULL && size != 0)
  {
#   if(APILIB_OS_WINNT != 0)
    {
      ::PVOID BaseAddress1 = address;
      ::SIZE_T RegionSize1 = size;
      if(::NtAllocateVirtualMemory((::HANDLE)-1, &BaseAddress1, 0, &RegionSize1, MEM_COMMIT, PAGE_EXECUTE_READWRITE) == STATUS_SUCCESS)
      {
        if(BaseAddress1 != NULL && RegionSize1 >= size)
        {
          r = BaseAddress1;
        }
      }
      else
      {
        ::PVOID BaseAddress2 = address;
        ::SIZE_T RegionSize2 = size;
        if(::NtAllocateVirtualMemory((::HANDLE)-1, &BaseAddress2, 0, &RegionSize2, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE) == STATUS_SUCCESS)
        {
          if(BaseAddress2 != NULL && RegionSize2 >= size)
          {
            r = BaseAddress2;
          }
        }
      }
    }
#   endif
  }

  return r;
}

bool apilib::spliceimport_memfree(void* address)
{
  bool r = false;

  if(address != NULL)
  {
#   if(APILIB_OS_WINNT != 0)
    {
      ::PVOID BaseAddress = address;
      ::SIZE_T RegionSize = 0;
      if(::NtFreeVirtualMemory((::HANDLE)-1, &BaseAddress, &RegionSize, MEM_RELEASE) == STATUS_SUCCESS)
      {
        r = true;
      }
    }
#   endif
  }

  return r;
}

bool apilib::spliceimport_memquery(const void* address, memquery_t* mq)
{
  bool r = false;

  if(mq != NULL)
  {
#   if(APILIB_OS_WINNT != 0)
    {
      ::MEMORY_BASIC_INFORMATION mbi;
      ::SIZE_T dwReturnLength = 0;

      if(::NtQueryVirtualMemory((::HANDLE)-1, (::PVOID)address, MemoryBasicInformation, &mbi, sizeof(mbi), &dwReturnLength) == STATUS_SUCCESS && dwReturnLength == sizeof(mbi))
      {
        mq->addr = mbi.BaseAddress;
        mq->size = mbi.RegionSize;
        mq->free = ((mbi.State & MEM_FREE   ) != 0) ? true : false;
        mq->rsrv = ((mbi.State & MEM_RESERVE) != 0) ? true : false;
        r = true;
      }
    }
#   endif
  }

  return r;
}

bool apilib::spliceimport_memread(const void* address, void* data, size_t size)
{
  bool r = false;

  if(address != NULL && data != NULL && size != 0)
  {
#   if(APILIB_OS_WINNT != 0)
    {
      ::SIZE_T dwNumberOfBytesRead = 0;
      if(::NtReadVirtualMemory((::HANDLE)-1, (::PVOID)address, data, size, &dwNumberOfBytesRead) == STATUS_SUCCESS && dwNumberOfBytesRead == size)
      {
        r = true;
      }
    }
#   endif
  }

  return r;
}

bool apilib::spliceimport_memwrite(const void* address, const void* data, size_t size)
{
  bool r = false;

  if(address != NULL && data != NULL && size != 0)
  {
#   if(APILIB_OS_WINNT != 0)
    {
      ::SIZE_T dwNumberOfBytesWritten = 0;
      if(::NtWriteVirtualMemory((::HANDLE)-1, (::PVOID)address, (::PVOID)data, size, &dwNumberOfBytesWritten) == STATUS_SUCCESS && dwNumberOfBytesWritten == size)
      {
        r = true;
      }
      else
      {
        ::PVOID  BaseAddress = (::PVOID)address;
        ::SIZE_T RegionSize  = size;
        ::ULONG  OldProtect  = 0;

        if(::NtProtectVirtualMemory((::HANDLE)-1, &BaseAddress, &RegionSize, PAGE_EXECUTE_READWRITE, &OldProtect) == STATUS_SUCCESS)
        {
          if(::NtWriteVirtualMemory((::HANDLE)-1, (::PVOID)address, (::PVOID)data, size, &dwNumberOfBytesWritten) == STATUS_SUCCESS && dwNumberOfBytesWritten == size)
          {
            r = true;
          }
          ::NtProtectVirtualMemory((::HANDLE)-1,&BaseAddress, &RegionSize, OldProtect, &OldProtect);
        }
      }
    }
#   endif
  }

  return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////