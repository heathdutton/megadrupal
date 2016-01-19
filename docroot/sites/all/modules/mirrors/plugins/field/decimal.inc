<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: decimal.
 */
$plugin = array(
  'title' => t('Decimal'),
  'category' => 'Field',
  'views' => array(
    'settings' => array(
      'thousand_separator' => '',
      'decimal_separator' => '.',
      'scale' => '10',
      'prefix_suffix' => 1,
    ),
  ),
  'feeds' => array(),
);
