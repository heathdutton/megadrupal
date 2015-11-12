<?php

/**
 * @file
 * path.module integration for entity galleries.
 */

/**
 * Implements hook_form_BASE_FORM_ID_alter() for entity_gallery_form() on behalf of path.module.
 *
 * @see path_form_element_validate()
 */
function path_form_entity_gallery_form_alter(&$form, $form_state) {
  $path = array();
  if (!empty($form['#entity_gallery']->egid)) {
    $conditions = array('source' => 'gallery/' . $form['#entity_gallery']->egid);
    $langcode = entity_language('entity_gallery', $form['#entity_gallery']);
    if ($langcode != LANGUAGE_NONE) {
      $conditions['language'] = $langcode;
    }
    $path = path_load($conditions);
    if ($path === FALSE) {
      $path = array();
    }
  }
  $path += array(
    'pid' => NULL,
    'source' => isset($form['#entity_gallery']->egid) ? 'gallery/' . $form['#entity_gallery']->egid : NULL,
    'alias' => '',
    'language' => isset($langcode) ? $langcode : LANGUAGE_NONE,
  );

  $form['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('URL path settings'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($path['alias']),
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('path-form'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'path') . '/path.js'),
    ),
    '#access' => user_access('create url aliases') || user_access('administer url aliases'),
    '#weight' => 30,
    '#tree' => TRUE,
    '#element_validate' => array('path_form_element_validate'),
  );
  $form['path']['alias'] = array(
    '#type' => 'textfield',
    '#title' => t('URL alias'),
    '#default_value' => $path['alias'],
    '#maxlength' => 255,
    '#description' => t('Optionally specify an alternative URL by which this gallery can be accessed. For example, type "wildlife" when creating a photo gallery featuring a variety of animals. Use a relative path and don\'t add a trailing slash or the URL alias won\'t work.'),
  );
  $form['path']['pid'] = array('#type' => 'value', '#value' => $path['pid']);
  $form['path']['source'] = array('#type' => 'value', '#value' => $path['source']);
  $form['path']['language'] = array('#type' => 'value', '#value' => $path['language']);
}

/**
 * Implements hook_entity_gallery_insert() on behalf of path.module.
 */
function path_entity_gallery_insert($entity_gallery) {
  if (isset($entity_gallery->path)) {
    $path = $entity_gallery->path;
    $path['alias'] = trim($path['alias']);
    // Only save a non-empty alias.
    if (!empty($path['alias'])) {
      // Ensure fields for programmatic executions.
      $langcode = entity_language('entity_gallery', $entity_gallery);
      $path['source'] = 'gallery/' . $entity_gallery->egid;
      $path['language'] = isset($langcode) ? $langcode : LANGUAGE_NONE;
      path_save($path);
    }
  }
}

/**
 * Implements hook_entity_gallery_update() on behalf of path.module.
 */
function path_entity_gallery_update($entity_gallery) {
  if (isset($entity_gallery->path)) {
    $path = $entity_gallery->path;
    $path['alias'] = trim($path['alias']);
    // Delete old alias if user erased it.
    if (!empty($path['pid']) && empty($path['alias'])) {
      path_delete($path['pid']);
    }
    path_entity_gallery_insert($entity_gallery);
  }
}

/**
 * Implements hook_entity_gallery_delete() on behalf of path.module.
 */
function path_entity_gallery_delete($entity_gallery) {
  // Delete all aliases associated with this entity gallery.
  path_delete(array('source' => 'gallery/' . $entity_gallery->egid));
}
