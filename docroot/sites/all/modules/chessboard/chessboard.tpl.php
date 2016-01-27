<?php

/**
 * @file
 * Displays a chessboard.
 *
 * Available variables:
 * - $table_xhtml: A translation table for empty and marked squares, pieces and borders.
 * - $file_max: Number of files: any integer (default: 8).
 * - square_color_first: Color of the upper left square: 0 for light (default), or 1 for dark.
 * - $border: Valid flags are T, B, L, and R, which correspond to the top
 *   border, the bottom border, the left border, and the right border
 *   respectively. The corresponding borders of the chessboard will be rendered
 *   for the flags specified. (Default: none is specified.)
 * - $board: An array of board rows. Each row contains an array with codes for
 *   empty squares, marked squares, and pieces.
 *
 * @see template_preprocess_chessboard()
 *
 * @ingroup themeable
 */

// Top border
if ($border['T']) {
  print '<div class="border-top">';
  if ($border['L']) {
    print $table_xhtml['TL'];
  }
  print str_repeat($table_xhtml['T'], $file_max);
  if ($border['R']) {
    print $table_xhtml['TR'];
  }
  $file = $file_max;
}
else {
  print '<div class="row">';
  $file = 0;
  $square_color = $square_color_first;
  $square_color_first = 1 - $square_color_first;
}

// Left border, board, right border
foreach ($board as $board_row) {
  for ($i=0; isset($board_row[$i]); $i++) {

    if ($file >= $file_max) {
      print '</div><div class="row">';
      $file = 0;
      $square_color = $square_color_first;
      $square_color_first = 1 - $square_color_first;
      if ($border['L'])
        print $table_xhtml['L'];
    }

    $key = $board_row[$i] . $square_color;
      // $table_xhtml[$key] is defined as we have filtered unknown characters
      print $table_xhtml[$key];
      $square_color = 1 - $square_color;
      $file++;

    if ($file >= $file_max) {
      if ($border['R']) {
        print $table_xhtml['R'];
      }
    }
  }
}

print '</div>';

// Bottom border
if ($border['B']) {
  print '<div class="border-bottom">';
  if ($border['L']) {
    print $table_xhtml['BL'];
  }
  print str_repeat($table_xhtml['B'], $file_max);
  if ($border['R']) {
    print $table_xhtml['BR'];
  }
  print '</div>';
}
