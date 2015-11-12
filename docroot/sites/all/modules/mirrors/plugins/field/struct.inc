<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: struct.
 */
$plugin = array(
  'title' => t('Struct'),
  'category' => 'Field',
  'views' => array(
    'type' => 'date_plain',
    'settings' => array(
      'format_type' => 'long',
      'fromto' => 'both',
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
    ),
  ),
  'feeds' => array(),
);
