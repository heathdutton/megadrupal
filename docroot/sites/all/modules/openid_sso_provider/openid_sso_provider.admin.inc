<?php

/**
 * Generates the relying party editing entity form.
 */
function openid_sso_provider_rps_form($form, &$form_state, $entity_type, $op = 'edit') {

  $options = array();

  if ($op == 'clone') {
    $entity_type->label .= ' (cloned)';
    $entity_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $entity_type->label,
    '#description' => t('The human-readable name of the relying party.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($entity_type->machine_name) ? $entity_type->machine_name : '',
    '#maxlength' => 32,
    '#disabled' => $entity_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'openid_sso_provider_get_relying_parties',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name of the relying party. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['realm'] = array(
    '#title' => t('Realm'),
    '#type' => 'textfield',
    '#default_value' => $entity_type->realm,
    '#description' => t('The realm URL of the relying party.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $entity_type->enabled,
    '#description' => t('Allow this relying party to be used.'),
  );

  $form['trusted'] = array(
    '#type' => 'checkbox',
    '#title' => t('Trusted'),
    '#default_value' => $entity_type->trusted,
    '#description' => t('Indicate a generally trustworthy relying party.'),
  );

  $form['trusted_before'] = array(
    '#type' => 'hidden',
    '#value' => $entity_type->trusted,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save relying party'),
    '#weight' => 40,
  );

  if (!$entity_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete relying party'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('openid_sso_provider_rps_form_submit_delete')
    );
  }

  // Making this entity fieldable
  field_attach_form('openid_sso_provider_rps', $entity_type, $form, $form_state);

  return $form;
}

function openid_sso_provider_rps_form_validate($form, &$form_state) {

  // Assure trailing slash '/' at the end of a realm
  $form_state['values']['realm'] = trim($form_state['values']['realm'], '/') . '/';

  $realm = $form_state['values']['realm'];
  $rp = openid_sso_provider_rps_load_by_realm($realm);

   // Realm has no valid URL.
  if (!valid_url($realm, TRUE)) {
    form_set_error('realm', t('Please provide a valid URL.'));
  }

  // Realm has been changed/added and already exists.
  elseif ($realm != $form['realm']['#default_value'] && $rp) {
    form_set_error('realm', t('The relying party is already registered.'));
  }

  // Can these two lines being deleted?
  $openid_sso_provider_rps_submission = (object) $form_state['values'];
  field_attach_form_validate('openid_sso_provider_rps', $openid_sso_provider_rps_submission, $form, $form_state);

}

/**
 * Entity submit handler; Relying party.
 */
function openid_sso_provider_rps_form_submit($form, &$form_state) {

  // Can these two lines being deleted?
  $openid_sso_provider_rps_submission = (object) $form_state['values'];
  field_attach_submit('openid_sso_provider_rps', $openid_sso_provider_rps_submission, $form, $form_state);

  // Save entity of relying party.
  $entity = $form_state['openid_sso_provider_rps'];
  entity_form_submit_build_entity('openid_sso_provider_rps', $entity, $form, $form_state);
  openid_sso_provider_rps_save($entity);

  $form_state['redirect'] = 'admin/structure/relying-parties/manage';
  drupal_set_message(t('Relying party has been saved.'));
}

/**
 * Entity delete handler; Relying party.
 */
function openid_sso_provider_rps_form_submit_delete($form, &$form_state) {
  $entity = $form_state['openid_sso_provider_rps'];
  $form_state['redirect'] = 'admin/structure/relying-parties/manage/' . $entity->machine_name . '/delete';
}

/**
 * Delete confirmation form; Relying party.
 */
function openid_sso_provider_rps_delete_form($form, &$form_state, $entity) {
  $form_state['entity'] = $entity;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['entity_type_id'] = array('#type' => 'value', '#value' => entity_id('openid_sso_provider_rps' ,$entity));
  $entity_uri = entity_uri('openid_sso_provider_rps', $entity);
  return confirm_form($form,
    t('Are you sure you want to delete relying party %title?', array('%title' => entity_label('openid_sso_provider_rps', $entity))),
    $entity_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler; Relying party.
 */
function openid_sso_provider_rps_delete_form_submit($form, &$form_state) {
  $entity = $form_state['entity'];
  openid_sso_provider_rps_delete($entity);
  drupal_set_message(t('Relying party %title deleted.', array('%title' => entity_label('openid_sso_provider_rps', $entity))));
  $form_state['redirect'] = 'admin/structure/relying-parties/manage';
}
