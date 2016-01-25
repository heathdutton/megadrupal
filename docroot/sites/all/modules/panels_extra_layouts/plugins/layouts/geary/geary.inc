<?php
/**
 * @file   geary.inc
 * @date   Tue Dec 18 11:03:00 2012
 * @brief  The panels layout plugin for the Geary panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Geary'),
  'icon' => 'geary.png',
  'category' => t('Panopoly'),
  'theme' => 'geary',
  'css' => 'geary.css',
  'regions' => array(
    'header' => t('Header'),
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'column3' => t('Third Column'),
  ),
);
