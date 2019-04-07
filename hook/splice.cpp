#include "../cfg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if(APILIB_32 != 0 || APILIB_64 != 0)

#include "../include/lib.h"
#include "../include/log.h"

#include "splice.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

bool apilib::splice_entry_hook(spliceitem_t* item)
{
  bool r = false;

  do 
  {
    if(item == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    r = splice_group_hook(item, 1);         
  }
  while(0);

  return r;
}

bool apilib::splice_entry_unhook(spliceitem_t* item)
{
  bool r = false;

  do 
  {
    if(item == NULL)
    {
      loge("Invalid argument.");
      break;
    }

    r = splice_group_unhook(item, 1);
  }
  while(0);

  return r;
}

bool apilib::splice_group_hook(spliceitem_t* items, size_t items_count)
{
  bool r = false;

  do 
  {
    bool error = false;

    if(items == NULL || items_count == 0)
    {
      loge("Invalid argument.");
      break;
    }

    for(size_t i = 0; i < items_count && error == false; i++)
    {
      if(items[i].splice != NULL)
      {
        loge("Invalid context of entry #%u (splice context is not empty).", i);
        error = true;
        break;
      }
      if(items[i].target == NULL)
      {
        loge("Invalid context of entry #%u (target address is not defined).", i);
        error = true;
        break;
      }
      if(items[i].hooker == NULL)
      {
        loge("Invalid context of entry #%u (hooker address is not defined).", i);
        error = true;
        break;
      }
    }    

    for(size_t i = 0; i < items_count && error == false; i++)
    {
      void* target = items[i].target;
      void* hooker = items[i].hooker;

      if((items[i].params & splice_param_search_redirect) != 0)
      {
        void* redirect_address = splice_search_redirect(target);
        if(redirect_address != NULL)
        {
          logi("Target address of entry #%u is going to be replaced from '%p' to '%p'", i, target, redirect_address);
          target = redirect_address;
        }
      }

      if((items[i].splice = splice_create(target, hooker)) == NULL)
      {
        loge("Failed to create splice context for entry #%u.", i);
        error = true;
        break;
      }

      if((items[i].bridge = splice_get_bridge_location(items[i].splice)) == NULL)
      {
        loge("Failed to get bridge location for entry #%u.", i);
        error = true;
        break;
      }

      if(items[i].notify != NULL)
      {
        items[i].notify(items[i].target, items[i].hooker, items[i].splice, items[i].bridge);
      }
    }

    for(size_t i = 0; i < items_count && error == false; i++)
    {
      if((items[i].params & splice_param_create_disabled) != 0)
      {
        if(splice_disable(items[i].splice) == false)
        {
          loge("Failed to change jumper activity state for entry #%u.", i);
          error = true;
          break;
        }
      }
      else
      {
        if(splice_enable(items[i].splice) == false)
        {
          loge("Failed to change jumper activity state for entry #%u.", i);
          error = true;
          break;
        }
      }
    }

    for(size_t i = 0; i < items_count && error == false; i++)
    {
      if(splice_write_modified_bytes(items[i].splice) == false)
      {
        loge("Failed to write modified bytes into memory.");
        error = true;
        break;
      }
    }

    if(error == false)
    {
      r = true;
    }
    else
    {
      splice_group_unhook(items, items_count);
    }
  }
  while(0);

  return r;
}

bool apilib::splice_group_unhook(spliceitem_t* items, size_t items_count)
{
  bool r = false;

  do 
  {
    bool error = false;

    if(items == NULL || items_count == 0)
    {
      loge("Invalid argument.");
      break;
    }

    for(size_t i = 0; i < items_count && error == false; i++)
    {
      if(items[i].splice == NULL)
      {
        loge("Invalid context of entry #%u (splice context is empty).", i);
        error = true;
        break;
      }
    }

    for(size_t i = 0; i < items_count && error == false; i++)
    {
      if(splice_write_original_bytes(items[i].splice) == false)
      {
        loge("Failed to write modified bytes into memory.");
        error = true;
        break;
      }
    }

    for(size_t i = 0; i < items_count && error == false; i++)
    {
      if(splice_disable(items[i].splice) == false)
      {
        loge("Failed to change jumper activity state.");
        error = true;
        break;
      }
    }

    for(size_t i = 0; i < items_count && error == false; i++)
    {
      if(splice_destroy(items[i].splice) == true)
      {
        items[i].splice = NULL;
      }
      else
      {
        loge("Failed to destroy splice context.");
        error = true;
        break;
      }
    }

    if(error == false)
    {
      r = true;
    }
  }
  while(0);

  return r;
}

# endif //#if(APILIB_32 != 0 || APILIB_64 != 0)

///////////////////////////////////////////////////////////////////////////////////////////////////