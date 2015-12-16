<?php
/**
 * @file   bryant.inc
 * @date   Tue Dec 18 11:25:00 2012
 * @brief  The panels layout plugin for the Bryant layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Bryant'),
  'icon' => 'bryant.png',
  'category' => t('Panopoly'),
  'theme' => 'bryant',
  'css' => 'bryant.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'content_main' => t('Content'),
  ),
);
