<?php
/**
 * @file   hewston_flipped.inc
 * @date   Tue Dec 18 11:03:00 2012
 * @brief  The panels layout plugin for the Hewston Flipped panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Hewston Flipped'),
  'icon' => 'hewston-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'hewston_flipped',
  'css' => 'hewston-flipped.css',
  'regions' => array(
    'slider' => t('Slider'),
    'slidergutter' => t('Slider Gutter'),
    'column1' => t('First Column'),
    'column2' => t('Second Column'),
    'column3' => t('Third Column'),
  ),
);
