#pragma once

#define REG2ID(r)		(uint8_t)( ( ((uint32_t)(&(r)))-0x04000400 ) >> 2 ) /*!< \brief converts a GFX command for use in a packed command list */

#define GFX_CONTROL           (*(uint16_t*) 0x04000060)

#define GFX_RDLINES_COUNT     (*(uint32_t*) 0x04000320)

#define GFX_FIFO              (*(uint32_t*) 0x04000400)
#define GFX_STATUS            (*(uint32_t*) 0x04000600)
#define GFX_COLOR             (*(uint32_t*) 0x04000480)

#define GFX_VERTEX10          (*(uint32_t*) 0x04000490)
#define GFX_VERTEX16          (*(uint32_t*) 0x0400048C)
#define GFX_VERTEX_XY         (*(uint32_t*) 0x04000494)
#define GFX_VERTEX_XZ         (*(uint32_t*) 0x04000498)
#define GFX_VERTEX_YZ         (*(uint32_t*) 0x0400049C)
#define GFX_VERTEX_DIFF       (*(uint32_t*) 0x040004A0)

#define GFX_TEX_COORD         (*(uint32_t*) 0x04000488)
#define GFX_TEX_FORMAT        (*(uint32_t*) 0x040004A8)
#define GFX_PAL_FORMAT        (*(uint32_t*) 0x040004AC)

#define GFX_CLEAR_COLOR       (*(uint32_t*) 0x04000350)
#define GFX_CLEAR_DEPTH       (*(uint16_t*) 0x04000354)
#define GFX_CLRIMAGE_OFFSET   (*(uint16_t*) 0x04000356)

#define GFX_LIGHT_VECTOR      (*(uint32_t*) 0x040004C8)
#define GFX_LIGHT_COLOR       (*(uint32_t*) 0x040004CC)
#define GFX_NORMAL            (*(uint32_t*) 0x04000484)

#define GFX_DIFFUSE_AMBIENT   (*(uint32_t*) 0x040004C0)
#define GFX_SPECULAR_EMISSION (*(uint32_t*) 0x040004C4)
#define GFX_SHININESS         (*(uint32_t*) 0x040004D0)

#define GFX_POLY_FORMAT       (*(uint32_t*) 0x040004A4)
#define GFX_ALPHA_TEST        (*(uint16_t*) 0x04000340)

#define GFX_BEGIN			(*(uint32_t*) 0x04000500)
#define GFX_END				(*(uint32_t*) 0x04000504)
#define GFX_FLUSH			(*(uint32_t*) 0x04000540)
#define GFX_VIEWPORT		(*(uint32_t*) 0x04000580)
#define GFX_TOON_TABLE		((uint16_t*)  0x04000380)
#define GFX_EDGE_TABLE		((uint16_t*)  0x04000330)
#define GFX_FOG_COLOR		(*(uint32_t*) 0x04000358)
#define GFX_FOG_OFFSET		(*(uint32_t*) 0x0400035C)
#define GFX_FOG_TABLE		((uint8_t*)   0x04000360)
#define GFX_BOX_TEST		(*(int32_t*)  0x040005C0)
#define GFX_POS_TEST		(*(uint32_t*) 0x040005C4)
#define GFX_POS_RESULT		((int32_t*)   0x04000620)
#define GFX_VEC_TEST		(*(uint32_t*) 0x040005C8)
#define GFX_VEC_RESULT      ((int16_t*)   0x04000630)



#define FIFO_NOP				REG2ID(GFX_FIFO)
#define FIFO_STATUS				REG2ID(GFX_STATUS)
#define FIFO_COLOR				REG2ID(GFX_COLOR)

#define FIFO_VERTEX16			REG2ID(GFX_VERTEX16)
#define FIFO_VERTEX10			REG2ID(GFX_VERTEX10)
#define FIFO_VERTEX_XY			REG2ID(GFX_VERTEX_XY)
#define FIFO_VERTEX_XZ			REG2ID(GFX_VERTEX_XZ)
#define FIFO_VERTEX_YZ			REG2ID(GFX_VERTEX_YZ)
#define FIFO_TEX_COORD			REG2ID(GFX_TEX_COORD)
#define FIFO_TEX_FORMAT			REG2ID(GFX_TEX_FORMAT)
#define FIFO_PAL_FORMAT			REG2ID(GFX_PAL_FORMAT)

#define FIFO_CLEAR_COLOR		REG2ID(GFX_CLEAR_COLOR)
#define FIFO_CLEAR_DEPTH		REG2ID(GFX_CLEAR_DEPTH)

#define FIFO_LIGHT_VECTOR		REG2ID(GFX_LIGHT_VECTOR)
#define FIFO_LIGHT_COLOR		REG2ID(GFX_LIGHT_COLOR) 
#define FIFO_NORMAL				REG2ID(GFX_NORMAL)

#define FIFO_DIFFUSE_AMBIENT	REG2ID(GFX_DIFFUSE_AMBIENT) 
#define FIFO_SPECULAR_EMISSION	REG2ID(GFX_SPECULAR_EMISSION)
#define FIFO_SHININESS			REG2ID(GFX_SHININESS)

#define FIFO_POLY_FORMAT		REG2ID(GFX_POLY_FORMAT)

#define FIFO_BEGIN				REG2ID(GFX_BEGIN)
#define FIFO_END				REG2ID(GFX_END)
#define FIFO_FLUSH				REG2ID(GFX_FLUSH)
#define FIFO_VIEWPORT			REG2ID(GFX_VIEWPORT)