#pragma once


#define FIFO_COMMAND_PACK(c1,c2,c3,c4) (((c4) << 24) | ((c3) << 16) | ((c2) << 8) | (c1))

typedef short t16;        /*!< \brief text coordinate 12.4 fixed point */
#define f32tot16(n)          ((t16)(n >> 8)) /*!< \brief convert f32 to t16 */
#define inttot16(n)          ((n) << 4) /*!< \brief convert int to t16 */
#define t16toint(n)          ((n) >> 4) /*!< \brief convert t16 to int */
#define floattot16(n)        ((t16)((n) * (1 << 4))) /*!< \brief convert float to t16 */
#define TEXTURE_PACK(u,v)    (((u) & 0xFFFF) | ((v) << 16)) /*!< \brief Pack 2 t16 texture coordinate values into a 32bit value */

typedef short int v16;       /*!< \brief vertex 4.12 fixed format */
#define inttov16(n)          ((n) << 12) /*!< \brief convert int to v16 */
#define f32tov16(n)          (n) /*!< \brief f32 to v16 */
#define v16toint(n)          ((n) >> 12) /*!< \brief convert v16 to int */
#define floattov16(n)        ((v16)((n) * (1 << 12))) /*!< \brief convert float to v16 */
#define VERTEX_PACK(x,y)     (uint32_t)(((x) & 0xFFFF) | ((y) << 16)) /*!< \brief Pack to v16 values into one 32bit value */

typedef short int v10;        /*!< \brief vertex 4.6 fixed point, USED FOR 10bit VERTEXES!!!*/
#define inttov10(n)          ((n) << 6) /*!< \brief convert int to v10 */
#define f32tov10(n)          ((n) >> 6) /*!< \brief f32 to v10 */
#define v10toint(n)          ((n) >> 6) /*!< \brief convert v10 to int */
#define floattov10(n)        ((v10)((n) * (1 << 6))) /*!< \brief convert float to v10 */
#define VERTEX10_PACK(x,y,z) (uint32_t)(((x) & 0x3FF) | ((y & 0x3FF) << 10) | ((z & 0x3FF) << 20)) /*!< \brief Pack to v16 values into one 32bit value */

typedef short int vn10;       /*!< \brief normal .10 fixed point, NOT USED FOR 10bit VERTEXES!!!*/
#define inttovn10(n)          ((n) << 9) /*!< \brief convert int to v10 */
#define f32tovn10(n)          ((vn10)(n >> 3)) /*!< \brief convert f32 to v10 */
#define vn10toint(n)          ((n) >> 9) /*!< \brief convert v10 to int */
#define floattovn10(n)        ((n>.998) ? 0x1FF : ((vn10)((n)*(1<<9)))) /*!< \brief convert float to v10 */
#define NORMAL_PACK(x,y,z)    (uint32_t)(((x) & 0x3FF) | (((y) & 0x3FF) << 10) | ((z) << 20)) /*!< \brief Pack 3 vn10 normals into a 32bit value */

typedef enum {
    GL_TRIANGLES = 0, /*!< draw triangles with each 3 vertices defining a triangle */
    GL_QUADS = 1, /*!< draw quads with each 4 vertices defining a quad */
    GL_TRIANGLE_STRIP = 2, /*!< draw triangles with the first triangle defined by 3 vertices, then each additional triangle being defined by one additional vertex */
    GL_QUAD_STRIP = 3, /*!< draw quads with the first quad being defined by 4 vertices, then each additional triangle being defined by 2 vertices. */
    GL_TRIANGLE = 0, /*!< same as GL_TRIANGLES, old non-OpenGL version */
    GL_QUAD = 1  /*!< same as GL_QUADS, old non-OpenGL version */
} GL_GLBEGIN_ENUM;

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