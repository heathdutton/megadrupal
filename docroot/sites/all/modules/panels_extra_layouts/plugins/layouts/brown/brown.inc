<?php
/**
 * @file   brown.inc
 * @date   Tue Dec 18 11:19:00 2012
 * @brief  The panels layout plugin for the Brown panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Brown'),
  'icon' => 'brown.png',
  'category' => t('Panopoly'),
  'theme' => 'brown',
  'css' => 'brown.css',
  'regions' => array(
    'slider' => t('Slider'),
    'slidergutter' => t('Slider Gutter'),
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'column3' => t('Third Column'),
    'footercolumn1' => t('Footer First Column'),
    'footercolumn2' => t('Footer Second Column'),
    'footercolumn3' => t('Footer Third Column'),
  ),
);
