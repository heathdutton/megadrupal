<?php

$plugin = array(
  'title' => t('Content: one column'),
  'theme' => 'ns_core_content_column_one',
  'icon' => 'ns-core-content-column-one.png',
  'category' => 'NS Theme',
  'regions' => array(
    'main' => t('Main'),
  ),
);

/**
 * Implementation of theme_preprocess_precision_column_one().
 */
function ns_core_preprocess_ns_core_content_column_one(&$vars) {
  ns_core_check_layout_variables($vars);
}
