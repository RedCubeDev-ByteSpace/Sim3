//
// Created by redcube on 26/05/2026.
//

#include "AppData.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <json-c/json_object.h>
#include <json-c/json_util.h>

#include "../../GUI/Drawables/FixedContact.h"

appdata_recent_record_list_t *APPDATA_lstRecentFiles;

void APPDATA_init() {
    APPDATA_lstRecentFiles = malloc(sizeof(appdata_recent_record_list_t));
    APPDATA_RECENT_RECORD_LIST_init(APPDATA_lstRecentFiles);

    APPDATA_read();
}

void APPDATA_recordFileAccess(char *filename) {

    // remove this file from the list if its already on there
    APPDATA_RECENT_RECORD_LIST_remove(APPDATA_lstRecentFiles, filename);

    // create a new record for it
    appdata_recent_record_t recentRecord;
    snprintf(recentRecord.path, sizeof(recentRecord.path), "%s", filename);
    snprintf(recentRecord.filename, sizeof(recentRecord.path), "%s", GetFileName(filename));

    // this includes a timestamp
    time_t rawTime = time(NULL);
    struct tm *localTime = localtime(&rawTime);
    strftime(recentRecord.accessTime, sizeof(recentRecord.accessTime), "%Y-%m-%d %H:%M", localTime);

    // push it to the top of the list
    APPDATA_RECENT_RECORD_LIST_insertAt(APPDATA_lstRecentFiles, recentRecord, 0);

    // store the list to disk
    APPDATA_save();
}

void APPDATA_read() {
    json_object *root = json_object_from_file("appdata.json");
    if (root == NULL) return;
    APPDATA_RECENT_RECORD_LIST_clear(APPDATA_lstRecentFiles);
    // -----------------------------------------------------------------------------------------------------------------

    json_object *recentFiles = json_object_object_get(root, "recent_files");
    int len = json_object_array_length(recentFiles);
    for (int i = 0; i < len; i++) {
        json_object *record = json_object_array_get_idx(recentFiles, i);
        appdata_recent_record_t recentRecord;

        char *path = json_object_get_string(json_object_object_get(record, "path"));
        char *accessTime = json_object_get_string(json_object_object_get(record, "access_time"));

        // copy over the path and filename
        snprintf(recentRecord.path, sizeof(recentRecord.path), "%s", path);
        snprintf(recentRecord.filename, sizeof(recentRecord.filename), "%s", GetFileName(path));

        // copy over the timestamp
        snprintf(recentRecord.accessTime, sizeof(recentRecord.accessTime), "%s", accessTime);

        APPDATA_RECENT_RECORD_LIST_append(APPDATA_lstRecentFiles, recentRecord);
    }

    // -----------------------------------------------------------------------------------------------------------------
    json_object_put(root);
}

void APPDATA_save() {
    json_object *root = json_object_new_object();
    // -----------------------------------------------------------------------------------------------------------------

    json_object *recentFiles = json_object_new_array();
    for (int i = 0; i < APPDATA_lstRecentFiles->length; ++i) {
        json_object *record = json_object_new_object();
        json_object_object_add(record, "path", json_object_new_string(APPDATA_lstRecentFiles->buffer[i].path));
        json_object_object_add(record, "access_time", json_object_new_string(APPDATA_lstRecentFiles->buffer[i].accessTime));

        json_object_array_add(recentFiles, record);
    }
    json_object_object_add(root, "recent_files", recentFiles);

    // -----------------------------------------------------------------------------------------------------------------
    json_object_to_file_ext("appdata.json", root, JSON_C_TO_STRING_PRETTY);
    json_object_put(root);
}