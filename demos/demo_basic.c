/*
 * demo_basic.c: a simple tuibox.h demo
 */

#include <math.h>

#include "../tuibox.h"

/* Global UI struct */
ui_t u;

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
      sprintf(tmp, "\x1b[48;2;%i;%i;%im ", (int)round(255 * ((double)x / (double)b->w)), (int)round(255 * ((double)y / (double)b->h)), (int)round(255 * ((double)x * (double)y / ((double)b->w * (double)b->h))));
      strcat(out, tmp);
    }
    strcat(out, "\x1b[0m\n");
  }
}

/* Function that runs on box click */
void click(ui_box_t *b, int x, int y){
  b->data1 = "\x1b[0m                \n  you clicked me!  \n                ",
  ui_draw_one(b, 1, &u);
}

/* Function that runs on box hover */
void hover(ui_box_t *b, int x, int y, int down){
  b->data1 = "\x1b[0m                \n  you hovered me!  \n                ",
  ui_draw_one(b, 1, &u);
}

void stop(){
  ui_free(&u);
  exit(0);
}

int main(){
  /* Initialize UI data structures */
  ui_new(0, &u);

  /* Add new UI elements to screen 0 */
  ui_add(
    1, 1,
    u.ws.ws_col, u.ws.ws_row,
    0,
    NULL, 0,
    draw,
    NULL,
    NULL,
    NULL,
    NULL,
    &u
  );

  ui_text(
    ui_center_x(19, &u), UI_CENTER_Y,
    "\x1b[0m                   \n    click on me!   \n                   ",
    0,
    click, hover,
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
