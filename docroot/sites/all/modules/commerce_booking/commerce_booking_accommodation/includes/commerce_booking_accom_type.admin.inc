<?php
/**
 * @file
 * Administrative page and form callbacks.
 */

/**
 * Page callback for the add accommodation type page.
 */
function commerce_booking_accom_type_add($entity_type, $entity, $bundle) {
  // Get our entity wrapper.
  $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);

  // Create our new type entity.
  $type = entity_create('commerce_booking_accom_type', array('category' => $bundle));
  $type->commerce_booking_event[LANGUAGE_NONE][0] = array(
    'entity_type' => $entity_type,
    'entity_id' => $entity_wrapper->getIdentifier(),
  );

  // Get our form.
  return drupal_get_form('commerce_booking_accom_type_edit', $entity_type, $entity, $type);
}

/**
 * Form constructor for editing an accommodation type.
 *
 * @param $type
 *   The accommodation type we're editing.
 *
 * @see commerce_booking_accom_type_edit_validate().
 * @see commerce_booking_accom_type_edit_submit().
 */
function commerce_booking_accom_type_edit($form, &$form_state, $entity_type, $entity, $type) {
  // Get our wrappers.
  $form['#event'] = entity_metadata_wrapper($entity_type, $entity);
  $form['#accom_type'] = $type;
  $type_wrapper = entity_metadata_wrapper('commerce_booking_accom_type', $type);

  if (empty($type->is_new)) {
    drupal_set_title(t('Edit @label', array('@label' => $type_wrapper->label())));
  }
  else {
    drupal_set_title(t('Add accommodation type for @event', array('@event' => $form['#event']->label())));
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('The label for this accommodation type.'),
    '#default_value' => $type->label,
    '#required' => TRUE,
  );

  field_attach_form('commerce_booking_accom_type', $type, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 99,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($type->is_new) ? t('Save'): t('Create'),
  );

  if (!empty($_GET['destination'])) {
    $uri = array(
      'path' => $_GET['destination'],
      'options' => array(),
    );
  }
  else {
    $uri = entity_uri($entity_type, $entity);
    $uri['path'] .= '/accommodation/manage';
  }
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $uri['path'], $uri['options']),
  );

  return $form;
}

/**
 * Validation handler for commerce_booking_accom_type_edit().
 *
 * @see commerce_booking_accom_type_edit_submit().
 */
function commerce_booking_accom_type_edit_validate(&$form, &$form_state) {
  field_attach_form_validate('commerce_booking_accom_type', $form['#accom_type'], $form, $form_state);
}

/**
 * Submission handler for commerce_booking_accom_type_edit().
 *
 * @see commerce_booking_accom_type_edit_validate().
 */
function commerce_booking_accom_type_edit_submit(&$form, &$form_state) {
  // Set the values and save.
  $form['#accom_type']->label = $form_state['values']['label'];
  field_attach_submit('commerce_booking_accom_type', $form['#accom_type'], $form, $form_state);
  $form['#accom_type']->save();

  // Work out the redirection URL.
  $uri = entity_uri($form['#event']->type(), $form['#event']->value());
  $form_state['redirect'] = $uri['path'] . '/accommodation/manage';

  // Set a success message.
  drupal_set_message(t('Accommodation type %type successfully created for %event', array(
    '%type' => $form['#accom_type']->label,
    '%event' => $form['#event']->label(),
  )));
}
