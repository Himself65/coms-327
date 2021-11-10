# Developer

Zeyu Yang <zeyuyang@iastate.edu> <himself65@outlook.com>

## key data structure

- struct
- if/else statement
- for loop
- standard I/O
- memory allocation

## headers

- ca.h

  the whole header of the assignment needed

## source files

### ca.c

the implementation of `ca.h`

- `void displayCA(struct ca_data *ca);`

  using for loop to display the ca_data

- `int set1DCACell(struct ca_data *ca, unsigned int x, unsigned char state);`

  set 1 dimension cadata using array index

- `int set2DCACell(struct ca_data *ca, unsigned int x, unsigned int y, unsigned char state);`

  set 2 dimension cadata using array index

- `void initCA(struct ca_data *ca, int state);`

  initializing cadata with state, using for loop

- `void step1DCA(struct ca_data *ca, unsigned char (*rule)(struct ca_data *, int x));`

  pass in a function pointer for 1 dimension, and simulate the ca, which each step call the function and set the result
  to the new ca states.


- `void step2DCA(struct ca_data *ca, unsigned char (*rule)(struct ca_data *, int x, int y));`

  pass in a function pointer for 2 dimension, and simulate the ca, which each step call the function and set the result
  to the new ca states.

- `struct ca_data *create1DCA(int width, unsigned char q_state);`

  using `malloc` to create 1 dimension ca_data

- `struct ca_data *create2DCA(int w, int h, unsigned char q_state);`

  using `malloc` to create 2 dimension ca_data

### main.c

the main function, which reading the data from the `argv`, then load file and execute the `ca->step2DCA`.
