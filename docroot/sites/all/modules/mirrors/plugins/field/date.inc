<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: date.
 */
$plugin = array(
  'title' => t('Date'),
  'category' => 'Field',
  'views' => array(
    'date_format' => 'custom',
    'custom_date_format' => 'U',
  ),
  'feeds' => array(),
);
