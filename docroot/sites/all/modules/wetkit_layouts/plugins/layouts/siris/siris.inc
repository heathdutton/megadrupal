<?php
/**
 * @file
 * siris.inc
 */


// Plugin definition.
$plugin = array(
  'title' => t('Siris'),
  'icon' => 'siris.png',
  'category' => t('Layouts BackEnd'),
  'theme' => 'siris',
  'css' => 'siris.css',
  'regions' => array(
    'header' => t('Header'),
    'banner' => t('Banner'),
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'footer' => t('Footer'),
  ),
);
