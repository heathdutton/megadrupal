<?php

/**
 * @file
 * Provides administrative forms for managing bundles (deleting, editing,
 * adding new).
 */

/**
 * Generates the entity bundle editing form.
 */
function _entityspice_entity_bundle_form($etype, $f, &$fs, $op, $bundle_entity,  array $overrides = array()) {
  $info = _entityspice_getinfo_override($etype, $overrides);
  $exists_callback = $info['bundle exists callback'];;

  if ($op == 'clone') {
    $bundle_entity->label .= ' (cloned)';
    $bundle_entity->name = '';
  }

  $f['#base_type'] = $etype;
  $f['#bundle_entity'] = $bundle_entity;
  $fs['bundle_entity'] = $bundle_entity;

  $f['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $bundle_entity->label,
    '#description' => t('The human-readable name of this type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $f['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($bundle_entity->name) ? $bundle_entity->name : '',
    '#maxlength' => 32,
    '#disabled' => $bundle_entity->locked || $op == 'edit',
    '#machine_name' => array(
      'exists' => $exists_callback,
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this bundle. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $f['actions'] = array('#type' => 'actions');
  $f['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  if (!$bundle_entity->locked && $op == 'edit') {
    $f['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array($info['bundle form submit delete']),
    );
  }

  return $f;
}

/**
 * Form API submit callback for the bundle form.
 */
function _entityspice_entity_bundle_form_submit($etype, &$f, &$fs, array $overrides = array()) {
  $bundle_entity = entity_ui_form_submit_build_entity($f, $fs);
  $bundle_entity->save();
  menu_rebuild();
  $fs['redirect'] = _entityspice_getinfo_override($etype, $overrides, 'bundle submit redirect');
}

/**
 * Form API submit callback for the bundle delete button.
 */
function _entityspice_entity_bundle_form_submit_delete($etype, &$f, &$fs, array $overrides = array()) {
  if(isset($overrides['_redirect'])) {
    $r = $overrides['_redirect'];
  }
  else {
    $r = _entityspice_getinfo_override($etype, $overrides, 'parent admin path');
    $r .=  '/manage/' . $fs['bundle_entity']->name . '/delete';
  }

  $fs['redirect'] .= $r;
}
