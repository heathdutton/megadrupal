<?php

/**
 * @file
 * Studyroom space editing UI.
 */

/**
 * UI controller.
 */
class StudyroomSpaceUIController extends EntityDefaultUIController {
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

    $items[$this->path] = array(
      'title' => 'Space',
      'description' => 'Add, edit and update spaces.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
      'type' => MENU_LOCAL_TASK,
    );
    $items[$this->path . '/add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType),
      'page callback' => 'entity_ui_bundle_add_page',
      'page arguments' => array($this->entityType),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_ACTION,
    ) + $defaults;
    $items[$this->path . '/add/%'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType, $id_count + 1),
      'page callback' => 'entity_ui_get_bundle_add_form',
      'page arguments' => array($this->entityType, $id_count + 1),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
    ) + $defaults;
    $items[STUDYROOM_SPACE_PATH . $wildcard] = array(
      'title callback' => 'entity_ui_get_page_title',
      'title arguments' => array('view', $this->entityType, 1),
      'page callback' => 'entity_ui_entity_page_view',
      'page arguments' => array(1),
      'load arguments' => array($this->entityType),
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType, 1),
    );
    $items[STUDYROOM_SPACE_PATH . $wildcard . '/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'load arguments' => array($this->entityType),
      'weight' => -10,
    );
    $items[STUDYROOM_SPACE_PATH . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'studyroom_space_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'entity_access',
      'access arguments' => array('edit', $this->entityType),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    ) + $defaults;
    $items[STUDYROOM_SPACE_PATH . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'studyroom_space_delete_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'entity_access',
      'access arguments' => array('delete', $this->entityType),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
    ) + $defaults;

    $items['admin/config/studyroom/space'] = array(
      'title' => 'Space',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('studyroom_space_settings_form'),
      'access arguments' => array('administer studyroom_space locations'),
    ) + $defaults;

    return $items;
  }
}

/**
 * Form callback wrapper: create or edit a space.
 *
 * @param object $entity
 *   The space object being edited by this form.
 *
 * @see studyroom_space_edit_form()
 */
function studyroom_space_form_wrapper($entity) {
  return drupal_get_form('studyroom_space_form', $entity);
}

/**
 * Form callback wrapper: delete a space.
 *
 * @param object $entity
 *   The space object being deleted by this form.
 *
 * @see studyroom_space_delete_form()
 */
function studyroom_space_delete_form_wrapper($entity) {
  return drupal_get_form('studyroom_space_delete_form', $entity);
}

/**
 * Form callback: create or edit a space.
 */
function studyroom_space_form($form, &$form_state, $entity) {
  if (!isset($form_state['studyroom_space'])) {
    $form_state['studyroom_space'] = $entity;
  }

  if (isset($entity->is_new)) {
    drupal_set_title(t('Add new space'));
  }
  else {
    drupal_set_title(t('Edit: %title', array('%title' => $entity->label())), PASS_THROUGH);
  }

  // Add the default field elements.
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Space label'),
    '#default_value' => isset($entity->label) ? $entity->label : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -50,
  );
  $form['capacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Capacity'),
    '#size' => 4,
    '#default_value' => isset($entity->capacity) ? $entity->capacity : '',
    '#description' => t('How many people can this room hold.'),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $langcode = entity_language('studyroom_space', $entity);
  $form['language'] = array(
    '#type' => 'value',
    '#value' => isset($langcode) ? $langcode : LANGUAGE_NONE,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Node options for administrators.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Publishing options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('node-form-options'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'node') . '/node.js'),
    ),
    '#weight' => 95,
  );
  $form['options']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Published'),
    '#default_value' => isset($entity->status) ? $entity->status : 1,
  );

  $studyroom = new Studyroom();
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
    '#description' => t("Override the minimum duration for a reservation for this space.
      If no value is specified, the value of %value will be used.", array(
        '%value' => format_interval($studyroom->getMinDuration($entity, NULL, TRUE)))),
  );
  $form['rules']['max_duration'] = array(
    '#type' => 'select',
    '#title' => t('Set maximum duration'),
    '#default_value' => isset($entity->max_duration) ? $entity->max_duration : '',
    '#options' => $studyroom->optionsMaxDuration(),
    '#description' => t("Override the maximum duration for a reservation for this space.
      If no value is specified, the value of %value will be used.", array(
        '%value' => format_interval($studyroom->getMaxDuration($entity, NULL, TRUE)))),
  );
  $form['rules']['max_days_advance'] = array(
    '#type' => 'select',
    '#title' => t('Set maximum days in advance'),
    '#default_value' => isset($entity->max_days_advance) ? $entity->max_days_advance : '',
    '#options' => $studyroom->optionsMaxDayAdvance(),
    '#description' => t("Override the maximum days in advance a reservation can be made for this space.
      If no value is specified, the value of %value will be used.", array(
        '%value' => studyroom_format_day($studyroom->getMaxDaysAdvance($entity, NULL, TRUE)))),
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save space'),
    '#submit' => $submit + array('studyroom_space_form_submit'),
  );

  if (!empty($entity->space_id) && entity_access('delete', 'studyroom_space', $entity)) {
    $form['actions']['delete'] = array(
      '#title' => 'delete',
      '#type' => 'submit',
      '#value' => t('Delete space'),
      '#submit' => array('studyroom_space_form_submit_delete'),
      '#limit_validation_errors' => array(),
    );
  }

  if (isset($entity->space_id)) {
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), STUDYROOM_SPACE_PATH . $entity->space_id),
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'studyroom_space_form_validate';
  field_attach_form('studyroom_space', $entity, $form, $form_state, $langcode);
  return $form;
}

/**
 * Form API validate callback for the studyroom_space form.
 */
function studyroom_space_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  $entity = $form_state['studyroom_space'];

  $studyroom = new Studyroom();
  $studyroom->validateDurations($form, $form_state, 'studyroom_space', $entity);

  if (empty($values['max_days_advance'])) {
    form_set_value($form['rules']['max_days_advance'], NULL, $form_state);
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('studyroom_space', $entity, $form, $form_state);
}


/**
 * Form API submit callback for the studyroom_space form.
 *
 * Save the space and go back to the list of spaces.
 *
 * @todo remove hard-coded link
 */
function studyroom_space_form_submit(&$form, &$form_state) {
  $space = entity_ui_controller('studyroom_space')->entityFormSubmitBuildEntity($form, $form_state);

  // Add in created and changed times.
  if ($space->is_new = isset($space->is_new) ? $space->is_new : 0) {
    $space->created = time();
  }
  $space->changed = time();

  $space->save();
  $form_state['redirect'] = STUDYROOM_SPACE_MENU_SPACE;
}

/**
 * Form API submit callback for the delete button.
 *
 * Unset destination to fix views destination issue.
 */
function studyroom_space_form_submit_delete(&$form, &$form_state) {
  if (isset($_GET['destination'])) {
    unset($_GET['destination']);
  }
  $form_state['redirect'] = STUDYROOM_SPACE_PATH . $form_state['studyroom_space']->space_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a space.
 *
 * @see confirm_form()
 */
function studyroom_space_delete_form($form, &$form_state, $entity) {
  $form_state['studyroom_space'] = $entity;
  $form['#submit'][] = 'studyroom_space_delete_form_submit';
  $form = confirm_form($form,
    t('Are you sure you want to delete space %label?', array('%label' => $entity->label())),
    STUDYROOM_SPACE_MENU_SPACE,
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for studyroom_space_delete_form.
 */
function studyroom_space_delete_form_submit($form, &$form_state) {
  $entity = $form_state['studyroom_space'];
  studyroom_space_delete($entity);

  drupal_set_message(t('The space %label has been deleted.', array('%label' => $entity->label())));
  watchdog('space', 'Deleted space %label.', array('%label' => $entity->label()));

  $form_state['redirect'] = STUDYROOM_SPACE_MENU_SPACE;
}

/**
 * Configuration settings form.
 */
function studyroom_space_settings_form() {
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display'),
    '#description' => t('Customize the display of Studyroom Spaces.'),
  );
  $form['display']['studyroom_space_title_location'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include location in space page title.'),
    '#default_value' => variable_get('studyroom_space_title_location', 0),
  );

  return system_settings_form($form);
}

