<?php

/**
 * Allow administrators to map mailing list fields to user object properites.
 * The user object will be defined by a view that takes a uid as an argument.
 */
function vr_list_member_map($form, $form_state, $list = NULL) {
  // Use default values if no list is provided
  if (empty($list)) {
    $list = _vr_default_list();
  }

  // Retrieve views
  $all_views = views_get_all_views();
  foreach ($all_views as $view) {
    // Only 'node' views that have fields will work for our purpose.
    if (!empty($view->display['default']->display_options['fields'])) {
      if ($view->type == 'Default') {
        $views[t('Default Views')][$view->name] = $view->name;
      }
      else {
        $views[t('Existing Views')][$view->name] = $view->name;
      }
    }
  }

  // Look for a view to map.
  $view = variable_get('vr_list_view_' . $list->id, NULL);
  // Allow administrator to select a view with to map list fields.
  $form['view'] = array(
    '#type' => 'fieldset',
    '#title' => t('Local User Object'),
  );
  $form['view']['vr_list_view_' . $list->id] = array(
    '#type' => 'select',
    '#title' => t('Select a view for the user to list field map.'),
    '#description' => t("The selected view should return all the fields required for
      this list's field map.  It should take one argument uid."),
    '#options' => array(NULL => '-- Select One --') + $views,
    '#default_value' => $view,
  );
  $form['view']['vr_list_subcribe_throttle_' . $list->id] =   array(
    '#type' => 'textfield',
    '#title' => t('Subscription Throttle'),
    '#description' => t('Set a throttle for list members to subscribe in a single
      batch execution.  This number can be much higher than the update throttle because
      users are subscribed in a single API transaction.'),
    '#default_value' => variable_get('vr_list_subscribe_throttle_' . $list->id, 100),
  );
  $form['view']['vr_list_throttle_' . $list->id] = array(
    '#type' => 'textfield',
    '#title' => t('Update Throttle'),
    '#description' => t('When requested, Drupal will update list member information
      before sending out an email.  This field allows one to specify the number of records
      to be updated per batch execution.  This number will likely be much lower than
      the subscription throttle because one API call is required per record.'),
    '#default_value' => variable_get('vr_list_throttle_' . $list->id, 10),
  );
  // Provide field mapping objects if and only if a view has been selected and saved.
  $view = variable_get('vr_list_view_' . $list->id, NULL);
  if (!empty($view)) {
    $view = views_get_view($view);
    $form['fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field Map'),
    );
    // Retrieve fields from selected view.  TODO: there must be a better way of doing
    // this!
    $view = $view->load($view->name);
    $fields_raw = $view->display['default']->display_options['fields'];
    foreach ($fields_raw as $name => $field) {
      $fields[$name] = (empty($field['label'])) ? $name : $field['label'];
    }
    // Get default values
    $values = variable_get('vr_list_view_map_' . $list->id, array());
    foreach ($list->fields as $key => $field) {
      // TODO: manage skip fields with a constant.
      $skip = array(
        'id',
        'optin_status',
        'optin_status_last_updated',
        'create_date',
        'last_updated',
      );
      if (in_array($field, $skip)) {
        continue;
      }
      $title = str_replace('_', ' ', $field);
      $form['fields']['field_map'][$key] = array(
        '#type' => 'select',
        '#title' => ucwords($title),
        '#options' => array('-- Select One --') + $fields,
        '#default_value' => $values[$key],
      );
    }
    $form['fields']['field_map']['#tree'] = TRUE;
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for field mapping utility.
 */
function vr_list_member_map_submit($form, &$form_state) {
  $list = $form['#parameters'][2];
  // Use default values if no list is provided
  if (empty($list)) {
    $list = _vr_default_list();
  }

  $values = $form_state['values'];

  $current_view = variable_get('vr_list_view_' . $list->id, NULL);
  $message = t('The settings have been saved. Please configure the fields below.');
  drupal_set_message($message);
  variable_set('vr_list_view_' . $list->id, $values['vr_list_view_' . $list->id]);
  variable_set('vr_list_throttle_' . $list->id, $values['vr_list_throttle_' . $list->id]);
  if ($current_view != $values['vr_list_view_' . $list->id]) {
    // A new view has been selected for this field map.  Clear stale settings.
    variable_set('vr_list_view_map_' . $list->id, array());
  }
  else {
    // A new view has been selected for this field map.  Clear stale settings.
    variable_set('vr_list_view_map_' . $list->id, $values['field_map']);
  }
}

/**
 * Return a basic VerticalResponse list. With only an id of 'default'
 * and the default fields.  This list can be used when one wants to manage
 * lists generally, without regard to specific lists.
 *
 * @return object
 *  mock VerticalResponse list object.
 */
function _vr_default_list() {
  $list = (object) NULL;
  $list->id = 'default';
  $list->fields = array(
    'id',
    'create_date',
    'last_updated',
    'optin_status',
    'optin_status_last_updated',
    'email_address',
    'first_name',
    'last_name',
    'address_1',
    'address_2',
    'city',
    'state',
    'postalcode',
    'country',
    'work_phone',
    'home_phone',
    'mobile_phone',
    'fax',
    'gender',
    'marital_status',
    'ip_address',
    'company_name',
    'title',
    'hash',
    'address_hash',
  );
  return $list;
}
