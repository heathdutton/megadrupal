<?php
/**
 * @file   bryant_flipped_flipped.inc
 * @date   Tue Dec 18 11:19:00 2012
 * @brief  The panels layout plugin for the Bryant Flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Bryant Flipped'),
  'icon' => 'bryant-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'bryant_flipped',
  'css' => 'bryant-flipped.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'content_main' => t('Content'),
  ),
);
