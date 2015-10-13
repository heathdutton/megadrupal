<?php
/**
 * @file   brenham_flipped.inc
 * @date   Tue Dec 18 11:25:00 2012
 * @brief  The panels layout plugin for the Brenham Flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Brenham Flipped'),
  'icon' => 'brenham-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'brenham_flipped',
  'css' => 'brenham-flipped.css',
  'regions' => array(
    'header' => t('Header'),
    'sidebar' => t('Content Sidebar'),
    'content_main' => t('Content'),
  ),
);
