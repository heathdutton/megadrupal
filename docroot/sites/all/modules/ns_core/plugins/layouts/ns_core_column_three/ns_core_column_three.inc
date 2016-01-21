<?php

$plugin = array(
  'title' => t('Three columns'),
  'theme' => 'ns_core_column_three',
  'icon' => 'ns-core-column-three.png',
  'category' => 'NodeStream',
  'regions' => array(
    'header_alpha' => t('Header alpha'),
    'header_beta' => t('Header beta'),
    'main' => t('Main'),
    'aside_alpha' => t('Aside alpha'),
    'aside_beta' => t('Aside beta'),
    'footer_alpha' => t('Footer alpha'),
    'footer_beta' => t('Footer beta'),
  ),
);

/**
 * Implementation of theme_preprocess_ns_core_column_three().
 */
function ns_core_preprocess_ns_core_column_three(&$vars) {
  ns_core_check_layout_variables($vars);
}
