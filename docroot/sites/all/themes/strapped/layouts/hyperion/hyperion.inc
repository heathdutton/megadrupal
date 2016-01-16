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
  'title' => t('Hyperion'),
  'category' => t('Bootstrap (Static)'),
  'icon' => 'hyperion.png',
  'theme' => 'hyperion',
  'admin css' => '../admin.css',
  'regions' => array(
    'layer_1' => t('Layer 1'),
    'layer_2_left' => t('Layer 2 Left'),
    'layer_2_right' => t('Layer 2 Right'),
    'layer_3' => t('Layer 3'),
    'layer_4_left' => t('Layer 4 Left'),
    'layer_4_right' => t('Layer 4 Right'),
    'layer_5_left' => t('Layer 5 Left'),
    'layer_5_right' => t('Layer 5 Right'),
    'layer_6_left' => t('Layer 6 Left'),
    'layer_6_center' => t('Layer 6 Center'),
    'layer_6_right' => t('Layer 6 Right'),
  ),
);
