<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: field_item_image.
 */
$plugin = array(
  'title' => t('Field Item Image'),
  'category' => 'Field',
  'views' => array(
    'type' => 'image_url',
    'settings' => array(
      'url_type' => '0',
      'image_style' => '',
      'image_link' => '',
    ),
  ),
  'feeds' => array(),
);
