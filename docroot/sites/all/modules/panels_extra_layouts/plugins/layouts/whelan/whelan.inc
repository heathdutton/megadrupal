<?php
/**
 * @file   whelan.inc
 * @date   Tue Dec 18 9:15:00 2012
 * @brief  The panels layout plugin for a three column responsive panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Whelan'),
  'icon' => 'whelan.png',
  'category' => t('Panopoly'),
  'theme' => 'whelan',
  'css' => 'whelan.css',
  'regions' => array(
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'content_main' => t('Content'),
  ),
);
