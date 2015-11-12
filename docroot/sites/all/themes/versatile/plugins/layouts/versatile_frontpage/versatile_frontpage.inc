<?php
/**
 * @file
 * Plugin definition for Versatile frontpage layout.
 */

$plugin = array(
  'title' => t('Versatile frontpage'),
  'theme' => 'versatile_frontpage',
  'icon' => 'versatile-frontpage.png',
  'admin css' => 'versatile_frontpage.css',
  'category' => 'Versatile',
  'regions' => array(
    'main_alpha' => t('Main alpha'),
    'column_first_alpha' => t('Alpha column one'),
    'column_second_alpha' => t('Alpha column two'),
    'main_beta' => t('Main beta'),
    'column_first_beta' => t('Column one beta'),
    'column_second_beta' => t('Column two beta'),
    'column_third_beta' => t('Column three beta'),
    'main_gamma' => t('Main gamma'),
  ),
);