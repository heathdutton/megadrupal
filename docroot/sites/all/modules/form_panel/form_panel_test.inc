<?php

/**
 * @file 
 *   Test skeleton for form_panel
 */

function form_panel_test() {
  $form = array();
  $mark = array();

// This section of code can be used to generate a particular test case on-the-fly.
//     $form = array(
//       '#theme' => 'form_panel_table',
//       '#form_panel_table_attributes' => array('border' => 1));
//     $i = ord('A');
//     $form[chr($i)] = array('#markup' => chr($i++), '#weight' => 2001);
//     $form[chr($i)] = array('#markup' => chr($i++), '#weight' => 2003);
//     $form[chr($i)] = array('#markup' => chr($i++), '#weight' => 2005);
//     $form[chr($i)] = array('#markup' => chr($i++), '#weight' => 2009);
//     $form[chr($i)] = array('#markup' => chr($i++), '#weight' => 3009);
//     $form[chr($i)] = array('#markup' => chr($i++), '#weight' => 4009);

// This section of code generates a comprehensive series of tables, using a
// single, increasing integer to decide which table cells have content.
//
// You can set $mark with the indices of any interesting examples, i.e.: ones
// that are failing for some reason. The following example is for a 3 x 3 grid:
//   $mark = array(450, 452, 454, 458, 460, 462, 464, 465);
//   for ($i=0x1C0; $i<0x200; $i++) {
// and this code is for a 4 x 4 grid:
  for ($i = 0xFC00; $i <= 0xFCFF; $i++) {
    $bg = array_search($i, $mark) === FALSE ? 'FFFFFF' : 'FFDDDD';
    $form["x$i"]['#prefix'] = "<table cellpadding=\"5\"><tr valign=\"top\" bgcolor=\"#$bg\"><td>";
    $form["x$i"][] = _test_grid(4, $i, $b);
    $form["x$i"][] = array('#markup' => "</td><td><code>$b</code></td><td><code>" . sprintf("0x%X", $i) . "<br />$i</code></td></tr></table>");
  }

  return $form;
}

/**
 * Generate a $wh x $wh grid, using $i as a bitmap for which cells in the grid
 * to fill with data. Return a text-only version of the grid in $b.
 */
function _test_grid($wh, $i, &$b) {
  $start_row = 1;
  $form = array(
    '#theme' => 'form_panel_table',
    '#form_panel_table_filler' => '-',
    '#form_panel_table_span_rows' => TRUE,
    '#form_panel_table_attributes' => array('border' => 1));
  $a = ord('A');
  $r = $start_row * 1000;
  $c = 1;
  $b = '';
  while ($i && $r < ($wh + $start_row) * 1000) {
    if ($i&1) {
      // By using ($c*2)+1 for the column, we are improving the test, by making
      // the column numbers non-contiguous.
      $w = $r + ($c * 2 + 1);
      $form[chr($a)] = array('#markup' => chr($a), '#weight' => $w);
    }
    $b .= $i&1 ? chr($a) : '-';
    if (++$c > $wh) {
      $c = 1;
      $r += 1000;
      $b .= '<br />';
    }
    $a++;
    $i >>= 1;
  }
  return $form;
}
