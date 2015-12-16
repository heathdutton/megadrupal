<?php
/**
 * @file   bartlett.inc
 * @date   Tue Dec 18 11:19:00 2012
 * @brief  The panels layout plugin for the Bartlett panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Bartlett'),
  'icon' => 'bartlett.png',
  'category' => t('Panopoly'),
  'theme' => 'bartlett',
  'css' => 'bartlett.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'contentheader' => t('Content Header'),
    'contentcolumn1' => t('Content Column 1'),
    'contentcolumn2' => t('Content Column 2'),
  ),
);
