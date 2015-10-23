<?php

/**
 * @file
 * entity.module integration for entity galleries.
 */

/**
 * Callback for getting entity gallery properties.
 */
function entity_gallery_entity_api_get_properties($entity_gallery, array $options, $name, $entity_type) {
  switch ($name) {
    case 'is_new':
      return empty($entity_gallery->egid) || !empty($entity_gallery->is_new);

    case 'source':
      if (!empty($entity_gallery->tegid) && $source = entity_gallery_load($entity_gallery->tegid)) {
        return $source;
      }
      return NULL;

    case 'edit_url':
      return url('gallery/' . $entity_gallery->egid . '/edit', $options);

    case 'author':
      return !empty($entity_gallery->uid) ? $entity_gallery->uid : drupal_anonymous_user();
  }
}

/**
 * Callback for determing access for entity gallery revision related properties.
 */
function entity_gallery_entity_api_revision_access($op, $name, $entity = NULL, $account = NULL) {
  return $op == 'view' ? user_access('view entity gallery revisions', $account) : user_access('administer entity galleries', $account);
}

/**
 * Access callback for the entity gallery entity.
 *
 * This function does not implement hook_entity_gallery_access(), thus it may
 * not be called entity_gallery_entity_access().
 *
 * @see entity_access()
 *
 * @param $op
 *   The operation being performed. One of 'view', 'update', 'create' or
 *   'delete'.
 * @param $entity_gallery
 *   An entity gallery to check access for. Must be an entity gallery object.
 *   Must have egid, except in the case of 'create' operations.
 * @param $account
 *   The user to check for. Leave it to NULL to check for the global user.
 *
 * @throws EntityMalformedException
 *
 * @return boolean
 *   TRUE if access is allowed, FALSE otherwise.
 */
function entity_gallery_entity_api_no_hook_access($op, $entity_gallery = NULL, $account = NULL) {
  // First deal with the case where a $entity_gallery is provided.
  if (isset($entity_gallery)) {
    if ($op == 'create') {
      if (isset($entity_gallery->type)) {
        return entity_gallery_access($op, $entity_gallery->type, $account);
      }
      else {
        throw new EntityMalformedException('Permission to create an entity gallery was requested but no entity gallery type was given.');
      }
    }
    // If a non-default revision is given, incorporate revision access.
    $default_revision = entity_gallery_load($entity_gallery->egid);
    if ($entity_gallery->vid !== $default_revision->vid) {
      return _entity_gallery_revision_access($entity_gallery, $op, $account);
    }
    else {
      return entity_gallery_access($op, $entity_gallery, $account);
    }
  }
  // No entity gallery is provided. Check for access to all entity galleries.
  if (user_access('bypass entity gallery access', $account)) {
    return TRUE;
  }
  if (!user_access('access entity galleries', $account)) {
    return FALSE;
  }
  if ($op == 'view') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Callback to create a new entity gallery.
 */
function entity_gallery_entity_api_create($values = array()) {
  $entity_gallery = (object) array(
    'type' => $values['type'],
    'language' => LANGUAGE_NONE,
    'is_new' => TRUE,
  );
  // Set some defaults.
  $entity_gallery_options = variable_get('entity_gallery_options_' . $entity_gallery->type, array('status'));
  foreach (array('status', 'sticky') as $key) {
    $entity_gallery->$key = (int) in_array($key, $entity_gallery_options);
  }
  // Apply the given values.
  foreach ($values as $key => $value) {
    $entity_gallery->$key = $value;
  }
  return $entity_gallery;
}

/**
 * Callback to view entity galleries.
 */
function entity_gallery_entity_api_view($entities, $view_mode = 'full', $langcode = NULL) {
  $result = entity_gallery_view_multiple($entities, $view_mode, 0, $langcode);
  // Make sure to key the result with 'entity_gallery' instead of 'entity_galleries'.
  return array('entity_gallery' => reset($result));
}

/**
 * Callback to get the form of an entity gallery.
 */
function entity_gallery_entity_api_form($entity_gallery) {
  // Pre-populate the form-state with the right form include.
  $form_state['build_info']['args'] = array($entity_gallery);
  form_load_include($form_state, 'inc', 'entity_gallery', 'entity_gallery.pages');
  return drupal_build_form($entity_gallery->type . '_entity_gallery_form', $form_state);
}
