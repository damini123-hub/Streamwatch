/********************************************************************
created:    2009/07/20 - 12:00
file name:  sample_common_misc.cpp
purpose:    Auxinfo handling and other shared stuff

 Copyright (c) 2014 MainConcept GmbH or its affiliates.  All rights reserved.

 MainConcept and its logos are registered trademarks of MainConcept GmbH or its affiliates.  
 This software is protected by copyright law and international treaties.  Unauthorized 
 reproduction or distribution of any portion is prohibited by law.
*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if !defined(__linux__) && !defined(__APPLE__)
#include <conio.h> // _kbhit()
#endif

#include "sample_common_misc.h"
#include "mctypes.h"
#include "mcfourcc.h"

#if defined _MSC_VER
#define strcmp_lower_case _stricmp
#else
#define strcmp_lower_case strcasecmp
#endif

auxinfo_handler_t org_auxinfo;

pic_start_info pic_info;
FILE * enc_stat_file = NULL;

void info_printf(const char * fmt, ...)
{
    char lst[1024];
    va_list marker;

    va_start(marker, fmt);
    vsnprintf(lst, sizeof(lst), fmt, marker);
    va_end(marker);

    strncat(lst, "\r\n", sizeof(lst)-strlen(lst)-1);
    printf("%s\n", lst);
}


void warn_printf(const char * fmt, ...)
{
    char lst[256];
    va_list marker;

    va_start(marker, fmt);
    vsnprintf(lst, sizeof(lst), fmt, marker);
    va_end(marker);

    printf("%s\n", lst);
}


void error_printf(const char * fmt, ...)
{
    char lst[256];
    va_list marker;

    va_start(marker,fmt);
    vsnprintf(lst, sizeof(lst), fmt, marker);
    va_end(marker);

    printf("%s\n", lst);
}


void progress_printf(int32_t percent, const char * fmt, ...)
{
    char lst[256];
    va_list marker;

    va_start(marker,fmt);
    vsnprintf(lst, sizeof(lst), fmt, marker);
    va_end(marker);

    printf(" %d - %s\n", percent, lst);
}


int32_t yield()
{
	if (has_pressed_key(NULL))
		return 1;

	return 0;
}

void statistics_printf(char *fmt, ...) {
    va_list va;

    if(enc_stat_file)
    {
        fseek(enc_stat_file, 0, SEEK_END);

        va_start(va, fmt);
        vfprintf(enc_stat_file, fmt, va);
        va_end(va);

        fprintf(enc_stat_file, "\n");
        fflush(enc_stat_file);
    }
}

// resource functions dispatcher
void * MC_EXPORT_API get_rc(const char* name)
{
    if (!strcmp(name, "err_printf"))
        return (void*) error_printf;
    else if (!strcmp(name, "prg_printf"))
        return (void*) progress_printf;
    else if (!strcmp(name, "wrn_printf"))
        return (void*) warn_printf;
    else if (!strcmp(name, "inf_printf"))
        return (void*) info_printf;
    else if (!strcmp(name, "yield"))
        return (void*) yield;
    else if (!strcmp(name, "stat_printf"))
        return (void*) statistics_printf;

    return NULL;
}


#define V210_LINE_ALIGN     (128 - 1)

int32_t get_video_frame_size(int32_t width, int32_t height, uint32_t cs_fourcc, int32_t * line_size, int32_t * img_start)
{
    int frame_size = 0;

    *line_size = width;
    *img_start = 0;

    switch (cs_fourcc)
    {
    case FOURCC_YV12:
    case FOURCC_I420:
	case FOURCC_IYUV:
        *line_size = width;
        frame_size = width * height * 12 / 8;
        break;

    case FOURCC_I422:
        *line_size = width;
        frame_size = *line_size * height * 2;
        break;

    case FOURCC_I444:
        *line_size = width;
        frame_size = *line_size * height * 3;
        break;

    case FOURCC_YUYV:
	case FOURCC_YuYv:
    case FOURCC_YUY2:
    case FOURCC_UYVY:
	case FOURCC_UyVy:
	case FOURCC_VYUY:
    case FOURCC_YVYU:
         *line_size = width * 2;
         frame_size = *line_size * height;
         break;

    case FOURCC_YV16:
        *line_size = width;
        frame_size = *line_size * height * 2;
        break;

    case FOURCC_W009:
	case FOURCC_W010:
	case FOURCC_W011:
	case FOURCC_W012:
	case FOURCC_W016:
    case FOURCC_X009:
    case FOURCC_X010:
    case FOURCC_X011:
    case FOURCC_X012:
    case FOURCC_X014:
    case FOURCC_X016:
        *line_size = width * 2;
        frame_size = width * height * 3;
        break;

    case FOURCC_W209:
	case FOURCC_W210:
	case FOURCC_W211:
	case FOURCC_W212:
	case FOURCC_W216:
    case FOURCC_X209:
    case FOURCC_X210:
    case FOURCC_X211:
    case FOURCC_X212:
    case FOURCC_X214:
    case FOURCC_X216:
        *line_size = width * 2;
        frame_size = *line_size * height * 2;
        break;

    case FOURCC_W409:
	case FOURCC_W410:
	case FOURCC_W411:
	case FOURCC_W412:
	case FOURCC_W416:
    case FOURCC_X409:
    case FOURCC_X410:
    case FOURCC_X411:
    case FOURCC_X412:
    case FOURCC_X416:
    case FOURCC_R48P:
        *line_size = width * 2;
        frame_size = *line_size * height * 3;
		break;
    case FOURCC_V210:
        *line_size = ((width * 128 / 6) / 8 + V210_LINE_ALIGN) & (~V210_LINE_ALIGN);
        frame_size = *line_size * height;
        break;

        // RGB data is stored upside down
    case FOURCC_BGR3:
        *line_size = -width * 3;
        *img_start = (height - 1) * width * 3;
        frame_size = width * height * 3;
        break;

    case FOURCC_BGR4:
    case FOURCC_BGRA:
    case FOURCC_RGBA:
    case FOURCC_ARGB:
    case FOURCC_argb:
    case FOURCC_AR10:
    case FOURCC_AB10:
        *line_size = -width * 4;
        *img_start = (height - 1) * width * 4;
        frame_size = width * height * 4;
        break;

    case FOURCC_VUYA:
    case FOURCC_AYUV:
        *line_size = width * 4;
        frame_size = width * height * 4;
        break;

    case FOURCC_VUYA32F:
    case FOURCC_BGRA32F:
        *line_size = width * 16;
         frame_size = width * height * 16;
        break;

    case FOURCC_R32C:
        *line_size = -width * 4;
        *img_start = (height - 1) * width * 4;
        frame_size = width * height * 4;
        break;

    case FOURCC_R24C:
        *line_size = -width * 3;
        *img_start = (height - 1) * width * 3;
        frame_size = width * height * 3;
        break;

    case FOURCC_H264:
        frame_size = 32 * 1024;
        *line_size = frame_size;
        break;
    case FOURCC_XYZ12:
        *line_size = width * 3 * 2;
        frame_size = *line_size * height;
        break;
	case FOURCC_Y16:
		*line_size = width * 2;
		frame_size = *line_size * height;
		break;
	case FOURCC_V216:
	case FOURCC_Y216:
		*line_size = width * 2 * 2;
		frame_size = *line_size * height;
		break;
	case FOURCC_Y416:
		*line_size = width * 4 * 2;
		frame_size = *line_size * height;
		break;
	case FOURCC_YVU9:
	case FOURCC_YUV9:
		frame_size = (9*width*height)/8;
		break;
	case FOURCC_IMC1:
	case FOURCC_IMC3:
		frame_size = (((height + 15) & ~15) + ((height / 2 + 15) & ~15) + (height / 2)) * width;
		break;
	case FOURCC_IMC2:
	case FOURCC_IMC4:
		frame_size = (3*width*height)/2;
		break;
	case FOURCC_NV12:
	case FOURCC_NV21:
		frame_size = (3*width*height)/2;
		break;
	case FOURCC_NV24:
		frame_size = 3*width*height;
		break;
	case FOURCC_R565:
	case FOURCC_R555:
		*line_size = 2 * width;
		frame_size = *line_size * height;
		break;
	case FOURCC_DPX10:
	case FOURCC_Y410:
	case FOURCC_Y210:
		*line_size = 4 * width;
		frame_size = *line_size * height;
		break;
	case FOURCC_P210:
	case FOURCC_P216:
		*line_size = width * 2;
		frame_size = *line_size * height * 2;
		break;
	case FOURCC_Y41P:
		*line_size = ((width+7)&~7)*3/2;
		frame_size = *line_size * height;
		break;
	case FOURCC_CLJR:
	case FOURCC_Y211:
		frame_size = width * height;
		break;
	case FOURCC_YV24:
		frame_size = width * height * 3;
		break;
	case FOURCC_P010:
	case FOURCC_P016:
		*line_size = 2 * width;
		frame_size = 3 * height * width;
		break;
	}
    return frame_size;
}




int32_t init_frame(frame_tt *frame, uint32_t four_cc, int32_t width, int32_t height, uint8_t *p, int32_t bottomup, int32_t stride) {
	int32_t plane_count = 0;
	int32_t plane_height[4];
	stride = stride < 0 ? -stride : stride;

	frame->four_cc = four_cc;
	frame->width = width;
	frame->height = height;
	frame->stride[0] = stride;
	frame->plane[0] = p;

	plane_count = 1;
	plane_height[0] = height;

	switch (four_cc) {
		case FOURCC_AYUV:
		case FOURCC_VUYA:
		case FOURCC_YUY2:
		case FOURCC_YUYV:
		case FOURCC_YuYv:
		case FOURCC_UYVY:
		case FOURCC_UyVy:
		case FOURCC_VYUY:
		case FOURCC_V210:
		case FOURCC_argb:
		case FOURCC_ARGB:
		case FOURCC_BGRA:
		case FOURCC_RGBA:
		case FOURCC_BGR4:
		case FOURCC_BGR3:
		case FOURCC_R565:
		case FOURCC_R555:
		case FOURCC_R32C:
		case FOURCC_R24C:
		case FOURCC_Y210:
		case FOURCC_Y216:
		case FOURCC_Y410:
		case FOURCC_Y416:
		case FOURCC_Y211:
		case FOURCC_Y41P:
        case FOURCC_AR10:
        case FOURCC_AB10:
			break;
		case FOURCC_YV24:
		case FOURCC_I444:
			frame->stride[1] = width;
			frame->stride[2] = width;
			frame->plane[1] = p + width*height;
			frame->plane[2] = p + width*height + width*height;

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height;
			plane_height[2] = height;
			break;
		case FOURCC_YV16:
		case FOURCC_I422:
			frame->stride[1] = width/2;
			frame->stride[2] = width/2;
			frame->plane[1] = p + width*height;
			frame->plane[2] = p + (width*height) + (width/2)*height;

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height;
			plane_height[2] = height;
			break;
		case FOURCC_NV24:
			frame->stride[1] = width*2;
			frame->plane[1] = p + width*height;

			plane_count = 2;
			plane_height[0] = height;
			plane_height[1] = height;
			break;
		case FOURCC_YV12:
		case FOURCC_I420:
		case FOURCC_IYUV:
			frame->stride[1] = width/2;
			frame->stride[2] = width/2;
			frame->plane[1] = p + width*height;
			frame->plane[2] = p + (width*height) + (width/2)*(height/2);

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height/2;
			plane_height[2] = height/2;
			break;
		case FOURCC_NV12:
		case FOURCC_NV21:
			frame->stride[1] = width;
			frame->plane[1] = p + width*height;

			plane_count = 2;
			plane_height[0] = height;
			plane_height[1] = height/2;
			break;
		case FOURCC_IMC1:
		case FOURCC_IMC3:
			frame->stride[1] = width;
			frame->stride[2] = width;
			frame->plane[1] = p + ((height + 15) & ~15) * width;
			frame->plane[2] = p + (((height + 15) & ~15) + ((height / 2 + 15) & ~15)) * width;

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height/2;
			plane_height[2] = height/2;
			break;
		case FOURCC_IMC2:
		case FOURCC_IMC4:
			frame->stride[1] = width;
			frame->stride[2] = width;
			frame->plane[1] = p + width*height;
			frame->plane[2] = p + (width*height) + (width/2);

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height/2;
			plane_height[2] = height/2;
			break;
		case FOURCC_YVU9:
		case FOURCC_YUV9:
			frame->stride[1] = width/4;
			frame->stride[2] = width/4;
			frame->plane[1] = p + width*height;
			frame->plane[2] = p + (width*height) + (width/4)*(height/4);

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height/4;
			plane_height[2] = height/4;
			break;
        case FOURCC_X009:
        case FOURCC_X010:
        case FOURCC_X011:
        case FOURCC_X012:
        case FOURCC_X014:
        case FOURCC_X016:
		case FOURCC_W009:
		case FOURCC_W010:
		case FOURCC_W011:
		case FOURCC_W012:
		case FOURCC_W016:
			frame->stride[0] = width*2;
			frame->stride[1] = width;
			frame->stride[2] = width;
			frame->plane[1] = p + width*height*2;
			frame->plane[2] = p + ((width*height) + (width/2)*(height/2))*2;

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height/2;
			plane_height[2] = height/2;
			break;
        case FOURCC_X209:
        case FOURCC_X210:
        case FOURCC_X211:
        case FOURCC_X212:
        case FOURCC_X214:
        case FOURCC_X216:
		case FOURCC_W209:
		case FOURCC_W210:
		case FOURCC_W211:
		case FOURCC_W212:
		case FOURCC_W216:
			frame->stride[0] = width*2;
			frame->stride[1] = width;
			frame->stride[2] = width;
			frame->plane[1] = p + width*height*2;
			frame->plane[2] = p + width*height*2 + width*height;

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height;
			plane_height[2] = height;
			break;
        case FOURCC_X409:
        case FOURCC_X410:
        case FOURCC_X411:
        case FOURCC_X412:
        case FOURCC_X416:
		case FOURCC_W409:
		case FOURCC_W410:
		case FOURCC_W411:
		case FOURCC_W412:
		case FOURCC_W416:
        case FOURCC_R48P:
			frame->stride[0] = width*2;
			frame->stride[1] = width*2;
			frame->stride[2] = width*2;
			frame->plane[1] = p + width*height*2;
			frame->plane[2] = p + (width*height + width*height)*2;

			plane_count = 3;
			plane_height[0] = height;
			plane_height[1] = height;
			plane_height[2] = height;
			break;
		case FOURCC_P010:
		case FOURCC_P016:
			frame->stride[0] = width*2;
			frame->stride[1] = width*2;
			frame->plane[1] = p + (width*height)*2;
			frame->plane[2] = NULL;

			plane_count = 2;
			plane_height[0] = height;
			plane_height[1] = height/2;
			break;
		case FOURCC_P210:
		case FOURCC_P216:
			frame->stride[0] = width*2;
			frame->stride[1] = width*2;
			frame->plane[1] = p + (width*height)*2;
			frame->plane[2] = NULL;

			plane_count = 2;
			plane_height[0] = height;
			plane_height[1] = height;
			break;
		default:
			return 0;
	}

	// Apply flipping
	if( bottomup ) {
		int32_t i;
		for(i=0; i<plane_count; i++) {
			frame->plane[i] = frame->plane[i] + frame->stride[i]*(plane_height[i] - 1);
			frame->stride[i] = -frame->stride[i];
		}
	}

	return 1;
}



// encoder auxinfo handler
uint32_t auxinfo(bufstream_tt * bs, uint32_t offs, uint32_t info_id, void * info_ptr, uint32_t info_size)
{
    sample_info_struct * sample_info;
    encode_stat_struct * encode_stat;
    v_au_struct * vau;
    pic_start_info *ref_pic_info;

    switch (info_id)
    {
    case TIME_STAMP_INFO:
        sample_info = (sample_info_struct *) info_ptr;
        break;

    case STATISTIC_INFO:
        encode_stat = (encode_stat_struct *) info_ptr;

        printf("\r %d frames encoded @ %.2f fps", encode_stat->frames_encoded, encode_stat->average_speed);
        break;

    case VIDEO_AU_CODE:
        vau = (v_au_struct *)info_ptr;
        break;

   case ID_PICTURE_START_CODE:
        ref_pic_info = (pic_start_info*)info_ptr;
        pic_info = *ref_pic_info;
        break;
    }

    // call original auxinfo handler
    return org_auxinfo(bs, offs, info_id, info_ptr, info_size);
}

int has_pressed_key( const char* text )
{
#if !defined(__linux__) && !defined(__APPLE__)
    if( _kbhit() )
    {
        _getch();
        return 1;
    }
#endif
    return 0;
}



// WAVE PCM soundfile format

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT_ 0x20746d66
#define ID_DATA 0x61746164


int32_t wav_header_read(FILE *fp, wav_hdr_param* param)
{
	memset(param,0,sizeof(wav_hdr_param));
	uint32_t id;
    int32_t offset = 0;

	offset += fread(&id, 1, sizeof(uint32_t), fp);
	if (id != ID_RIFF)
	{
		fseek(fp, 0, 0);
		return 0;
	}

	uint32_t data_size;
	offset += fread(&data_size, 1, sizeof(uint32_t), fp);

    offset += fread(&id, 1, sizeof(uint32_t), fp);
	if (id != ID_WAVE)
	{
		fseek(fp, 0, 0);
		return 0;
	}

    while (!feof(fp))
    {
	    offset += fread(&id, 1, sizeof(uint32_t), fp);
        switch (id)
        {
            case ID_FMT_:
            {
	            uint32_t fmt_size;
	            offset += fread(&fmt_size, 1, sizeof(uint32_t), fp);
                offset += fmt_size;

	            int16_t audio_format;
                uint8_t wavx_flag = 0;
	            fread(&audio_format, sizeof(uint16_t), 1, fp);
	            if (audio_format == 1)
                    wavx_flag = 0;
                else if(audio_format == -2)
                    wavx_flag = 1;
                else
	            {
		            fseek(fp, 0, 0);
		            return 0;
	            }

	            fread(&param->num_channels, sizeof(uint16_t), 1, fp);
	            fread(&param->sample_rate, sizeof(uint32_t), 1, fp);
	            fread(&param->bytes_per_sec, sizeof(uint32_t), 1, fp);
	            fread(&param->block_align, sizeof(uint16_t), 1, fp);
	            fread(&param->bits_per_sample, sizeof(uint16_t), 1, fp);

                if (wavx_flag)
                {
                    // These parameters are necessary for wav-x header type
                    uint16_t cbsize, valid_bits_per_sample;
                    uint32_t channel_mapping;
                    uint16_t subformat_guid[8];

                    if (fread(&cbsize, 2, 1, fp) != 1) 
                        return 1;
                    if (cbsize != 22) 
                        return 1;
                    if (fread(&valid_bits_per_sample, 2, 1, fp) != 1) 
                        return 1;
                    if (fread(&channel_mapping, 4, 1, fp) != 1) 
                        return 1;
                    if (fread(subformat_guid, 2, 8, fp) != 8) 
                        return 1;
                    if (fmt_size > 40)
                    {
                        if (fseek(fp, fmt_size - 40, SEEK_CUR)) 
                            return 1;
                    }
                }
                else
                {
                    // Consume any remaining bytes 
                    if (fmt_size > 16)
                    {
                        if (fseek(fp, fmt_size - 16, SEEK_CUR)) 
                            return 1;
                    }
                    //fseek(fp, offset, SEEK_SET);    //to skip extra format bytes
                }
                break;
            }
            case ID_DATA:
            {
                offset += fread(&param->data_size, 1, sizeof(uint32_t), fp);
                return offset;
            }
            default:
            {   //skip all not supported ids
                uint32_t chunk_id = 0;
                uint32_t chunk_size = 0;
                size_t read = 0;

                offset += (read = fread(&chunk_size, 1, sizeof(chunk_size), fp));
                if (read != sizeof(chunk_size))
                    break;

                offset += chunk_size;
                fseek(fp, chunk_size, SEEK_CUR);
                break;
            }
        }
    }

    //no DATA header found
    fseek(fp, 0, 0);
    return 0;
}


int32_t wav_data_read(FILE *fp, wav_hdr_param* param, uint8_t *buf, uint32_t buf_size)
{
	if (param->data_size <= 0)
		return 0;

	uint32_t len = (uint32_t) fread(buf, 1, (param->data_size < (int32_t)buf_size) ? param->data_size : buf_size, fp);

	param->data_size -= len;

	return len;
}


int32_t wav_header_write(FILE *fp, wav_hdr_param* param)
{
	fseek(fp, 0, 0);

	uint32_t id = ID_RIFF;
	fwrite(&id, sizeof(uint32_t), 1, fp);

	uint32_t data_size = param->data_size + 36;
	fwrite(&data_size, sizeof(uint32_t), 1, fp);

	id = ID_WAVE;
	fwrite(&id, sizeof(uint32_t), 1, fp);

	id = ID_FMT_;
	fwrite(&id, sizeof(uint32_t), 1, fp);

	uint32_t fmt_size = 16;
	fwrite(&fmt_size, sizeof(uint32_t), 1, fp);

	int16_t audio_format = 1;
	fwrite(&audio_format, sizeof(uint16_t), 1, fp);

	fwrite(&param->num_channels, sizeof(uint16_t), 1, fp);
	fwrite(&param->sample_rate, sizeof(uint32_t), 1, fp);
	fwrite(&param->bytes_per_sec, sizeof(uint32_t), 1, fp);
	fwrite(&param->block_align, sizeof(uint16_t), 1, fp);
	fwrite(&param->bits_per_sample, sizeof(uint16_t), 1, fp);

	id = ID_DATA;
	fwrite(&id, sizeof(uint32_t), 1, fp);

	fwrite(&param->data_size, sizeof(uint32_t), 1, fp);

	return 28 + fmt_size;
}

int32_t wav_data_write(FILE *fp, wav_hdr_param* param, uint8_t *buf, uint32_t size)
{
	int32_t len = (uint32_t) fwrite(buf, 1, size, fp);
	param->data_size += len;
	return len;
}

static void MD5Transform(uint32_t buf[4], uint32_t const in[16]);

#ifndef __BIG_ENDIAN__
# define byteReverse(buf, len)    /* Nothing */
#else
/*
 * Note: this code is harmless on little-endian machines.
 */
void byteReverse(uint32_t *buf, unsigned len)
{
  uint32_t t;
  do {
    char* bytes = (char *) buf;
    t = ((unsigned) bytes[3] << 8 | bytes[2]) << 16 |
        ((unsigned) bytes[1] << 8 | bytes[0]);
    *buf = t;
    buf++;
  } while (--len);
}
#endif

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void MD5Init(context_md5_t *ctx)
{
  ctx->buf[0] = 0x67452301;
  ctx->buf[1] = 0xefcdab89;
  ctx->buf[2] = 0x98badcfe;
  ctx->buf[3] = 0x10325476;

  ctx->bits[0] = 0;
  ctx->bits[1] = 0;
}

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
void MD5Update(context_md5_t *ctx, uint8_t *buf, size_t len)
{
  uint32_t t;

  /* Update bitcount */

  t = ctx->bits[0];
  if ((ctx->bits[0] = t + ((uint32_t) len << 3)) < t)
    ctx->bits[1]++;        /* Carry from low to high */
  ctx->bits[1] += len >> 29;

  t = (t >> 3) & 0x3f;    /* Bytes already in shsInfo->data */

  /* Handle any leading odd-sized chunks */

  if (t) {
    uint8_t *p = ctx->in.b8 + t;

    t = 64 - t;
    if (len < t) {
      memcpy(p, buf, len);
      return;
    }
    memcpy(p, buf, t);
    byteReverse(ctx->in.b32, 16);
    MD5Transform(ctx->buf, ctx->in.b32);
    buf += t;
    len -= t;
  }
  /* Process data in 64-byte chunks */

  while (len >= 64) {
    memcpy(ctx->in.b8, buf, 64);
    byteReverse(ctx->in.b32, 16);
    MD5Transform(ctx->buf, ctx->in.b32);
    buf += 64;
    len -= 64;
  }

    /* Handle any remaining bytes of data. */

  memcpy(ctx->in.b8, buf, len);
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void MD5Final(uint8_t digest[16], context_md5_t *ctx, uint32_t invert)
{
  size_t count;
  uint8_t *p;

  /* Compute number of bytes mod 64 */
  count = (ctx->bits[0] >> 3) & 0x3F;

  /* Set the first char of padding to 0x80.  This is safe since there is
     always at least one byte free */
  p = ctx->in.b8 + count;
  *p++ = 0x80;

  /* Bytes of padding needed to make 64 bytes */
  count = 64 - 1 - count;

  /* Pad out to 56 mod 64 */
  if (count < 8) {
    /* Two lots of padding:  Pad the first block to 64 bytes */
    memset(p, 0, count);
    byteReverse(ctx->in.b32, 16);
    MD5Transform(ctx->buf, ctx->in.b32);

    /* Now fill the next block with 56 bytes */
    memset(ctx->in.b8, 0, 56);
  } else {
    /* Pad block to 56 bytes */
    memset(p, 0, count - 8);
  }
  byteReverse(ctx->in.b32, 14);

  /* Append length in bits and transform */
  ctx->in.b32[14] = ctx->bits[0];
  ctx->in.b32[15] = ctx->bits[1];

  MD5Transform(ctx->buf, ctx->in.b32);
  byteReverse((uint32_t *) ctx->buf, 4);
  if (invert) {
    uint8_t *p = (uint8_t*)(ctx->buf);
    for (int32_t i = 15; i >= 0; i--)
      digest[i] = *p++;
  } else
    memcpy(digest, ctx->buf, 16);

  memset(ctx, 0, sizeof(* ctx));    /* In case it's sensitive */
  /* The original version of this code omitted the asterisk. In
     effect, only the first part of ctx was wiped with zeros, not
     the whole thing. Bug found by Derek Jones. Original line: */
  // memset(ctx, 0, sizeof(ctx));    /* In case it's sensitive */
}

/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
    ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
static void MD5Transform(uint32_t buf[4], uint32_t const in[16])
{
  register uint32_t a, b, c, d;

  a = buf[0];
  b = buf[1];
  c = buf[2];
  d = buf[3];

  MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
  MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
  MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
  MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
  MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
  MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
  MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
  MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
  MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
  MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
  MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
  MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
  MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
  MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
  MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
  MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

  MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
  MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
  MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
  MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
  MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
  MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
  MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
  MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
  MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
  MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
  MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
  MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
  MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
  MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
  MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
  MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

  MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
  MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
  MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
  MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
  MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
  MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
  MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
  MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
  MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
  MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
  MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
  MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
  MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
  MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
  MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
  MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

  MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
  MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
  MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
  MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
  MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
  MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
  MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
  MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
  MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
  MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
  MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
  MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
  MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
  MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
  MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
  MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}
