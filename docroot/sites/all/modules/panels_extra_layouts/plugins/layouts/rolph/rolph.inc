<?php
/**
 * @file   rolph.inc
 * @date   Tue Dec 18 9:41:00 2012
 * @brief  The panels layout plugin for the Rolph panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Rolph'),
  'icon' => 'rolph.png',
  'category' => t('Panopoly'),
  'theme' => 'rolph',
  'css' => 'rolph.css',
  'regions' => array(
    'header' => t('Header'),
    'quarter1' => t('Quarter 1 Column'),
    'quarter2' => t('Quarter 2 Column'),
    'quarter3' => t('Quarter 1 Column'),
    'quarter4' => t('Quarter 2 Column'),
    'footer' => t('Footer'),
  ),
);
