<?php
/**
 * @file   sutro.inc
 * @date   Tue Dec 18 9:39:00 2012
 * @brief  The panels layout plugin for the sutro panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Sutro'),
  'icon' => 'sutro.png',
  'category' => t('Panopoly'),
  'theme' => 'sutro',
  'css' => 'sutro.css',
  'regions' => array(
    'header' => t('Header'),
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'footer' => t('Footer'),
  ),
);
