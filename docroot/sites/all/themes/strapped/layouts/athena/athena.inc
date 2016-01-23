<?php

/**
 * Implements hook_panels_layouts().
 */
// Plugin definition
$plugin = array(
  'title' => t('Athena'),
  'category' => t('Bootstrap (Static)'),
  'icon' => 'athena.png',
  'theme' => 'athena',
  'admin css' => '../admin.css',
  'regions' => array(
    'layer_1_left' => t('Layer 1 Left'),
    'layer_1_right' => t('Layer 1 Right'),
  ),
);
