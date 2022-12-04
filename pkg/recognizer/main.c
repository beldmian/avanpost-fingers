#include "main.h"
#include "locale.h"
#include "stdio.h"
#include "stdlib.h"
#include "wchar.h"
#define PI 3.141592653
const unsigned short RULE[] = {
    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
    0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};

void skeletonization(short pixels[N][M], short skeletonized[N][M]) {
  for (size_t j = 0; j < M; ++j) {
    skeletonized[0][j] = 0;
  }
  for (size_t j = 0; j < N; ++j) {
    skeletonized[j][0] = 0;
  }
  for (size_t j = 0; j < M; ++j) {
    skeletonized[N - 1][j] = 0;
  }
  for (size_t j = 0; j < N; ++j) {
    skeletonized[j][M - 1] = 0;
  }
  for (size_t i = 1; i < N - 1; ++i) {
    for (size_t j = 1; j < M - 1; ++j) {
      // printf("(%d %d)", i, j);
      if (pixels[i][j] == 0) {
        skeletonized[i][j] = 0;
        continue;
      }
      int temp = pixels[i + 1][j + 1] + (pixels[i + 1][j] << 1) +
                 (pixels[i + 1][j - 1] << 2) + (pixels[i][j + 1] << 3) +
                 (pixels[i][j - 1] << 4) + (pixels[i - 1][j + 1] << 5) +
                 (pixels[i - 1][j] << 6) + (pixels[i - 1][j - 1] << 7);
      // printf("%d: %d \n", pixels[i][j], temp);
      switch (RULE[temp]) {
      case 0:
        skeletonized[i][j] = pixels[i][j];
        break;
      case 1:
        skeletonized[i][j] = 0;
        break;
      case 2:
        skeletonized[i][j] = 0;
        skeletonized[i - 1][j] = pixels[i][j];
        break;
      case 3: // left
        skeletonized[i][j] = 0;
        skeletonized[i][j - 1] = pixels[i][j];
        break;
      case 4: // down
        skeletonized[i][j] = 0;
        skeletonized[i + 1][j] = pixels[i][j];
        break;
      case 5: // right
        skeletonized[i][j] = 0;
        skeletonized[i][j + 1] = pixels[i][j];
        break;
      }
    }
  }
}

// void minutia_search(short pixels[N][M], short minutia_pixels[N-2][M-2]){
//     // short count_of_black_pixels;
//     // minutia_pixels[0][0] = 0;
//     for(size_t i = 1; i < N - 1; ++i){
//         // printf("0 ");
//         for(size_t j = 1; j < M - 1; ++j){
//             // if(pixels[i][j] == 0){
//             //     // printf("0 ");
//             //     minutia_pixels[i - 1][j - 1] = 0;
//             //     continue;
//             // }
//             // minutia_pixels[i - 1][j - 1] =  pixels[i - 1][j - 1] +
//             pixels[i - 1][j] + pixels[i - 1][j + 1] +
//             //                                 pixels[i][j - 1] + pixels[i][j
//             + 1] +
//             //                                 pixels[i + 1][j - 1] +
//             pixels[i + 1][j] + pixels[i + 1][j + 1];
//             // if(minutia_pixels[i - 1][j - 1] > 4)
//             //     minutia_pixels[i - 1][j - 1] = 4;
//             // // printf("%d ", minutia_pixels[i - 1][j - 1]);
//         }
//         // printf("0 ");
//         // printf("\n");
//     }
//     // return minutia_pixels;
// }

// size_t minutia_search(short pixels[N][M], short (*minutia_pixels)[3]){
size_t minutia_search(short pixels[N][M], short minutia_pixels[500][3]) {
  size_t counter = 0;
  short count_of_black_pixels;
  for (size_t i = 1; i < N - 1; ++i) {
    for (size_t j = 1; j < M - 1; ++j) {
      // printf("%d", counter);
      if (pixels[i][j] == 0)
        continue;
      count_of_black_pixels = pixels[i - 1][j - 1] + pixels[i - 1][j] +
                              pixels[i - 1][j + 1] + pixels[i][j - 1] +
                              pixels[i][j + 1] + pixels[i + 1][j - 1] +
                              pixels[i + 1][j] + pixels[i + 1][j + 1];
      if ((count_of_black_pixels == 0) || (count_of_black_pixels == 2) ||
          (count_of_black_pixels > 5))
        continue;
      minutia_pixels[counter][0] = i;
      minutia_pixels[counter][1] = j;
      if (count_of_black_pixels == 5)
        minutia_pixels[counter][2] = 4;
      else
        minutia_pixels[counter][2] = count_of_black_pixels;
      ++counter;
      // printf("%d", counter);
    }
  }
  // printf("%d", counter);
  // minutia_pixels = realloc(minutia_pixels, counter*sizeof(short[3]));
  return counter;
}

void smooth(short pixels[N][M]) {
  size_t box_width = 4, box_height = 2;
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < M; j++) {
    }
  }
}

// int** binarization(short** pixels){
void binarization(short pixels[N][M]) {
  // for (size_t i = 0; i < N; ++i){
  //     for (size_t j = 0; j < M; ++j){
  //         if(pixels[i][j] > 50)
  //             pixels[i][j] = 0;
  //         else
  //             pixels[i][j] = 1;
  //     }
  // }

  const int k = 5;
  const int k2 = 2 * k;
  const int k2_2 = k2 * k2;
  const double r = 0.9;
  const size_t n = N / k2;
  const size_t m = M / k2;
  // printf("1");
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      int x = i * k2 + k, y = j * k2 + k;
      int sum = 0;
      for (size_t ii = x - k; ii < x + k; ++ii) {
        for (size_t jj = y - k; jj < y + k; ++jj) {
          sum += pixels[ii][jj];
        }
      }
      int floor = (r * sum / k2_2);
      // printf("(%d) - %d\n", i, floor);
      // printf("%d %d\n",sum, floor);
      // if(floor > 245){
      //     for(size_t ii = x - k; ii < x + k; ++ii){
      //         for(size_t jj = y - k; jj < y + k; ++jj){
      //             pixels[ii][jj] = 0;
      //         }
      //     }
      //     continue;
      // }
      for (size_t ii = x - k; ii < x + k; ++ii) {
        // printf("\t");
        for (size_t jj = y - k; jj < y + k; ++jj) {
          // printf("%d ", pixels[ii][j]);
          if (pixels[ii][jj] > floor)
            pixels[ii][jj] = 0;
          else
            pixels[ii][jj] = 1;
        }
        // printf("\n");
      }
    }
  }

  for (size_t i = 0; i < n; ++i) {
    int x = i * k2 + k;
    int sum = 0;
    for (size_t j = m * k2; j < M; ++j) {
      for (size_t ii = x - k; ii < x + k; ++ii) {
        sum += pixels[ii][j];
      }
    }
    int floor = (r * sum / k2_2);
    // printf("(%d) - %d\n", i, floor);
    // printf("%d %d\n",sum, floor);
    for (size_t j = m * k2; j < M; ++j) {
      // printf("\t");
      for (size_t ii = x - k; ii < x + k; ++ii) {
        // printf("%d ", pixels[ii][j]);
        if (pixels[ii][j] > floor)
          pixels[ii][j] = 0;
        else
          pixels[ii][j] = 1;
      }
      // printf("\n");
    }
  }
  for (size_t i = 0; i < m; ++i) {
    int x = i * k2 + k;
    int sum = 0;
    for (size_t j = n * k2; j < N; ++j) {
      for (size_t ii = x - k; ii < x + k; ++ii) {
        sum += pixels[j][ii];
      }
    }
    int floor = (r * sum / k2_2);
    // printf("%d %d\n",sum, floor);
    for (size_t j = n * k2; j < N; ++j) {
      for (size_t ii = x - k; ii < x + k; ++ii) {
        if (pixels[j][ii] > floor)
          pixels[j][ii] = 0;
        else
          pixels[j][ii] = 1;
      }
    }
  }
  int sum = 0;
  for (size_t i = n * k2; i < N; ++i) {
    for (size_t j = m * k2; j < M; ++j) {
      sum += pixels[i][j];
    }
  }
  int floor = (r * sum / ((N - n * k2) * (M - m * k2)));
  for (size_t i = n * k2; i < N; ++i) {
    for (size_t j = m * k2; j < M; ++j) {
      if (pixels[i][j] > floor)
        pixels[i][j] = 0;
      else
        pixels[i][j] = 1;
    }
  }
}

void print_array(short array[N][M], char *s) {
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < M; j++) {
      // printf("%d%s", array[i][j], s);
      // printf("(%d, %d) ", i, j);
      if (array[i][j] == 0) {
        // printf(" ");
        printf("0 ");
      } else {
        printf("1 ");
        // wprintf(L"#");
        // printf("\U0001F600");
      }
    }
    // printf("\n");
  }
}

// void print_dynamic_array(short** array){
//     for (size_t i = 0; i < N; i++){
//         for (size_t j = 0; j < M; j++)
//             printf("%d ", array[i][j]);
//         printf("\n");
//     }
// }

void print_dynamic_array(short array[N - 2][M - 2], size_t n, size_t m,
                         char *s) {
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      printf("%d%s", array[i][j], s);
    }
    printf("\n");
  }
}

void print_minutia(short (*array)[3], size_t n) {
  for (size_t i = 0; i < n; i++) {
    printf("(%d, %d): %d\n", array[3][0], array[i][1], array[i][2]);
  }
}

int algorithm(short pixels[N][M], short minutia_pixels[5000][3]) {
  for (size_t i = 0; i < 2; ++i)
    for (size_t j = 0; j < M; ++j)
      pixels[i][j] = 255;

  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < 2; ++j)
      pixels[i][j] = 255;

  binarization(pixels);
  short skeletonized[N][M], skeletonized2[N][M];
  skeletonization(pixels, skeletonized);
  for (size_t i = 0; i < 7; ++i) {
    skeletonization(skeletonized, skeletonized2);
    skeletonization(skeletonized2, skeletonized);
  }
  size_t count_of_minutia = minutia_search(skeletonized, minutia_pixels);
  return count_of_minutia;
}

// int abs(short a) {
//   if (a > 0)
//     return a;
//   return -a;
// }

// n1 < n2!!!
#define DX 20
#define DY 18
double compare(short minutia1[5000][3], int n1, short minutia2[5000][3],
               int n2) {
  // size_t counter = 0;
  // double distance_correction = 0;
  // size_t j = 0;
  // for (size_t i = 0; i < n1; ++i) {
  //   if (j >= n2)
  //     break;
  //   while ((abs(minutia1[i][0] - minutia2[j][0]) < DY) &&
  //          (abs(minutia1[i][1] - minutia2[j][1]) < DX)) {
  //     if (minutia1[i][2] == minutia2[j][2]) {
  //       ++counter;
  //       distance_correction +=
  //           2 * (double)(DY - minutia1[i][0] + minutia2[j][0]) / DY +
  //           2 * (double)(DX - minutia1[i][1] + minutia2[j][1]) / DX;
  //       ++j;
  //       break;
  //     }
  //     ++j;
  //   }
  // }
  // return (double)(counter * counter + distance_correction) / (n1 * n2);

  size_t max_counter = 0;
  for (int horizontal_shift = -25; horizontal_shift < 26;
       horizontal_shift += 5) {
    for (int verticle_shift = -25; verticle_shift < 26; verticle_shift += 5) {
      size_t counter = 0, j = 0;
      for (size_t i = 0; i < n1; ++i) {
        if (j >= n2)
          break;
        while ((abs(minutia1[i][0] + verticle_shift - minutia2[j][0]) < DY) &&
               (abs(minutia1[i][1] + horizontal_shift - minutia2[j][1]) < DX)) {
          if (minutia1[i][2] == minutia2[j][2]) {
            ++counter;
            // printf("%d %d  %d\n", i, j, counter);
            ++j;
            break;
          }
          ++j;
        }
      }
      printf("%d ", counter);
      if (counter > max_counter)
        max_counter = counter;
    }
  }
  return (double)(max_counter * max_counter) / (n1 * n2);

  // size_t max_counter = 0;
  // for (int angle = -180; angle < 181; angle += 60) {
  //   for (int horizontal_shift = -20; horizontal_shift < 21;
  //        horizontal_shift += 8) {
  //     for (int verticle_shift = -20; verticle_shift < 21; verticle_shift +=
  //     8) {
  //       size_t counter = 0, j = 0;
  //       for (size_t i = 0; i < n1; ++i) {
  //         if (j >= n2)
  //           break;
  //         double x = minutia1[i][0] - M / 2, y = minutia1[i][1] - N / 2;
  //         double ang_rad = PI * angle / 180;
  //         x = x * cos(ang_rad) - y * sin(ang_rad);
  //         y = x * sin(ang_rad) + y * cos(ang_rad);
  //         x += M / 2;
  //         y += N / 2;
  //         while ((abs(x + verticle_shift - minutia2[j][0]) < DY) &&
  //                (abs(y + horizontal_shift - minutia2[j][1]) < DX)) {
  //           if (minutia1[i][2] == minutia2[j][2]) {
  //             ++counter;
  //             // printf("%d %d  %d\n", i, j, counter);
  //             ++j;
  //             break;
  //           }
  //           ++j;
  //         }
  //       }
  //       printf("%d ", counter);
  //       if (counter > max_counter)
  //         max_counter = counter;
  //     }
  //   }
  // }
  // return (double)(max_counter * max_counter) / (n1 * n2);
}
