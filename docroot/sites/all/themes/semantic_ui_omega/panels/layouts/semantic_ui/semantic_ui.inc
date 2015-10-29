<?php

/**
 * @file
 * Defines an asymetric, semantic ratio based panels layout.
 */

// Plugin definition.
$plugin = array(
  'title' => t('Semantic UI'),
  'icon' => 'preview.png',
  'category' => t('Semantic UI'),
  'theme' => 'semantic_ui',
  'css' => '../../../css/layouts/semantic_ui/semantic-ui.layout.css',
  'regions' => array(
    'first' => t('First'),
    'second' => t('Second'),
    'third' => t('third'),
  ),
);

/**
 * Implements hook_preprocess_semantic_ui().
 */
function template_preprocess_semantic_ui(&$variables) {
  $variables['attributes_array']['class'][] = 'panel-semantic-ui';
  $variables['attributes_array']['class'][] = 'panel-display--semantic-ui';

  foreach($variables['content'] as $name => $item) {
    $variables['region_attributes_array'][$name]['class'][] = 'semantic-ui-region';
    $variables['region_attributes_array'][$name]['class'][] = 'semantic-ui-region--' . drupal_clean_css_identifier($name);
  }
}
