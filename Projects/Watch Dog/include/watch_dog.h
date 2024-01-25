/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      -----
* Date          November 24, 2021
* Description   Watchdog functions Module - Header
* Version:      1.0.0
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#ifndef __OL110WATCH_DOG_H__
#define __OL110WATCH_DOG_H__

int WDStart(char *argv[], const char *wd_name,
            unsigned int prog_sig_interval, unsigned int prog_attempts,
            unsigned int wd_sig_interval, unsigned int wd_attempts);

int WDStop();

#endif
