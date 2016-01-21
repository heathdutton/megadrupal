<?php

$plugin = array(
  'title' => t('One column'),
  'theme' => 'ns_core_column_one',
  'icon' => 'ns-core-column-one.png',
  'category' => 'NodeStream',
  'regions' => array(
    'main' => t('Main'),
  ),
);

/**
 * Implementation of theme_preprocess_ns_core_column_one().
 */
function ns_core_preprocess_ns_core_column_one(&$vars) {
  ns_core_check_layout_variables($vars);
}
