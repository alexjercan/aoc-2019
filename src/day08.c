#include "../include/day08.h"
#include "../include/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 128

#define IMAGE_WIDTH 25
#define IMAGE_HEIGHT 6
#define LAYER_SIZE IMAGE_WIDTH *IMAGE_HEIGHT

static void parse_row(char *input, int row[]) {
  for (int i = 0; i < IMAGE_WIDTH; i++) {
    row[i] = input[i] - '0';
  }
}

static void parse_layer(char *input, int layer[][IMAGE_WIDTH]) {
  for (int i = 0; i < IMAGE_HEIGHT; i++) {
    parse_row(input + i * IMAGE_WIDTH, layer[i]);
  }
}

static void parse_input(char *input, struct array *layers) {
  for (int i = 0; i < strlen(input) - 1; i += LAYER_SIZE) {
    int layer[IMAGE_HEIGHT][IMAGE_WIDTH];

    parse_layer(input + i, layer);
    array_append(layers, layer);
  }
}

static int part1(struct array *layers) {
  int min_zeros = LAYER_SIZE;
  int result = 0;

  for (int i = 0; i < array_size(layers); i++) {
    int pixels[3] = {0, 0, 0};

    for (int j = 0; j < IMAGE_HEIGHT; j++) {
      for (int k = 0; k < IMAGE_WIDTH; k++) {
        int layer[IMAGE_HEIGHT][IMAGE_WIDTH];
        array_get(layers, i, layer);

        pixels[layer[j][k]]++;
      }
    }

    if (pixels[0] < min_zeros) {
      min_zeros = pixels[0];
      result = pixels[1] * pixels[2];
    }
  }

  return result;
}

static int part2(struct array *layers, char image[]) {
  int result[IMAGE_HEIGHT][IMAGE_WIDTH];

  for (int i = 0; i < IMAGE_HEIGHT; i++) {
    for (int j = 0; j < IMAGE_WIDTH; j++) {
      result[i][j] = 2;
    }
  }

  for (int i = 0; i < array_size(layers); i++) {
    int layer[IMAGE_HEIGHT][IMAGE_WIDTH];
    array_get(layers, i, layer);

    for (int j = 0; j < IMAGE_HEIGHT; j++) {
      for (int k = 0; k < IMAGE_WIDTH; k++) {
        if (result[j][k] == 2) {
          result[j][k] = layer[j][k];
        }
      }
    }
  }

  int index = 0;
  for (int i = 0; i < IMAGE_HEIGHT; i++) {
    for (int j = 0; j < IMAGE_WIDTH; j++) {
      if (result[i][j] == 1) {
        image[index] = '#';
      } else {
        image[index] = ' ';
      }
      index++;
    }
    image[index++] = '\n';
  }

  return 0;
}

void day08_solve(char *input, char *output) {
  struct array *layers =
      array_new(CAPACITY, sizeof(int[IMAGE_HEIGHT][IMAGE_WIDTH]));
  parse_input(input, layers);

  char image[LAYER_SIZE + IMAGE_HEIGHT + 1] = {0};
  part2(layers, image);
  sprintf(output, "Day08\nPart1: %d\nPart2: \n%s", part1(layers), image);

  array_destroy(layers);
}
