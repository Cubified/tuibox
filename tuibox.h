/*
 * tuibox.h: simple tui library
 */

#ifndef TUIBOX_H
#define TUIBOX_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

/** BEGIN vec.h **/

/** 
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

#define VEC_VERSION "0.2.1"


#define vec_unpack_(v)\
  (char**)&(v)->data, &(v)->length, &(v)->capacity, sizeof(*(v)->data)


#define vec_t(T)\
  struct { T *data; int length, capacity; }


#define vec_init(v)\
  memset((v), 0, sizeof(*(v)))


#define vec_deinit(v)\
  ( free((v)->data),\
    vec_init(v) ) 


#define vec_push(v, val)\
  ( vec_expand_(vec_unpack_(v)) ? -1 :\
    ((v)->data[(v)->length++] = (val), 0), 0 )


#define vec_pop(v)\
  (v)->data[--(v)->length]


#define vec_splice(v, start, count)\
  ( vec_splice_(vec_unpack_(v), start, count),\
    (v)->length -= (count) )


#define vec_swapsplice(v, start, count)\
  ( vec_swapsplice_(vec_unpack_(v), start, count),\
    (v)->length -= (count) )


#define vec_insert(v, idx, val)\
  ( vec_insert_(vec_unpack_(v), idx) ? -1 :\
    ((v)->data[idx] = (val), 0), (v)->length++, 0 )
    

#define vec_sort(v, fn)\
  qsort((v)->data, (v)->length, sizeof(*(v)->data), fn)


#define vec_swap(v, idx1, idx2)\
  vec_swap_(vec_unpack_(v), idx1, idx2)


#define vec_truncate(v, len)\
  ((v)->length = (len) < (v)->length ? (len) : (v)->length)


#define vec_clear(v)\
  ((v)->length = 0)


#define vec_first(v)\
  (v)->data[0]


#define vec_last(v)\
  (v)->data[(v)->length - 1]


#define vec_reserve(v, n)\
  vec_reserve_(vec_unpack_(v), n)

 
#define vec_compact(v)\
  vec_compact_(vec_unpack_(v))


#define vec_pusharr(v, arr, count)\
  do {\
    int i__, n__ = (count);\
    if (vec_reserve_po2_(vec_unpack_(v), (v)->length + n__) != 0) break;\
    for (i__ = 0; i__ < n__; i__++) {\
      (v)->data[(v)->length++] = (arr)[i__];\
    }\
  } while (0)


#define vec_extend(v, v2)\
  vec_pusharr((v), (v2)->data, (v2)->length)


#define vec_find(v, val, idx)\
  do {\
    for ((idx) = 0; (idx) < (v)->length; (idx)++) {\
      if ((v)->data[(idx)] == (val)) break;\
    }\
    if ((idx) == (v)->length) (idx) = -1;\
  } while (0)


#define vec_remove(v, val)\
  do {\
    int idx__;\
    vec_find(v, val, idx__);\
    if (idx__ != -1) vec_splice(v, idx__, 1);\
  } while (0)


#define vec_reverse(v)\
  do {\
    int i__ = (v)->length / 2;\
    while (i__--) {\
      vec_swap((v), i__, (v)->length - (i__ + 1));\
    }\
  } while (0)


#define vec_foreach(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = 0;\
        (iter) < (v)->length && (((var) = (v)->data[(iter)]), 1);\
        ++(iter))


#define vec_foreach_rev(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = (v)->length - 1;\
        (iter) >= 0 && (((var) = (v)->data[(iter)]), 1);\
        --(iter))


#define vec_foreach_ptr(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = 0;\
        (iter) < (v)->length && (((var) = &(v)->data[(iter)]), 1);\
        ++(iter))


#define vec_foreach_ptr_rev(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = (v)->length - 1;\
        (iter) >= 0 && (((var) = &(v)->data[(iter)]), 1);\
        --(iter))



int vec_expand_(char **data, int *length, int *capacity, int memsz);
int vec_reserve_(char **data, int *length, int *capacity, int memsz, int n);
int vec_reserve_po2_(char **data, int *length, int *capacity, int memsz,
                     int n);
int vec_compact_(char **data, int *length, int *capacity, int memsz);
int vec_insert_(char **data, int *length, int *capacity, int memsz,
                int idx);
void vec_splice_(char **data, int *length, int *capacity, int memsz,
                 int start, int count);
void vec_swapsplice_(char **data, int *length, int *capacity, int memsz,
                     int start, int count);
void vec_swap_(char **data, int *length, int *capacity, int memsz,
               int idx1, int idx2);


typedef vec_t(void*) vec_void_t;
typedef vec_t(char*) vec_str_t;
typedef vec_t(int) vec_int_t;
typedef vec_t(char) vec_char_t;
typedef vec_t(float) vec_float_t;
typedef vec_t(double) vec_double_t;

int vec_expand_(char **data, int *length, int *capacity, int memsz) {
  if (*length + 1 > *capacity) {
    void *ptr;
    int n = (*capacity == 0) ? 1 : *capacity << 1;
    ptr = realloc(*data, n * memsz);
    if (ptr == NULL) return -1;
    *data = ptr;
    *capacity = n;
  }
  return 0;
}


int vec_reserve_(char **data, int *length, int *capacity, int memsz, int n) {
  (void) length;
  if (n > *capacity) {
    void *ptr = realloc(*data, n * memsz);
    if (ptr == NULL) return -1;
    *data = ptr;
    *capacity = n;
  }
  return 0;
}


int vec_reserve_po2_(
  char **data, int *length, int *capacity, int memsz, int n
) {
  int n2 = 1;
  if (n == 0) return 0;
  while (n2 < n) n2 <<= 1;
  return vec_reserve_(data, length, capacity, memsz, n2);
}


int vec_compact_(char **data, int *length, int *capacity, int memsz) {
  if (*length == 0) {
    free(*data);
    *data = NULL;
    *capacity = 0;
    return 0;
  } else {
    void *ptr;
    int n = *length;
    ptr = realloc(*data, n * memsz);
    if (ptr == NULL) return -1;
    *capacity = n;
    *data = ptr;
  }
  return 0;
}


int vec_insert_(char **data, int *length, int *capacity, int memsz,
                 int idx
) {
  int err = vec_expand_(data, length, capacity, memsz);
  if (err) return err;
  memmove(*data + (idx + 1) * memsz,
          *data + idx * memsz,
          (*length - idx) * memsz);
  return 0;
}


void vec_splice_(char **data, int *length, int *capacity, int memsz,
                 int start, int count
) {
  (void) capacity;
  memmove(*data + start * memsz,
          *data + (start + count) * memsz,
          (*length - start - count) * memsz);
}


void vec_swapsplice_(char **data, int *length, int *capacity, int memsz,
                     int start, int count
) {
  (void) capacity;
  memmove(*data + start * memsz,
          *data + (*length - count) * memsz,
          count * memsz);
}


void vec_swap_(char **data, int *length, int *capacity, int memsz,
               int idx1, int idx2 
) {
  unsigned char *a, *b, tmp;
  int count;
  (void) length;
  (void) capacity;
  if (idx1 == idx2) return;
  a = (unsigned char*) *data + idx1 * memsz;
  b = (unsigned char*) *data + idx2 * memsz;
  count = memsz;
  while (count--) {
    tmp = *a;
    *a = *b;
    *b = tmp;
    a++, b++;
  }
}

#endif

/** END vec.h **/

/*
 * PREPROCESSOR
 */
#define MAXCACHESIZE 65535

#define CURSOR_Y(b) (b->y+(n+1)+(u->canscroll ? u->scroll : 0))

#define box_contains(x, y, b) (x >= b->x && x <= b->x + b->w && y >= b->y && y <= b->y + b->h)

#define ui_screen(s, u) u->screen = s;u->force = 1

#define ui_center_x(w, u) (((u)->ws.ws_col - w) / 2)
#define ui_center_y(h, u) (((u)->ws.ws_row - h) / 2)

#define UI_CENTER_X -1
#define UI_CENTER_Y -1

/* The argument isn't actually necessary here, but it helps with design consistency */
#define ui_loop(u) char buf[64];int n;while((n=read(STDIN_FILENO, buf, sizeof(buf))) > 0)

#define ui_update(u) _ui_update(buf, n, u)

#define ui_get(id, u) ((u)->b.data[id])

#define COORDINATE_DECODE() \
  tok = strtok(NULL, ";"); \
  x = atoi(tok); \
  tok = strtok(NULL, ";"); \
  y = strtol(tok, NULL, 10) - (u->canscroll ? u->scroll : 0)

#define CLICK_COMPARATOR(x, y, tmp) \
  (u->click == tmp || \
   (box_contains(x, y, tmp) && u->click == NULL))

#define HOVER_COMPARATOR(x, y, tmp) \
  (box_contains(x, y, tmp))

#define LOOP_AND_EXECUTE(f, c) \
  do { \
    vec_foreach(&(u->b), tmp, ind){ \
      if(tmp->screen == u->screen && \
         f != NULL && \
         (c ? CLICK_COMPARATOR(x, y, tmp) : HOVER_COMPARATOR(x, y, tmp)) \
      ){ \
        f(tmp, x, y, u->mouse); \
        if(c){ \
          u->click = tmp; \
        } \
      } \
    } \
  } while(0)

/*
 * TYPES
 */
typedef void (*func)();

typedef struct ui_box_t {
  int id;
  int x, y;
  int w, h;
  int screen;
  char *cache;
  char *watch;
  char last;
  func draw;
  func onclick;
  func onhover;
  void *data1;
  void *data2;
} ui_box_t;

typedef struct ui_evt_t {
  char *c;
  func f;
} ui_evt_t;

typedef vec_t(ui_box_t*) vec_box_t;
typedef vec_t(ui_evt_t*) vec_evt_t;

typedef struct ui_t {
  struct termios tio;
  struct winsize ws;
  vec_box_t b;
  vec_evt_t e;
  ui_box_t *click;
  int mouse, screen,
      scroll, canscroll,
      id, force;
} ui_t;

/* =========================== */

/*
 * Initializes a new UI struct,
 *   puts the terminal into raw
 *   mode, and prints out the
 *   necessary escape codes
 *   for mouse support.
 */
void ui_new(int s, ui_t *u){
  struct termios raw;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &(u->ws));

  tcgetattr(STDIN_FILENO, &(u->tio));
  raw = u->tio;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

  vec_init(&(u->b));
  vec_init(&(u->e));

  u->click = NULL;

  printf("\x1b[?1049h\x1b[0m\x1b[2J\x1b[?1003h\x1b[?1015h\x1b[?1006h\x1b[?25l");

  u->mouse = 0;

  u->screen = s;
  u->scroll = 0;
  u->canscroll = 1;
  
  u->id = 0;

  u->force = 0;
}

/*
 * Frees the given UI struct,
 *   and takes the terminal
 *   out of raw mode.
 */
void ui_free(ui_t *u){
  ui_box_t *val;
  ui_evt_t *evt;
  int i;
  char *term;

  printf("\x1b[0m\x1b[2J\x1b[?1049l\x1b[?1003l\x1b[?1015l\x1b[?1006l\x1b[?25h");
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &(u->tio));

  vec_foreach(&(u->b), val, i){
    free(val->cache);
    free(val);
  }
  vec_deinit(&(u->b));

  vec_foreach(&(u->e), evt, i){
    free(evt);
  }
  vec_deinit(&(u->e));

  term = getenv("TERM");
  if(strncmp(term, "screen", 6) == 0 ||
     strncmp(term, "tmux", 4) == 0){
    printf("Note: Terminal multiplexer detected.\n  For best performance (i.e. reduced flickering), running natively inside\n  a GPU-accelerated terminal such as alacritty or kitty is recommended.\n");
  }
}

/*
 * Adds a new box to the UI.
 *
 * This function is very simple in
 *   nature, but the variety of
 *   properties associated with
 *   an individual box makes it
 *   intimidating to look at.
 * TODO: Find some way to
 *   strip this down.
 */
int ui_add(
  int x, int y, int w, int h, int screen,
  char *watch, char initial,
  func draw, func onclick, func onhover,
  void *data1, void *data2,
  ui_t *u
){
  ui_box_t *b = malloc(sizeof(ui_box_t));

  b->id = u->id++;

  b->x = (x == UI_CENTER_X ? ui_center_x(w, u) : x);
  b->y = (y == UI_CENTER_Y ? ui_center_y(h, u) : y);
  b->w = w;
  b->h = h;

  b->screen = u->screen;

  b->watch = watch;
  b->last = initial;

  b->draw = draw;
  b->onclick = onclick;
  b->onhover = onhover;

  b->data1 = data1;
  b->data2 = data2;

  b->cache = malloc(MAXCACHESIZE);
  draw(b, b->cache);
  b->cache = realloc(b->cache, strlen(b->cache) * 2);

  vec_push(&(u->b), b);

  return b->id;
}

/*
 * Adds a new key event listener
 *   to the UI.
 */
void ui_key(char *c, func f, ui_t *u){
  ui_evt_t *e = malloc(sizeof(ui_evt_t));
  e->c = c;
  e->f = f;

  vec_push(&(u->e), e);
}

/*
 * Clears all elements from
 *   the UI.
 */
void ui_clear(ui_t *u){
  int tmp = u->screen;

  ui_free(u);
  ui_new(tmp, u);
}

/*
 * Draws a single box to the
 *   screen.
 */
void ui_draw_one(ui_box_t *tmp, int flush, ui_t *u){
  char *buf, *tok;
  int n = -1;

  if(tmp->screen != u->screen) return;
  
  buf = calloc(1, strlen(tmp->cache) * 2);
  if(u->force ||
     tmp->watch == NULL ||
     *(tmp->watch) != tmp->last
  ){
    tmp->draw(tmp, buf);
    if(tmp->watch != NULL) tmp->last = *(tmp->watch);
    strcpy(tmp->cache, buf);
  } else {
    /* buf is allocated proportionally to tmp->cache, so strcpy is safe */
    strcpy(buf, tmp->cache);
  }
  tok = strtok(buf, "\n");
  while(tok != NULL){
    if(tmp->x > 0 &&
       tmp->x < u->ws.ws_col &&
       CURSOR_Y(tmp) > 0 &&
       CURSOR_Y(tmp) < u->ws.ws_row){
      printf("\x1b[%i;%iH%s", CURSOR_Y(tmp), tmp->x, tok);
      n++;
    }
    tok = strtok(NULL, "\n");
  }
  free(buf);

  if(flush) fflush(stdout);
}

/*
 * Draws all boxes to the screen.
 */
void ui_draw(ui_t *u){
  ui_box_t *tmp;
  int i;

  printf("\x1b[0m\x1b[2J");

  vec_foreach(&(u->b), tmp, i){
    ui_draw_one(tmp, 0, u);
  }
  fflush(stdout);
  u->force = 0;
}

/*
 * Forces a redraw of the screen,
 *   updating all boxes' caches.
 */
void ui_redraw(ui_t *u){
  u->force = 1;
  ui_draw(u);
}

/*
 * Handles mouse and keyboard
 *   events, given a read()
 *   buffer.
 *
 * This is prefixed with an underscore
 *   to ensure consistency with the
 *   ui_loop macro, ensuring that the
 *   variables buf and n remain
 *   opaque to the user.
 */
void _ui_update(char *c, int n, ui_t *u){
  ui_box_t *tmp;
  ui_evt_t *evt;
  int ind, x, y;
  char cpy[n], *tok;

  if(n >= 4 &&
     c[0] == '\x1b' &&
     c[1] == '[' &&
     c[2] == '<'){
    strncpy(cpy, c, n);
    tok = strtok(cpy+3, ";");
    
    switch(tok[0]){
      case '0':
        u->mouse = (strchr(c, 'm') == NULL);
        COORDINATE_DECODE();
        LOOP_AND_EXECUTE(tmp->onclick, 1);
        if(!u->mouse){
          u->click = NULL;
        }
        break;
      case '3':
        u->mouse = (strcmp(tok, "32") == 0);
        COORDINATE_DECODE();
        LOOP_AND_EXECUTE(tmp->onhover, u->mouse);
        break;
      case '6':
        if(u->canscroll){
          u->scroll += (4 * (tok[1] == '4')) - 2;
          printf("\x1b[0m\x1b[2J");
          ui_draw(u);
        }
        break;
    }
  }

  vec_foreach(&(u->e), evt, ind){
    if(strncmp(c, evt->c, strlen(evt->c)) == 0) evt->f();
  }
}

/*
 * HELPERS
 */
void _ui_text(ui_box_t *b, char *out){
  sprintf(out, "%s", (char*)b->data1);
}

int ui_text(
  int x, int y, char *str,
  int screen,
  func click, func hover,
  ui_t *u
){
  return ui_add(
    x, y,
    strlen(str), 1,
    screen,
    NULL, 0,
    _ui_text,
    click,
    hover,
    str,
    NULL,
    u
  );
}

#endif
