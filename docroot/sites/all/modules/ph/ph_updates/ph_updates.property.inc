<?php
/**
 * @file
 * Entity property callback implementations.
 *
 * This file is loaded with the main module file.
 */

/**
 * Entity property info alter callback for a regular file in the list of files
 * included in a release.
 */
function ph_updates_release_file_property_info_alter(EntityMetadataWrapper $wrapper, $property_info) {
  $property_info['properties'] = ph_updates_release_file_add_property_md5hash($property_info['properties']);
  $property_info['properties'] = ph_updates_release_file_add_property_archive_type($property_info['properties']);
  return $property_info;
}

/**
 * Entity property info alter callback for release default file.
 */
function ph_updates_release_default_file_property_info_alter(EntityMetadataWrapper $wrapper, $property_info) {
  $property_info['properties'] = ph_updates_release_file_add_property_md5hash($property_info['properties']);
  return $property_info;
}

/**
 * Adds a 'md5hash' property to a file.
 */
function ph_updates_release_file_add_property_md5hash($properties) {
  $properties['md5hash'] = array(
    'type' => 'text',
    'label' => t('MD5 hash'),
    'getter callback' => 'ph_package_archive_get_hash',
  );
  return $properties;
}

/**
 * Adds an 'archive_extension' property to a file.
 */
function ph_updates_release_file_add_property_archive_type($properties) {
  $properties['archive_extension'] = array(
    'type' => 'text',
    'label' => t('Archive type'),
    'getter callback' => 'ph_package_archive_get_extension',
  );
  return $properties;
}
