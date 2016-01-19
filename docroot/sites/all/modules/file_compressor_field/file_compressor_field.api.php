<?php

/**
 * @file
 * Documentation for hooks defined by File Compressor field.
 */

/**
 * Registers new compression providers.
 *
 * @return array
 *   An array whose keys are provider type names and whose values are arrays
 *   describing its structure:
 *   - name: The human name for the provider.
 *   - class: The name of the class implementing the provider
 *   - path: The path where the class file is placed.
 *
 * @see hook_file_compressor_field_provider_info_alter()
 */
function hook_file_compressor_field_provider_info() {
  $providers = array(
    'zip_built_in' => array(
      'name' => t('Zip (Built in)'),
      'class' => 'CompressProviderZip',
      'path' => drupal_get_path('module', 'file_compressor_field') . '/includes',
    ),
    'gzip_zlib' => array(
      'name' => t('GZip (Zlib)'),
      'class' => 'CompressProviderGZip',
      'path' => drupal_get_path('module', 'file_compressor_field') . '/includes',
    ),
  );

  return $providers;
}

/**
 * Perform alterations on the list of available compression providers.
 *
 * @param array $info
 *   Array of information on compression providers exposed by
 *   hook_file_compressor_field_provider_info() implementations.
 *
 * @see hook_file_compressor_field_provider_info()
 */
function hook_file_compressor_field_provider_info_alter(&$info) {
  if (isset($info['gzip_zlib'])) {
    $info['gzip_zlib']['name'] = t('Other name for GZip');
  }
}

/**
 * Defines field types available for File Compressor field.
 *
 * @return array
 *   An array whose keys are file field type names and whose values are arrays
 *   describing the field type structure necessary for File Compressor field:
 *   - column: The column name for the field where stores the reference to the
 *     stored file.
 */
function hook_file_compressor_field_type_info() {
  return array(
    'image' => array(
      'column' => 'fid',
    ),
    'file' => array(
      'column' => 'fid',
    ),
  );
}
