<?php

$plugin = array(
  'title' => t('Content: two columns'),
  'theme' => 'ns_core_content_column_two',
  'icon' => 'ns-core-content-column-two.png',
  'settings form' => 'ns_core_content_column_two_settings',
  'category' => 'NodeStream',
  'regions' => array(
    'left' => t('Left'),
    'right' => t('Right'),
  ),
);

/**
 * Implementation of theme_preprocess_precision_column_two().
 */
function ns_core_preprocess_ns_core_content_column_two(&$vars) {
  ns_core_check_layout_variables($vars);
  $vars['wrapper'] = !empty($vars['settings']['float']) && $vars['settings']['float'] == 'float';
}

/**
 * Settings form for content column two.
 */
function ns_core_content_column_two_settings($display, $plugin, $settings) {
  $form['float'] = array(
    '#type' => 'radios',
    '#title' => t('Float options'),
    '#options' => array(
      'wrap' => t('Let content in the left column wrap the content in the left column.'),
      'float' => t('Make both regions take up as much height'),
    ),
    '#default_value' => isset($settings['float']) ? $settings['float'] : 'wrap',
  );
  return $form;
}
