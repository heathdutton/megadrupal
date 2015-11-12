<?php

/**
 * @file
 * StudyroomClosed editing UI.
 */

/**
 * UI controller.
 */
class StudyroomClosedUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $defaults = array(
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => isset($this->entityInfo['admin ui']['file path']) ? $this->entityInfo['admin ui']['file path'] : drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$this->path . '/add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType),
      'description' => 'Add a new studyroom_closed',
      'page callback'  => 'studyroom_closed_form_wrapper',
      'page arguments' => array(studyroom_closed_create()),
      'access arguments' => array('administer studyroom_closed'),
      'type' => MENU_CALLBACK,
    ) + $defaults;
    $items[$this->path . '/' . $wildcard] = array(
      'page callback' => 'studyroom_closed_form_wrapper',
      'page arguments' => array($id_count),
      'access arguments' => array('administer studyroom_closed'),
      'weight' => 0,
    ) + $defaults;
    $items[$this->path . '/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_INLINE,
    );
    $items[$this->path . '/' . $wildcard . '/delete'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('delete', $this->entityType),
      'page callback' => 'studyroom_closed_delete_form_wrapper',
      'page arguments' => array($id_count),
      'access arguments' => array('administer studyroom_closed'),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
    ) + $defaults;
    $items[$this->path . '/' . $wildcard . '/conflict'] = array(
      'title' => 'Conflicts',
      'page callback' => 'studyroom_closed_conflict_form_wrapper',
      'page arguments' => array($id_count),
      'access arguments' => array('administer studyroom_closed'),
      'type' => MENU_LOCAL_TASK,
      'weight' => 20,
    ) + $defaults;

    return $items;
  }
}

/**
 * Form callback wrapper: create or edit a studyroom_closed.
 *
 * @see studyroom_closed_edit_form()
 */
function studyroom_closed_form_wrapper($studyroom_closed) {
  return drupal_get_form('studyroom_closed_edit_form', $studyroom_closed);
}

/**
 * Form callback wrapper: delete a studyroom_closed.
 *
 * @see studyroom_closed_delete_form()
 */
function studyroom_closed_delete_form_wrapper($studyroom_closed) {
  return drupal_get_form('studyroom_closed_delete_form', $studyroom_closed);
}

/**
 * Form callback wrapper: manage conflict with a studyroom_closed.
 *
 * @see studyroom_closed_conflict_form()
 */
function studyroom_closed_conflict_form_wrapper($studyroom_closed) {
  return drupal_get_form('studyroom_closed_conflict_form', $studyroom_closed);
}

/**
 * Form callback wrapper: delete a studyroom_closed.
 *
 * @see studyroom_closed_delete_form()
 */
function studyroom_closed_conflict_confirm_form_wrapper() {
  return drupal_get_form('studyroom_closed_conflict_delete_confirm');
}

/**
 * Form callback: create or edit a studyroom_closed.
 */
function studyroom_closed_edit_form($form, &$form_state, $studyroom_closed) {
  $spaces = new StudyroomSpace();
  $options = $spaces->optionsSpaces();
  if (empty($options)) {
    drupal_set_message(t('You must have at least one space to create a closed reservation.'));
    drupal_goto(STUDYROOM_CLOSED_MENU_ADMIN_SPACE);
  }

  drupal_add_css(drupal_get_path('module', 'studyroom_closed') . '/studyroom_closed.css');
  // Need to set title.
  if (isset($studyroom_closed->name)) {
    drupal_set_title($studyroom_closed->name);
  }

  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Purpose'),
    '#default_value' => isset($studyroom_closed->name) ? $studyroom_closed->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  $form['spaces_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply reservation to all spaces. Setting this value will override individual space settings.'),
    '#default_value' => ($studyroom_closed->spaces_all) ? TRUE : FALSE,
  );
  $form['spaces_fs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Spaces'),
    '#description' => t('Select which spaces this policy applies to.'),
    '#tree' => FALSE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="spaces_all"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['spaces_fs']['spaces'] = array(
    '#title' => t('Spaces'),
    '#type' => 'tableselect',
    '#header' => array(
      'title' => array('data' => t('Space')),
      'location' => array('data' => t('Location')),
    ),
    '#options' => $options,
    '#multiple' => TRUE,
    '#default_value' => isset($studyroom_closed->spaces) ? $studyroom_closed->spaces : array(),
    '#empty' => t('No spaces available.'),
    '#prefix' => '<div class="studyroom-closed-table">',
    '#suffix' => '</div>',
  );

  // Add the field related form elements.
  field_attach_form('studyroom_closed', $studyroom_closed, $form, $form_state);
  $form_state['studyroom_closed'] = $studyroom_closed;

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save reservation'),
    '#submit' => $submit + array('studyroom_closed_edit_form_submit'),
  );

  if (!empty($studyroom_closed->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete reservation'),
      '#suffix' => l(t('Cancel'), STUDYROOM_CLOSED_MENU_ADMIN_SPACE),
      '#submit' => $submit + array('studyroom_closed_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'studyroom_closed_edit_form_validate';
  return $form;
}

/**
 * Form API validate callback for the studyroom_closed form.
 */
function studyroom_closed_edit_form_validate(&$form, &$form_state) {
  $studyroom_closed = $form_state['studyroom_closed'];

  if (!$form_state['values']['spaces_all']) {
    $spaces = array_filter($form_state['values']['spaces']);
    if (count($spaces) == 0) {
      form_set_error('spaces', t('If not applying to all spaces, at least one space must be selected.'));
    }
    elseif (count($spaces) == count($form_state['values']['spaces'])) {
      form_set_value($form['spaces_all'], TRUE, $form_state);
    }
  }
  // Notify field widgets to validate their data.
  field_attach_form_validate('studyroom_closed', $studyroom_closed, $form, $form_state);
}

/**
 * Form API submit callback for the studyroom_closed form.
 */
function studyroom_closed_edit_form_submit(&$form, &$form_state) {
  $studyroom_closed = entity_ui_controller('studyroom_closed')->entityFormSubmitBuildEntity($form, $form_state);

  // Add in created and changed times.
  if ($studyroom_closed->is_new = isset($studyroom_closed->is_new) ? $studyroom_closed->is_new : 0) {
    $studyroom_closed->created = time();
  }

  $studyroom_closed->changed = time();

  $studyroom_closed->save();
  $form_state['redirect'] = STUDYROOM_CLOSED_MENU_ADMIN_SPACE;
}

/**
 * Form API submit callback for the delete button.
 */
function studyroom_closed_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = STUDYROOM_CLOSED_MENU_ADMIN_SPACE . '/' . $form_state['studyroom_closed']->studyroom_closed_id . '/delete';
}

/**
 * Form callback: confirmation form for deleting a studyroom_closed.
 *
 * @see confirm_form()
 */
function studyroom_closed_delete_form($form, &$form_state, $studyroom_closed) {
  $form_state['studyroom_closed'] = $studyroom_closed;

  $form['#submit'][] = 'studyroom_closed_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete studyroom_closed %name?', array('%name' => $studyroom_closed->name)),
    STUDYROOM_CLOSED_MENU_ADMIN_SPACE,
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for studyroom_closed_delete_form.
 */
function studyroom_closed_delete_form_submit($form, &$form_state) {
  $studyroom_closed = $form_state['studyroom_closed'];

  studyroom_closed_delete($studyroom_closed);

  drupal_set_message(t('The studyroom_closed %name has been deleted.', array('%name' => $studyroom_closed->name)));
  watchdog('studyroom_closed', 'Deleted studyroom_closed %name.', array('%name' => $studyroom_closed->name));

  $form_state['redirect'] = STUDYROOM_CLOSED_MENU_ADMIN_SPACE;
}

/**
 * Form callback: manage conflicts.
 */
function studyroom_closed_conflict_form($form, &$form_state, $studyroom_closed) {
  if (isset($form_state['storage']['confirm'])) {
    return studyroom_closed_conflict_form_confirm($form, $form_state);
  }

  $count = db_select('studyroom_closed_conflict', 'c')
    ->fields('c', array('reservation'))
    ->condition('closed', $studyroom_closed->studyroom_closed_id)
    ->execute()->rowCount();

  if ($count == 0) {
    drupal_set_message(t('No conflicts found for the closed reservation %label.', array('%label' => $studyroom_closed->name)));
    drupal_goto(STUDYROOM_CLOSED_MENU_ADMIN_SPACE);
  }

  drupal_add_css(drupal_get_path('module', 'studyroom_closed') . '/studyroom_closed.css');
  // Need to set title.
  if (isset($studyroom_closed->name)) {
    drupal_set_title($studyroom_closed->name . ' conflicts');
  }

  $result = db_select('studyroom_closed_conflict', 'c')
    ->fields('c', array('reservation'))
    ->condition('closed', $studyroom_closed->studyroom_closed_id)
    ->execute();

  $reservations = array();
  while ($reservation_id = $result->fetchField()) {
    $resv = studyroom_reservation_load($reservation_id);

    $uri = studyroom_reservation_uri($resv);
    $reservations[$reservation_id]['reservation'] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => check_plain('View ' . $resv->reservation_id),
        '#href' => $uri['path'],
      ),
      'class' => 'title',
    );
    $reservations[$reservation_id]['type'] = $resv->type;
    $reservations[$reservation_id]['space_id'] = $resv->space_id;
    $reservations[$reservation_id]['duration'] = format_interval($resv->duration);

    $field = $resv->field_reservation_datetime[LANGUAGE_NONE][0];
    $start_time = new DateObject($field['value'], $field['timezone']);
    $format = variable_get('date_format_medium', 'D, m/d/Y - H:i');
    $reservations[$reservation_id]['start_time'] = $start_time->format($format);

    // User information.
    $user = user_load($resv->uid);
    $reservations[$reservation_id]['user'] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => check_plain($user->name),
        '#href' => drupal_get_path_alias('user/' . $user->uid),
      ),
      'class' => 'username',
    );
  }

  $form['reservations'] = array(
    '#title' => t('Reservations'),
    '#type' => 'tableselect',
    '#header' => array(
      'reservation' => array('data' => t('Reservation')),
      'type' => array('data' => t('Type')),
      'space_id' => array('data' => t('Space')),
      'user' => array('data' => t('User')),
      'start_time' => array('data' => t('Time')),
      'duration' => array('data' => t('Duration')),
    ),
    '#options' => $reservations,
    '#multiple' => TRUE,
    '#empty' => t('No conflicts exist.'),
    '#prefix' => '<div class="studyroom-closed-table">',
    '#suffix' => '</div>',
  );

  $form['customize_notice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check here if you want to customize the notification to the users.'),
  );
  $form['message'] = array(
    '#type' => 'textfield',
    '#title' => t('Message'),
    '#maxlength' => 180,
    '#description' => t('What should the message say?'),
    '#states' => array(
      'visible' => array(
        ':input[name="customize_notice"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form_state['studyroom_closed'] = $studyroom_closed;
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete reservations'),
    '#validate' => array('studyroom_closed_conflict_form_validate'),
  );
  return $form;
}

/**
 * Form API validate callback for the studyroom_closed_conflict_form.
 */
function studyroom_closed_conflict_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  if (($values['customize_notice']) && empty($values['message'])) {
    form_set_error('message', t('You must provide a message.'));
  }
}

/**
 * Form API submit callback for the studyroom_closed_conflict_form.
 */
function studyroom_closed_conflict_form_submit($form, &$form_state) {
  if (!isset($form_state['storage']['confirm'])) {
    // Rebuild form, entering the confirm part of the form.
    $form_state['storage']['confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
  }
  else {
    $values = $form_state['values'];

    // @todo Figure out how to nofity user with this message.
    $msg = ($values['customize_notice']) ? trim($values['message']) : FALSE;
    studyroom_reservation_delete_multiple($values['reservations']);

    $form_state['redirect'] = STUDYROOM_CLOSED_MENU_ADMIN_SPACE;
  }
}


/**
 * Form API submit callback for the studyroom_closed_conflict_form.
 *
 * @see confirm_form()
 */
function studyroom_closed_conflict_form_confirm($form, &$form_state) {
  $values = $form_state['values'];
  $studyroom_closed = $form_state['studyroom_closed'];

  $form['reservations'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );

  // Use array_filter() to only return elements with actual values.
  $reservation_counter = 0;
  if (isset($values['reservations'])) {
    foreach (array_filter($values['reservations']) as $id) {
      $form['reservations'][$id] = array(
        '#type' => 'hidden',
        '#value' => $id,
        '#prefix' => '<li>',
        '#suffix' => 'Reservation ' . $id . '</li>',
      );
      ++$reservation_counter;
    }
  }

  if ($values['customize_notice']) {
    $form['preview_message'] = array(
      '#type' => 'item',
      '#title' => t('Message to be sent to users:'),
      '#markup' => $values['message'],
    );
  }
  $form['customize_notice'] = array(
    '#type' => 'value',
    '#value' => $values['customize_notice'],
  );
  $form['message'] = array(
    '#type' => 'value',
    '#value' => $values['message'],
  );

  if (!$reservation_counter) {
    drupal_set_message(t('There do not appear to be any reservations to delete, or your selected reservations was deleted by another administrator.'));
    drupal_goto(STUDYROOM_CLOSED_MENU_ADMIN_SPACE . '/' . $studyroom_closed->studyroom_closed_id . '/conflict');
  }
  else {
    return confirm_form($form,
      t('Are you sure you want to delete these reservations?'),
      STUDYROOM_CLOSED_MENU_ADMIN_SPACE,
      t('This action cannot be undone.'),
      t('Delete reservations'),
      t('Cancel')
    );
  }
}
