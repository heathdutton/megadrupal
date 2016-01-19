<?php
/**
 * @file   mccoppin.inc
 * @date   Tue Dec 18 11:03:00 2012
 * @brief  The panels layout plugin for the McCoppin panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('McCoppin'),
  'icon' => 'mccoppin.png',
  'category' => t('Panopoly'),
  'theme' => 'mccoppin',
  'css' => 'mccoppin.css',
  'regions' => array(
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'column3' => t('Third Column'),
  ),
);
