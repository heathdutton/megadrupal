<?php
/**
 * @file   burr.inc
 * @date   Tue Dec 18 11:19:00 2012
 * @brief  The panels layout plugin for the burr panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Burr'),
  'icon' => 'burr.png',
  'category' => t('Panopoly'),
  'theme' => 'burr',
  'css' => 'burr.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'content_main' => t('Content'),
  ),
);
