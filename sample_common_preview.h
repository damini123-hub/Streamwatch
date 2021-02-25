/**
@file: sample_common_preview.h
@brief Common preview reconstructed stream routines

@verbatim
File: sample_common_preview.h
Desc: Common preview reconstructed stream routines
 
 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.  
 This software is protected by copyright law and international treaties.  Unauthorized 
 reproduction or distribution of any portion is prohibited by law.
@endverbatim
 **/

#include "stdio.h"
#include "mcdefs.h"




/**
 * @name Preview frame node structure
 * @{
 **/
 
 /**
 *@brief Preview frame node structure used to configure node of preview frames queue
 */
typedef struct preview_frame_node_s
{
    frame_tt                       frame;               /**<@brief Pointer to frame_tt structure */
    uint32_t                       number;              /**<@brief Number of frame in DISPLAY order */
    uint32_t                       taken;               /**<@brief In process of encoding */
    uint32_t                       pic_type;            /**<@brief Type of picture. Not used yet */
    uint32_t                       ready;                /**<@brief Ready for dumping */
    struct preview_frame_node_s   *prev;                /**<@brief Pointer to the previous node */
    struct preview_frame_node_s   *next;                /**<@brief Pointer to the next node */
} preview_frame_node_t;
/**@}*/

/* reconstructed frames preview queue */
/**
 * @name Preview queue structure
 * @{
 **/
 
 /**
 *@brief Preview queue structure used to configure preview frames queue
 */
typedef struct preview_queue_s
{
    preview_frame_node_t*        q_nodes;                /**<@brief Nodes buffer */
    int32_t                      length;                 /**<@brief Number of nodes in queue */
     preview_frame_node_t*                      q_head;                 /**<@brief The queue head number */
     preview_frame_node_t*                      q_tail;                 /**<@brief The queue tail number */

    uint32_t                     frame_size;             /**<@brief Size of frame (for printing) */

    int width;
    int height; 
    int chroma; 
    int bit_depth;
    int32_t four_cc;


//    preview_frame_node_t       **buffer;                 /**<@brief Pointer array with frame_tt addresses */
//    uint32_t                    *pos;                    /**<@brief Flag array for indicating which positions are filled in buffer */
//    uint32_t                     nbufframes;             /**<@brief Number of frames in queue buffer */
//    uint32_t                     buffer_full;            
//    uint32_t                     point;                  /**<@brief Number of the frame we have buffer frames to be printed before */

//    CRITICAL_SECTION             cs;                     /**<@brief Critical section */
    FILE*                        reconst_file;           /**<@brief Pointer ro file for storage of reconstructed frames */
} preview_queue_t;
/**<@}*/


struct queue_manager_s
{
    preview_queue_t* preview_queues[10];
    int32_t count;
};

/**
 * @brief Creates preview frame
 * @param[in] frame        - Pointer to frame_tt structure
 * @param[in] width        - Width of frame
 * @param[in] height       - Height of frame
 * @param[in] chroma       - Chroma sampling (2 - 4:2:0; 3 - 4:2:2; 4 = 4:4:4)
 * @param[in] four_cc      - Pixel FOURCC code (Planar types only)
 * @return     NONE
 */
void  preview_frame_create(frame_tt* frame, int width, int height, int chroma, uint32_t four_cc, void(*done)(void *));
/**
 * @brief Deletes preview frame
 * @param[in] frame        - Pointer to frame_tt structure
 * @return     NONE
 */
void  preview_frame_delete(frame_tt* frame);

/**
 * @brief Creates preview frames queue.
 * @param[in] queue                - Pointer to preview frames queue
 * @param[in] nframes              - Length of queue
 * @param[in] frames_in_buffer     - Number of frames in buffer
 * @param[in] width                - Width of frame
 * @param[in] height               - Height of frame
 * @param[in] chroma               - Chroma sampling (2 - 4:2:0; 3 - 4:2:2; 4 = 4:4:4)
 * @param[in] bit_depth            - Pixel color bit depth
 * @param[in] rec_file          - File name for storage reconstructed files
 * @return     NONE
 */
void  preview_queue_create(preview_queue_t* queue, int32_t nframes, int32_t frames_in_buffer, int32_t width, int32_t height, int32_t chroma, int32_t bit_depth, char * rec_file);
/**
 * @brief Deletes preview frames queue.
 * @param[in] queue                - Pointer to preview frames queue
 * @return     NONE
 */
void  preview_queue_delete(preview_queue_t* queue);

// May be __inline
int NextQueueNum(int num, int size);
// May be __inline
int PrevQueueNum(int num, int size);

/**
 * @brief Gets preview frame from queue queue by number.
 * @param[in] queue                - Pointer to preview frames queue
 * @param[in] n                    - Number of getting frame
 * @return     NONE
 */
void* preview_queue_get_frame(preview_queue_t* queue, uint32_t n);

/**
 * @brief Writes picture into file
 * @param[in] queue             - Pointer to reconstructed frame queue
 * @param[in] node              - Pointer to node structure for output
 * @return     NONE
 */
void  put_into_file(preview_queue_t* queue, preview_frame_node_t* node);

/**
 * @brief Writes all buffered frames ftom queue into file
 * @param[in] queue              - Pointer to reconstructed frame queue
 * @param[in] pos                - Current frame position in the queue
 * @return     NONE
 */
void  print_buffer(preview_queue_t* queue, preview_frame_node_t* pos);


// Do not use it! Will removed in the future.
extern preview_queue_t* preview_queue;

/**
 * @brief Puts frame into buffer if bufer is not full, else prints buffer and put frame
 * @param[in] this_object         - Pointer to frame
 * @return     NONE
 */
void  done_frame(void* this_object);

/**
 * @brief Puts frame into buffer
 * @param[in] queue               - Pointer to reconstructed frame queue
 * @param[in] node                - Pointer to preview_frame_node_t structure which will be added to queue
 * @return     NONE
 */
//void  put_into_buffer(preview_queue_t* queue, preview_frame_node_t *node);

