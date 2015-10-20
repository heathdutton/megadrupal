<?php

/**
 * @file
 * Provides forms for working with entities, adding them, deleting and editing
 * them.
 */

/**
 * Get required file path which should be included with form_state.
 *
 * Modules may indicate they have no such file by setting 'has forms file'
 * property in their info hook as FALSE. The caller of this function must make
 * sure the file exists and is needed, otherwise $info['forms file'] might not
 * be set.
 */
function _entityspice_entity_form_file_path($etype=NULL) {
  // If a file should be included
  if($etype) {
    $info = _entityspice_getinfo($etype);
    $file = $info['forms file'];
    $file = $file['directory'] . $file['file'] . '.' . $file['suffix'];

    return drupal_get_path('module', $info['module']) . '/' . $file;
  }

  return drupal_get_path('module', 'entityspice') . '/entity/entity_forms.inc';
}

/**
 * Set required properties in $form and $form_state for entity's form.
 *
 * Every entity form needs some properties set in it's form or form_state such
 * as entity_type or entity object itself, title field and ...
 *
 * Form elements added here are:
 *   - Form include file if entity defines one.
 *   - Title field if entity has title.
 *   - form actions element.
 *   - form validation callback.
 *   - entity fields widgets -> delegated to field API.
 */
function _entityspice_entity_form_create_base_form($etype, $f, &$fs, $entity, array $overrides = array()) {
  $info = _entityspice_getinfo_override($etype, $overrides);
  $file =  _entityspice_entity_form_file_path(
    $info['has forms file'] ? $info['module'] : NULL);
  $entity = _entityspice_get_entity($etype, $entity);

  // Ensure the include file is loaded when the form is rebuilt from the cache.
  // If the entity type's module provides an include file use that otherwise
  // use our own file (it only 'might' be needed though).
  $fs['build_info']['files']['form'] = $file;

  // Add the field related form elements.
  $fs['entity'] = $entity;
  $fs['entity_type'] = $etype;

  // If entity type has a title field add it's form element.
  if($info['has title']) {
    $key = $info['title key'];
    $label = $info['title label'];
    $f[$key] =
      _entityspice_f_text(t($label), $entity->{$key}, TRUE, NULL, 255);
  }
  $f['#weight'] = -6;

  field_attach_form($etype, $entity, $f, $fs);

  $f['actions'] = _entityspice_f_actions();
  $f['#validate'][] = $info['entity form validation callback'];

  return $f;
}

/**
 * Builds an entity from user input values in form.
 *
 * Tasks done here:
 *   - Set entity UID.
 *   - Set entity title if entity type defines title field.
 *   - Call field_attach_submit.
 */
function _entityspice_entity_form_build_entity_from_form($etype, $f, &$fs, array $overrides = array()) {
  $entity = &$fs['entity'];
  $v = $fs['values'];
  $info = _entityspice_getinfo_override($etype, $overrides);

  $entity->uid = _entityspice_user_get_uid();
  if($info['has title']) {
    $entity->{$info['title key']}= $v[$info['title key']];
  }

  // In case calling function does not want this hook fired they can set this
  // to FALSE.
  // Notify field widgets.
  if (!isset($overrides['_field submit']) || $overrides['_field submit']) {
    field_attach_submit($etype, $entity, $f, $fs);
  }

  return $entity;
}

/**
 * Set page redirect to $entity view page.
 *
 * Check if entity has ID, which means it is successfully saved, if not,
 * only rebuild the form without redirecting to a new page.
 *
 * Then if entity is successfully saved:
 * If user has permission "view entity of type $etype", sets page redirect to
 * $entity view page. If not, sets redirect to front page.
 */
function _entityspice_entity_form_set_redirect_to_entity($etype, &$fs, $entity, array $overrides = array()) {
  if ($id = $entity->getID()) {
    $info = entity_get_info($etype);
    $idkey = $info['entity keys']['id'];

    $fs['values']['eid'] = $id;
    $fs['values'][$idkey] = $id;
    $fs['eid'] = $id;
    $fs[$idkey] = $id;

    $access = entity_access('view', $etype, $entity);
    $uri = entity_uri($etype, $entity);
    $fs['redirect'] =  $access ? $uri['path'] : '<front>';

    return TRUE;
  }
  else {
    $fs['rebuild'] = TRUE;

    return FALSE;
  }
}

// ___________________________________________________________________________
/**
 * Generates base form for creating/editing entities.
 */
function _entityspice_entity_form($etype, $f, &$fs, $entity, array $overrides = array()) {
  $scb = array(
    _entityspice_getinfo_override($etype, $overrides, 'entity form submit callback')
  );
  $f = _entityspice_entity_form_create_base_form($etype, $f, $fs, $entity, $overrides);

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  if(empty($fs['#submit'])) {
    $fs['#submit'] = array();
  }
  $f['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array_merge($scb, $fs['#submit']),
  );

  return $f;
}

/**
 * Validation callback for entity add/edit form.
 */
function _entityspice_entity_form_validate($etype, $f, &$fs) {
  $entity = $fs['entity'];

  // Notify field widgets to validate their data.
  field_attach_form_validate($etype, $entity, $f, $fs);
}

/**
 * Submit callback for entity add/edit form.
 */
function _entityspice_entity_form_submit($etype, $f, &$fs, array $overrides = array()) {
  $info = _entityspice_getinfo_override($etype, $overrides);
  $entity = _entityspice_entity_form_build_entity_from_form($etype, $f, $fs, $overrides);

  // Save the entity.
  $entity = entity_save($etype, $entity);

  if($info['has title']) {
    $key = $info['title key'];
    $entity->{$key} = $fs['values'][$key];
  }

  $name = _entityspice_get_entity_name($etype, $entity, $overrides);
  $msg = t('@entity saved.', array('@entity' => $name));
  drupal_set_message($msg, 'status', FALSE);

  _entityspice_entity_form_set_redirect_to_entity($etype, $fs, $entity, $overrides);
}

/**
 * Form callback: confirmation form for deleting an entity.
 *
 * @see confirm_form()
 */
function _entityspice_entity_delete_form($etype, $f, &$fs, $entity, array $overrides = array()) {
  if(!_entityspice_entity_in_path_exists($etype, $entity)) {
    return drupal_not_found();
  }

  $scb = _entityspice_getinfo_override($etype, $overrides, 'entity delete form submit callback');
  $entity = _entityspice_get_entity($etype, $entity);
  $info = _entityspice_getinfo_override($etype, $overrides);

  $fs['entity'] = $entity;
  $fs['entity_type'] = $etype;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $fs['build_info']['files']['form'] = _entityspice_entity_form_file_path(
    $info['has forms file'] ? $info['module'] : NULL);

  $f['#submit'][] = $scb;

  $name = _entityspice_get_entity_name($etype, $entity, $overrides);
  $targs = array('@title' => $name);
  $msg = t('Are you sure you want to delete entity @title?', $targs);

  $f = confirm_form($f,
    $msg,
    entity_uri($etype, $entity),
    '<p>' . t('Deleting this entity cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $f;
}

/**
 * Submit callback for entity delete form.
 */
function _entityspice_entity_form_entity_delete_form_submit($etype, $f, &$fs, array $overrides = array()) {
  $entity = $fs['entity'];
  $name = _entityspice_get_entity_name($etype, $entity, $overrides);

  $targ = array('@name' => $name);
  if (_entityspice_entity_delete($etype, $entity->getID())) {
    drupal_set_message(t('@name has been deleted.', $targ));
    watchdog($etype, 'Deleted entity @name.', $name, WATCHDOG_NOTICE);
    $fs['redirect'] = _entityspice_getinfo_override($etype, $overrides, 'entity delete redirect');
  }
  else {
    drupal_set_message(t('@name could not be deleted.', $targ), 'error');
  }
}
