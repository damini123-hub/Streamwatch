/********************************************************************
created:    2012/03/19 - 13:36
file name:  sample_common_preview.h
purpose:    Common preview reconstructed stream routines

Copyright (c) 2010 MainConcept GmbH. All rights reserved.

This software is the confidential and proprietary information of
MainConcept GmbH and may be used only in accordance with the terms of
your license from MainConcept GmbH.
*********************************************************************/

#include <stdlib.h>
#include <string.h>

#include "sample_common_preview.h"
#include "sample_common_misc.h"
#include "mcfourcc.h"
#include "v_frame.h"


queue_manager_s queues=
{
    {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
    0
};

// Do not use it! Will removed in the future.


/**
* @brief Puts frame into buffer if bufer is not full, else prints buffer and put frame
* @param[in] this_object         - Pointer to frame
* @return     NONE
*/
#include "windows.h"

void done(void* this_object)
{
    frame_tt* frame = (frame_tt*)this_object;

    int pict_type = pic_info.pict_type;

    if (frame->plane[0] != NULL)
    {
        int32_t founded = 0;
        for(int32_t i = 0; i < queues.count; i++)
        {

            preview_queue_t * queue = queues.preview_queues[i];
            if(queue != NULL)
            {
                preview_frame_node_t* cur_node = queue->q_tail;
                while (cur_node != queue->q_head)
                {
                    if (&cur_node->frame == frame)
                    {
                        cur_node->ready = 1;
                        founded = 1;
                        break;
                    }  // looking for the node with frame_tt
                    cur_node = cur_node->next;
                }
                print_buffer(queue, cur_node);
                if(founded) break;
            }
        }
    }
}


/**
* @brief Deletes preview frame
* @param[in] frame        - Pointer to frame_tt structure
* @return     NONE
*/
void preview_frame_delete(frame_tt *frame)
{
    if (frame->plane[0]) free(frame->plane[0]);
}



// Return only planar types.
static uint32_t GetPlanarFOURCC(int chroma_id, int bit_depth)
{
    uint32_t four_cc;

    if( bit_depth == 8 )
    {
        switch(chroma_id)
        {
          case 2:  four_cc = FOURCC_I420; break;
          case 3:  four_cc = FOURCC_I422; break;
          case 4:  four_cc = FOURCC_I444; break;
          default: four_cc = FOURCC_I420; break;
        }
    }
    else 
    {
        switch(chroma_id)
        {
          case 2:  four_cc = FOURCC_W010; break;
          case 3:  four_cc = FOURCC_W210; break;
          case 4:  four_cc = FOURCC_W410; break;
          default: four_cc = FOURCC_W010; break;
        }
    }

    return four_cc;
}

/**
 * @brief Creates preview frame
 * @param[in] frame        - Pointer to frame_tt structure
 * @param[in] width        - Width of frame
 * @param[in] height       - Height of frame
 * @param[in] chroma       - Chroma sampling (2 - 4:2:0; 3 - 4:2:2; 4 = 4:4:4)
 * @param[in] four_cc      - Pixel FOURCC code (Planar types only)
 * @return     NONE
 */
void preview_frame_create(frame_tt* frame, int width, int height, int chroma, uint32_t four_cc, void(*done)(void *))
{
    frame->width  = width;
    frame->height = height;

    int32_t line_size, img_start;
    int32_t video_frame_size = get_video_frame_size(width, height, four_cc, &line_size, &img_start);

    frame->four_cc = four_cc;
    frame->done = done;
    
    switch(chroma)
    {
    case 2:
        frame->stride[0] = line_size;
        frame->stride[1] = line_size / 2;
        frame->stride[2] = line_size / 2;
        frame->plane[0]  = (uint8_t*)malloc(video_frame_size);
        frame->plane[1]  = frame->plane[0] + height * line_size;
        frame->plane[2]  = frame->plane[1] + height * line_size / 4;
        break;
    case 3:
        frame->stride[0] = line_size;
        frame->stride[1] = line_size / 2;
        frame->stride[2] = line_size / 2;
        frame->plane[0]  = (uint8_t*)malloc(video_frame_size);
        frame->plane[1]  = frame->plane[0] + height * line_size;
        frame->plane[2]  = frame->plane[1] + height * line_size / 2;
        break;
    case 4:
        frame->stride[0] = line_size;
        frame->stride[1] = line_size;  
        frame->stride[2] = line_size;
        frame->plane[0]  = (uint8_t*)malloc(video_frame_size);
        frame->plane[1]  = frame->plane[0] + height * line_size;
        frame->plane[2]  = frame->plane[1] + height * line_size;
        break;
    }
}

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

void preview_queue_create(preview_queue_t* queue, int nframes, int frames_in_buffer, int width, int height, int chroma, int bit_depth, char* rec_file)
{

    if(queues.count < 10)
    {
        queues.preview_queues[queues.count] = queue;
        queues.count++;
    }

    queue->width = width;
    queue->height= height;
    queue->chroma = chroma;
    queue->bit_depth = bit_depth;
    int32_t line_size, img_start;
    queue->four_cc = GetPlanarFOURCC(chroma, bit_depth);
    queue->frame_size = get_video_frame_size(width, height, queue->four_cc, &line_size, &img_start);
    preview_frame_node_t* p_node, *tmp;
    uint32_t four_cc;
    int i;

    // Create first node buffer.
    queue->q_nodes = (preview_frame_node_t*)malloc(sizeof(preview_frame_node_t));
    preview_frame_create(&queue->q_nodes->frame, width, height, chroma, queue->four_cc, done);
    queue->q_nodes->ready = 0;
    queue->q_nodes->next = queue->q_nodes->prev = NULL;
    queue->length = 1;


    p_node = queue->q_nodes;
    for( i = 1; i < nframes; i++ )
    {
        tmp = (preview_frame_node_t*)malloc(sizeof(preview_frame_node_t));
        preview_frame_create(&tmp->frame, width, height, chroma, queue->four_cc, done);
        tmp->ready = 0;

        p_node->next = tmp;
        tmp->prev = p_node;
        queue->q_nodes->prev = tmp;
        tmp->next = queue->q_nodes;

        p_node = p_node->next;
        queue->length++;
    }
    queue->q_head = queue->q_nodes;
    queue->q_tail = queue->q_nodes;

    queue->reconst_file = fopen(rec_file, "wb");
}

/**
 * @brief Deletes preview frames queue.
 * @param[in] queue                - Pointer to preview frames queue
 * @return     NONE
 */
void preview_queue_delete(preview_queue_t* queue)
{
    preview_frame_node_t* p_node = queue->q_nodes->next;
    int i;

    print_buffer(queue, queue->q_head);

    while(p_node != queue->q_nodes)
    {
        preview_frame_delete(&p_node->frame);
        p_node = p_node->next;
        free(p_node->prev);
    }

    fclose(queue->reconst_file);

    preview_frame_delete(&queue->q_nodes->frame);
    if( queue->q_nodes ) free(queue->q_nodes);
    queue = NULL;
    queues.count--;
}


/**
 * @brief Gets preview frame from queue queue by number.
 * @param[in] queue                - Pointer to preview frames queue
 * @param[in] n                    - Number of getting frame
 * @return     NONE
 */
void* preview_queue_get_frame(preview_queue_t* queue, uint32_t n)
{
    preview_frame_node_t* p_node = queue->q_head;

    p_node->taken  = 1;
    p_node->number = n;

    if( queue->q_head->next == queue->q_tail)
    {
        preview_frame_node_t* tmp = (preview_frame_node_t*)malloc(sizeof(preview_frame_node_t));
        preview_frame_create(&tmp->frame, queue->width, queue->height, queue->chroma, queue->four_cc, queue->q_nodes->frame.done);
        tmp->ready = 0;
        queue->q_head->next =tmp;
        tmp->prev = queue->q_head;
        tmp->next = queue->q_tail;
        queue->q_tail->prev = tmp;

    }

    queue->q_head =  queue->q_head->next;

    return (void*)(&p_node->frame);
}

/**
 * @brief Writes picture into file
 * @param[in] queue             - Pointer to reconstructed frame queue
 * @param[in] node              - Pointer to node structure for output
 * @return     NONE
 */
void put_into_file(preview_queue_t* queue, preview_frame_node_t* node)
{
    if(node->ready)
    {
        frame_tt* frame = &node->frame;
    //printf("\nWrite to file: %i", queue->reconst_file);

        switch(frame->four_cc)    // double check for functional limit
        {
        case FOURCC_I420:
        case FOURCC_YV12: fwrite(frame->plane[0], frame->width * frame->height, 1, queue->reconst_file);
                          fwrite(frame->plane[1], frame->width * frame->height / 4, 1, queue->reconst_file);
                          fwrite(frame->plane[2], frame->width * frame->height / 4, 1, queue->reconst_file);
                          break;

        case FOURCC_I422: fwrite(frame->plane[0], frame->width * frame->height, 1, queue->reconst_file);
                          fwrite(frame->plane[1], (frame->width / 2)* frame->height, 1, queue->reconst_file);
                          fwrite(frame->plane[2], (frame->width / 2)* frame->height, 1, queue->reconst_file);
                          break;

        case FOURCC_I444: fwrite(frame->plane[0], frame->width * frame->height, 1, queue->reconst_file);
                          fwrite(frame->plane[1], frame->width * frame->height, 1, queue->reconst_file);
                          fwrite(frame->plane[2], frame->width * frame->height, 1, queue->reconst_file);
                          break;

        case FOURCC_W010: fwrite(frame->plane[0], frame->width * frame->height * 2, 1, queue->reconst_file);
                          fwrite(frame->plane[1], frame->width * frame->height / 2, 1, queue->reconst_file);
                          fwrite(frame->plane[2], frame->width * frame->height / 2, 1, queue->reconst_file);
                          break;

        case FOURCC_W210: fwrite(frame->plane[0], frame->width * frame->height * 2, 1, queue->reconst_file);
                          fwrite(frame->plane[1], frame->width * frame->height, 1, queue->reconst_file);
                          fwrite(frame->plane[2], frame->width * frame->height, 1, queue->reconst_file);
                          break;

        case FOURCC_W410: fwrite(frame->plane[0], frame->width * frame->height * 2, 1, queue->reconst_file);
                          fwrite(frame->plane[1], frame->width * frame->height * 2, 1, queue->reconst_file);
                          fwrite(frame->plane[2], frame->width * frame->height * 2, 1, queue->reconst_file);
                          break;

        default:
                          fwrite(frame->plane[0], queue->frame_size, 1, queue->reconst_file);
                          break;
        }

        frame->state         &= ~(FRM_BUF_DONE | FRM_BUF_FILLED);   // invert frame_tt state
        node->taken          = 0;
        node->ready         =0;
    }
}

/**
 * @brief Writes all buffered frames ftom queue into file
 * @param[in] queue              - Pointer to reconstructed frame queue
 * @param[in] pos                - Current frame position in the queue
 * @return     NONE
 */
void print_buffer(preview_queue_t* queue, preview_frame_node_t* pos)
{
    while( queue->q_tail != pos && queue->q_tail->ready)
    {
      put_into_file(queue, queue->q_tail); // and put them into file
   
      queue->q_tail = queue->q_tail->next;
   }
}



