<?php

/**
 * @file
 * Studyroom Reservation editing UI.
 */

/**
 * UI controller.
 */
class StudyroomReservationUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $id_count = count(explode('/', $this->path)) - 1;
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $defaults = array(
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => isset($this->entityInfo['admin ui']['file path']) ? $this->entityInfo['admin ui']['file path'] : drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$this->path] = array(
      'title' => 'Reservations',
      'description' => 'Add, edit and update studyroom reservations.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
      'type' => MENU_LOCAL_TASK,
    );

    $items[$this->path . 'add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType),
      'page callback'  => 'studyroom_reservation_add_page',
      'page arguments' => array($this->entityType),
      'access callback'  => 'studyroom_reservation_access',
      'access arguments' => array('create-reservation'),
      'type' => MENU_LOCAL_ACTION,
    ) + $defaults;

    foreach (studyroom_reservation_types() as $type) {
      $items[$this->path . 'add/' . $type->type] = array(
        'title' => 'Add ' . drupal_strtolower($type->label) . ' reservation',
        'page callback' => 'studyroom_reservation_form_wrapper',
        'page arguments' => array(studyroom_reservation_create(array('type' => $type->type))),
        'access callback' => 'studyroom_reservation_access',
        'access arguments' => array('create', $type),
        'description' => $type->description,
      ) + $defaults;
      $items[$this->path . 'add/' . $type->type . '/%/%'] = array(
        'title' => 'Add ' . drupal_strtolower($type->label) . ' reservation',
        'page callback' => 'studyroom_reservation_form_wrapper',
        'page arguments' => array(studyroom_reservation_create(array(
          'type' => $type->type)), 3, 4),
        'access callback' => 'studyroom_reservation_access',
        'access arguments' => array('create', $type),
        'description' => $type->description,
      ) + $defaults;
    }

    $items[$this->path . $wildcard] = array(
      'title callback' => 'entity_ui_get_page_title',
      'title arguments' => array('view', $this->entityType, $id_count),
      'page callback' => 'entity_ui_entity_page_view',
      'page arguments' => array($id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'studyroom_reservation_access',
      'access arguments' => array('view', $id_count),
    );
    $items[$this->path . $wildcard . '/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'load arguments' => array($this->entityType),
      'weight' => -10,
    );
    $items[$this->path . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'studyroom_reservation_form_wrapper',
      'page arguments' => array($id_count),
      'access callback' => 'studyroom_reservation_access',
      'access arguments' => array('edit', $id_count),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => 0,
    ) + $defaults;
    $items[$this->path . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'studyroom_reservation_delete_form_wrapper',
      'page arguments' => array($id_count),
      'access callback' => 'studyroom_reservation_access',
      'access arguments' => array('delete', $id_count),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
    ) + $defaults;

    return $items;
  }

  /**
   * Function to display add space content.
   *
   * Create the markup for the add StudyroomReservation Entities page
   * within the class so it can easily be extended/overriden.
   */
  public function addPage() {
    $spaces = studyroom_space_get_spaces();
    if (empty($spaces)) {
      drupal_set_message(t('No spaces have been defined yet, so no reservations can be made.'), 'error');
      drupal_goto();
    }

    $item = menu_get_item();
    $content = system_admin_menu_block($item);
    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    $types = studyroom_reservation_type_user_access();
    // If user only has access to create one type, redirect to that one.
    if (count($types) == 1) {
      $type = array_shift($types);
      $link = array('add', $type->type, arg(2), arg(3));
      drupal_goto(STUDYROOM_RESERVATION_PATH . implode('/', array_filter($link)));
    }

    $content = theme('studyroom_reservation_choose_type', array(
      'types' => $types, 'space' => arg(2), 'datetime' => arg(3)));
    return $content;
  }
}

/**
 * Page to add StudyroomReservation Entities.
 *
 * @todo Pass this through a proper theme function
 */
function studyroom_reservation_add_page() {
  $controller = entity_ui_controller('studyroom_reservation');
  return $controller->addPage();
}

/**
 * Form callback wrapper: create or edit a reservation.
 *
 * @see studyroom_reservation_edit_form()
 */
function studyroom_reservation_form_wrapper($entity, $space = NULL, $date_time = NULL) {
  return drupal_get_form('studyroom_reservation_edit_form', $entity, $space, $date_time);
}

/**
 * Form callback wrapper: delete a reservation.
 *
 * @see studyroom_studyroom_reservation_delete_form()
 */
function studyroom_reservation_delete_form_wrapper($entity) {
  return drupal_get_form('studyroom_reservation_delete_form', $entity);
}

/**
 * Form callback: create or edit a studyroom_reservation.
 */
function studyroom_reservation_edit_form($form, &$form_state, $entity, $space = NULL, $date_time = NULL) {
  if (isset($entity->id)) {
    drupal_set_title(t('Reservation %id', array('%id' => $entity->id)));
  }
  elseif (!user_access('access reservations overview')) {
    drupal_set_title(t('Create new reservation'));
  }

  $type = isset($entity->type) ? $entity->type : studyroom_reservation_type_default();
  $studyroom_reservation_type = studyroom_reservation_type_load($type);
  if (isset($space) && is_numeric($space)) {
    $studyroom_space = studyroom_space_load($space);
  }
  elseif (isset($entity->space_id)) {
    $studyroom_space = studyroom_space_load($entity->space_id);
  }
  else {
    $studyroom_space = NULL;
  }

  // Get studyroom values.
  $studyroom = new Studyroom();
  $studyroom->entity_type = $studyroom_reservation_type;
  $studyroom->space = $studyroom_space;
  $studyroom->min_duration = $studyroom->getMinDuration($studyroom_space, $studyroom->entity_type);
  $studyroom->max_duration = $studyroom->getMaxDuration($studyroom_space, $studyroom->entity_type);
  $studyroom->max_days_advance = $studyroom->getMaxDaysAdvance($studyroom_space, $studyroom->entity_type);

  // Space(s).
  if (!empty($studyroom_space)) {
    $form['space_id_display'] = array(
      '#type' => 'item',
      '#title' => t('Space'),
      '#markup' => $studyroom_space->label(),
    );
    $form['space_id'] = array(
      '#type' => 'hidden',
      '#value' => $studyroom_space->space_id,
    );
  }
  else {
    $spaces = array();
    foreach (studyroom_space_get_spaces() as $space) {
      $spaces[$space->space_id] = $space->label();
    }

    $form['space_id'] = array(
      '#type' => 'select',
      '#title' => t('Space'),
      '#options' => $spaces,
      '#default_value' => isset($entity->space_id) ? $entity->space_id : '',
    );
  }

  $form['duration'] = array(
    '#type' => 'select',
    '#title' => t('Duration'),
    '#options' => drupal_map_assoc($studyroom->durationMinutes(), 'format_interval'),
    '#default_value' => isset($entity->duration) ? $entity->duration : '3600',
  );

  if (isset($entity->uid)) {
    $user_obj = user_load($entity->uid);
    $name = $user_obj->name;
  }
  else {
    // If no uid set, use current user.
    $name = $GLOBALS['user']->name;
  }

  if ($studyroom->entity_type->public) {
    $form['user_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Reservation owner'),
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => !empty($name) ? $name : 0,
      '#weight' => -1,
      '#required' => TRUE,
      '#access' => user_access("access user profiles"),
      // Need "View user profiles" to use this.
    );
  }
  else {
    $form['user_name'] = array(
      '#type' => 'hidden',
      '#value' => $name,
    );
  }

  // Add the field related form elements.
  // @todo Do I need to add all of these?  studyroom/studyroom_reservation yes.
  $form_state['studyroom'] = $studyroom;
  $form_state['studyroom_reservation'] = $entity;

  field_attach_form('studyroom_reservation', $entity, $form, $form_state);

  // If date passed in, then set default value for datetime field.
  if (isset($date_time)) {
    $date_time = new DateObject($date_time);
    $today = new DateObject();
    $day_diff = $today->difference($date_time, 'days', FALSE);
    if ($day_diff > $studyroom->max_days_advance) {
      $date_time = $today->add(new DateInterval('P' . $studyroom->max_days_advance . 'D'));
      // @todo Better message
      drupal_set_message(t('The date trying to be used is to far in the future.'));
    }

    if ($today->difference($date_time, 'minutes', FALSE) < 0) {
      $date_time = $today;
      drupal_set_message(t('You cannot try to create a reservation in the past.'));
    }

    // Set default date/time.
    $lang = $form['field_reservation_datetime']['#language'];
    $form['field_reservation_datetime'][$lang][0]['#default_value']['value'] = $date_time->format('Y-m-d h:i a');
  }

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
    '#submit' => $submit + array('studyroom_reservation_edit_form_submit'),
  );

  $delete_button = TRUE;
  if ($entity->isLocked()) {
    $delete_button = FALSE;
  }
  elseif (isset($entity->is_new)) {
    $delete_button = !$entity->is_new;
  }

  $path = user_access('access reservations overview') ? STUDYROOM_RESERVATION_ADMIN_PATH : 'user/' . $GLOBALS['user']->uid . '/reservations';
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete reservation'),
    '#suffix' => l(t('Cancel'), $path),
    '#submit' => array('studyroom_reservation_form_submit_delete'),
    '#weight' => 45,
    '#access' => $delete_button,
    '#limit_validation_errors' => array(),
  );

  // Allow modules to modify the form.
  drupal_alter('studyroom_reservation_edit_form', $form, $form_state);

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'studyroom_reservation_edit_form_validate';
  return $form;
}

/**
 * Form API validate callback for the studyroom_reservation form.
 */
function studyroom_reservation_edit_form_validate(&$form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    drupal_goto(STUDYROOM_RESERVATION_PATH . $form_state['studyroom_reservation']->reservation_id . '/delete');
  }
  // Get form submitted values.
  $values = $form_state['values'];
  $entity = $form_state['studyroom_reservation'];
  $entity->space_id = $values['space_id'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('studyroom_reservation', $entity, $form, $form_state);

  // Get field_reservation_datetime state & value.
  $langcode = $form['field_reservation_datetime']['#language'];
  $resv_date = array_shift($values['field_reservation_datetime'][$langcode]);
  $tz = $form['field_reservation_datetime'][$langcode][0]['#date_timezone'];

  // Get start and end time.
  $entity->start_time = new DateObject($resv_date['value'], $tz, 'Y-m-d H:i:s');
  $entity->end_time = clone $entity->start_time;
  $entity->end_time = $entity->end_time->add(new DateInterval('PT' . $values['duration'] . 'S'));

  // See if reservation request before now().
  $today = date_now();
  $interval = $today->difference($entity->start_time, 'minutes', FALSE);
  if ($interval < 0 && !user_access("edit any $entity->type studyroom_reservation")) {
    form_set_error('', t('You cannot start a reservation in the past. Please select a date/time in the future.'));
    return;
  }

  // Check if reservation spans more than one day.
  if ($entity->start_time->difference($entity->end_time, 'days', FALSE)) {
    if ($entity->end_time->format('H:i:s') != '00:00:00') {
      form_set_error('', t('Your reservation cannot span more than one day. Please select an early start time or shorter duration.'));
      return;
    }
  }

  $studyroom = $form_state['studyroom'];
  if (isset($studyroom->max_days_advance)) {
    if ($today->difference($entity->start_time, 'days', FALSE) > $studyroom->max_days_advance) {
      form_set_error('', t('The date you are trying to be use is beyond the limit of %days in the future.',
        array(
          '%days' => format_plural($studyroom->max_days_advance, '1 day', '@count days'))));
      return;
    }
  }

  // Check if time outside space/site availablity.
  if (!isset($form_state['studyroom']->space)) {
    $form_state['studyroom']->space = studyroom_space_load($entity->space_id);
  }

  $space_day_hours = $form_state['studyroom']->space->hoursPerDate($entity->start_time);
  if ($entity->start_time < $space_day_hours['start_time']) {
    form_set_error('', t('Your reservation cannot start before %time on this day.', array(
      '%time' => $space_day_hours['start_time']->format('h:i a'))));
    return;
  }
  if ($entity->end_time > $space_day_hours['end_time']) {
    form_set_error('', t('Your reservation cannot end after %time on this day.', array(
      '%time' => $space_day_hours['end_time']->format('h:i a'))));
    return;
  }

  $conflict = $entity->checkConflict();
  if ($conflict) {
    form_set_error('', t('Your requested reservation conflicts with another reservation.  Please select a different start time and/or duration.'));
  }

  // Check if user has max reservation per day - rule.
  if (isset($form_state['values']['user_name'])) {
    $rule_max_per = $entity->checkRuleMaxResvPer($form_state['values']['user_name']);
    $location = studyroom_space_location_load($studyroom->space->type);
    if (isset($location->max_times_per_week) && $location->max_times_per_week > 0) {
      // If more than rule, deny creation.
      if ($rule_max_per >= $location->max_times_per_week) {
        form_set_error('', t('You are trying to exceed the maximum reservation per week you are alllowed of %count.
          Please try again next week.', array('%count' => $location->max_times_per_week)));
      }
    }
  }
}

/**
 * Form API submit callback for the studyroom_reservation form.
 *
 * @todo remove hard-coded link
 */
function studyroom_reservation_edit_form_submit(&$form, &$form_state) {
  $entity = entity_ui_controller('studyroom_reservation')->entityFormSubmitBuildEntity($form, $form_state);

  // Add in created and changed times.
  if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0) {
    $entity->created = time();
  }
  $entity->changed = time();

  // Set uid field based on user_name value.
  $account = user_load_by_name($entity->user_name);
  if ($account) {
    $entity->uid = $account->uid;
  }
  else {
    $entity->uid = 0;
  }
  unset($entity->user_name);

  $entity->save();

  // Get year/week to redirect to correct week of resrevation.
  $date = new DateObject($entity->start_time);
  $url = array($entity->space_id, $date->format('Y'), $date->format('W'));

  $form_state['redirect'] = STUDYROOM_SPACE_PATH . implode('/', $url);
}

/**
 * Form API submit callback for the delete button.
 *
 * Unset destination to fix views destination issue.
 */
function studyroom_reservation_form_submit_delete(&$form, &$form_state) {
  if (isset($_GET['destination'])) {
    unset($_GET['destination']);
  }
  $form_state['redirect'] = STUDYROOM_RESERVATION_PATH . $form_state['studyroom_reservation']->reservation_id . '/delete';
}

/**
 * Form callback: confirmation form for deleting a reservation.
 *
 * @see confirm_form()
 */
function studyroom_reservation_delete_form($form, &$form_state, $entity) {
  if ($entity->isLocked()) {
    drupal_set_message(t('Reservation %id is locked and can not be deleted.', array('%id' => $entity->reservation_id)), 'error');
    drupal_goto(STUDYROOM_RESERVATION_ADMIN_PATH);
  }

  $form_state['studyroom_reservation'] = $entity;
  $form['#submit'][] = 'studyroom_reservation_delete_form_submit';

  $path = user_access('access reservations overview') ? STUDYROOM_RESERVATION_ADMIN_PATH : 'user/' . $GLOBALS['user']->uid . '/reservations';
  $form_state['studyroom_redirect_path'] = $path;

  $form = confirm_form($form,
    t('Are you sure you want to delete reservation %id?', array('%id' => $entity->reservation_id)),
    $path,
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for studyroom_reservation_delete_form.
 */
function studyroom_reservation_delete_form_submit($form, &$form_state) {
  $entity = $form_state['studyroom_reservation'];
  $entity->delete();

  drupal_set_message(t('Reservation %id has been deleted.', array('%id' => $entity->reservation_id)));
  watchdog('studyroom_reservation', 'Deleted reservation %id.', array('%id' => $entity->reservation_id));

  $form_state['redirect'] = $form_state['studyroom_redirect_path'];
}
