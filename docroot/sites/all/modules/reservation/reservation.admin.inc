<?php

/**
 * @file
 * Admin functions for the Reservation module.
 */

/**
 * Page to add Reservations.
 *
 * @see ReservationUIController::addPage()
 */
function reservation_add_page() {
  return entity_ui_controller('reservation')->addPage();
}

/**
 * Form for creating and editing reservations.
 */
function reservation_form($form, &$form_state, Reservation $reservation) {
  $form_state['reservation'] = $reservation;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $reservation->title,
    '#required' => TRUE,
  );

  $form['unit'] = array(
    '#type' => 'value',
    '#value' => $reservation->unit,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Approved'),
    '#default_value' => $reservation->status,
    '#access' => user_access('administer reservations'),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save reservation'),
  );

  if (!isset($reservation->is_new)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete reservation'),
      '#limit_validation_errors' => array(),
      '#submit' => array('reservation_form_submit_delete'),
    );
  }

  field_attach_form('reservation', $reservation, $form, $form_state);

  return $form;
}

/**
 * Validation callback for reservation_form().
 *
 * @see reservation_form()
 */
function reservation_form_validate($form, &$form_state) {
  $reservation = entity_ui_controller('reservation')->entityFormSubmitBuildEntity($form, $form_state);
  rules_invoke_event('reservation_validate', $reservation);
}

/**
 * Submit callback for reservation_form().
 *
 * @see reservation_form()
 */
function reservation_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if (isset($values['delete']) && $values['op'] == $values['delete']) {
    drupal_goto('reservation/' . $form_state['reservation']->reservation_id . '/delete');
  }

  $reservation = entity_ui_controller('reservation')->entityFormSubmitBuildEntity($form, $form_state);
  $reservation->changed = REQUEST_TIME;
  $unit = reservation_unit_get_name($reservation->unit);

  if (empty($reservation->uid)) {
    global $user;
    $reservation->uid = $user->uid;
  }

  if (isset($reservation->is_new)) {
    $reservation->created = REQUEST_TIME;
    $message = t('@unit %reservation created.',
      array('@unit' => $unit, '%reservation' => $reservation->title));
  }
  else {
    $message = t('@unit %reservation updated.',
      array('@unit' => $unit, '%reservation' => $reservation->title));
  }

  $reservation->save();
  drupal_set_message($message);
}

/**
 * Button callback for reservation_form().
 *
 * @see reservation_form()
 */
function reservation_form_submit_delete($form, &$form_state) {
  drupal_goto('reservation/' . $form_state['reservation']->reservation_id . '/delete');
}

/**
 * Form for reassiging reservations to another unit.
 */
function reservation_reassign_form($form, &$form_state, Reservation $reservation) {
  $form_state['reservation'] = $reservation;

  $options = array();
  foreach (reservation_get_units() as $unit) {
    if ($unit->unit != $reservation->unit) {
      $options[$unit->unit] = $unit->name;
    }
  }

  $form['unit'] = array(
    '#type' => 'select',
    '#title' => t('Unit'),
    '#description' => t('Select the unit to reassign this reservation to.'),
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save reservation'),
  );

  return $form;
}

/**
 * Submit callback for reservation_reassign_form().
 *
 * @see reservation_reassign_form().
 */
function reservation_reassign_form_submit($form, &$form_state) {
  $reservation = $form_state['reservation'];

  $reservation->unit = $form_state['values']['unit'];
  $reservation->save();
}

/**
 * Form builder for the overview form.
 *
 * @see ReservationUIController::overviewForm()
 */
function reservation_overview_form($form, &$form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    $reservations = array_filter($form_state['values']['reservations']);
    return reservation_multiple_delete_confirm($form, $form_state, $reservations);
  }

  return entity_ui_controller('reservation')->overviewForm($form, $form_state);
}

/**
 * Validate reservation_overview_form submissions.
 *
 * Check if any reservations have been selected to perform the chosen
 * 'Update option' on.
 */
function reservation_overview_form_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['reservations']) || !count(array_filter($form_state['values']['reservations']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Process reservation_overview_form submissions.
 *
 * Execute the chosen 'Update option' on the selected reservations.
 */
function reservation_overview_form_submit($form, &$form_state) {
  $operations = reservation_operations();
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked nodes
  $reservations = array_filter($form_state['values']['reservations']);
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($reservations), $operation['callback arguments']);
    }
    else {
      $args = array($reservations);
    }
    call_user_func_array($function, $args);

    cache_clear_all();
  }
  else {
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of reservations.
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Form for deleting a reservation.
 */
function reservation_delete_confirm($form, &$form_state, Reservation $reservation) {
  $form['#reservation'] = $reservation;

  return confirm_form($form,
    t('Are you sure you want to delete the reservation %title?', array('%title' => $reservation->title)),
    'reservation/' . $reservation->reservation_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form for deleting a reservation.
 */
function reservation_delete_confirm_submit($form, &$form_state) {
  $reservation = $form['#reservation'];
  $reservation->delete();

  drupal_set_message(t('Reservation %title has been deleted.', array('%title' => $reservation->title)));
  $form_state['redirect'] = '<front>';
}

function reservation_multiple_delete_confirm($form, &$form_state, $reservations) {
  $form['reservations'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  foreach ($reservations as $reservation_id => $value) {
    $title = db_query('SELECT title FROM {reservation} WHERE reservation_id = :reservation_id',
      array(':reservation_id' => $reservation_id))->fetchField();
    $form['reservations'][$reservation_id] = array(
      '#type' => 'hidden',
      '#value' => $reservation_id,
      '#prefix' => '<li>',
      '#suffix' => check_plain($title) . "</li>\n",
    );
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'reservation_multiple_delete_confirm_submit';

  return confirm_form($form,
    t('Are you sure you want to delete these reservations?'),
    'admin/reservation', t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

function reservation_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    reservation_delete_multiple(array_keys($form_state['values']['reservations']));
    $count = count($form_state['values']['reservations']);
    drupal_set_message(t('Deleted @count reservations.', array('@count' => $count)));
  }
  $form_state['redirect'] = 'admin/reservation';
}

/**
 * Gets the available reservation operations.
 *
 * @return array
 *   An array of operations.
 */
function reservation_operations() {
  $operations = array(
    'delete' => array(
      'label' => t('Delete selected reservations'),
      'callback' => NULL,
    ),
    'approve' => array(
      'label' => t('Approve selected reservations'),
      'callback' => 'reservation_approve_multiple',
    ),
    'unapprove' => array(
      'label' => t('Unapprove selected reservations'),
      'callback' => 'reservation_unapprove_multiple',
    ),
  );

  return $operations;
}

/**
 * Approve multiple reservations at once.
 *
 * @param Array $reservation_ids
 *   An array of reservation_ids to approve.
 */
function reservation_approve_multiple($reservation_ids) {
  $reservations = reservation_load_multiple($reservation_ids);

  foreach ($reservations as $reservation) {
    if ($reservation->status == RESERVATION_NOT_APPROVED) {
      $reservation->status = RESERVATION_APPROVED;
      $reservation->save();
    }
  }
}

/**
 * Unapprove multiple reservations at once.
 *
 * @param Array $reservation_ids
 *   An array of reservation_ids to unapprove.
 */
function reservation_unapprove_multiple($reservation_ids) {
  $reservations = reservation_load_multiple($reservation_ids);

  foreach ($reservations as $reservation) {
    if ($reservation->status == RESERVATION_APPROVED) {
      $reservation->status = RESERVATION_NOT_APPROVED;
      $reservation->save();
    }
  }
}

/**
 * Form builder for creating and editing reservations.
 */
function reservation_unit_form($form, &$form_state, ReservationUnit $unit) {
  $form['#reservation_unit'] = $unit;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $unit->name,
    '#required' => TRUE,
  );

  $form['unit'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#description' => t('The machine name for this unit.'),
    '#default_value' => $unit->unit,
    '#machine_name' => array(
      'exists' => 'reservation_unit_exists',
      'source' => array('name'),
    ),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Description',
    '#default_value' => $unit->description,
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save unit'),
  );

  if ($unit->status == ENTITY_CUSTOM && !isset($unit->is_new)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete unit'),
    );
  }

  return $form;
}

/**
 * Submit callback for reservation_unit_form().
 *
 * @see reservation_unit_form()
 */
function reservation_unit_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op == t('Delete unit')) {
    $unit = $form_state['values']['unit'];
    $form_state['redirect'] = 'admin/reservation/units/manage/' . $unit . '/delete';
    return;
  }

  $unit = entity_ui_controller('reservation_unit')->entityFormSubmitBuildEntity($form, $form_state);

  if (isset($unit->is_new)) {
    $message = t('Reservation unit %unit added.', array('%unit' => $unit->name));
  }
  else {
    $message = t('Reservation unit %unit updated.', array('%unit' => $unit->name));
  }

  $unit->save();
  drupal_set_message($message);
  $form_state['redirect'] = 'admin/reservation/units';
}

/**
 * Form builder for the overview form.
 *
 * @see ReservationTypeUIController::overviewForm()
 */
function reservation_type_overview_form($form, &$form_state) {
  return entity_ui_controller('reservation_unit')->overviewForm($form, $form_state);
}
