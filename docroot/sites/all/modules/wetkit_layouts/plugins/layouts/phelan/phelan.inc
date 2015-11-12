<?php
/**
 * @file
 * phelan.inc
 */

// Plugin definition.
$plugin = array(
  'title' => t('Phelan'),
  'icon' => 'phelan.png',
  'category' => t('Layouts BackEnd'),
  'theme' => 'phelan',
  'css' => 'phelan.css',
  'regions' => array(
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
  ),
);
