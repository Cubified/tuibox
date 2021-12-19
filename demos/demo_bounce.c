/*
 * demo_bounce.c: bouncing physics box
 */

#include <math.h>
#include <signal.h>

#include "../tuibox.h"

ui_t u;

void draw(ui_box_t *b, char *out){
  int x, y;
  char tmp[256];

  sprintf(out, "\x1b[48;2;255;255;255m");
  for(y=0;y<b->h;y++){
    for(x=0;x<b->w;x++){
      strcat(out, " ");
    }
    strcat(out, "\n");
  }
}

void stop(){
  ui_free(&u);
  exit(0);
}

int main(){
  char watch = 0;
  int id;
  ui_box_t *b;

  double vx = 2.0, vy = 0.0,
         ax = 0.0, ay = 0.1;

  ui_new(0, &u);

  /* Add new UI elements to screen 0 */
  id = ui_add(
    10, 10,
    20, 10,
    0,
    &watch, 1, /* Dirty bit: Run draw() once, and never again */
    draw,
    NULL, NULL,
    NULL, NULL,
    &u
  );
  b = ui_get(id, &u);

  signal(SIGTERM, stop);
  signal(SIGQUIT, stop);
  signal(SIGINT,  stop);

  for(;;){
    b->x = round((double)b->x + vx);
    b->y = round((double)b->y + vy);

    vx += ax;
    vy += ay;

    if(b->x < 1){
      b->x = 1;
      vx *= -1.0;
    } else if(b->x+b->w > u.ws.ws_col){
      b->x = u.ws.ws_col - b->w;
      vx *= -1.0;
    }

    if(b->y < 1){
      b->y = 1;
      vy *= -1.0;
    } else if(b->y+b->h > u.ws.ws_row){
      b->y = u.ws.ws_row - b->h;
      vy *= -1.0;
    }

    ui_draw(&u);
    usleep(10000);
  }

  return 0;
}
