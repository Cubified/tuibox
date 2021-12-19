/*
 * demo_drag.c: click and drag
 */

#include <math.h>

#include "../tuibox.h"

/* Global UI struct */
ui_t u;

int sx = 15, sy = 7;

/* Functions that generate box contents */
void text(ui_box_t *b, char *out){
  sprintf(out, "%s", (char*)b->data1);
}

void draw(ui_box_t *b, char *out){
  int x, y;
  char tmp[256];

  sprintf(out, "");
  for(y=0;y<b->h;y++){
    for(x=0;x<b->w;x++){
      /* Truecolor string to generate gradient */
      sprintf(tmp, "\x1b[48;2;%i;%i;%im ", (int)round(255 * ((double)x * (double)y / ((double)b->w * (double)b->h))), (int)round(255 * ((double)y / (double)b->h)), (int)round(255 * ((double)x / (double)b->w)));
      strcat(out, tmp);
    }
    strcat(out, "\x1b[0m\n");
  }
}

/* Function that runs on box click */
void click(ui_box_t *b, int x, int y){
  sx = x-b->x;
  sy = y-b->y;
}

/* Function that runs on box hover */
void hover(ui_box_t *b, int x, int y, int down){
  if(down){
    b->x = x-sx;
    b->y = y-sy;
    ui_draw(&u);
  }
}

void stop(){
  ui_free(&u);
  exit(0);
}

int main(){
  char watch = 0;

  /* Initialize UI data structures */
  ui_new(0, &u);

  /* Add new UI elements to screen 0 */
  ui_add(
    10, 10,
    30, 15,
    0,
    &watch, 1,
    draw,
    click,
    hover,
    NULL,
    NULL,
    &u
  );

  /* Register an event on the q key */
  ui_key("q", stop, &u);

  /* Render the screen */
  ui_draw(&u);

  ui_loop(&u){
    /* Check for mouse/keyboard events */
    ui_update(&u);
  }

  return 0;
}
