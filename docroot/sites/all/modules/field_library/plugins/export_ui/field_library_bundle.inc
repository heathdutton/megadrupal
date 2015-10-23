<?php

/**
 * @file
 * Contains ctools_export_ui plugin for field library bundles.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  // As defined in hook_schema().
  'schema' => 'field_library_bundle',
  'handler' => array(
    'class' => 'FieldFieldLibraryBundleUI',
  ),
  // Define a permission users must have to access these pages.
  'access' => 'administer field_library',
  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/structure/field-library',
    'menu item' => 'field_library_bundle',
    'menu title' => 'Field library bundles',
    'menu description' => 'Field library overview.',
  ),
  // Define user interface texts.
  'title singular' => t('Bundle'),
  'title plural' => t('Bundles'),
  'title singular proper' => t('Field library bundle'),
  'title plural proper' => t('Field library bundles'),
);
