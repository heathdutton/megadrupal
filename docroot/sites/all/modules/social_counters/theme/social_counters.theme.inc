<?php
/**
 * @file
 * Theme, preprocess and process functions.
 */

/**
 * Template preprocess for theme_social_counters_block().
 */
function template_preprocess_social_counters_block(&$vars) {
  $items = &$vars['items'];
  $delimiter = variable_get('social_counters_delimiter', ': ');

  foreach ($items as $sn => &$item) {
    $item['classes'][] = 'item';
    $item['classes'][] = drupal_html_class($sn);
    $item['delimiter'] = social_counters_format_delimiter($delimiter);
  }
}

/**
 * Template process for theme_social_counters_block().
 */
function template_process_social_counters_block(&$vars) {
  $items = &$vars['items'];

  foreach ($items as $sn => &$item) {
    $item['classes'] = implode(' ', $item['classes']);
  }
}
