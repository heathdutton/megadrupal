<?php
/**
 * @file
 * Panels theme - Hyperion
 */

/**
 * Implements hook_panels_layouts().
 */
// Plugin definition
$plugin = array(
  'title' => t('prometheus'),
  'category' => t('Bootstrap (Static)'),
  'icon' => 'prometheus.png',
  'theme' => 'prometheus',
  'admin css' => '../admin.css',
  'regions' => array(
    'layer_1' => t('Layer 1'),

    'layer_2_left' => t('Layer 2 Left'),
    'layer_2_right' => t('Layer 2 Right'),

      'layer_3_left' => t('Layer 3 Left'),
      'layer_3_right' => t('Layer 3 Right'),

      'layer_4_left' => t('Layer 4 Left'),
      'layer_4_center' => t('Layer 4 Center'),
      'layer_4_right' => t('Layer 4 Right'),


      'layer_5_left' => t('Layer 5 Left'),
      'layer_5_right' => t('Layer 5 Right'),

      'layer_6' => t('Layer 6'),

      'layer_7_left' => t('Layer 7 Left'),
      'layer_7_right' => t('Layer 7 Right'),


      'layer_8_left' => t('Layer 8 Left'),
      'layer_8_right' => t('Layer 8 Right'),

      'layer_9_left' => t('Layer 9 Left'),
      'layer_9_center' => t('Layer 9 Center'),
      'layer_9_right' => t('Layer 9 Right'),


      'layer_10_left' => t('Layer 10 Left'),
      'layer_10_right' => t('Layer 10 Right'),

      'layer_11' => t('Layer 11'),

  ),
);
