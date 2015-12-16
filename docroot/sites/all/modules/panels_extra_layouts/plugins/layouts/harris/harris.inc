<?php
/**
 * @file   harris.inc
 * @date   Tue Dec 18 11:03:00 2012
 * @brief  The panels layout plugin for the Harris panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Harris'),
  'icon' => 'harris.png',
  'category' => t('Panopoly'),
  'theme' => 'harris',
  'css' => 'harris.css',
  'regions' => array(
    'header' => t('Header'),
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'content_main' => t('Content'),
  ),
);
