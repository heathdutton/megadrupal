<?php
/**
 * @file   bartlett_flipped.inc
 * @date   Tue Dec 18 11:19:00 2012
 * @brief  The panels layout plugin for the Bartlett Flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Bartlett Flipped'),
  'icon' => 'bartlett-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'bartlett_flipped',
  'css' => 'bartlett-flipped.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'contentheader' => t('Content Header'),
    'contentcolumn1' => t('Content Column 1'),
    'contentcolumn2' => t('Content Column 2'),
  ),
);
