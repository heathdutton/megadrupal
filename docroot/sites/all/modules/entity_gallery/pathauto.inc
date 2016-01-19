<?php

/**
 * @file
 * pathauto.module integration for entity galleries.
 */

/**
 * Implements hook_entity_gallery_insert() on behalf of pathauto.module.
 */
function pathauto_entity_gallery_insert($entity_gallery) {
  // @todo Remove the next line when http://drupal.org/node/1025870 is fixed.
  unset($entity_gallery->uri);
  pathauto_entity_gallery_update_alias($entity_gallery, 'insert');
}

/**
 * Implements hook_entity_gallery_update() on behalf of pathauto.module..
 */
function pathauto_entity_gallery_update($entity_gallery) {
  pathauto_entity_gallery_update_alias($entity_gallery, 'update');
}

/**
 * Implements hook_entity_gallery_delete() on behalf of pathauto.module..
 */
function pathauto_entity_gallery_delete($entity_gallery) {
  pathauto_entity_path_delete_all('entity_gallery', $entity_gallery, "gallery/{$entity_gallery->egid}");
}

/**
 * Implements hook_form_BASE_FORM_ID_alter() on behalf of pathauto.module..
 *
 * Add the Pathauto settings to the entity gallery form.
 */
function pathauto_form_entity_gallery_form_alter(&$form, &$form_state) {
  $entity_gallery = $form_state['entity_gallery'];
  $langcode = pathauto_entity_language('entity_gallery', $entity_gallery);
  pathauto_field_attach_form('entity_gallery', $entity_gallery, $form, $form_state, $langcode);
}

/**
 * Implements hook_entity_gallery_operations() on behalf of pathauto.module..
 */
function pathauto_entity_gallery_operations() {
  $operations['pathauto_update_alias'] = array(
    'label' => t('Update URL alias'),
    'callback' => 'pathauto_entity_gallery_update_alias_multiple',
    'callback arguments' => array('bulkupdate', array('message' => TRUE)),
  );
  return $operations;
}

/**
 * Update the URL aliases for an individual entity gallery.
 *
 * @param $entity_gallery
 *   An entity gallery object.
 * @param $op
 *   Operation being performed on the entity gallery ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function pathauto_entity_gallery_update_alias(stdClass $entity_gallery, $op, array $options = array()) {
  // Skip processing if the user has disabled pathauto for the entity gallery.
  if (isset($entity_gallery->path['pathauto']) && empty($entity_gallery->path['pathauto'])) {
    return;
  }

  $options += array('language' => pathauto_entity_language('entity_gallery', $entity_gallery));

  // Skip processing if the entity gallery has no pattern.
  if (!pathauto_pattern_load_by_entity('entity_gallery', $entity_gallery->type, $options['language'])) {
    return;
  }

  module_load_include('inc', 'pathauto');
  $uri = entity_uri('entity_gallery', $entity_gallery);
  pathauto_create_alias('entity_gallery', $op, $uri['path'], array('entity_gallery' => $entity_gallery), $entity_gallery->type, $options['language']);
}

/**
 * Update the URL aliases for multiple entity galleries.
 *
 * @param $egids
 *   An array of entity gallery IDs.
 * @param $op
 *   Operation being performed on the entity galleries ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function pathauto_entity_gallery_update_alias_multiple(array $egids, $op, array $options = array()) {
  $options += array('message' => FALSE);

  $entity_galleries = entity_gallery_load_multiple($egids);
  foreach ($entity_galleries as $entity_gallery) {
    pathauto_entity_gallery_update_alias($entity_gallery, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($egids), 'Updated URL alias for 1 entity gallery.', 'Updated URL aliases for @count entity galleries.'));
  }
}

/**
 * Update action wrapper for pathauto_entity_gallery_update_alias().
 */
function pathauto_entity_gallery_update_action($entity_gallery, $context = array()) {
  pathauto_entity_gallery_update_alias($entity_gallery, 'bulkupdate', array('message' => TRUE));
}
