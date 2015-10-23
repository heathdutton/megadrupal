<?php
/**
 * @file   burr_flipped.inc
 * @date   Tue Dec 18 11:19:00 2012
 * @brief  The panels layout plugin for the burr flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Burr Flipped'),
  'icon' => 'burr-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'burr_flipped',
  'css' => 'burr-flipped.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'content_main' => t('Content'),
  ),
);
