<?php
/**
 * @file   taylor_flipped.inc
 * @date   Tue Dec 18 9:15:00 2012
 * @brief  The panels layout plugin for the taylor flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Taylor Flipped'),
  'icon' => 'taylor-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'taylor_flipped',
  'css' => 'taylor-flipped.css',
  'regions' => array(
    'header' => t('Header'),
    'quarter1' => t('Quarter 1 Column'),
    'quarter2' => t('Quarter 2 Column'),
    'half' => t('Half Column'),
    'footer' => t('Footer'),
  ),
);
