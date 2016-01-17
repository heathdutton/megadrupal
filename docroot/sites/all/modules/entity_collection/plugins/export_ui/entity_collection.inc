<?php
/**
 * @file
 * Plugin definition for the ctools bundle export ui.
 */

/**
 * Plugin definition.
 */
$plugin = array(
  'schema' => 'entity_collection_bundles',
  'access' => 'administer entity_collection types',
  'create access' => 'create entity_collections',
  'menu' => array(
    'menu item' => 'entity_collection_bundle',
    'menu title' => 'Entity Collection types',
    'menu description' => 'Administer entity_collection types',
  ),
  'title singular' => t('Entity collection type'),
  'title plural' => t('Entity collection types'),
  'title singular proper' => t('Entity Collection type'),
  'title plural proper' => t('Entity Collection types'),
  'export' => array(
    'admin_title' => 'label',
  ),
  'form' => array(
     'settings' => 'entity_collection_export_ui_form',
     'submit' => 'entity_collection_export_ui_form_submit'
   ),
  'handler' => array(
    'class' => 'entity_collection_ui',
    'parent' => 'ctools_export_ui',
  ),
);

/**
 * Form for adding/editing an entity collection bundle.
 */
function entity_collection_export_ui_form(&$form, &$form_state) {
  ctools_include('export');
  $bundle = $form_state['item'];

  $form['storage'] = array(
    '#type' => 'select',
    '#title' => t('Storage'),
    '#default_value' => isset($bundle->storage) ? $bundle->storage : NULL,
    '#options' => EntityCollectionStorage::getStorageOptions(),
    '#ajax' => array(
      'callback' => 'entity_collection_export_ui_storage_settings',
      'wrapper' => 'storage-settings',
    ),
    '#required' => TRUE,
  );
  $form['storage_settings'] = array(
    '#type' => 'item',
    '#tree' => TRUE,
    '#prefix' => '<div id="storage-settings">',
    '#suffix' => '</div>',
  );
  if (isset($form_state['values']['storage'])) {
    $storage = EntityCollectionStorage::getStorage($form_state['values']['storage']);
  }
  elseif (isset($bundle->storage)) {
    $storage = EntityCollectionStorage::getStorage($bundle->storage);
  }
  if (isset($storage) && method_exists($storage, 'bundleSettingsForm')) {
    $storage->bundleSettingsForm($bundle, $form['storage_settings'], $form_state);
  }
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $bundle->description,
  );
  return $form;
}

/**
 * Get the storage settings from the bundle form.
 */
function entity_collection_export_ui_storage_settings(&$form, &$form_state) {
  return $form['storage_settings'];
}

/**
 * Submit function for posting an entity collection bundle.
 */
function entity_collection_export_ui_form_submit(&$form, &$form_state) {
  $form_state['item']->label = $form_state['values']['label'];
  $form_state['item']->storage = $form_state['values']['storage'];
  $form_state['item']->description = $form_state['values']['description'];
  $form_state['item']->settings = $form_state['values']['storage_settings'];
}
