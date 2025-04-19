#include "io.h"
#include "mb.h"
#include "terminal.h"

unsigned int width, height, pitch, isrgb;
unsigned char* fb;

void fb_init() {
  mbox[0] = 35 * 4;             // Leght of message in bytes
  mbox[1] = MBOX_REQUEST;

  mbox[2] = MBOX_TAG_SETPHYWH;  // Tag identifier
  mbox[3] = 8;                  // value size in byte
  mbox[4] = 0;
  mbox[5] = 1920;               // value (width)
  mbox[6] = 1080;               // value (height)

  mbox[7]  = MBOX_TAG_SETVIRTWH;
  mbox[8]  = 8;
  mbox[9]  = 8;
  mbox[10] = 1920;
  mbox[11] = 1080;

  mbox[12] = MBOX_TAG_SETVIRTOFF;
  mbox[13] = 8;
  mbox[14] = 8;
  mbox[15] = 0;                 // value (x)
  mbox[16] = 0;                 // value (y)

  mbox[17] = MBOX_TAG_SETDEPTH;
  mbox[18] = 4;
  mbox[19] = 4;
  mbox[20] = 32;                // Bits per pixel

  mbox[21] = MBOX_TAG_SETPXLORDR;
  mbox[22] = 4;
  mbox[23] = 4;
  mbox[24] = 1;                 // RGB

  mbox[25] = MBOX_TAG_GETFB;
  mbox[26] = 8;
  mbox[27] = 8;
  mbox[28] = 4096;              // FrameBufferInfo.pointer
  mbox[29] = 0;                 // FrameBufferInfo.size

  mbox[30] = MBOX_TAG_GETPITCH;
  mbox[31] = 4;
  mbox[32] = 4;
  mbox[33] = 0;                 // Bytes per line

  mbox[34] = MBOX_TAG_LAST;

  // Check call is successful and we have a pointer with depth 32
  if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0) {
    mbox[28] &= 0x3FFFFFFF;  // convert GPU address to ARM address
    width  = mbox[10];       // Actual physical width
    height = mbox[11];       // Actual physical height
    pitch  = mbox[33];       // Number of bytes per line
    isrgb  = mbox[24];       // Pixel order
    fb     = (unsigned char*)((long)mbox[28]);
  }
}

/**
 * x: x axis position,
 * y: y axis position,
 * attr: Color with the 4 most significant bits representing the background colour and the 4 least significant bits, the foreground color.
 */
void drawPixel(int x, int y, unsigned char attr) {
  int offs                      = (y * pitch) + (x * 4);                                     // There are 4 bytes(or 32 bits) per pixel (ARGB).
  *((unsigned int*)(fb + offs)) = vgapal[attr & 0x0f];
}

void drawChar(unsigned char ch, int x, int y, unsigned char attr) {
  unsigned char* glyph = (unsigned char*)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;  // Get the array pointer for the character

  for (int i = 0; i < FONT_HEIGHT; i++) {
    for (int j = 0; j < FONT_WIDTH; j++) {
      unsigned char mask = 1 << j;                                                           // Mask the significant bit
      unsigned char col =
          (*glyph & mask) ? attr & 0x0f : (attr & 0xf0);  // If the byte & mask = 1 we print the character color, otherwise (black), the color is in the 4 least significant bits.

      drawPixel(x + j, y + i, col);
    }

    glyph += FONT_BPL;                                    // next byte
  }
}

void drawString(int x, int y, char* s, unsigned char attr) {
  while (*s) {
    if (*s == '\r') {
      x = 0;
    } else if (*s == '\n') {
      x = 0;
      y += FONT_HEIGHT;
    } else {
      drawChar(*s, x, y, attr);
      x += FONT_WIDTH;
    }
    s++;
  }
}
