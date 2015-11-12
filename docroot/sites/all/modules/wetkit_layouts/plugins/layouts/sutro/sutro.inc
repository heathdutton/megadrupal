<?php
/**
 * @file
 * sutro.inc
 */


// Plugin definition.
$plugin = array(
  'title' => t('Sutro'),
  'icon' => 'sutro.png',
  'category' => t('Layouts BackEnd'),
  'theme' => 'sutro',
  'css' => 'sutro.css',
  'regions' => array(
    'header' => t('Header'),
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'footer' => t('Footer'),
  ),
);
