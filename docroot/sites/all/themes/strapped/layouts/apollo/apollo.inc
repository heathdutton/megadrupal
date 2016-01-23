<?php
/**
 * @file
 * Panels theme - Apollo
 */

// Plugin definition.
$plugin = array(
  'title' => t('Apollo'),
  'category' => t('Bootstrap (Adaptive)'),
  'icon' => 'apollo.png',
  'theme' => 'apollo',
  'admin css' => '../admin.css',
  'regions' => array(
    '11' => t('Header left'),
    '12' => t('Header center left'),
    '13' => t('Header center right'),
    '14' => t('Header right'),
    '21' => t('Top left'),
    '22' => t('Top center left'),
    '23' => t('Top center right'),
    '24' => t('Top right'),
    '31' => t('Middle top left'),
    '32' => t('Middle top center left'),
    '33' => t('Middle top center right'),
    '34' => t('Middle top right'),
    '41' => t('Middle below top left'),
    '42' => t('Middle below top center left'),
    '43' => t('Middle below top center right'),
    '44' => t('Middle below top right'),
    '51' => t('Middle above bottom left'),
    '52' => t('Middle above bottom center'),
    '53' => t('Middle above bottom right'),
    '61' => t('Bottom left'),
    '62' => t('Bottom center'),
    '63' => t('Bottom right'),
    '71' => t('Footer left'),
    '72' => t('Footer center'),
    '73' => t('Footer right'),
  ),
);
