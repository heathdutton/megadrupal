<?php
/**
 * @file   webb.inc
 * @date   Tue Dec 18 9:15:00 2012
 * @brief  The panels layout plugin for the webb panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Webb'),
  'icon' => 'webb.png',
  'category' => t('Panopoly'),
  'theme' => 'webb',
  'css' => 'webb.css',
  'regions' => array(
    'header' => t('Header'),
    'sidebar' => t('Sidebar'),
    'contentheader' => t('Content Header'),
    'contentcolumn1' => t('Content Column 1'),
    'contentcolumn2' => t('Content Column 2'),
  ),
);
