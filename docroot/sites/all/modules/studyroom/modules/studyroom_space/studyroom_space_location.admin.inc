<?php

/**
 * @file
 * Studyroom_space location editing UI.
 */

/**
 * UI controller.
 */
class StudyroomSpaceLocationUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    // Remove menu items not used.
    unset($items[$this->path . '/import']);
    // Make changes to menu.
    $items[$this->path]['description'] = 'Add and modify locations for the studyroom module. Also, manage fields for spaces.';

    return $items;
  }

  /**
   * Mofify the overviewTableHeaders to add description.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $additional_header[] = t('Description');
    return parent::overviewTableHeaders($conditions, $rows, $additional_header);
  }

  /**
   * Modify the overviewTableRow to add description.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $additional_cols[] = array('data' => check_plain($entity->description));
    return parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
  }
}

/**
 * Theme entity ui admin form.
 *
 * @ingroup themeable
 */
function theme_studyroom_space_location_overview_form($variables) {
  // Remove clone link to prevent confusion.
  foreach ($variables['form']['table']['#rows'] as $id => &$row) {
    foreach ($row as $key => $col) {
      if (is_string($col) && strpos($col, 'clone')) {
        unset($row[$key]);
      }
    }
  }
  $variables['form']['table']['#header']['3']['colspan'] = $variables['form']['table']['#header']['3']['colspan'] - 1;

  return theme('table', array(
    'header' => $variables['form']['table']['#header'],
    'rows' => $variables['form']['table']['#rows'],
    'empty' => t('No locations available. !link.', array('!link' => l(t('Add a location'), STUDYROOM_SPACE_MENU_LOCATION . '/add'))),
    'attributes' => array('id' => 'studyroom')));
}

/**
 * Generates the studyroom_space type editing form.
 */
function studyroom_space_location_form($form, &$form_state, $entity, $op = 'edit') {
  $studyroom = new Studyroom();
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($entity->type) ? $entity->type : '',
    '#description' => t('The human-readable name of this location.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($entity->type) ? $entity->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'studyroom_space_location_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this location. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => isset($entity->description) ? $entity->description : '',
    '#maxlength' => 255,
  );

  if (module_exists('studyroom_reservation')) {
    $form['allowed_reservation_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Reservation types'),
      '#options' => studyroom_reservation_types_options(FALSE),
      '#default_value' => isset($entity->allowed_reservation_types) ? $entity->allowed_reservation_types : array('user'),
      '#required' => TRUE,
      '#description' => t('Select the type of reservations that can be created in this location.'),
    );
    foreach (studyroom_reservation_types_admin() as $type => $label) {
      $form['allowed_reservation_types'][$type] = array('#disabled' => TRUE, '#default_value' => TRUE);
    }
  }

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );
  $form['rules'] = array(
    '#type' => 'fieldset',
    '#title' => t('Override Rules'),
    '#tree' => FALSE,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#group' => 'additional_settings',
  );
  $form['rules']['min_duration'] = array(
    '#type' => 'select',
    '#title' => t('Set minimum duration'),
    '#default_value' => isset($entity->min_duration) ? $entity->min_duration : '',
    '#options' => $studyroom->optionsMinDuration(),
    '#description' => t("Override the minimum duration for a reservation for this location.
      If no value is specified, the value of %value will be used.", array(
        '%value' => format_interval($studyroom->getMinDuration()))),
  );
  $form['rules']['max_duration'] = array(
    '#type' => 'select',
    '#title' => t('Set maximum duration'),
    '#default_value' => isset($entity->max_duration) ? $entity->max_duration : '',
    '#options' => $studyroom->optionsMaxDuration(),
    '#description' => t("Override the maximum duration for a reservation for this location.
      If no value is specified, the value of %value will be used.", array(
        '%value' => format_interval($studyroom->getMaxDuration()))),
  );
  $form['rules']['max_days_advance'] = array(
    '#type' => 'select',
    '#title' => t('Set maximum days in advance'),
    '#default_value' => isset($entity->max_days_advance) ? $entity->max_days_advance : '',
    '#options' => $studyroom->optionsMaxDayAdvance(),
    '#description' => t("Override the maximum days in advance a reservation can be made for this location.
      If no value is specified, the value of %value will be used.", array(
        '%value' => studyroom_format_day($studyroom->getMaxDaysAdvance()))),
  );
  $form['rules_space'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rules'),
    '#tree' => FALSE,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#group' => 'additional_settings',
  );
  $form['rules_space']['max_times_per_week'] = array(
    '#type' => 'select',
    '#title' => t('Maximum reservations per week'),
    '#default_value' => isset($entity->max_times_per_week) ? $entity->max_times_per_week : '',
    '#options' => array(0 => '- None -', 1, 2, 3, 4),
    '#description' => t("The maximum reservations that a user can make per week (Sun-Sat). 'None' means no limit."),
  );

  $form_state['studyroom_space_location'] = $entity;
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save location'),
    '#weight' => 40,
  );

  if ($op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete location'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('studyroom_space_location_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'studyroom_space_location_form_validate';
  return $form;
}

/**
 * Form validation handler for studyroom_space_form_location().
 *
 * Makes sure that the machine name of the location is not in the
 * disallowed list (names that conflict with menu items, such as 'list'
 * and 'add').
 *
 * @see studyroom_space_location_form()
 * @see studyroom_space_location_form_submit()
 */
function studyroom_space_location_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $entity = $form_state['studyroom_space_location'];
  if (isset($values['type'])) {
    // Do not allow machine names to conflict with studyroom path arguments.
    $type = $values['type'];
    $disallowed = array('add', 'list', 'delete');
    if (in_array($type, $disallowed)) {
      form_set_error('type', t('The machine-readable name cannot be "add", "delete" or "list".'));
    }
  }

  $studyroom = new Studyroom();
  $studyroom->validateDurations($form, $form_state, 'studyroom_space_location', $entity);

  if (empty($values['max_days_advance'])) {
    form_set_value($form['rules']['max_days_advance'], NULL, $form_state);
  }
}


/**
 * Form API submit callback for the location form.
 */
function studyroom_space_location_form_submit(&$form, &$form_state) {
  $entity = $form_state['studyroom_space_location'];
  $new = (isset($entity->is_new) && $entity->is_new) ? TRUE : FALSE;

  $entity = entity_ui_form_submit_build_entity($form, $form_state);
  $entity->save();

  if ($new) {
    // field_space_hours is defined in the optional studyroom_hours sub-module. It is optional because
    // one can specify the hours for a given location and space by implementing the hook_studyroom_space_hours hook.
    if (module_exists('studyroom_hours')) {
      drupal_set_message(t('Please configure the default hours for each space in this location.'));
      $form_state['redirect'] = STUDYROOM_SPACE_MENU_LOCATION . '/manage/' . $entity->type . '/fields/field_space_hours';
    } else {
      $form_state['redirect'] = STUDYROOM_SPACE_MENU_LOCATION;
    }
  }
  else {
    $form_state['redirect'] = STUDYROOM_SPACE_MENU_LOCATION;
  }
}

/**
 * Form API submit callback for the delete button.
 */
function studyroom_space_location_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = STUDYROOM_SPACE_MENU_LOCATION . '/manage/' . $form_state['studyroom_space_location']->type . '/delete';
}
