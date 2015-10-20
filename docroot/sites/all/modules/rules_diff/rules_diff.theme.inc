<?php

/**
 * @file
 * Theme implementation file.
 */

/**
 * Theme function to print all the differences for a given property.
 *
 * @param array $variables
 *   Must contain the keys:
 *     - 'differences': A numeric array containing the differences as expected
 *       by theme_rules_diff_diff.
 *     - 'name': The name of the property.
 *     - 'sanitized': A boolen indicating if the output may not contain HTML.
 */
function theme_rules_diff_property_differences($variables) {
  if (count($variables['differences']) == 0) {
    return t('No differences found.');
  }
  // Apply the theming function for each difference.
  $items = array();
  foreach ($variables['differences'] as $difference) {
    $items[] = theme('rules_diff_diff', $difference + array('sanitized' => $variables['sanitized']));
  }

  $args = array('items' => $items);
  if (!empty($variables['name'])) {
    $args['title'] = t('Differences for property "@name"', array('@name' => $variables['name']));
  }
  if ($variables['sanitized']) {
    // HTML free list.
    $output = $args['title'] . ":\n";
    foreach ($items as $item) {
      $output .= "\t* " . $item . "\n";
    }
    return $output;
  }
  else {
    // Show it as a list.
    return theme('item_list', $args);
  }
}

/**
 * Theme function to print a difference.
 */
function theme_rules_diff_diff($variables) {
  $line_str = '';
  $output = '';
  if (!is_null($variables['line'])) {
    $line_str = t('Line number: @line', array('@line' => $variables['line']));
  }
  if ($variables['sanitized']) {
    $output = $line_str == '' ? check_plain($variables['difference']) : $line_str . ' => ' . check_plain($variables['difference']);
    $type = $variables['type'] == DIFF_DEL ? '(-)' : '(+)';
    return $type . ' ' . $output;
  }
  else {
    $output = ($line_str == '') ? check_plain($variables['difference']) : '<span class="line-num">' . $line_str . '</span> ' . check_plain($variables['difference']);
    return '<span class="difference diff-type-' . $variables['type'] . '">' . $output . '</span>';
  }
}