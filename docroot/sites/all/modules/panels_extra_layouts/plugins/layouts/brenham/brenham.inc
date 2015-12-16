<?php
/**
 * @file   brenham.inc
 * @date   Tue Dec 18 11:25:00 2012
 * @brief  The panels layout plugin for the Brenham panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Brenham'),
  'icon' => 'brenham.png',
  'category' => t('Panopoly'),
  'theme' => 'brenham',
  'css' => 'brenham.css',
  'regions' => array(
    'header' => t('Header'),
    'sidebar' => t('Content Sidebar'),
    'content_main' => t('Content'),
  ),
);
