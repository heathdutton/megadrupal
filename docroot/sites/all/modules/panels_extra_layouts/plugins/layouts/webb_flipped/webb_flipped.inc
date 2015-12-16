<?php
/**
 * @file   webb_flipped.inc
 * @date   Tue Dec 18 9:15:00 2012
 * @brief  The panels layout plugin for the webb_flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Webb Flipped'),
  'icon' => 'webb-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'webb_flipped',
  'css' => 'webb-flipped.css',
  'regions' => array(
    'header' => t('Header'),
    'sidebar' => t('Sidebar'),
    'contentheader' => t('Content Header'),
    'contentcolumn1' => t('Content Column 1'),
    'contentcolumn2' => t('Content Column 2'),
  ),
);
