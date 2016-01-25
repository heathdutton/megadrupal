<?php
/**
 * @file   selby_flipped.inc
 * @date   Tue Dec 18 9:41:00 2012
 * @brief  The panels layout plugin for the selby flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Selby Flipped'),
  'icon' => 'selby-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'selby_flipped',
  'css' => 'selby-flipped.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'contentheader' => t('Content Header'),
    'contentcolumn1' => t('Content Column 1'),
    'contentcolumn2' => t('Content Column 2'),
    'contentfooter' => t('Content Footer'),
  ),
);
