// SpiceWare Sound demo ver 1.0 - released August 24, 1997

#include "dialog.h"
#include "sndos2.h"
#include "dsp.h"

#define INCL_WIN
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// you do not have to have a normal window.  A dialog window all by itself is A-OK.

MRESULT EXPENTRY SoundDemoDlgProc(HWND hwndWnd, ULONG ulMsg, MPARAM mpParm1, MPARAM mpParm2);

void main(void)
{
   ERRORID error;
   HAB hab;
   HMQ hmq;
   hab = WinInitialize(0);
   hmq = WinCreateMsgQueue(hab, 0);

   audio_init(22050,2048);

   WinDlgBox(HWND_DESKTOP, HWND_DESKTOP, SoundDemoDlgProc, NULLHANDLE, SD_DLG_BOX, NULL);

   audio_close();
   return;
}

MRESULT EXPENTRY SoundDemoDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static unsigned long freq[4] = {0,0,0,0}; // variables are static or they keep changing back to 0
   static int volume[4] = {31,21,11,31};
   static char text[80];
   static long int frequency = 22050;
   static int buffer = 2048;
   static int i;

   switch(msg)
   {
      case WM_INITDLG:   // initialize all the sliders and numeric displays

         WinSetDlgItemShort(hwnd, SD_V1_FT, freq[0], FALSE);        // init freq 1 text
         WinSetDlgItemShort(hwnd, SD_V1_VT, volume[0], TRUE);       // init vol 1 text
         WinSendDlgItemMsg(hwnd, SD_V1_V, SLM_SETSLIDERINFO,        // init vol 1 slider position
              MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE),
              MPFROMLONG(volume[0]) );

         WinSetDlgItemShort(hwnd, SD_V2_FT, freq[1], FALSE);        // init freq 2 text
         WinSetDlgItemShort(hwnd, SD_V2_VT, volume[1], TRUE);       // init vol 2 text
         WinSendDlgItemMsg(hwnd, SD_V2_V, SLM_SETSLIDERINFO,        // init vol 2 slider position
              MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE),
              MPFROMLONG(volume[1]) );

         WinSetDlgItemShort(hwnd, SD_V3_FT, freq[2], FALSE);        // init freq 3 text
         WinSetDlgItemShort(hwnd, SD_V3_VT, volume[2], TRUE);       // init vol 3 text
         WinSendDlgItemMsg(hwnd, SD_V3_V, SLM_SETSLIDERINFO,        // init vol 3 slider position
              MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE),
              MPFROMLONG(volume[2]) );

         WinSetDlgItemShort(hwnd, SD_V4_FT, freq[3], FALSE);        // init freq 4 text
         WinSetDlgItemShort(hwnd, SD_V4_VT, volume[3], TRUE);       // init vol 4 text
         WinSendDlgItemMsg(hwnd, SD_V4_V, SLM_SETSLIDERINFO,        // int vol 4 slider position
              MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE),
              MPFROMLONG(volume[3]) );

         WinSetDlgItemShort(hwnd, SD_BS_BT, buffer, TRUE);          // init buffer size text
         WinSendDlgItemMsg(hwnd, SD_BS_B, SLM_SETSLIDERINFO,        // init buffer size slider position
              MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE),
              MPFROMLONG(60) );

         WinCheckButton(hwnd, SD_FREQ_22, 1);  // supposed to set the frequency 22050 button on, but doesn't work
                                               // if you know how this should be done, please e-mail me at spice@ibm.net.
         return 0;

      case WM_CONTROL:
         switch(SHORT1FROMMP(mp1))
         {
            case SD_FREQ_8: audio_close();                     // whenever the frequency changes
                            frequency = 8000;                  // the sound info for all 4 channels
                            audio_init(frequency,buffer);      // needs to be resent, and the audio
                            for (i=0;i<=3;i++)                 // buffer must be recalculated
                               Play(i, freq[i], volume[i]);
                            FillBuffer();
                            return 0;
            case SD_FREQ_11:audio_close();
                            frequency = 11025;
                            audio_init(frequency,buffer);
                            for (i=0;i<=3;i++)
                               Play(i, freq[i], volume[i]);
                            FillBuffer();
                            return 0;
            case SD_FREQ_22:audio_close();
                            frequency = 22050;
                            audio_init(frequency,buffer);
                            for (i=0;i<=3;i++)
                               Play(i, freq[i], volume[i]);
                            FillBuffer();
                            return 0;
            case SD_FREQ_33:audio_close();
                            frequency = 33075;
                            audio_init(frequency,buffer);
                            for (i=0;i<=3;i++)
                               Play(i, freq[i], volume[i]);
                            FillBuffer();
                            return 0;
            case SD_FREQ_44:audio_close();
                            frequency = 44100;
                            audio_init(frequency,buffer);
                            for (i=0;i<=3;i++)
                               Play(i, freq[i], volume[i]);
                            FillBuffer();
                            return 0;
            case SD_BS_B: if (SHORT2FROMMP(mp1) == SLN_CHANGE)  // for the buffer size, don't change on a slidetrack message
                          {
                             buffer = (int) mp2 * 32 + 128;
                             WinSetDlgItemShort(hwnd, SD_BS_BT, buffer, TRUE);
                             audio_close();
                             audio_init(frequency,buffer);
                             for (i=0;i<=3;i++)
                                Play(i, freq[i], volume[i]);
                             FillBuffer();
                          }
                          if (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK) // on slidetrack message, only update the text for feedback
                          {
                             buffer = (int) mp2 * 32 + 128;
                             WinSetDlgItemShort(hwnd, SD_BS_BT, buffer, TRUE);
                          }
                          return 0;
            case SD_V1_F: if ((SHORT2FROMMP(mp1) == SLN_CHANGE) || (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK))
                          {
                             freq[0] = (int) mp2 * 40;
                             WinSetDlgItemShort(hwnd, SD_V1_FT, freq[0], FALSE);
                             Play(0, freq[0], volume[0]);
                             FillBuffer();
                          }
                          return 0;
            case SD_V1_V: if ((SHORT2FROMMP(mp1) == SLN_CHANGE) || (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK))
                          {
                             volume[0] = (int) mp2;
                             WinSetDlgItemShort(hwnd, SD_V1_VT, volume[0], TRUE);
                             Play(0, freq[0], volume[0]);
                             FillBuffer();
                          }
                          return 0;
            case SD_V2_F: if ((SHORT2FROMMP(mp1) == SLN_CHANGE) || (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK))
                          {
                             freq[1] = (int) mp2 * 40;
                             WinSetDlgItemShort(hwnd, SD_V2_FT, freq[1], FALSE);
                             Play(1, freq[1], volume[1]);
                             FillBuffer();
                          }
                          return 0;
            case SD_V2_V: if ((SHORT2FROMMP(mp1) == SLN_CHANGE) || (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK))
                          {
                             volume[1] = (int) mp2;
                             WinSetDlgItemShort(hwnd, SD_V2_VT, volume[1], TRUE);
                             Play(1, freq[1], volume[1]);
                             FillBuffer();
                          }
                          return 0;
            case SD_V3_F: if ((SHORT2FROMMP(mp1) == SLN_CHANGE) || (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK))
                          {
                             freq[2] = (int) mp2 * 40;
                             WinSetDlgItemShort(hwnd, SD_V3_FT, freq[2], FALSE);
                             Play(2, freq[2], volume[2]);
                             FillBuffer();
                          }
                          return 0;
            case SD_V3_V: if ((SHORT2FROMMP(mp1) == SLN_CHANGE) || (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK))
                          {
                             volume[2] = (int) mp2;
                             WinSetDlgItemShort(hwnd, SD_V3_VT, volume[2], TRUE);
                             Play(2, freq[2], volume[2]);
                             FillBuffer();
                          }
                          return 0;
            case SD_V4_F: if ((SHORT2FROMMP(mp1) == SLN_CHANGE) || (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK))
                          {
                             freq[3] = (int) mp2 * 40;
                             WinSetDlgItemShort(hwnd, SD_V4_FT, freq[3], FALSE);
                             Play(3, freq[3], volume[3]);
                             FillBuffer();
                          }
                          return 0;
            case SD_V4_V: if ((SHORT2FROMMP(mp1) == SLN_CHANGE) || (SHORT2FROMMP(mp1) == SLN_SLIDERTRACK))
                          {
                             volume[3] = (int) mp2;
                             WinSetDlgItemShort(hwnd, SD_V4_VT, volume[3], TRUE);
                             Play(3, freq[3], volume[3]);
                             FillBuffer();
                          }
                          return 0;
            }
      return 0;
   }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}
