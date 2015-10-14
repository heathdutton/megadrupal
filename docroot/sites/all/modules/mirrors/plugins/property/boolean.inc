<?php

/**
 * @file
 * Mirrors property plugin declaration.
 */

/**
 * Property Declaration: boolean.
 */
$plugin = array(
  'title' => t('Boolean'),
  'category' => 'Property',
  'views' => array(
    'type' => 'boolean',
    'settings' => array(
      'format' => 'boolean',
      'custom_on' => '',
      'custom_off' => '',
      'reverse' => 1,
    ),
  ),
  'feeds' => array(),
);
