<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: video_embed_field.
 */
$plugin = array(
  'title' => t('Video Field Embed'),
  'category' => 'Field',
  'views' => array(
    'type' => 'video_embed_field_url',
    'settings' => array(
      'video_style' => 'normal',
    ),
  ),
  'feeds' => array(),
);
