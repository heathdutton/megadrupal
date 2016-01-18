<?php

/**
 * @file
 * This file contains functions pertaining to user setting page for module namecards
 *
 * Copyright 2010 Ben Gunn
 *
 * Licensed under the GNU Public License
 */

/**
 * Define user settings form
 */
function namecards_user_settings_form() {
  global $user;
  $form = array();

  // Add js libraries required for ctools modal.
  _namecards_ctools_modal_parent();
  
  // Calculate default value
  $user_id = $user->uid;
  $results = db_query('SELECT * FROM {user_namecard_settings} WHERE uid = :uid', array(':uid' => $user_id));
  $result = array();
  foreach ($results as $record) {
    // Convert result object to an array.
    $result = (array)$record;
  }

  switch ($result['default_page']) {
    case NAMECARDS_DEFAULT_PAGE_BROWSE: // 'browse'
      $default_page = NAMECARDS_DEFAULT_PAGE_BROWSE;
      break;
    case NAMECARDS_DEFAULT_PAGE_ORGS: // 'organizations'
      $default_page = NAMECARDS_DEFAULT_PAGE_ORGS;
      break;
    case NAMECARDS_DEFAULT_PAGE_EVENTS: // 'events'
      $default_page = NAMECARDS_DEFAULT_PAGE_EVENTS;
      break;
  }

  $form['default_page'] = array(
    '#type' => 'select',
    '#title' => t('Default page to display when you click <em>Namecards</em> link'),
    '#default_value' => $default_page,
    '#options' => array(
      NAMECARDS_DEFAULT_PAGE_BROWSE => t('Browse'),
      NAMECARDS_DEFAULT_PAGE_ORGS => t('Organizations'),
      NAMECARDS_DEFAULT_PAGE_EVENTS => t('Events'),
    ),
  );

  // Calculate default value
  $default_checked_checkboxes = array();
  while (list($key, $val) = each($result)) {
    if ($key != 'uid' && $key != 'default_page' && $val == 1) {
      $default_checked_checkboxes[] = t('@key', array('@key' => $key));
    }
  }

  $form['browse_contacts_view_settings'] = array(
    '#type' => 'checkboxes',
//    '#title' => t('Select the fields to display for the selected tabs'),
    '#default_value' => $default_checked_checkboxes,
    '#options' => array(
      'browse_surname' => '',
      'browse_given_name' => '',
      'browse_nickname' => '',
      'browse_organization' => '',
      'browse_position' => '',
      'browse_department' => '',
      'browse_email' => '',
      'browse_mobile' => '',
      'browse_phone' => '',
      'browse_fax' => '',
      'browse_address' => '',
      'browse_event' => '',
    ),
  );
  $form['contacts_by_org_view_settings'] = array(
    '#type' => 'checkboxes',
//    '#title' => t('Fields to display when listing contacts in <em>Organizations</em> tab'),
    '#default_value' => $default_checked_checkboxes,
    '#options' => array(
      'contact_by_org_surname' => '',
      'contact_by_org_given_name' => '',
      'contact_by_org_nickname' => '',
      'contact_by_org_organization' => '',
      'contact_by_org_position' => '',
      'contact_by_org_department' => '',
      'contact_by_org_email' => '',
      'contact_by_org_mobile' => '',
      'contact_by_org_phone' => '',
      'contact_by_org_fax' => '',
      'contact_by_org_address' => '',
      'contact_by_org_event' => '',
    ),
  );
  $form['contacts_by_event_view_settings'] = array(
    '#type' => 'checkboxes',
//    '#title' => t('Fields to display when listing contacts in <em>Events</em> tab'),
    '#default_value' => $default_checked_checkboxes,
    '#options' => array(
      'contact_by_event_surname' => '',
      'contact_by_event_given_name' => '',
      'contact_by_event_nickname' => '',
      'contact_by_event_organization' => '',
      'contact_by_event_position' => '',
      'contact_by_event_department' => '',
      'contact_by_event_email' => '',
      'contact_by_event_mobile' => '',
      'contact_by_event_phone' => '',
      'contact_by_event_fax' => '',
      'contact_by_event_address' => '',
      'contact_by_event_event' => '',
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('namecards_user_settings_form_submit'),
    '#weight' => 99,
  );
  $form['defaults'] = array(
    '#type' => 'submit',
    '#value' => t('Reset defaults'),
    '#submit' => array('namecards_user_settings_form_defaults'),
    '#weight' => 100,
  );
  $form['#theme'] = 'namecards_user_settings_theme';
  return $form;
}

/**
 * Submit function for user settings form
 */
function namecards_user_settings_form_submit($form, &$form_state) {
  global $user;
  $new_values = new stdClass();
  $new_values->uid = $user->uid;
  $new_values->default_page = $form_state['values']['default_page'];
  $new_values->browse_surname = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_surname']);
  $new_values->browse_given_name = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_given_name']);
  $new_values->browse_nickname = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_nickname']);
  $new_values->browse_organization = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_organization']);
  $new_values->browse_position = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_position']);
  $new_values->browse_department = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_department']);
  $new_values->browse_email = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_email']);
  $new_values->browse_mobile = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_mobile']);
  $new_values->browse_phone = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_phone']);
  $new_values->browse_fax = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_fax']);
  $new_values->browse_address = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_address']);
  $new_values->browse_event = _namecards_user_settings_get_boolean_int($form_state['values']['browse_contacts_view_settings']['browse_event']);
  $new_values->contact_by_org_surname = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_surname']);
  $new_values->contact_by_org_given_name = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_given_name']);
  $new_values->contact_by_org_nickname = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_nickname']);
  $new_values->contact_by_org_organization = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_organization']);
  $new_values->contact_by_org_position = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_position']);
  $new_values->contact_by_org_department = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_department']);
  $new_values->contact_by_org_email = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_email']);
  $new_values->contact_by_org_mobile = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_mobile']);
  $new_values->contact_by_org_phone = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_phone']);
  $new_values->contact_by_org_fax = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_fax']);
  $new_values->contact_by_org_address = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_address']);
  $new_values->contact_by_org_event = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_org_view_settings']['contact_by_org_event']);
  $new_values->contact_by_event_surname = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_surname']);
  $new_values->contact_by_event_given_name = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_given_name']);
  $new_values->contact_by_event_nickname = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_nickname']);
  $new_values->contact_by_event_organization = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_organization']);
  $new_values->contact_by_event_position = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_position']);
  $new_values->contact_by_event_department = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_department']);
  $new_values->contact_by_event_email = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_email']);
  $new_values->contact_by_event_mobile = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_mobile']);
  $new_values->contact_by_event_phone = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_phone']);
  $new_values->contact_by_event_fax = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_fax']);
  $new_values->contact_by_event_address = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_address']);
  $new_values->contact_by_event_event = _namecards_user_settings_get_boolean_int($form_state['values']['contacts_by_event_view_settings']['contact_by_event_event']);

  $result = drupal_write_record('user_namecard_settings', $new_values, 'uid');
  if ($result == FALSE) { // See "http://api.drupal.org/api/drupal/includes--common.inc/function/drupal_write_record/6"
    drupal_set_message('User namecard settings not updated', 'error');
  }
  elseif ($result == SAVED_UPDATED) { // See "http://api.drupal.org/api/drupal/includes--common.inc/function/drupal_write_record/6"
    drupal_set_message('User namecard settings updated');
  }

  // Clear user's cached views data.
  _namecards_delete_users_cached_data($user->uid);

  // Update session variable containing user settings info
  _namecards_update_user_settings_session_variables($new_values);
}

/**
 * updates session variable containing user settings data
 */
function _namecards_update_user_settings_session_variables($values) {
  if (isset($_SESSION['namecards_user_namecard_settings'])) {
    $_SESSION['namecards_user_namecard_settings'] = $values;
  }
}

/**
 * Sets the user settings back to their default values
 */
function namecards_user_settings_form_defaults($form, &$form_state) {
  $new_values = new stdClass();
  global $user;

  $new_values->uid = $user->uid;
  $new_values->default_page = 1;
  $new_values->browse_surname = 1;
  $new_values->browse_given_name = 1;
  $new_values->browse_nickname = 1;
  $new_values->browse_organization = 1;
  $new_values->browse_position = 1;
  $new_values->browse_department = 0;
  $new_values->browse_email = 1;
  $new_values->browse_mobile = 1;
  $new_values->browse_phone = 1;
  $new_values->browse_fax = 1;
  $new_values->browse_address = 0;
  $new_values->browse_event = 0;
  $new_values->contact_by_org_surname = 1;
  $new_values->contact_by_org_given_name = 1;
  $new_values->contact_by_org_nickname = 1;
  $new_values->contact_by_org_organization = 1;
  $new_values->contact_by_org_position = 1;
  $new_values->contact_by_org_department = 0;
  $new_values->contact_by_org_email = 1;
  $new_values->contact_by_org_mobile = 1;
  $new_values->contact_by_org_phone = 1;
  $new_values->contact_by_org_fax = 1;
  $new_values->contact_by_org_address = 0;
  $new_values->contact_by_org_event = 0;
  $new_values->contact_by_event_surname = 1;
  $new_values->contact_by_event_given_name = 1;
  $new_values->contact_by_event_nickname = 1;
  $new_values->contact_by_event_organization = 1;
  $new_values->contact_by_event_position = 1;
  $new_values->contact_by_event_department = 0;
  $new_values->contact_by_event_email = 1;
  $new_values->contact_by_event_mobile = 1;
  $new_values->contact_by_event_phone = 1;
  $new_values->contact_by_event_fax = 1;
  $new_values->contact_by_event_address = 0;
  $new_values->contact_by_event_event = 0;
  $result = drupal_write_record('user_namecard_settings', $new_values, 'uid');
  if ($result == 0) { // See "http://api.drupal.org/api/drupal/includes--common.inc/function/drupal_write_record/6"
    drupal_set_message('User record not updated', 'error');
  }
  elseif ($result == SAVED_NEW || $result == SAVED_UPDATED) { // See "http://api.drupal.org/api/drupal/includes--common.inc/function/drupal_write_record/6"
    drupal_set_message('User record updated');
  }

  // Update session variable containing user settings info
  _namecards_update_user_settings_session_variables($new_values);
}

/**
 * Convert to an integer representation of true or false
 *
 * @param $value
 *   Value of checkbox submitted by form. Can be either String or Int
 *   depending on whether checkbox was checked or not respectively.
 */
function _namecards_user_settings_get_boolean_int($value) {
  if (is_string($value)) {
    $int = 1;
  }
  else {
    $int = 0;
  }
  return $int;
}

/**
 * Theme function for user settings form
 */
function theme_namecards_user_settings_theme($variables) {
  $form = $variables['form'];

  // Set table headers.
  $header = array(t('Field'), t('Browse'), t('Organizations'), t('Events'));

  // Set table rows.
  $rows = array(
    array(
      t('Surname'),
      drupal_render($form['browse_contacts_view_settings']['browse_surname']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_surname']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_surname']),
    ),
    array(
      t('Given name'),
      drupal_render($form['browse_contacts_view_settings']['browse_given_name']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_given_name']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_given_name']),
    ),
    array(
      t('Nickname'),
      drupal_render($form['browse_contacts_view_settings']['browse_nickname']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_nickname']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_nickname']),
    ),
    array(
      t('Organization'),
      drupal_render($form['browse_contacts_view_settings']['browse_organization']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_organization']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_organization']),
    ),
    array(
      t('Position'),
      drupal_render($form['browse_contacts_view_settings']['browse_position']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_position']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_position']),
    ),
    array(
      t('Department'),
      drupal_render($form['browse_contacts_view_settings']['browse_department']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_department']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_department']),
    ),
    array(
      t('Email'),
      drupal_render($form['browse_contacts_view_settings']['browse_email']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_email']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_email']),
    ),
    array(
      t('Mobile'),
      drupal_render($form['browse_contacts_view_settings']['browse_mobile']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_mobile']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_mobile']),
    ),
    array(
      t('Phone'),
      drupal_render($form['browse_contacts_view_settings']['browse_phone']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_phone']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_phone']),
    ),
    array(
      t('Fax'),
      drupal_render($form['browse_contacts_view_settings']['browse_fax']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_fax']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_fax']),
    ),
    array(
      t('Address'),
      drupal_render($form['browse_contacts_view_settings']['browse_address']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_address']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_address']),
    ),
    array(
      t('Event'),
      drupal_render($form['browse_contacts_view_settings']['browse_event']),
      drupal_render($form['contacts_by_org_view_settings']['contact_by_org_event']),
      drupal_render($form['contacts_by_event_view_settings']['contact_by_event_event']),
    ),

  );

  // Set table attributes.
  $attributes = array(
    'class' => array('namecards-user-settings-table'),
  );

  $output = '';
  $output .= drupal_render($form['default_page']);
  $output .= '<div class="form-item">';
  $output .= '<label >' . t('Select the fields to display for the selected tabs:') . '</label>';
  $output .= '</div>';
  $output .= '<div class="namecards-user-settings-table-wrapper">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  $output .= '</div>';
  $output .= drupal_render_children($form);
  return $output;
}