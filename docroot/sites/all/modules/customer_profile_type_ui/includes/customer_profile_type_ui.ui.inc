<?php

/**
 * @file
 * Page callbacks and form builder functions for administering customer profiles type ui.
 */


/**
 * Form callback wrapper: create or edit a customer profile type.
 */
function customer_profile_type_ui_add_page() {
  // Add the breadcrumb for the form's location.
  //commerce_customer_ui_set_breadcrumb();

  return drupal_get_form('customer_profile_type_ui_add_form');
}
 
 
/**
 * Form callback: create a customer profile type.
 */
function customer_profile_type_ui_add_form($form, &$form_state) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'customer_profile_type_ui') . '/includes/customer_profile_type_ui.ui.inc';

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter a human readable name for this new customer profile type.'),
    '#maxlength' => 60,
    '#weight' => -1,
  );

  // Have to limit this to 13 chars since the customer profile type reference field
  // that is auto added to the order type appends 'commerce_customer_' on the front
  // and field machine names must also be less than 32 chars.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#title' => t('Type'),
    '#description' => t('Enter a machine name for this new customer profile type. Has to be alpha-numeric/underscores and less then 13 characters.'),
    '#maxlength' => 13,
    '#weight' => 5,
    '#machine_name' => array(
      'exists' => '_customer_profile_type_ui_type_exists',
    ),
  );

  $form['addressfield'] = array(
    '#type' => 'radios',
    '#title' => t('Include the default Address field'),
    '#description' => t('If you plan on having an address field in this profile type then it is recommended that you select \'yes\' to use the default that comes with customer profile types. This will ensure that address fields are kept in sync accross customer profile types.'),
    '#options' => array(0 => 'no', 1 => 'yes'),
    '#default_value' => 0,
    '#weight' => 10,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('Describe the new Customer profile type. This will show up on admin pages.'),
    '#maxlength' => 255,
    '#weight' => 15,
  );

  $form['help'] = array(
    '#type' => 'textfield',
    '#title' => t('Help text'),
    '#description' => t('Enter some help text to aid users in filling out the customer profile form on checkout.'),
    '#maxlength' => 255,
    '#weight' => 20,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 100,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile type'),
    '#submit' => $submit + array('customer_profile_type_ui_add_form_submit'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link', 
    '#title' => t('Cancel'), 
    '#href' => 'admin/commerce/customer-profiles/types',
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'customer_profile_type_ui_form_validate';

  return $form;
}

/**
 * Form callback: Edit a customer profile type.
 */
function customer_profile_type_ui_edit_form($form, &$form_state) {
  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  // Might as well reuse what we got from the create form.
  $form += customer_profile_type_ui_add_form($form, $form_state);

  $form['name']['#default_value'] = $form_state['build_info']['profile_type']['name'];
  $form['description']['#default_value'] = $form_state['build_info']['profile_type']['description'];
  $form['help']['#default_value'] = $form_state['build_info']['profile_type']['help'];
  $form['addressfield']['#default_value'] = ($form_state['build_info']['profile_type']['addressfield']) ? 1 : 0;

  $form['type'] = array(
    '#title' => t('Type'),
    '#type' => 'item',
    '#markup' => $form_state['build_info']['profile_type']['type'],
    '#description' => t('The machine name for this customer profile type. This cannot change now that it has been created.'),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update profile type'),
    '#submit' => $submit + array('customer_profile_type_ui_edit_form_submit'),
  );

  return $form;
}


/**
 * Validation callback for customer_profile_type_ui_customer_profile_type_form().
 */
function customer_profile_type_ui_form_validate($form, &$form_state) {

  if (((int)$form_state['values']['addressfield'] < 0) || ((int)$form_state['values']['addressfield'] > 1)) form_set_error('addressfield', t('You must choose a valid addressfield option.'));
  
  if (isset($form_state['values']['type']) && is_numeric($form_state['values']['type'])) {
    form_set_error('type', t('Machine-readable names must not consist of numbers only.'));
  }
}


/**
 * Submit callback for customer_profile_type_ui_customer_profile_type_form().
 */
function customer_profile_type_ui_add_form_submit($form, &$form_state){
  $result = db_insert('customer_profile_type_ui')
    ->fields(array(
      'type' => check_plain($form_state['values']['type']),
      'name' => check_plain($form_state['values']['name']),
      'addressfield' => ((int)$form_state['values']['addressfield']) ? 1 : 0,
      'description' => check_plain($form_state['values']['description']),
      'help' => check_plain($form_state['values']['help']),
    ))
    ->execute();

  // Configure the new customer profile type.
  commerce_customer_configure_customer_fields(array('customer_profile_type_ui'));
  commerce_order_configure_order_fields(array('customer_profile_type_ui'));
  menu_rebuild();
  // Redirect based on the button clicked.
  drupal_set_message(t('Customer profile type \'!type\' has been created.', array('!type' => $form_state['values']['type'])));
  drupal_goto('admin/commerce/customer-profiles/types');
}


/**
 * Submit callback for customer_profile_type_ui_customer_profile_type_form().
 */
function customer_profile_type_ui_edit_form_submit($form, &$form_state){
  $fields = array();
  foreach (array('name', 'description', 'help') as $field) {
    if ($form_state['build_info']['profile_type'][$field] != $form_state['values'][$field]) {
      $fields[$field] = check_plain($form_state['values'][$field]);
    }
  }
  $address_field = ((int)$form_state['values']['addressfield']) ? 1 : 0;
  if ($form_state['build_info']['profile_type']['addressfield'] != $address_field) {
    $fields['addressfield'] = $address_field;
  }
  
  if (!empty($fields)) {
    $result = db_update('customer_profile_type_ui')
      ->fields($fields)
      ->condition('type', $form_state['build_info']['profile_type']['type'])
      ->execute();
    if (!empty($fields['addressfield'])) {
      // Address field was toggled on, so lets make sure it is attached to the profile type.
      commerce_customer_configure_customer_fields(array('customer_profile_type_ui'));
    } else if (isset($fields['addressfield'])) {
      // Address field was toggled off, but we don't want to delete the field on the
      // assumption the user meant to do this. It could be a mistake. Notify them.
      // TODO: add a link to the manage fields page for this type.
      drupal_set_message(t('The address field setting for this profile was turned off. However if you want to remove the address field from the profile you will still have to go to the \'manage fields\' page for this profile type and delete it manually.'), 'warning');
    }
    drupal_set_message(t('Customer profile type \'!type\' has been updated.', array('!type' => $form_state['build_info']['profile_type']['type'])));
  } else {
    drupal_set_message(t('There was nothing to update for the Customer profile type \'!type\'. Settings remain unchanged.', array('!type' => $form_state['build_info']['profile_type']['type'])));
  }

  drupal_goto('admin/commerce/customer-profiles/types');
}


/**
 * Callback to determine if the customer profile type already exists.
 */
function _customer_profile_type_ui_type_exists($type) {
  return array_key_exists ($type , commerce_customer_profile_types());
}


/**
 * Callback to edit a customer profile type.
 * 
 * @param $profile_type
 *   The profile type to edit.
 */
function customer_profile_type_ui_edit_page($type) {
  $profile_type = commerce_customer_profile_type_load($type);
  if ($profile_type) {
    // Add the breadcrumb for the form's location.
    //commerce_customer_ui_set_breadcrumb();
    
    $form_state = array();
    $form_state['build_info']['args'] = array();
    $form_state['build_info']['profile_type'] = $profile_type;

    $form = drupal_build_form('customer_profile_type_ui_edit_form', $form_state);
    return $form;
  }
  drupal_goto('admin/commerce/customer-profiles/types');
}


/**
 * Callback to delete a customer profile type.
 * 
 * @param $profile_type
 *   The profile type to delete.
 */
function customer_profile_type_ui_delete_page($type) {
  $profile_type = commerce_customer_profile_type_load($type);
  if ($profile_type) {
    $form_state = array('profile_type' => $profile_type);
    return drupal_build_form('customer_profile_type_ui_confirm_delete_form', $form_state);
  }
  drupal_goto('admin/commerce/customer-profiles/types');
}

/**
 * Confirmation form for deleting a customer profile type.
 */
function customer_profile_type_ui_confirm_delete_form($form, &$form_state) {
  return confirm_form($form, t('Are you certain you want to delete the \'%profile_type\' customer profile type?', array('%profile_type' => $form_state['profile_type']['type'])), 'admin/commerce/customer-profiles/types', t('By clicking \'<em>confirm</em>\' below you will permanently delete the \'%profile_type\' profile type. This means that all profiles of that type in existence will also deleted <strong>PERMANENTLY</strong>.', array('%profile_type' => $form_state['profile_type']['type'])), t('Confirm'));
}

/*
 * Submit callback for deleting a customer profile type.
 */
function customer_profile_type_ui_confirm_delete_form_submit($form, &$form_state) {
  $profile_type = $form_state['profile_type'];
  // Delete the respective reference field on the order entity, all profiles
  // of that type, the profile type itself, and all of the fields attached to it. 
  if (_customer_profile_type_ui_profile_delete($profile_type)) {
    // Allow other modules to react to this profile type being deleted.
    module_invoke_all('customer_profile_type_ui_delete', $profile_type);
    // Clear the customer profile type and entity type caches.
    commerce_customer_profile_types_reset();
    // Rebuild the cache now... not sure if this is entirely necessary. Probably done
    // anyway immediately after this.
    commerce_customer_profile_types();
    menu_rebuild();
    drupal_set_message(t('Customer profile type \'!type\' has been deleted.', array('!type' => $form_state['profile_type']['type'])));
  } else {
    drupal_set_message(t('Customer profile type \'!type\' could not be deleted.', array('!type' => $form_state['profile_type']['type'])), 'error');
  }
  drupal_goto('admin/commerce/customer-profiles/types');
}

/*
 * Removes all information related to a Customer Profile Type UI profile type.
 *
 * The following is what has to happen:
 *
 *   1. Remove all the respective profile reference fields from the order entity.
 *   2. Delete all the stored profile entities from the db.
 *   3. Remove all fields from each profile type.
 *   4. Delete the profile types from the db.
 *   5. We're done.
 *
 * @param $profile_type
 *   The profile type to delete permanently.
 *
 */
function _customer_profile_type_ui_profile_delete($profile_type) {
  if ($profile_type) {
    // Loop through the fields looking for customer profile reference fields
    // that reference this customer profile type.
    foreach (field_read_fields(array('type' => 'commerce_customer_profile_reference')) as $field_name => $field) {
      if ($field['type'] == 'commerce_customer_profile_reference') {
        if (!empty($field['settings']['profile_type']) && $field['settings']['profile_type'] == $profile_type['type']) {
          field_delete_field($field_name);
        }
      }
    }
    // Delete all the profiles of this type.
    $sql = 'SELECT profile_id as pid FROM {commerce_customer_profile} p WHERE p.type = :type';
    $result = db_query($sql, array(':type' => $profile_type['type']));
    $profile_ids = array();
    foreach ($result as $row) {
      $profile_ids[] = $row->pid;
    }
    // TODO: This may need to be done in a batch process.
    commerce_customer_profile_delete_multiple($profile_ids);

    // Loop over any remaining field instances attached to the profile type
    // and delete them individually.
    $instances = field_info_instances('commerce_customer_profile', $profile_type['type']);
    foreach ($instances as $instance_name => $instance) {
      field_delete_instance($instance);
    }
    // Now delete the type definition from the db.
    db_delete('customer_profile_type_ui')
      ->condition('type', $profile_type['type'])
      ->execute();
    // Ensure that any remaining info or field instances related to this profile
    // type get removed.
    field_attach_delete_bundle('commerce_customer_profile', $profile_type['type']);
    return TRUE;
  }
  return FALSE;
}
