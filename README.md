# tuibox

tuibox ("toybox") is a single-header terminal UI library, capable of creating mouse-driven, interactive applications on the command line.

It is completely dependency-free, using nothing other than ANSI escape sequences to handle parsing input and rendering the screen.  Additionally, its individual parts work largely independently, meaning as much (or as little) of the library as is necessary can be used.

## Demos

Basic button demo w/ truecolor background (note:  ugly color banding is from GIF compression):
![demo_basic.gif](https://github.com/Cubified/tuibox/blob/main/demos/demo_basic.gif)

Bouncing box using custom render loop:
![demo_bounce.gif](https://github.com/Cubified/tuibox/blob/main/demos/demo_bounce.gif)

Click and drag:
![demo_drag.gif](https://github.com/Cubified/tuibox/blob/main/demos/demo_drag.gif)

## Features

tuibox currently contains the following:

- Event-driven terminal render loop
- Mouse click and hover events on individual UI elements
- Keyboard events (including escape sequence events, such as arrow keys)
- Render caching (controlled by a user-defined dirty bit)
- Completely dependency-free, single-header library using pure ANSI escape sequences (no ncurses)
- Incrementally-adoptable -- rendering, events, and looping can all be used independently

## Design Overview

The basic hierarchy of a UI looks like this:

- UI:  A collection of screens
    - Screen:  A collection of boxes
        - Box:  Any UI element
            - X/Y, width/height
            - Render function
            - Click event function (optional)
            - Hover event function (optional)
            - Arbitrary user data (see below example)
- Main loop:  Infinite read() loop gathering user input
    - Update:  Parses user input and fires events (if applicable)

## Example Code

A complete example is as follows:

```c
/* Global UI struct */
ui_t u;

/* Function that generates box contents */
void text(ui_box_t *b, char *out){
  sprintf(out, "%s", (char*)b->data1);
}

/* Function that runs on box click */
void click(ui_box_t *b, int x, int y){
  b->data1 = "you clicked me!";
  ui_draw(&u);
}

/* Function that runs on box hover */
void hover(ui_box_t *b, int x, int y, int down){
  b->data1 = "you hovered me!";
  ui_draw(&u);
}

void stop(){
  ui_free(&u);
  exit(0);
}

int main(){
  /* Initialize UI data structures */
  ui_new(0, &u);

  /* Add a new text box to screen 0 */
  ui_text(
    UI_CENTER_X, UI_CENTER_Y,
    "hello world!",
    0,
    click, hover,
    &u
  );

  /* Register an event on the q key */
  ui_key('q', stop, &u);

  /* Render the screen */
  ui_draw(&u);

  ui_loop(&u){
    /* Check for mouse/keyboard events */
    ui_update(&u);
  }

  return 0;
}
```

## Compiling and Running Demos

To compile all demos in one go:

     $ make
     $ ls demos
     ...
     demo_basic
     demo_bounce
     demo_drag
     ...

## Libraries/See Also

- [vec](https://github.com/rxi/vec):  Dynamic arrays used to store events and boxes
- [bdfedit](https://github.com/Cubified/bdfedit):  Bitmap font editor written alongside `tuibox`
