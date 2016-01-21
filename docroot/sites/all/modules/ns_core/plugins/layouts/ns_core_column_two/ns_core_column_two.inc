<?php

$plugin = array(
  'title' => t('Two columns'),
  'theme' => 'ns_core_column_two',
  'icon' => 'ns-core-column-two.png',
  'category' => 'NodeStream',
  'regions' => array(
    'header_alpha' => t('Header alpha'),
    'main' => t('Main'),
    'aside_beta' => t('Aside beta'),
    'footer_alpha' => t('Footer alpha'),
  ),
);

/**
 * Implementation of theme_preprocess_ns_core_column_two().
 */
function ns_core_preprocess_ns_core_column_two(&$vars) {
  ns_core_check_layout_variables($vars);
}
