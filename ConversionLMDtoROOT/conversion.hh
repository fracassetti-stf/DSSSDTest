#ifndef conversion_h
#define conversion_h 0



TFile* rootFile;

//Event* ge_data;

typedef struct {
  uint32_t dlen;
  uint32_t type;
  uint32_t subtype;
  uint16_t fragments;
  uint16_t begin;
  uint16_t end;
  uint16_t used;
  uint32_t buf;
  uint32_t evt;
  uint32_t current;
  uint32_t l_time[2];
  uint32_t l_free[4];
} gsibuffer;


typedef struct {
  uint32_t dlen;
  uint16_t type;
  uint16_t subtype;
  uint16_t dummy;
  uint16_t trigger;
  uint32_t count;
} eventhead;


typedef struct {
  uint32_t dlen;
  uint16_t type;
  uint16_t subtype;
  uint16_t procid;
  uint16_t fragments;
  uint16_t control;
  uint16_t subcrate;

} sub_eventhead;

typedef struct {
  uint32_t channel;
  uint32_t bufsize;
  
} gosiphead;


typedef struct {
  uint16_t size;
  uint16_t magic;
  uint32_t trigger;
  uint32_t time[2];
  uint64_t timestamp;
  uint32_t overflow;
  uint8_t self;
  uint16_t pileup;
  uint16_t discard;
  int16_t height;
} febex;

typedef struct {
  uint64_t timestamp;
  int16_t height;
} event;

typedef struct {
  uint32_t dlen;
  uint16_t type;
  uint16_t subtype;
} fragment;


#endif
