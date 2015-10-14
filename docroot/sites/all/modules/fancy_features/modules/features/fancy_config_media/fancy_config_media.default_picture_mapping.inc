<?php
/**
 * @file
 * fancy_config_media.default_picture_mapping.inc
 */

/**
 * Implements hook_default_picture_mapping().
 */
function fancy_config_media_default_picture_mapping() {
  $export = array();

  $picture_mapping = new PictureMapping();
  $picture_mapping->disabled = FALSE; /* Edit this to true to make a default picture_mapping disabled initially */
  $picture_mapping->api_version = 2;
  $picture_mapping->label = 'Fancy Picture';
  $picture_mapping->machine_name = 'fancy_picture';
  $picture_mapping->breakpoint_group = 'fancy_picture';
  $picture_mapping->mapping = array(
    'custom.user.mobile' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fancy_mobile',
      ),
    ),
    'custom.user.tablet' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fancy_tablet',
      ),
    ),
    'custom.user.desktop' => array(
      '1x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fancy_desktop',
      ),
      '2x' => array(
        'mapping_type' => 'image_style',
        'image_style' => 'fancy_desktop_hidpi',
      ),
    ),
  );
  $export['fancy_picture'] = $picture_mapping;

  return $export;
}
