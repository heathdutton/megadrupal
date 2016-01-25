<?php
/**
 * @file   hewston.inc
 * @date   Tue Dec 18 11:03:00 2012
 * @brief  The panels layout plugin for the Hewston panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Hewston'),
  'icon' => 'hewston.png',
  'category' => t('Panopoly'),
  'theme' => 'hewston',
  'css' => 'hewston.css',
  'regions' => array(
    'slider' => t('Slider'),
    'slidergutter' => t('Slider Gutter'),
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'column3' => t('Third Column'),
  ),
);
