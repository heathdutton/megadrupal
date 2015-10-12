<?php

/**
 * @file entity_operations_test.module
 * Contains form builders for entity forms.
 */

/**
 * The test entity form.
 *
 * @param $entity
 *  The entity. If this is a create operation, the entity must have the type
 *  set on it.
 */
function entity_operations_test_form($form, &$form_state, $entity) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The title of this entity.'),
    '#default_value' => isset($entity->title) ? $entity->title : '',
    '#required' => TRUE,
  );

  // Set the bundle on the form so entity_form_field_validate() and
  // entity_ui_form_submit_build_entity() have it to build the entity.
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $entity->type,
  );

  field_attach_form('entity_operations_test', $entity, $form, $form_state);

  $form['actions'] = array(
    '#weight' => 10,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate handler for edit/add form.
 */
function entity_operations_test_form_validate($form, &$form_state) {
  // Notify field widgets to validate their data.
  entity_form_field_validate('entity_operations_test', $form, $form_state);
}

/**
 * Submit handler for edit/add form.
 */
function entity_operations_test_form_submit($form, &$form_state) {
  $entity = entity_ui_form_submit_build_entity($form, $form_state);

  $entity->uid = $GLOBALS['user']->uid;

  // Save the entity.
  $entity->save();

  $entity_info = entity_get_info($entity->entityType());
  // Show a message so we're consistent with what normal entity forms would do.
  drupal_set_message(t('The @entity-type %label has been saved.', array(
    '@entity-type' => $entity_info['label'],
    '%label' => $entity->label(),
  )));

  // Redirect to the entity.
  $uri = $entity->uri();

  $form_state['redirect'] = $uri['path'];
}
