<?php
/**
 * @file   sanderson.inc
 * @date   Tue Dec 18 9:41:00 2012
 * @brief  The panels layout plugin for the sanderson panels layout.
 *
 *
 */

// Plugin definition
$plugin = array(
  'title' => t('Sanderson Flipped'),
  'icon' => 'sanderson-flipped.png',
  'category' => t('Panopoly'),
  'theme' => 'sanderson_flipped',
  'css' => 'sanderson-flipped.css',
  'regions' => array(
    'column1' => t('Row 1: First Column'),
    'column2' => t('Row 1: Second Column'),
    'column3' => t('Row 1: Third Column'),
    'secondarycolumn1' => t('Row 2: First Column'),
    'secondarycolumn2' => t('Row 2: Second Column'),
  ),
);

$plugin = array(
  'title' => t('Sanderson'),
  'icon' => 'sanderson.png',
  'category' => t('Panopoly'),
  'theme' => 'sanderson',
  'css' => 'sanderson.css',
  'regions' => array(
    'column1' => t('Row 1: First Column'),
    'column2' => t('Row 1: Second Column'),
    'secondarycolumn1' => t('Row 2: First Column'),
    'secondarycolumn2' => t('Row 2: Second Column'),
    'secondarycolumn3' => t('Row 2: Third Column'),
  ),
);
