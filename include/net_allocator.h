/**
@file  net_allocator.h
@brief Memory Buffer Allocator API

@verbatim
File: net_allocator.h
Desc:Memory Buffer Allocator API

Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.
This software is protected by copyright law and international treaties.  Unauthorized
reproduction or distribution of any portion is prohibited by law.
@endverbatim
**/

#if !defined (__MC_CIRCULAR_ALLOCATOR_API_H__)
#define __MC_CIRCULAR_ALLOCATOR_API_H__

#include <mctypes.h>
#include "net_common.h"

/**
* namespace MainConcept
* @brief Global SDK namespace
**/
namespace MainConcept {

    /**
    * namespace NetworkStreaming
    * Network streaming specific namespace
    **/
    namespace NetworkStreaming {

        /**
        * @brief Allocates memory buffers
        **/
        class INetBufferAllocator : public INetRefCounted
        {
        public:
            virtual ~INetBufferAllocator() {}

            /**
            * @brief Check if allocator has free buffers.
            *
            * Use this method to check for free buffers before calling GetBuffer to avoid blocking.
            *
            * @param[in]  uiRequestedSize Size of desired buffer. Set to 0 to use default value specified in constructor.
            * @return                     Returns true if free buffers are available and false if no buffers left and GetBuffer will block.
            */
            virtual bool IsBufferAvailable( uint32_t uiRequestedSize ) = 0;

            /**
            * @brief Check if allocator has free buffers or block until free buffers are available.
            *
            * Use this method to wait for free buffers.
            *
            * @param[in] uiRequestedSize Size of desired buffer. Set to 0 to use default value specified in constructor.
            * @return                    Returns true when free buffers are available or false if wait was aborted.
            */
            virtual bool WaitForFreeBuffer( uint32_t uiRequestedSize ) = 0;

            /**
            * @brief Returns maximum possible allocation size.
            *
            * Return value may depend on Freeze state.
            *
            * @return Returns maximum possible allocation size.
            */
            virtual uint32_t GetMaximumAllocatedSize() = 0;

            /**
            * @brief Returns total size of currently allocated memory.
            *
            * @return Returns total size of currently allocated memory.
            */
            virtual uint32_t GetAllocatedSize() = 0;

            /**
            * @brief Returns size of memory currently used by buffers.
            *
            * @return Returns size of memory currently used by buffers.
            */
            virtual uint32_t GetUsedSize() = 0;

            /**
            * @brief Request new buffer from allocator.
            *
            * This method can block if no free buffers are available. Use IsBufferAvailable to avoid blocking.
            *
            * @param[out] puiBufferSize   Size of allocated buffer.
            * @param[in]  uiRequestedSize Size of desired buffer. Set to 0 to use default value specified in constructor.
            * @return                     Returns pointer to allocated buffer on success or NULL if allocation fails.
            */
            virtual int8_t *GetBuffer( uint32_t *puiBufferSize, uint32_t uiRequestedSize ) = 0;

            /**
            * @brief Notify allocator about used bytes in buffer.
            *
            * This method should be called whenever client writes data to the buffer
            * or before requesting new buffer with GetBuffer.
            *
            * @param[in]  pBuffer     Pointer returned by the GetBuffer call.
            * @param[in]  uiUsedBytes Number of used bytes in buffer.
            * @return                 Return MCNETRESULT
            */
            virtual MCNETRESULT Confirm( int8_t *pBuffer, uint32_t uiUsedBytes ) = 0;

            /**
            * @brief Release allocated buffer.
            *
            * @param[in] pBuffer Pointer returned by the GetBuffer call.
            * @return            Return MCNETRESULT
            */
            virtual MCNETRESULT ReleaseBuffer( int8_t *pBuffer ) = 0;

            /**
            * @brief Prevent or allow any further memory allocations.
            *
            * If bFreeze is set to true, no physical allocation calls will be made by allocator.
            * Application is responsible for releasing previously allocated buffers before requesting new to avoid deadlocks.
            *
            * @param[in] bFreeze Set to true to prevent new memory allocations.
            * @return            Return MCNETRESULT
            */
            virtual MCNETRESULT Freeze( bool bFreeze ) = 0;

            /**
            * @brief Free allocated memory.
            *
            * All buffers must be released before this call.
            *
            * @return Return MCNETRESULT
            */
            virtual MCNETRESULT Free() = 0;

			/**
            * @brief Return false from any GetBuffer, IsBufferAvailable or WaitForFreeBuffer calls.
            *
            * When bCancel set to true - any pending wait requests will be also released and new requests will return false immediately.
            *
			* @param[in] bCancel Set to true to prevent new memory allocations and return false from GetBuffer, IsBufferAvailable and WaitForFreeBuffer calls; Set to false to return to normal operation.
            * @return Return MCNETRESULT
            */
			virtual MCNETRESULT CancelWaiting( bool bCancel ) = 0;
        };

#if defined (__cplusplus)
        extern "C" {
#endif
            /**
            * @brief Create instance of circular buffer allocator.
            *
            * This allocator preallocates continuous chunks of memory and returns
            * pointers to buffers inside of these chunks in circular motion.
			* INetBufferAllocator::GetBuffer() always return all remaining free space in current chunk, caller is responsible for reporting
			* count of bytes it actually used via INetBufferAllocator::Confirm() call. Confirm updates internal counters for current chunk and buffer
			* and allows following GetBuffer call to use remaining free space in chunk.
			* If Confirm call is not used, GetBuffer will always return buffers that occupy entire chunks. This behavior is not recommended,
			* unless all buffers are of equal size.
			* Once chunk is full - it will be marked as full until all buffers that reference this chunk are released. Because of this,
			* it's preferable to use at least two chunks to avoid locks.
            *
            * @param[in]  uiMinChunks     Minimal count of chunks to allocate. This is the minimal count of chunks that can be frozen, see INetBufferAllocator::Freeze()
            * @param[in]  uiMaxChunks     Maximal count of memory chunks that can be allocated
            * @param[in]  uiChunkSize     Size of each memory chunk
            * @param[in]  uiMinBufferSize Minimal buffer size that allocator can return.
            * @param[in]  uiAlign         Align of memory block address
            * @param[in]  get_rc          Pointer to get_rc memory manager implementation or NULL to use standard allocators
            * @return                     Returns pointer to allocator instance on success or NULL if allocation fails
            */
            INetBufferAllocator* NET_API_CALL GetCircularAllocatorInterface( uint32_t uiMinChunks, uint32_t uiMaxChunks, uint32_t uiChunkSize, uint32_t uiMinBufferSize, uint32_t uiAlign, void *(MC_EXPORT_API *get_rc)(const char*) );
#if defined (__cplusplus)
        }
#endif
    };
};

#endif // __MC_CIRCULAR_ALLOCATOR_API_H__

