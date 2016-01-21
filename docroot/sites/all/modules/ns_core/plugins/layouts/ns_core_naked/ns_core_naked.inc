<?php

$plugin = array(
  'title' => t('Naked'),
  'theme' => 'ns_core_naked',
  'icon' => 'ns-core-naked.png',
  'category' => 'NodeStream',
  'regions' => array(
    'main' => t('Main'),
  ),
);

/**
 * Implementation of theme_preprocess_ns_core_naked().
 */
function ns_core_preprocess_ns_core_naked(&$vars) {
  ns_core_check_layout_variables($vars);
}
