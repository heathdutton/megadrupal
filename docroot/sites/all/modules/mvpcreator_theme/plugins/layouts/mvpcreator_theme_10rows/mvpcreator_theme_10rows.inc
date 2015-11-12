<?php
// Plugin definition
$plugin = array(
  'title' => t('Ten rows'),
  'icon' => 'mvpcreator-theme-10rows.png',
  'category' => t('MVPCreator'),
  'theme' => 'mvpcreator_theme_10rows',
  'regions' => array(
    'row01' => t('Row 1'),
    'row02' => t('Row 2'),
    'row03' => t('Row 3'),
    'row04' => t('Row 4'),
    'row05' => t('Row 5'),
    'row06' => t('Row 6'),
    'row07' => t('Row 7'),
    'row08' => t('Row 8'),
    'row09' => t('Row 9'),
    'row10' => t('Row 10'),
  ),

  // Signals to mvpcreator_theme that we can work as in 'full width' mode
  // if instructed.
  'full width' => TRUE,
  'full width regions' => array(
    'row01',
    'row02',
    'row03',
    'row04',
    'row05',
    'row06',
    'row07',
    'row08',
    'row09',
    'row10',
  ),
);
