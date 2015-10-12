<?php
/**
 * @file   boxton.inc
 * @date   Tue Dec 18 11:24:00 2012
 * @brief  The panels layout plugin for the Boxton panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Boxton'),
  'icon' => 'boxton.png',
  'category' => t('Panopoly'),
  'theme' => 'boxton',
  'regions' => array(
    'content_main' => t('Content'),
  ),
);
