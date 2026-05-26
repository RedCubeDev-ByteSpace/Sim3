//
// Created by redcube on 26/05/2026.
//

#ifndef SIM3_APPDATA_H
#define SIM3_APPDATA_H
#include <stdint.h>

typedef struct APPDATA_RECENT_RECORD {
    char filename[255];
    char path[255];
    char accessTime[17];
} appdata_recent_record_t;

typedef struct APPDATA_RECENT_RECORD_LIST {
    appdata_recent_record_t *buffer;
    uint32_t bufferSize;
    uint32_t length;
} appdata_recent_record_list_t;

void APPDATA_RECENT_RECORD_LIST_init(appdata_recent_record_list_t *me);
void APPDATA_RECENT_RECORD_LIST_append(appdata_recent_record_list_t *me, appdata_recent_record_t newRecord);
void APPDATA_RECENT_RECORD_LIST_insertAt(appdata_recent_record_list_t *me, appdata_recent_record_t newRecord, uint32_t index);
void APPDATA_RECENT_RECORD_LIST_remove(appdata_recent_record_list_t *me, char *path);
void APPDATA_RECENT_RECORD_LIST_removeAt(appdata_recent_record_list_t *me, uint32_t index);
void APPDATA_RECENT_RECORD_LIST_clear(appdata_recent_record_list_t *me);
void APPDATA_RECENT_RECORD_LIST_grow(appdata_recent_record_list_t *me);
void APPDATA_RECENT_RECORD_LIST_shrink(appdata_recent_record_list_t *me);

extern appdata_recent_record_list_t *APPDATA_lstRecentFiles;

void APPDATA_init();
void APPDATA_recordFileAccess(char *filename);
void APPDATA_read();
void APPDATA_save();

#endif //SIM3_APPDATA_H
