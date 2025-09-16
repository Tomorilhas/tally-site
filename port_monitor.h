#pragma once
void setupPortMonitor();
void monitorPortasTask(void *pvParameters);
void getExternaTally(size_t porta_idx, int &externa_num, const char* &cor_tally);
extern bool porta_last_state[10];