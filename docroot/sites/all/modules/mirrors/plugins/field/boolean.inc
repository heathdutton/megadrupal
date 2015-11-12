<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: boolean.
 */
$plugin = array(
  'title' => t('Boolean'),
  'category' => 'Field',
  'views' => array(
    'type' => 'boolean_yes_no',
    'settings' => array(
      'format' => 'boolean',
      'custom_on' => '',
      'custom_off' => '',
      'reverse' => 0,
    ),
  ),
  'feeds' => array(),
);
