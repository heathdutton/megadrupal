<?php
/**
 * @file
 * Stamps.com administration menu items.
 */

/**
 * Generate the form to configure settings for the Stamps.com module.
 *
 * @see uc_stamps_menu()
 * @ingroup forms
 */
function uc_stamps_config_form($form, &$form_state) {
  // Set for sample mode.
  $form['uc_stamps_print_samples'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sample mode'),
    '#description' => t('Print sample labels for testing.'),
    '#default_value' => variable_get('uc_stamps_print_samples', TRUE),
  );

  // Test mode.
  $form['uc_stamps_test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test mode'),
    '#description' => t('Importing and exporting do not update the order database.'),
    '#default_value' => variable_get('uc_stamps_test_mode', FALSE),
  );

  // Default domestic shipping mode.
  $domestic_modes = uc_stamps_domestic_modes();
  $domestic_choices = array();
  foreach ($domestic_modes as $mode_id => $mode) {
    $domestic_choices[$mode_id] = $mode->name;
  }
  $form['uc_stamps_default_domestic_mode'] = array(
    '#type' => 'select',
    '#title' => t('Default Domestic Shipping Mode'),
    '#options' => $domestic_choices,
    '#description' => t('The shipping mode to use for domestic orders if none match.'),
    '#default_value' => variable_get('uc_stamps_default_domestic_mode', 0),
  );

  // Default foreign shipping mode.
  $foreign_modes = uc_stamps_foreign_modes();
  $foreign_choices = array();
  foreach ($foreign_modes as $mode_id => $mode) {
    $foreign_choices[$mode_id] = $mode->name;
  }
  $form['uc_stamps_default_foreign_mode'] = array(
    '#type' => 'select',
    '#title' => t('Default Foreign Shipping Mode'),
    '#options' => $foreign_choices,
    '#description' => t('The shipping mode to use for foreign orders if none match.'),
    '#default_value' => variable_get('uc_stamps_default_foreign_mode', 0),
  );

  // Default country of origin (stored as a country code).
  $countries = uc_country_option_list();
  $form['uc_stamps_default_origin_country'] = array(
    '#type' => 'select',
    '#title' => t('Default Country of Origin'),
    '#options' => $countries,
    '#description' => t('The country of origin for products shipped overseas.  This country is used if the individual products or their product classes do not specify a value.'),
    '#default_value' => variable_get('uc_stamps_default_origin_country', 840), // USA
  );

  // E-mail address.
  $form['uc_stamps_email'] = array(
    '#type' => 'textfield',
    '#title' => "E-mail address",
    '#description' => 'E-mail address for Stamps.com notifications.  Leave blank to use store address.',
    '#default_value' => variable_get('uc_stamps_email', ''),
    '#element_validate' => array('uc_stamps_validate_email_element'),
  );

  // Sender e-mail options.
  $form['sender_email_options'] = array(
    '#type' => 'fieldset',
    '#title' => 'Sender e-mail options',
    '#description' => 'Notifications to the configured store e-mail address',
  );
  $form['sender_email_options']['uc_stamps_email_delivery_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delivery notification'),
    '#description' => t('Get e-mail when packages are delivered.'),
    '#default_value' => variable_get('uc_stamps_email_delivery_notification', FALSE),
  );
  $form['sender_email_options']['uc_stamps_email_print_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Print notification'),
    '#description' => t('Get price and tracking number e-mail when labels are printed.'),
    '#default_value' => variable_get('uc_stamps_email_print_notification', FALSE),
  );

  // Recipient e-mail options.
  if (isset($form_state['values']['uc_stamps_email_customer'])) {
    $email_customer_default = $form_state['values']['uc_stamps_email_customer'];
  }
  else {
    $email_customer_default = variable_get('uc_stamps_email_customer', FALSE);
  }
  $form['recipient_email_options'] = array(
    '#type' => 'fieldset',
    '#title' => 'Recipient e-mail options',
    '#prefix' => '<div id="recipient_email_options">',
    '#suffix' => '</div>',
    '#description' => 'Notifications to customers',
  );
  $form['recipient_email_options']['uc_stamps_email_customer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify recipient'),
    '#description' => t('Send e-mail to customers when labels are printed.'),
    '#default_value' => $email_customer_default,
    '#ajax' => array(
      'wrapper' => 'recipient_email_options',
      'callback' => 'uc_stamps_recipient_email_options_callback',
    ),
  );
  $form['recipient_email_options']['uc_stamps_email_company_in_subject'] = array(
    '#type' => $email_customer_default ? 'checkbox' : 'hidden',
    '#title' => t('Company in Subject line'),
    '#description' => t('Use your Stamps.com company name instead of your personal name in e-mail Subject lines.'),
    '#default_value' => variable_get('uc_stamps_email_company_in_subject', TRUE),
  );
  $form['recipient_email_options']['uc_stamps_email_company_in_from'] = array(
    '#type' => $email_customer_default ? 'checkbox' : 'hidden',
    '#title' => t('Company in From line'),
    '#description' => t('Use your Stamps.com company name instead of your personal name in e-mail From lines.'),
    '#default_value' => variable_get('uc_stamps_email_company_in_from', TRUE),
  );

  // Name/company in printed return address.
  $form['uc_stamps_return_address_selection'] = array(
    '#type' => 'radios',
    '#title' => 'Return address name',
    '#default_value' => variable_get('uc_stamps_return_address_selection', 'both'),
    '#options' => array(
      'personal' => t('Personal name only'),
      'company' => t('Company name only'),
      'both' => t('Both personal and company name'),
    ),
    '#description' => 'Select the name or names to appear on the return address on shipping labels.',
  );

  return system_settings_form($form);
}

/**
 * Callback to show/hide recipient e-mail options for uc_stamps_config_form.
 */
function uc_stamps_recipient_email_options_callback($form, &$form_state) {
  return $form['recipient_email_options'];
}

/**
 * Form element validator check for a valid e-mail address.
 *
 * Accepts a blank element or a valid e-mail address.  Raises
 * a form error if there's a problem.
 */
function uc_stamps_validate_email_element($element, &$form_state, $form) {
  if (!empty($element['#value']) && !valid_email_address($element['#value'])) {
    form_error($element, t('Invalid e-mail address.'));
  }
}

/**
 * Returns list of domestic shipping methods.
 *
 * @return array
 *   Array of configured shipping mode objects with domestic classes.
 */
function uc_stamps_domestic_modes() {
  // Load all the modes.
  $modes = uc_stamps_modes_load();

  // Remove the modes with foreign classes.
  foreach ($modes as $mode_id => $mode) {
    if (!uc_stamps_class_is_domestic($mode->options['class'])) {
      unset($modes[$mode_id]);
    }
  }

  return $modes;
}

/**
 * Returns list of domestic foreign methods.
 *
 * @return array
 *   Array of configured shipping mode objects with foreign classes.
 */
function uc_stamps_foreign_modes() {
  // Load all the modes.
  $modes = uc_stamps_modes_load();

  // Remove the modes with foreign classes.
  foreach ($modes as $mode_id => $mode) {
    if (uc_stamps_class_is_domestic($mode->options['class'])) {
      unset($modes[$mode_id]);
    }
  }

  return $modes;
}

/**
 * Form for creating or editing a shipping container.
 *
 * @param object $container
 *   If specified, the container object to edit (from the DB).  If not
 *   specified, create a new container.
 *
 * @see uc_stamps_container_form_submit()
 * @ingroup forms
 */
function uc_stamps_container_form($form, &$form_state, $container = NULL) {
  // Create "hidden" field for cid for existing containers.
  if (!empty($container)) {
    $form['cid'] = array('#type' => 'value', '#value' => $container->cid);
    drupal_set_title(t('Edit container: %name', array('%name' => $container->name)), PASS_THROUGH);
  }
  else {
    drupal_set_title(t('Create new container'));
  }

  if (isset($container->name)) {
    $name = $container->name;
  }
  else {
    $name = '';
  }

  // Name field.
  $form['name'] = array(
    '#title' => t('Container name'),
    '#type' => 'textfield',
    '#default_value' => $name,
    '#maxlength' => 50,
    '#size' => 50,
    '#required' => TRUE,
  );

  // Container type field.
  $form['container_type'] = array(
    '#title' => t('Container type'),
    '#type' => 'select',
    '#options' => uc_stamps_usps_container_types(),
    '#default_value' => isset($container->container_type) ? $container->container_type : 'package',
    '#maxlength' => 50,
  );

  // Tare, capacity, and weight unit fields.
  $form['tare_container'] = array(
    '#type'        => 'container',
    '#attributes'  => array('class' => array('uc-inline-form', 'clearfix')),
    '#title'       => t('Tare weight'),
    '#description'   => t('Weight of container and packaging.'),
    '#weight'      => 10,
  );
  $form['tare_container']['tare'] = array(
    '#type'          => 'textfield',
    '#title'       => t('Tare'),
    '#default_value' => isset($container->tare) ? $container->tare : 0,
    '#size'          => 8,
  );
  $form['tare_container']['tare_units'] = array(
    '#type'    => 'select',
    '#title' => t('Units'),
    '#options' => array(
      'oz' => 'Ounces',
      'lb' => 'Pounds',
      'g' => 'Grams',
      'kg' => 'Kilograms',
    ),
    '#default_value' => isset($container->tare_units) ?
      $container->tare_units : variable_get('uc_weight_unit', 'lb'),
  );
  $form['capacity_container'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('uc-inline-form', 'clearfix')),
    '#title' => t('Capacity'),
    '#description' => t('Maximum product weight container can hold'),
    '#weight' => 10,
  );
  $form['capacity_container']['capacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Capacity'),
    '#default_value' => isset($container->capacity) ? $container->capacity : 0,
    '#size' => 8,
  );
  $form['capacity_container']['capacity_units'] = array(
    '#type'    => 'select',
    '#title' => t('Units'),
    '#options' => array(
      'oz' => 'Ounces',
      'lb' => 'Pounds',
      'g' => 'Grams',
      'kg' => 'Kilograms',
    ),
    '#default_value' => isset($container->capacity_units) ?
      $container->capacity_units : variable_get('uc_weight_unit', 'lb'),
  );

  // Length, width, height, and length unit fields.
  $form['dimensions'] = array(
    '#type' => 'container',
    '#attributes'  => array('class' => array('uc-inline-form', 'clearfix')),
    '#title' => t('Dimensions'),
    '#description' => t('Physical dimensions of the container.'),
    '#weight' => 20,
  );
  $form['dimensions']['length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length'),
    '#default_value' => isset($container->length) ? $container->length : 1,
    '#size' => 8,
  );
  $form['dimensions']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => isset($container->width) ? $container->width : 1,
    '#size' => 8,
  );
  $form['dimensions']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => isset($container->height) ? $container->height : 1,
    '#size' => 8,
  );
  $form['dimensions']['length_units'] = array(
    '#type' => 'select',
    '#title' => t('Units'),
    '#options' => array(
      'in' => t('Inches'),
      'ft' => t('Feet'),
      'mm' => t('Millimeters'),
      'cm' => t('Centimeters'),
      'm'  => t('Meters'),
    ),
    '#default_value' => isset($container->length_units) ?
      $container->length_units : variable_get('uc_length_unit', 'in'),
  );

  // Boxlike checkbox.
  $form['boxlike'] = array(
    '#type' => 'checkbox',
    '#title' => t('Box-shaped'),
    '#default_value' => isset($container->boxlike) ?
      $container->boxlike : TRUE,
    '#description' => t('Uncheck for non-rectangular containers.'),
    '#weight'      => 21,
  );

  // Priority field.
  if (isset($container->priority)) {
    $default_priority = $container->priority;
  }
  else {
    $default_priority = 0;
  }
  $form['priority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#options' => drupal_map_assoc(range(-20, 20)),
    '#default_value' => $default_priority,
    '#description' => t('Priority in case multiple containers are valid.  Automation chooses the one with the highest priority.'),
  );

  // Submit button.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save container'),
  );

  return $form;
}

/**
 * Form submission handler for uc_stamps_container_form().
 *
 * @see uc_stamps_container_form()
 */
function uc_stamps_container_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['cid'])) {
    drupal_write_record('uc_stamps_containers', $form_state['values'], 'cid');
  }
  else {
    drupal_write_record('uc_stamps_containers', $form_state['values']);
  }
  $form_state['redirect'] = 'admin/store/settings/stamps/container';
}

/**
 * Displays a paged list and overview of existing containers.
 */
function uc_stamps_container_admin() {
  // Get the nice container type names.
  $container_types = uc_stamps_usps_container_types();

  $header = array(
    array('data' => t('Name'), 'field' => 'c.name', 'sort' => 'asc'),
    array('data' => t('Container Type'), 'field' => 'c.container_type'),
    array('data' => t('Capacity'), 'field' => 'c.capacity'),
    array('data' => t('Priority'), 'field' => 'c.priority'),
    array('data' => t('Operations'), 'colspan' => 2),
  );

  $query = db_select('uc_stamps_containers', 'c')->extend('PagerDefault')->extend('TableSort')
    ->fields('c', array('cid', 'name', 'container_type', 'capacity', 'priority'))
    ->orderByHeader($header)
    ->limit(30);

  $rows = array();

  $result = $query->execute();
  foreach ($result as $container) {
    $rows[] = array(
      check_plain($container->name),
      $container_types[$container->container_type],
      check_plain($container->capacity),
      check_plain($container->priority),
      l(t('edit'), 'admin/store/settings/stamps/container/' . $container->cid . '/edit'),
      l(t('delete'), 'admin/store/settings/stamps/container/' . $container->cid . '/delete'),
    );
  }

  $build['containers'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No containers have been added yet.'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
  );

  return $build;
}

/**
 * Confirms the deletion of the given container.
 *
 * @see uc_stamps_container_delete_confirm_submit()
 */
function uc_stamps_container_delete_confirm($form, &$form_state, $container) {
  // If we got a bunk attribute, kick out an error message.
  if (empty($container)) {
    drupal_set_message(t('There is no containers with that ID.'), 'error');
    drupal_goto('admin/store/settings/stamps/container');
  }

  // Check if the container is in use.
  // Check if any shipping modes use this container as default.
  $mode_query = db_select('uc_stamps_modes', 'm')
    ->fields('m', array('cid', 'name'))
    ->condition('cid', $container->cid);
  $mode_records = $mode_query->execute();
  if ($mode_records->rowCount()) {
    $mode_names = $mode_records->fetchCol(1);
    drupal_set_message(t('Container \"%name\" can not be deleted because it is the default for the following shipping mode(s): %modes.',
        array(
          '%name' => $container->name,
          '%modes' => implode(', ', $mode_names),
        )
      ), 'error');
    drupal_goto('admin/store/settings/stamps/container');
  }

  // The actual form, after dealing with error checks.
  $form['cid'] = array('#type' => 'value', '#value' => $container->cid);

  $output = confirm_form($form, t('Are you sure you want to delete the container %name?', array('%name' => $container->name)),
              'admin/store/settings/stamps/container', NULL,
              t('Delete'), t('Cancel'));

  return $output;
}

/**
 * Actually delete a container.
 *
 * @see uc_stamps_container_delete_confirm()
 */
function uc_stamps_container_delete_confirm_submit($form, &$form_state) {
  db_delete('uc_stamps_containers')
    ->condition('cid', $form_state['values']['cid'])
    ->execute();

  drupal_set_message(t('Container deleted.'));

  $form_state['redirect'] = 'admin/store/settings/stamps/container';
}

/**
 * Form for creating or editing a shipping mode.
 *
 * A shipping mode is a combination of a package type, shipping class,
 * and shipping options.
 *
 * @param object $mode
 *   If specified, the mode object to edit (from the DB).  If not
 *   specified, create a new mode.
 *
 * @see uc_stamps_mode_form_submit()
 * @ingroup forms
 */
function uc_stamps_mode_form($form, &$form_state, $mode = NULL) {
  // Get container options.
  $container_options = uc_stamps_container_names();

  // If there are no containers, this won't work, so print an error message
  // and a button to go to the containers page.
  if (empty($container_options)) {
    drupal_set_title(t('No containers yet'));
    $form = array();
    $form['error'] = array(
      '#type' => 'item',
      '#markup' => t('No containers are defined.  You must define a container before defining a shipping mode.'),
    );
    // Submit button.
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Define containers'),
      '#suffix' => l(t('Cancel'), 'admin/store/stamps/mode'),
      '#submit' => array('uc_stamps_mode_form_go_to_containers'),
    );

    return $form;
  }

  // The real form starts here.
  // Create "hidden" field for mid for existing containers.
  if (!empty($mode)) {
    $form['mid'] = array('#type' => 'value', '#value' => $mode->mid);
    drupal_set_title(t('Edit shipping mode: %name', array('%name' => $mode->name)), PASS_THROUGH);
  }
  else {
    drupal_set_title(t('Create new shipping mode'));
  }

  // Name field.
  if (isset($mode->name)) {
    $name = $mode->name;
  }
  else {
    $name = '';
  }
  $form['name'] = array(
    '#title' => t('Mode name'),
    '#type' => 'textfield',
    '#default_value' => $name,
    '#maxlength' => 50,
    '#size' => 50,
    '#required' => TRUE,
  );

  // Shipping class and options field.
  if (isset($mode->accessorials)) {
    $accessorials = $mode->accessorials;
  }
  else {
    $accessorials = '';
  }
  uc_stamps_services_form($form, $form_state, array('stamps'), $accessorials);

  // Default ontainer field.
  if (isset($mode->cid)) {
    $cid = $mode->cid;
  }
  else {
    $cid = 0;
  }
  $form['cid'] = array(
    '#type' => 'select',
    '#title' => 'Default container',
    '#options' => $container_options,
    '#default_value' => $cid,
    '#required' => TRUE,
  );

  // Container list field.
  $default_containers = array();
  if (!empty($mode->containers)) {
    $default_containers = array_keys($mode->containers);
  }
  $form['containers'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Available containers',
    '#options' => uc_stamps_container_names(),
    '#default_value' => $default_containers,
  );

  // Priority field.
  if (isset($mode->priority)) {
    $priority = $mode->priority;
  }
  else {
    $priority = 0;
  }
  $form['priority'] = array(
    '#type' => 'select',
    '#title' => 'Priority',
    '#options' => drupal_map_assoc(range(-20, 20)),
    '#default_value' => $priority,
    '#description' => 'Priority in case multiple shipping modes are valid.  Automation chooses the one with the highest priority.',
  );

  // Submit button.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save mode'),
  );

  return $form;
}

/**
 * Form submission handler for uc_stamps_mode_form().
 *
 * Saves the new or edited mode to the database.
 *
 * @see uc_stamps_mode_form()
 */
function uc_stamps_mode_form_submit($form, &$form_state) {
  // Fill in the (hidden) accessorials field, first.
  uc_stamps_services_form_fill_accessorials($form_state['values']);
  // If editing existing mode, just update it.
  if (!empty($form_state['values']['mid'])) {
    // Save the shipping mode.
    drupal_write_record('uc_stamps_modes', $form_state['values'], 'mid');
    $mid = $form_state['values']['mid'];

    // Remove entries in {uc_stamps_modes_containers}.
    db_delete('uc_stamps_modes_containers')
      ->condition('mid', $mid)
      ->execute();

    // Update the label for the associated rule.
    $condition = rules_config_load('uc_stamps_mode_condition_' .
                                   $form_state['values']['mid']);
    $condition->label = $form_state['values']['name'];
    $condition->save();

    drupal_set_message(t('Updated shipping mode @mode.', array('@mode' => $form_state['values']['name'])));
    $form_state['redirect'] = 'admin/store/settings/stamps/mode';
  }
  else {
    // Editing a new mode, so create the record then edit the condition.
    // Save the record, which creates $form_state['values']['mid'].
    drupal_write_record('uc_stamps_modes', $form_state['values']);
    $mid = $form_state['values']['mid'];
    // Ensure Rules picks up the new condition.
    // This creates the new condition by calling
    // uc_stamps_default_rules_configuration().
    entity_flush_caches();

    drupal_set_message(t('Created new shipping mode %name.  Now add the conditions under which it will be used.',
      array('%name' => $form_state['values']['name'])));
    // Edit the new condition.
    $form_state['redirect']
      = 'admin/store/settings/stamps/mode/manage/uc_stamps_mode_condition_' .
        $form_state['values']['mid'];
  }

  // Write out the new container list.
  foreach ($form_state['values']['containers'] as $cid) {
    if ($cid) {
      $record = array('mid' => $mid, 'cid' => $cid);
      drupal_write_record('uc_stamps_modes_containers', $record);
    }
  }
}

/**
 * Redirect to the containers config page.
 *
 * Used when there are no containers defined, and the user
 * tries to define a mode.
 *
 * @see uc_stamps_mode_form()
 */
function uc_stamps_mode_form_go_to_containers($form, &$form_state) {
  $form_state['redirect'] = 'admin/store/settings/stamps/container';
}

/**
 * Displays a paged list and overview of existing modes.
 *
 * @return array
 *   A table containing information about the shipping modes.
 */
function uc_stamps_mode_admin() {
  // Get the user-defined container names.
  $containers = uc_stamps_container_names();
  // Get the classes.
  $classes = _uc_stamps_class_list();

  $header = array(
    array('data' => t('Name'), 'field' => 'm.name', 'sort' => 'asc'),
    array('data' => t('Class')),
    array('data' => t('Container')),
    array('data' => t('Operations'), 'colspan' => 3),
  );

  $query = db_select('uc_stamps_modes', 'm')->extend('PagerDefault')->extend('TableSort')
    ->fields('m', array('mid', 'cid', 'name', 'accessorials'))
    ->orderByHeader($header)
    ->limit(30);

  $rows = array();

  $result = $query->execute();
  foreach ($result as $mode) {
    // The class is the first item in the accessorials field.
    // Convert the field to an array and extract it here.
    $shipping_options = uc_stamps_services_array($mode->accessorials);
    $rows[] = array(
      check_plain($mode->name),
      $classes[$shipping_options['class']],
      $containers[$mode->cid],
      l(t('edit'), 'admin/store/settings/stamps/mode/' . $mode->mid . '/edit'),
      l(t('conditions'), 'admin/store/settings/stamps/mode/manage/uc_stamps_mode_condition_' . $mode->mid),
      l(t('delete'), 'admin/store/settings/stamps/mode/' . $mode->mid . '/delete'),
    );
  }

  $build['modes'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No modes have been added yet.'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
  );

  return $build;
}

/**
 * Confirms the deletion of the given shipping mode.
 *
 * @see uc_stamps_mode_delete_confirm_submit()
 */
function uc_stamps_mode_delete_confirm($form, &$form_state, $mode) {
  // If we got a bunk attribute, kick out an error message.
  if (empty($mode)) {
    drupal_set_message(t('There is no shipping mode with that ID.'), 'error');
    drupal_goto('admin/store/settings/stamps/mode');
  }

  $form['mid'] = array('#type' => 'value', '#value' => $mode->mid);

  $output = confirm_form($form, t('Are you sure you want to delete the shipping mode %name?', array('%name' => $mode->name)),
              'admin/store/settings/stamps/mode', NULL,
              t('Delete'), t('Cancel'));

  return $output;
}

/**
 * Actually delete a shipping mode.
 *
 * @see uc_stamps_mode_delete_confirm()
 */
function uc_stamps_mode_delete_confirm_submit($form, &$form_state) {
  $mid = $form_state['values']['mid'];
  // Delete the record in the database.
  db_delete('uc_stamps_modes')
    ->condition('mid', $mid)
    ->execute();

  // Delete the list of valid containers from  {uc_stamps_modes_containers}.
  db_delete('uc_stamps_modes_containers')
    ->condition('mid', $mid)
    ->execute();

  // Delete the Rules condition.
  rules_config_delete(array('uc_stamps_mode_condition_' . $mid));

  drupal_set_message(t('Shipping mode deleted.'));

  $form_state['redirect'] = 'admin/store/settings/stamps/mode';
}

/**
 * Form to edit customs information for a product class.
 *
 * @see uc_stamps_product_class_form_submit()
 *
 * @param object $class_info
 *   The product class info object, containing the following elements:
 *   - pcid: String containing the product class ID.  Must be defined here.
 *   - description: String containing a short description of the products
 *     in the class.  Maximum 31 characters.
 *   - tariff_code: String containing the harmonized tariff code for the
 *     products in the class.  Maximum 6 characters.
 */
function uc_stamps_product_class_form($form, &$form_state, $class_info) {
  $id = $class_info->pcid;
  drupal_set_title($class_info->name);
  $stamps_class_info = uc_stamps_product_class_load($id);

  // Pcid is not editable here.  Just make it a value.
  $form['pcid'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#default_value' => $stamps_class_info->description,
    '#description' => t('Describe the products in this class for foreign customs officials in 31 or fewer characters.'),
    '#maxlength' => 31,
    '#size' => 31,
    '#required' => FALSE,
  );
  // Default country of origin (stored as a country code).
  $countries = uc_country_option_list();
  $form['origin_country'] = array(
    '#type' => 'select',
    '#title' => t('Country of Origin'),
    '#options' => $countries,
    '#required' => FALSE,
    '#description' => t('The country of origin for this product type.  Leave blank to use the global default value.  May be overridden by individual products.'),
    '#default_value' => $stamps_class_info->origin_country,
    '#empty_option' => '- Default -',
    '#empty_value' => 0,
  );
  $form['tariff_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Harmonized Tariff Code'),
    '#default_value' => $stamps_class_info->tariff_code,
    '#description' => t('The harmonized tariff code speeds customs processing, if available.'),
    '#size' => 6,
    '#maxlength' => 6,
  );
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Submission handler for uc_stamps_product_class_form().
 *
 * Just save the class customs information.
 *
 * @see uc_stamps_product_class_form()
 */
function uc_stamps_product_class_form_submit($form, &$form_state) {
  uc_stamps_product_class_save($form_state['values']);
  drupal_set_message(t('Stamps.com customs information saved.'));
}

/**
 * Save customs information for a product class.
 *
 * @param array $product_class_info
 *   The product class info array, containing the following elements:
 *   - pcid: String containing the product class ID.
 *   - description: String containing a short description of the products
 *     in the class.  Maximum 31 characters.
 *   - tariff_code: String containing the harmonized tariff code for the
 *     products in the class.  Maximum 6 characters.
 *   - origin_country: Integer containing the country code of the country of
 *     origin of products in this class.
 */
function uc_stamps_product_class_save($product_class_info) {
  db_merge('uc_stamps_product_classes')
    ->key(array('pcid' => $product_class_info['pcid']))
    ->fields(array(
       'description' => $product_class_info['description'],
       'tariff_code' => $product_class_info['tariff_code'],
       'origin_country' => $product_class_info['origin_country'],
      )
    )
    ->execute();
}
