<?php

/**
 * @file
 * File to implement all administrative tasks.
 */

/**
 * Function to implement admin panel menu callback.
 */
function product_case_tracker_admin_panel($form, &$form_state) {
  global $base_url;
  $product_type = array();
  $field = array();
  $options = array();
  $fields_needed = array();
  $content_types = node_type_get_types();
  $all_fields = field_info_instances();
  $nids = '';

  // Find class types.
  foreach ($content_types as $name => $details) {
    if ($details->module == 'uc_product') {
      $product_type[$name] = $details->name;
    }
  }

  // Check for every, already created, product class existence.
  $saved_class = db_select('uc_product_classes', 'upc')
  ->fields('upc', array('pcid'))
  ->execute()
  ->fetchCol();

  foreach ($product_type as $name => $detail) {
    if ($name != 'product') {
      if (!in_array($name, $saved_class)) {
        unset($product_type[$name]);

        // Get all node ids for this node type.
        $node_ids = db_select('product_case_tracker', 'pct')
        ->fields('pct', array('nid'))
        ->condition('content_type_name', $name, '=')
        ->execute()
        ->fetchCol();

        db_delete('product_case_tracker')
        ->condition('content_type_name', $name, '=')
        ->execute();

        db_delete('product_case_tracker_fields')
        ->condition('content_type_name', $name, '=')
        ->execute();

        db_delete('product_case_tracker_timeline')
        ->condition('content_type_name', $name, '=')
        ->execute();

        db_delete('product_case_tracker_timeline_details')
        ->condition('content_type_name', $name, '=')
        ->execute();

        if (!empty($node_ids)) {

          // Shows that the field already in use is deleted.
          // Delete all data for this node type.
          foreach ($node_ids as $key => $id) {
            $node = node_load($id);
            if (!empty($node)) {
              db_delete('product_case_tracker_details')
              ->condition('nid', $node->nid, '=')
              ->execute();

              db_delete('product_case_tracker_artisan')
              ->condition('nid', $node->nid, '=')
              ->execute();

              db_delete('product_case_tracker_customer')
              ->condition('nid', $node->nid, '=')
              ->execute();

              db_delete('product_case_tracker_collabo_details')
              ->condition('nid', $node->nid, '=')
              ->execute();
            }
          }
        }
      }
    }
  }

  // Find field details in this class type.
  $i = 0;
  foreach ($all_fields['node'] as $content_type => $fields) {
    foreach ($fields as $field_name => $field_details) {
      if ($field_details['widget']['type'] == 'options_select') {
        $field[$i]['name'] = $field_details['field_name'];
        $field[$i]['field_id'] = $field_details['field_id'];
        $field[$i]['content_type'] = $field_details['bundle'];
        ++$i;
      }
    }
  }
  foreach ($field as $key => $field_detail) {
    if ($field_detail['name'] == 'uc_product_image') {
      unset($field[$key]);
    }
    if ($field_detail['name'] == 'taxonomy_catalog') {
      unset($field[$key]);
    }
    if ($field_detail['name'] == 'body') {
      unset($field[$key]);
    }
  }

  // Check these fields if they are strictly of 'list_text' type.
  foreach ($field as $key => $detail) {
    foreach ($detail as $index => $value) {
      if ($index == 'name') {
        $field_check = field_info_field($value);
        if ($field_check['type'] != 'list_text') {
          unset($field[$key]);
        }
      }
    }
  }

  $form['select_content_types'] = array(
    '#title' => t('Select a Class Type'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );

  // Check for options already selected.
  $result = db_select('product_case_tracker_fields', 'pctf')
  ->fields('pctf')
  ->execute();

  $default_value = '';
  while ($row = $result->fetchAssoc()) {
    $default_value .= $row['content_type_name'] . ',';
  }
  $default_value = explode(',', $default_value);
  $form['select_content_types']['content_type'] = array(
    '#title' => t('Ubercart Classes that can be used for product case tracker'),
    '#type' => 'checkboxes',
    '#default_value' => $default_value,
    '#options' => $product_type,
  );

  foreach ($product_type as $name => $detail) {
    // Filter fields according to checkbox selected.
    $i = 0;
    foreach ($field as $key => $field_detail) {
      if (array_search($name, $field_detail)) {
        $fields_needed[$i]['name'] = $field_detail['name'];
        $fields_needed[$i]['field_id'] = $field_detail['field_id'];
        $fields_needed[$i]['content_type'] = $field_detail['content_type'];
        ++$i;
      }
    }

    // Check if there is an entry for this content type.
    $saved_fields = db_select('field_config', 'fc')
    ->fields('fc', array('field_name'))
    ->execute()
    ->fetchCol();

    $used_field = db_select('product_case_tracker_fields', 'pctf')
    ->fields('pctf', array('field_name'))
    ->condition('content_type_name', $name, '=')
    ->execute()
    ->fetchCol();

    if (isset($used_field[0])) {
      if (!in_array($used_field[0], $saved_fields)) {

        // Get all node ids for this node type.
        $node_ids = db_select('product_case_tracker', 'pct')
        ->fields('pct', array('nid'))
        ->condition('content_type_name', $name, '=')
        ->execute()
        ->fetchCol();

        if (!empty($node_ids)) {

          // Shows that the field already in use is deleted.
          // Delete all data for this node type.
          db_delete('product_case_tracker')
          ->condition('content_type_name', $name, '=')
          ->execute();

          db_delete('product_case_tracker_fields')
          ->condition('content_type_name', $name, '=')
          ->execute();

          db_delete('product_case_tracker_timeline')
          ->condition('content_type_name', $name, '=')
          ->execute();

          db_delete('product_case_tracker_timeline_details')
          ->condition('content_type_name', $name, '=')
          ->execute();

          foreach ($node_ids as $key => $id) {
            $node = node_load($id);
            if (!empty($node)) {
              db_delete('product_case_tracker_details')
              ->condition('nid', $node->nid, '=')
              ->execute();

              db_delete('product_case_tracker_artisan')
              ->condition('nid', $node->nid, '=')
              ->execute();

              db_delete('product_case_tracker_customer')
              ->condition('nid', $node->nid, '=')
              ->execute();

              db_delete('product_case_tracker_collabo_details')
              ->condition('nid', $node->nid, '=')
              ->execute();
            }
          }
        }
      }
    }

    foreach ($fields_needed as $key => $details) {
      $options[] = $details['name'];
    }

    $field_options[] = $options;
    $form['fields'][$name . '_selected_field'] = array(
      '#title' => t('Select fields from !detail class type', array('!detail' => $detail)),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#required' => isset($options) ? TRUE : FALSE,
      '#states' => array(
        'visible' => array(
          ':input[name="content_type[' . $name . ']"]' => array(
            'checked' => TRUE,
          ),
        ),
      ),
    );

    $nids = db_select('product_case_tracker', 'pct')
    ->fields('pct', array('nid'))
    ->condition('content_type_name', $name, '=')
    ->execute()
    ->fetchCol();

    if (empty($options)) {
      $form['fields'][$name . '_selected_field']['no_field'] = array(
        '#title' => t('Add a field of type <i>list(text) with select widget</i> for the class type "!detail" to use it for tracking current status of product development.', array('!detail' => $detail)),
        '#type' => 'item',
      );
    }
    else {
      $result = db_select('product_case_tracker_fields', 'pctf')
      ->fields('pctf')
      ->condition('content_type_name', $name, '=')
      ->execute();

      $default_value = '';
      $timeline_value = '';
      while ($row = $result->fetchAssoc()) {
        $default_value = $row['option_selected'];
        $timeline_value = $row['timeline'];
        $field_selected = $row['field_name'];
      }

      $check = db_select('product_case_tracker', 'pct')
      ->fields('pct', array('csid', 'nid', 'date_started', 'content_type_name'))
      ->condition('content_type_name', $name, '=')
      ->execute();

      while ($rows = $check->fetchAssoc()) {
        $field_checking[] = $rows;
      }

      $nids = '';
      if (isset($field_checking)) {
        foreach ($field_checking as $key => $check_detail) {
          if ($check_detail['content_type_name'] == $name) {
            $nids .= l($check_detail['nid'], 'node/' . $check_detail['nid']) . ', ';
          }
        }
        $nids = rtrim($nids, ', ');
        $form['fields'][$name . '_selected_field'][$name . '_no_change'] = array(
          '#type' => 'item',
          '#title' => t('Selected field from !detail is <i>!field_selected.</i>', array(
            '!detail' => strtoupper($detail),
            '!field_selected' => $field_selected,
          )),
          '#description' => t('<p>To change this field, all the nodes that use <i>!field_selected</i> for case tracking, need to be deleted.</p><p> Following are the nid of all those nodes that are currently using <i>!field_selected</i> : !nids.</p>', array(
            '!field_selected' => $field_selected,
            '!nids' => $nids,
          )),
        );
        unset($field_checking);
      }
      else {
        $form['fields'][$name . '_selected_field'][$name] = array(
          '#title' => t('Select a field from "!detail" to make it a case tracker field.', array('!detail' => $detail)),
          '#type' => 'select',
          '#default_value' => $default_value,
          '#options' => $options,
        );
      }
      $form['fields'][$name . '_selected_field'][$name . '_timeline'] = array(
        '#title' => t('Show timeline for <i>!name</i>', array('!name' => $name)),
        '#type' => 'checkbox',
        '#default_value' => isset($timeline_value) ? $timeline_value : 0,
        '#description' => t('Allow timeline on this case tracker. This would be a graphical representation chart of current status as compared to the actual set status. You would be prompted for "Timeline Entries" when you save <i><strong>!name</strong></i> node type', array('!name' => $name)),
      );
    }

    // Empty these arrays to load next content type.
    $fields_needed = array();
    $options = array();
  }

  $form['field_options'] = array(
    '#type' => 'value',
    '#value' => $field_options,
  );

  // Form for assigning roles.
  $artsn_roles = db_select('product_case_tracker_artisan_selected_roles', 'pctasr')
  ->fields('pctasr')
  ->execute()
  ->fetchCol();

  $cust_roles = db_select('product_case_tracker_customer_selected_roles', 'pctcsr')
  ->fields('pctcsr')
  ->execute()
  ->fetchCol();

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Assign Roles'),
    '#collapsible' => TRUE,
  );

  $roles = user_roles();
  foreach ($roles as $rid => $role_name) {
    if ($role_name == 'anonymous user' || $role_name == 'authenticated user') {
      unset($roles[$rid]);
    }
  }
  $form['roles']['artisan'] = array(
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#default_value' => $artsn_roles,
    '#title' => t('Select Roles for a product Designer'),
    '#options' => drupal_map_assoc($roles),
    '#description' => t('All users, with above roles, will be available for selection as a product designer when a commissioned product case tracker is initiated.'),
  );
  $form['roles']['customer'] = array(
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#default_value' => $cust_roles,
    '#title' => t('Select Roles for a Customer'),
    '#options' => drupal_map_assoc($roles),
    '#description' => t('All users, with above roles, will be available for selection as a customer when a commissioned product case tracker is initiated.'),
  );
  $form['role_value'] = array(
    '#type' => 'hidden',
    '#value' => $roles,
  );
  $form['submit'] = array(
    '#value' => 'Save Configuration',
    '#type' => 'submit',
  );
  return $form;
}

/**
 * Function for product_case_tracker_admin_panel submit.
 */
function product_case_tracker_admin_panel_submit($form, &$form_state) {
  $selected_content_types = array();
  $values = array();
  $role = $form_state['values']['role_value'];
  $artisan_roles = $form_state['values']['artisan'];
  $customer_roles = $form_state['values']['customer'];

  // Save entries for field to handle any tampering.
  foreach ($form_state['values']['field_options'] as $key => $options) {
    foreach ($options as $key => $name) {
      $fdetails = field_info_field($name);

      // Check for already existing field values.
      $existing = db_select('product_case_tracker_field_records', 'pctfr')
      ->fields('pctfr', array('field_name'))
      ->distinct()
      ->execute()
      ->fetchCol();

      if (!in_array($name, $existing)) {
        foreach ($fdetails['settings']['allowed_values'] as $status_key => $status_value) {
          db_insert('product_case_tracker_field_records')
          ->fields(array(
            'field_id',
            'field_name',
            'status_entries_id',
            'status_entries_value',
          ))
          ->values(array(
            'field_id' => $fdetails['id'],
            'field_name' => $fdetails['field_name'],
            'status_entries_id' => $status_key,
            'status_entries_value' => $status_value,
          ))
          ->execute();
        }
      }
    }
  }

  // Check which content types have been selected.
  foreach ($form_state['values']['content_type'] as $name => $content_type) {
    if ($content_type) {
      $selected_content_types[] = $content_type;
    }
    else {
      $unselected_content_types[] = $name;
    }
  }

  // Delete all unselected content type, field associations, timeline entries.
  if (isset($unselected_content_types)) {
    foreach ($unselected_content_types as $key => $name) {

      $field_name = db_select('product_case_tracker_fields', 'pctf')
      ->fields('pctf', array('field_name'))
      ->condition('content_type_name', $name, '=')
      ->execute()
      ->fetchCol();

      db_delete('product_case_tracker_fields')
      ->condition('content_type_name', $name, '=')
      ->execute();

      db_delete('product_case_tracker_timeline')
      ->condition('content_type_name', $name, '=')
      ->execute();

      db_delete('product_case_tracker_timeline_details')
      ->condition('content_type_name', $name, '=')
      ->execute();

      foreach ($field_name as $key => $fname) {
        db_delete('product_case_tracker_field_records')
        ->condition('field_name', $fname, '=')
        ->execute();
      }

      // Delete entries for artisan and customers.
      $nodes[$name] = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->condition('type', $name, '=')
      ->execute()
      ->fetchCol();

      $selected_nodes[$name] = db_select('product_case_tracker', 'pct')
      ->fields('pct', array('nid'))
      ->condition('content_type_name', $name, '=')
      ->execute()
      ->fetchCol();

      foreach ($nodes[$name] as $key => $nid) {
        foreach ($selected_nodes[$name] as $skeys => $snid) {
          if ($nid == $snid) {
            db_delete('product_case_tracker_artisan')
            ->condition('nid', $snid, '=')
            ->execute();

            db_delete('product_case_tracker_customer')
            ->condition('nid', $snid, '=')
            ->execute();
          }
        }
      }
      db_delete('product_case_tracker')
      ->condition('content_type_name', $name, '=')
      ->execute();
    }
  }

  // Form submission values for respective content types' fields.
  if (isset($selected_content_types)) {
    foreach ($selected_content_types as $key => $name) {
      if (isset($form_state['values'][$name])) {
        $values[$name]['option_selected'] = $form_state['values'][$name];
        $values[$name]['name'] = $form['fields'][$name . '_selected_field'][$name]['#options'][$form_state['values'][$name]];

        $field = field_info_field($values[$name]['name']);
        $values[$name]['field_id'] = $field['id'];
        $values[$name]['timeline'] = $form_state['values'][$name . '_timeline'];

        // Check if this content type already has a set field.
        $result = db_select('product_case_tracker_fields', 'pctf')
        ->fields('pctf')
        ->condition('content_type_name', $name, '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          if ($row['content_type_name'] == $name) {
            db_delete('product_case_tracker_fields')
            ->condition('content_type_name', $name, '=')
            ->execute();
          }
        }

        // Associate this field with the content type.
        if (isset($values[$name]['field_id']) && isset($values[$name]['name']) && isset($name) && isset($values[$name]['option_selected'])) {
          db_insert('product_case_tracker_fields')
          ->fields(array(
            'field_id',
            'field_name',
            'content_type_name',
            'option_selected',
            'timeline',
          ))
          ->values(array(
            'field_id' => $values[$name]['field_id'],
            'field_name' => $values[$name]['name'],
            'content_type_name' => $name,
            'option_selected' => $values[$name]['option_selected'],
            'timeline' => $values[$name]['timeline'],
          ))
          ->execute();
        }
      }
      elseif (isset($form_state['values'][$name . '_timeline'])) {
        db_update('product_case_tracker_fields')
        ->fields(array('timeline' => $form_state['values'][$name . '_timeline']))
        ->condition('content_type_name', $name, '=')
        ->execute();
      }
    }
  }

  // Remove all users with unselected artisan roles.
  db_delete('product_case_tracker_artisan_selected_roles')
  ->execute();

  // Remove all users with unselected customer roles.
  db_delete('product_case_tracker_customer_selected_roles')
  ->execute();

  // Save form values for role selections.
  foreach ($artisan_roles as $key => $role_value) {
    if ($role_value) {
      $result_roles = db_select('product_case_tracker_artisan_selected_roles', 'pctasr')
      ->fields('pctasr')
      ->condition('role', $key, '=')
      ->execute()
      ->fetchCol();

      if (empty($result_roles)) {
        db_insert('product_case_tracker_artisan_selected_roles')
        ->fields(array('role'))
        ->values(array('role' => $key))
        ->execute();
      }
    }
  }
  foreach ($customer_roles as $key => $role_value) {
    if ($role_value) {
      $result_roles = db_select('product_case_tracker_customer_selected_roles', 'pctcsr')
      ->fields('pctcsr')
      ->condition('role', $key, '=')
      ->execute()
      ->fetchCol();

      if (empty($result_roles)) {
        db_insert('product_case_tracker_customer_selected_roles')
        ->fields(array('role'))
        ->values(array('role' => $key))
        ->execute();
      }
    }
  }

  // Remove all users with unselected artisan roles.
  db_delete('product_case_tracker_artisan_roles')
  ->execute();

  // Remove all users with unselected customer roles.
  db_delete('product_case_tracker_customer_roles')
  ->execute();

  // Insert all users with selected artisan roles.
  foreach ($artisan_roles as $key => $role_value) {
    if ($role_value) {
      if (in_array($role_value, $role)) {

        // Retrieve all users with this role.
        $users = db_select('users_roles', 'ur')
        ->fields('ur', array('uid'))
        ->condition('rid', array_search($role_value, $role), '=')
        ->execute()
        ->fetchCol();

        foreach ($users as $key => $uid) {
          $user_load = user_load($uid);
          db_insert('product_case_tracker_artisan_roles')
          ->fields(array('id', 'name'))
          ->values(array('id' => $uid, 'name' => $user_load->name))
          ->execute();
        }
      }
    }
  }
  // Insert all users with selected customer roles.
  foreach ($customer_roles as $key => $role_value) {
    if ($role_value) {
      if (in_array($role_value, $role)) {

        // Retrieve all users with this role.
        $users = db_select('users_roles', 'ur')
        ->fields('ur', array('uid'))
        ->condition('rid', array_search($role_value, $role), '=')
        ->execute()
        ->fetchCol();

        foreach ($users as $key => $uid) {
          $user_load = user_load($uid);
          db_insert('product_case_tracker_customer_roles')
          ->fields(array('id', 'name'))
          ->values(array('id' => $uid, 'name' => $user_load->name))
          ->execute();
        }
      }
    }
  }
  drupal_set_message('Your settings have been saved.');
}

/**
 * Function to implement timeline panel menu callback.
 */
function product_case_tracker_timeline_panel($form, &$form_state) {
  global $base_url;
  global $user;

  if (isset($_SESSION['product_case_tracker_content_type_name']) && isset($_SESSION['product_case_tracker_nid'])) {
    $form['timeline'] = array(
      '#title' => t('Enter completion dates'),
      '#description' => t('<p><b>Important Instructions : </b> Date values you enter here will get <i>fixed</i> and <i>cannot be changed</i> at any cost. Enter completion dates accordingly as you may think would take up a specific time. These values will get compared with the current status of the product to get any deviations in the product making on the product designer side.</p><p>This comparison of status will be represented diagrammatically in !link detailed page of a specific product. You can use this timeline graph to measure the performances of your product manufacturers.</p>', array('!link' => l(t('Case Tracker'), 'admin/content'))),
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    // Load already set field of this content type.
    $result = db_select('product_case_tracker_fields', 'pctf')
    ->fields('pctf')
    ->condition('content_type_name', $_SESSION['product_case_tracker_content_type_name'], '=')
    ->execute()
    ->fetchAssoc();

    $field = field_info_field($result['field_name']);
    $values = $field['settings']['allowed_values'];

    foreach ($values as $key => $val) {
      $form['timeline'][$key] = array(
        '#title' => t('!value', array('!value' => $val)),
        '#type' => 'date',
        '#description' => t("Product Designers' commitment for <i>!value's</i> completion.", array('!value' => $val)),
        '#after_build' => array('product_case_tracker_start_year'),
      );
    }
    $form['timeline']['submit'] = array(
      '#value' => 'Save Timeline Configuration',
      '#type' => 'submit',
      '#description' => 'You will not be able to edit this timeline once you submit.',
    );
  }
  else {
    drupal_set_message(t('Timeline entries have been set. You can view these entries when you visit !link for this product.'), array('!link' => l(t('case tracker'), 'admin/content')));
  }
  return $form;
}

/**
 * Function to edit the form date field.
 */
function product_case_tracker_start_year($form_element, &$form_state) {
  unset($form_element['year']['#options']);
  $form_element['year']['#options'] = array();
  $start_year = date('Y');
  $end_year = date('Y') + 20;
  foreach (range($start_year, $end_year) as $year) {
    $form_element['year']['#options'][$year] = $year;
  }
  return $form_element;
}

/**
 * Function to validate product_case_tracker_timeline_panel form.
 */
function product_case_tracker_timeline_panel_validate(&$elements, &$form_state) {
  $i = 0;
  foreach ($form_state['values'] as $status => $date) {
    if (is_array($date)) {
      if (isset($date['month']) && isset($date['day']) && isset($date['year'])) {
        $timestamp = mktime(0, 0, 0, $date['month'], $date['day'], $date['year']);
        if ($timestamp < strtotime('today')) {
          form_set_error($status, t('No timeline date can be less than the current date.'));
        }
        if ($timestamp > 0) {
          $dates[$i]['status'] = $status;
          $dates[$i]['time'] = $timestamp;
        }
      }
    }
    ++$i;
  }

  if (isset($dates)) {
    $count = count($dates);
    $i = 0;
    while ($i < $count) {
      if (isset($dates[$i - 1])) {
        if ($dates[$i]['time'] < $dates[$i - 1]['time']) {
          form_set_error($dates[$i]['status'], t("<i> !date_status </i>( !date_time ) cannot be completed before <i>!date_previous_status</i>( !date_previous_time )", array(
            '!date_status' => $dates[$i]['status'],
            '!date_time' => date('jS M Y', $dates[$i]['time']),
            '!date_previous_status' => $dates[$i - 1]['status'],
            '!date_previous_time' => date('jS M Y', $dates[$i - 1]['time']),
          )));
        }
      }
      ++$i;
    }
  }
}

/**
 * Function to submit product_case_tracker_timeline_panel form.
 */
function product_case_tracker_timeline_panel_submit($form, &$form_state) {
  global $base_url;

  db_insert('product_case_tracker_timeline')
  ->fields(array('nid', 'content_type_name'))
  ->values(array(
    'nid' => $_SESSION['product_case_tracker_nid'],
    'content_type_name' => $_SESSION['product_case_tracker_content_type_name'],
  ))
  ->execute();

  foreach ($form_state['values'] as $status => $date) {
    if (is_array($date)) {
      if (isset($date['month']) && isset($date['day']) && isset($date['year'])) {
        $timestamp = mktime(0, 0, 0, $date['month'], $date['day'], $date['year']);

        db_insert('product_case_tracker_timeline_details')
        ->fields(array('nid', 'content_type_name', 'status', 'date'))
        ->values(array(
          'nid' => $_SESSION['product_case_tracker_nid'],
          'content_type_name' => $_SESSION['product_case_tracker_content_type_name'],
          'status' => $status,
          'date' => $timestamp,
        ))
        ->execute();
      }
    }
  }
  unset($_SESSION['product_case_tracker_content_type_name']);
  unset($_SESSION['product_case_tracker_nid']);

  // Redirect next to the credentials page.
  drupal_goto('admin/store/settings/' . $_SESSION['pct_credit_nid'] . '/credentials');
}

/**
 * Function product_case_tracker_credential form.
 */
function product_case_tracker_credential($form, &$form_state) {
  global $base_url;
  product_case_tracker_refresh_roles();

  if (isset($_SESSION['pct_credit_nid'])) {
    $form['credential'] = array(
      '#type' => 'fieldset',
      '#title' => t('Credentials'),
      '#collapsible' => FALSE,
    );
    $form['credential']['customer'] = array(
      '#type' => 'textfield',
      '#title' => t('Select <i>!value</i> for this product', array('!value' => strtoupper('customer'))),
      '#required' => TRUE,
      '#autocomplete_path' => 'customer/autocomplete',
    );
    $form['credential']['artisan'] = array(
      '#type' => 'textfield',
      '#title' => t('Select <i>!value</i> for this product', array('!value' => strtoupper('product designer'))),
      '#required' => TRUE,
      '#autocomplete_path' => 'artisan/autocomplete',
    );
    $form['credential']['submit'] = array(
      '#value' => 'Save Credentials',
      '#type' => 'submit',
    );

    return $form;
  }
  else {
    $node = node_load(arg(3));
    if (!empty($node)) {
      $csid = db_select('product_case_tracker', 'pct')
      ->fields('pct', array('csid'))
      ->condition('nid', $node->nid, '=')
      ->execute()
      ->fetchAssoc();

      drupal_set_message('Case tracker ID for this product is : ' . l($csid['csid'], 'admin/' . $node->nid . '/product_case_tracker') . '. Click here to ' . l(t('Save and Continue'), 'node/' . $node->nid . '/edit') . ' editing product.');
    }
    else {
      drupal_set_message('Case tracker does not exist for this node', 'warning');
    }
  }
}

/**
 * Function to validate product_case_tracker_credential form.
 */
function product_case_tracker_credential_validate(&$elements, &$form_state) {
  $ais = array();
  $cis = array();
  if (isset($form_state['values']['artisan']) && isset($form_state['values']['customer'])) {
    if ($form_state['values']['artisan'] == $form_state['values']['customer']) {
      form_set_error('customer', t('<i>Customer</i> and <i>Product Designer</i> cannot be the same person'));
      form_set_error('artisan', t('<i>Customer</i> and <i>Product Designer</i> cannot be the same person'));
    }
    if (!is_array($form_state['values']['artisan']) && isset($form_state['values']['customer'])) {
      $artisan = $form_state['values']['artisan'];
      $customer = $form_state['values']['customer'];

      // Check if this is a valid artisan.
      $artisan_details = user_load_by_name($artisan);
      $customer_details = user_load_by_name($customer);

      if (!empty($artisan_details)) {
        $aid = $artisan_details->uid;

        // Check if this is a valid artisan.
        $aids = db_select('product_case_tracker_artisan_roles', 'pctar')
        ->fields('pctar', array('id'))
        ->execute()
        ->fetchCol();

        if (!in_array($artisan_details->uid, $aids)) {
          form_set_error('artisan', t('<i>!artisan_name</i> - unrecognised product designer', array('!artisan_name' => $artisan_details->name)));
        }

        // Check to see if an artisan exists.
        $result = db_select('product_case_tracker_artisan', 'pcta')
        ->fields('pcta')
        ->condition('aid', $aid, '=')
        ->condition('nid', $_SESSION['pct_credit_nid'], '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          $ais['id'] = $row['aid'];
        }
        if (count($ais) == 1) {
          form_set_error('artisan', t('Product Designer named <i>!artisan</i> is already working on this product.', array('!artisan' => $artisan)));
        }
      }
      else {
        form_set_error('artisan', t('<i>!artisan</i> - unidentified product designer', array('!artisan' => $artisan)));
      }

      // Check if this is a valid customer.
      if (!empty($customer_details)) {
        $cid = $customer_details->uid;

        // Check if this is a valid customer.
        $cids = db_select('product_case_tracker_customer_roles', 'pctcr')
        ->fields('pctcr', array('id'))
        ->execute()
        ->fetchCol();

        if (!in_array($customer_details->uid, $cids)) {
          form_set_error('customer', t('<i>!customer_name</i> - unrecognised customer', array('!customer_name' => $customer_details->name)));
        }

        // Check to see if a customer exists.
        $result = db_select('product_case_tracker_customer', 'pctc')
        ->fields('pctc')
        ->condition('cid', $cid, '=')
        ->condition('nid', $_SESSION['pct_credit_nid'], '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          $cis['id'] = $row['cid'];
        }
        if (count($cis) == 1) {
          form_set_error('customer', t('Customer named <i>!customer</i> is an existing customer of this product.', array('!customer' => $customer)));
        }
      }
      else {
        form_set_error('customer', t('<i>!customer</i> - unidentified customer', array('!customer' => $customer)));
      }
    }
  }
}

/**
 * Function for submit of product_case_tracker_credential form.
 */
function product_case_tracker_credential_submit($form, &$form_state) {
  global $user;
  $artisan_details = user_load_by_name($form_state['values']['artisan']);
  $customer_details = user_load_by_name($form_state['values']['customer']);

  if (!empty($customer_details) && !empty($artisan_details)) {
    db_insert('product_case_tracker_artisan')
    ->fields(array('aid', 'name', 'nid', 'owner'))
    ->values(array(
      'aid' => $artisan_details->uid,
      'name' => $artisan_details->name,
      'nid' => $_SESSION['pct_credit_nid'],
      'owner' => 'y',
    ))
    ->execute();

    db_insert('product_case_tracker_customer')
    ->fields(array('cid', 'name', 'nid'))
    ->values(array(
      'cid' => $customer_details->uid,
      'name' => $customer_details->name,
      'nid' => $_SESSION['pct_credit_nid'],
    ))
    ->execute();
  }

  // Initiate the case tracker for this product here.
  $csid = db_select('product_case_tracker', 'pct')
  ->fields('pct', array('csid', 'date_started'))
  ->condition('nid', $_SESSION['pct_credit_nid'], '=')
  ->execute()
  ->fetchAssoc();

  db_insert('product_case_tracker_details')
  ->fields(array(
    'nid',
    'csid',
    'date_of_entry',
    'current_status_name',
    'current_status_key',
    'timeline_step',
    'correspondance_from',
    'email',
    'phone',
    'attachment',
  ))
  ->values(array(
    'nid' => $_SESSION['pct_credit_nid'],
    'csid' => $csid['csid'],
    'date_of_entry' => $csid['date_started'],
    'current_status_name' => 'CASE TRACKER STARTED',
    'current_status_key' => 'case_tracker_started',
    'timeline_step' => '0',
    'correspondance_from' => ($user->uid == 1) ? $user->name : 'Product Designer - ' . $user->name,
    'email' => '',
    'phone' => '',
    'attachment' => '',
  ))
  ->execute();
  unset($_SESSION['pct_credit_nid']);
}

/**
 * Function for product_case_tracker_track_report form.
 */
function product_case_tracker_track_report($form, &$form_state) {
  global $base_url;
  global $user;

  $nid = arg(1);
  $nids = array();
  $artisan = array();
  $customer = array();
  $default_value = '';
  $user_details = array();
  $collabs = array();
  $user_type = '';
  $add_or_not = '';
  $csid = "";
  $rows = array();

  product_case_tracker_refresh_roles();
  drupal_add_css(drupal_get_path('module', 'product_case_tracker') . '/product_case_tracker.css');

  // Check if a case tracker is associated with this node.
  $result = db_select('product_case_tracker', 'pct')
  ->fields('pct')
  ->execute();

  while ($row = $result->fetchAssoc()) {
    $details[] = $row;
    $nids[$row['nid']] = $row['nid'];
  }
  if (!empty($nids)) {
    if (in_array($nid, array_keys($nids))) {
      $node = node_load($nid);

      // Check if there is an entry for this content type.
      $saved_fields = db_select('field_config', 'fc')
      ->fields('fc', array('field_name'))
      ->execute()
      ->fetchCol();

      $used_field = db_select('product_case_tracker_fields', 'pctf')
      ->fields('pctf', array('field_name'))
      ->condition('content_type_name', $node->type, '=')
      ->execute()
      ->fetchCol();

      if (isset($used_field[0])) {
        if (!in_array($used_field[0], $saved_fields)) {

          // Get all node ids for this node type.
          $node_ids = db_select('product_case_tracker', 'pct')
          ->fields('pct', array('nid'))
          ->condition('content_type_name', $node->type, '=')
          ->execute()
          ->fetchCol();

          if (!empty($node_ids)) {

            // Shows that field already in use is deleted.
            // Delete all data for this node type.
            db_delete('product_case_tracker')
            ->condition('content_type_name', $node->type, '=')
            ->execute();

            db_delete('product_case_tracker_fields')
            ->condition('content_type_name', $node->type, '=')
            ->execute();

            db_delete('product_case_tracker_timeline')
            ->condition('content_type_name', $node->type, '=')
            ->execute();

            db_delete('product_case_tracker_timeline_details')
            ->condition('content_type_name', $node->type, '=')
            ->execute();

            foreach ($node_ids as $key => $id) {
              $node_detail = node_load($id);
              if (!empty($node_detail)) {
                db_delete('product_case_tracker_details')
                ->condition('nid', $node_detail->nid, '=')
                ->execute();

                db_delete('product_case_tracker_artisan')
                ->condition('nid', $node_detail->nid, '=')
                ->execute();

                db_delete('product_case_tracker_customer')
                ->condition('nid', $node_detail->nid, '=')
                ->execute();

                db_delete('product_case_tracker_collabo_details')
                ->condition('nid', $node_detail->nid, '=')
                ->execute();
              }
            }
          }
        }
      }

      // Check current user type for this case tracker.
      $owner_id = db_select('product_case_tracker_artisan', 'pcta')
      ->fields('pcta', array('aid'))
      ->condition('nid', $node->nid, '=')
      ->condition('owner', 'y', '=')
      ->execute()
      ->fetchCol();

      $cust_id = db_select('product_case_tracker_customer', 'pctc')
      ->fields('pctc', array('cid'))
      ->condition('nid', $node->nid, '=')
      ->execute()
      ->fetchCol();

      $collab_id = db_select('product_case_tracker_collabo_details', 'pctcd')
      ->fields('pctcd', array('collabo_id'))
      ->condition('nid', $node->nid, '=')
      ->execute()
      ->fetchCol();

      if (isset($owner_id[0])) {
        if ($user->uid == $owner_id[0]) {
          $user_type = 'designer';
        }
      }
      if (isset($cust_id[0])) {
        if ($user->uid == $cust_id[0]) {
          $user_type = 'customer';
        }
      }
      if (isset($collab_id[0])) {
        foreach ($collab_id as $key => $id) {
          if ($user->uid == $id) {
            $user_type = 'collab';
          }
        }
      }

      // If none above is concerned, save this user's name.
      if ($user_type == '') {
        if ($user->uid == 1) {
          $user_type = 'admin';
        }
        else {
          $user_type = 'unknown';
        }
      }

      // Stop illegal access to this case tracker by unknown user.
      if ($user->uid == 1 || $user_type == 'designer' || $user_type == 'collab' || $user_type == 'customer') {
        drupal_get_messages();

        // Retrieve case tracker ID.
        foreach ($details as $key => $case) {
          if ($case['nid'] == $node->nid) {
            $csid = $case['csid'];
            $form['csid'] = array(
              '#type' => 'item',
              '#title' => '<i>Case Tracker ID : ' . $case['csid'] . '</i>',
            );
          }
        }

        // Function to check if any of the existing users of the
        // product case tracker are deleted.
        product_case_tracker_refresh_users();

        // Retrieve artisan associated with this node.
        $result = db_select('product_case_tracker_artisan', 'pcta')
        ->fields('pcta', array('aid', 'name'))
        ->condition('nid', $node->nid, '=')
        ->condition('owner', 'y', '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          $artisan[$row['name']] = user_load($row['aid']);
        }

        // Retrieve collaborators associated with this node.
        $result = db_select('product_case_tracker_collabo_details', 'pctcd')
        ->fields('pctcd', array('collabo_id', 'collabo_name'))
        ->condition('nid', $node->nid, '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          $collabs[$row['collabo_name']] = user_load($row['collabo_id']);
        }

        // Retrieve customer associated with this node.
        $result = db_select('product_case_tracker_customer', 'pctc')
        ->fields('pctc')
        ->condition('nid', $node->nid, '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          $customer[$row['name']] = user_load($row['cid']);
        }

        // Retrieve field_id related to this node.
        $result = db_select('product_case_tracker_fields', 'pctf')
        ->fields('pctf', array('field_id', 'timeline'))
        ->condition('content_type_name', $node->type, '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          $detail[$node->type]['id'] = $row['field_id'];
          $detail[$node->type]['timeline'] = $row['timeline'];
        }

        // To be used for check, to display timeline if allowed.
        $timeline_allowed = $detail[$node->type]['timeline'];
        $fid = $detail[$node->type]['id'];
        if (isset($fid)) {
          $field = field_info_field_by_id($fid);
        }
        $options = $field['settings']['allowed_values'];

        // Check for any signs of changes in the status field.
        $fentry = db_select('product_case_tracker_field_records', 'pctfr')
        ->fields('pctfr', array('status_entries_id', 'status_entries_value'))
        ->condition('field_name', $used_field[0], '=')
        ->execute();

        while ($row = $fentry->fetchAssoc()) {
          $fentries_id[] = $row['status_entries_id'];
          $fentries_values[] = $row['status_entries_value'];
        }

        // Compare both id and value of the status field for any changes.
        $foptions_id = array_keys($options);
        $foptions_value = array_values($options);
        $diff_id = array_diff($foptions_id, $fentries_id);
        $diff_values = array_diff($foptions_value, $fentries_values);
        if (!empty($diff_id)) {
          return drupal_get_form('product_case_tracker_tracker_tampered', $csid);
        }
        elseif (!empty($diff_values)) {
          return drupal_get_form('product_case_tracker_tracker_tampered', $csid);
        }
        elseif (count($foptions_id) != count($fentries_id)) {
          return drupal_get_form('product_case_tracker_tracker_tampered', $csid);
        }

        $last_status = array_keys(array_slice($options, -1, 1));
        $current_status = db_select('product_case_tracker_details', 'pctd')
        ->fields('pctd', array('current_status_key'))
        ->condition('nid', $node->nid, '=')
        ->execute()
        ->fetchCol();
        if (isset($current_status) && count($current_status) > 0) {
          $current_status = $current_status[count($current_status) - 1];
        }

        if ($timeline = $detail[$node->type]['timeline']) {
          $timeline = t('Change status of the product as to reflect how far the development or product making has reached. This change would be reflected in the current timeline of this product.');
        }
        else {
          $timeline = t('Change status of the product as to reflect how far the development or product making has reached.');
        }

        // Since there will be many fields with same value 'Add More'.
        $form['#tree'] = TRUE;
        $form['#attributes'] = array('class' => array('form-item label'));
        $form['product_fieldset'] = array(
          '#type' => 'fieldset',
          '#title' => 'Product Summary',
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
        );
        $form['product_fieldset']['label_name'] = array(
          '#type' => 'item',
          '#prefix' => '<div class="pct_name">',
          '#suffix' => '</div>',
          '#title' => t('!name', array('!name' => strtoupper('product name'))),
        );
        $form['product_fieldset']['label_name']['name'] = array(
          '#type' => 'item',
          '#prefix' => '<div class="pct_name">',
          '#suffix' => '</div>',
          '#title' => l(t('!title', array('!title' => $node->title)), 'node/' . $node->nid),
        );

        // Retrieve the current status value.
        $result = db_select('product_case_tracker_details', 'pctd')
        ->fields('pctd', array(
          'timeline_step',
          'current_status_key',
          'current_status_name',
        ))
        ->condition('nid', $node->nid, '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          $default_key[] = $row['current_status_key'];
          $default_name[] = $row['current_status_name'];
        }

        if (isset($default_key)) {
          if ($default_key[count($default_key) - 1] == 'case_tracker_closed') {
            $form['product_fieldset']['status'] = array(
              '#type' => 'item',
              '#title' => t('Case Tracker closed at Status - %status', array('%status' => isset($default_name) ? $default_name[count($default_name) - 2] : '<i>Status Pending</i>')),
            );
          }
          else {
            if ($user_type == 'customer') {
              $form['product_fieldset']['status'] = array(
                '#type' => 'item',
                '#title' => t('Current Status - %status', array('%status' => isset($default_name) ? $default_name[count($default_name) - 1] : '<i>Status Pending</i>')),
              );
            }
            elseif ($user_type == 'admin' || $user_type == 'designer' || $user_type == 'collab') {
              $form['product_fieldset']['status'] = array(
                '#type' => 'select',
                '#required' => TRUE,
                '#title' => t('!status', array('!status' => strtoupper('current status'))),
                '#empty_option' => t('----- Select Current Status -----'),
                '#description' => '<div style="margin-left:50%;">' . $timeline . '</div>',
                '#options' => $options,
                '#default_value' => isset($default_key) ? $default_key[count($default_key) - 1] : '',
              );
            }
            elseif ($user_type == 'unknown') {
              $form['product_fieldset']['status'] = array(
                '#type' => 'item',
                '#title' => t('Current Status not available for unknown user'),
              );
            }
          }
        }
        // Product Description.
        if (!empty($node->body)) {
          $form['label_desc'] = array(
            '#type' => 'fieldset',
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#title' => t('!desc', array('!desc' => strtoupper('product description'))),
          );
          $form['label_desc']['desc'] = array(
            '#type' => 'item',
            '#title' => t('!value', array('!value' => $node->body['und'][0]['value'])),
          );
        }
        if (isset($node->uc_product_image['und'])) {
          $form['label_image'] = array(
            '#type' => 'fieldset',
            '#collapsed' => TRUE,
            '#collapsible' => TRUE,
            '#title' => t('!value', array('!value' => strtoupper('Image(s)'))),
          );

          $image_header = array(
            array(
              'data' => 'Name',
            ),
            array(
              'data' => 'Image',
            ),
          );
          foreach ($node->uc_product_image['und'] as $key => $details) {
            $image_rows[] = array(
              $details['filename'],
              '<img id="pct_images" src="' . image_style_url('medium', $details['uri']) . '" />',
            );
          }
          $form['label_image']['images'] = array(
            '#theme' => 'table',
            '#header' => $image_header,
            '#rows' => $image_rows,
          );
        }

        if (count($artisan) > 0) {
          $artisan_exist = user_load_by_name(key($artisan));
          if (isset($artisan_exist->uid)) {
            if ($artisan_exist->uid) {
              $form['artisan_fieldset'] = array(
                '#type' => 'fieldset',
                '#title' => 'Product Designer summary',
                '#collapsible' => TRUE,
                '#collapsed' => FALSE,
              );
              foreach ($artisan as $name => $detail) {
                $form['artisan_fieldset'][$name]['label_name'] = array(
                  '#type' => 'item',
                  '#prefix' => '<div class="pct_name">',
                  '#suffix' => '</div>',
                  '#title' => t('!name', array('!name' => strtoupper('product designer name'))),
                );
                $form['artisan_fieldset'][$name]['label_name']['name'] = array(
                  '#type' => 'item',
                  '#prefix' => '<div class="pct_name">',
                  '#suffix' => '</div>',
                  '#title' => l(t('!name', array('!name' => $detail->name)), 'user/' . $detail->uid),
                );
                $form['artisan_fieldset'][$name]['label_id'] = array(
                  '#type' => 'item',
                  '#prefix' => '<div class="pct_name">',
                  '#suffix' => '</div>',
                  '#title' => t('!id', array('!id' => strtoupper('product designer id'))),
                );
                $form['artisan_fieldset'][$name]['label_id']['id'] = array(
                  '#type' => 'item',
                  '#title' => l(t('!uid', array('!uid' => $detail->uid)), 'user/' . $detail->uid),
                );
              }

              // Change artisan. Only visible to super admin.
              if ($user->uid == 1) {
                if ($current_status != 'case_tracker_closed') {
                  if ($current_status != $last_status[0]) {
                    $form['change_artisan_container'] = array(
                      '#type' => 'container',
                    );
                    $form['change_artisan_container']['change_artisan'] = array(
                      '#type' => 'checkbox',
                      '#title' => 'Change Product Designer',
                    );
                    $form['change_artisan_container']['artisan'] = array(
                      '#type' => 'textfield',
                      '#title' => t('Select <i>!value</i>', array('!value' => strtoupper('product designer'))),
                      '#autocomplete_path' => 'artisan/autocomplete',
                      '#states' => array(
                        'visible' => array(
                          ':input[name="change_artisan_container[change_artisan]"]' => array(
                            'checked' => TRUE,
                          ),
                        ),
                      ),
                    );
                  }
                }
              }
            }
          }
        }
        else {
          return drupal_get_form('product_case_tracker_tracker_tampered', $csid);
        }

        // Email and phone form.
        $active = db_select('product_case_tracker_details', 'pctd')
        ->fields('pctd', array('current_status_key'))
        ->condition('nid', $node->nid, '=')
        ->execute()
        ->fetchCol();

        if ($active[count($active) - 1] != 'case_tracker_closed') {
          if ($user->uid == 1 || $user_type == 'designer' || $user_type == 'collab') {
            $form['email'] = array(
              '#type' => 'fieldset',
              '#title' => t('Email to Customer'),
              '#collapsed' => TRUE,
              '#collapsible' => TRUE,
            );
          }
          elseif ($user_type == 'customer') {
            $form['email'] = array(
              '#type' => 'fieldset',
              '#title' => t('Email to Product Designers / Collaborators'),
              '#collapsed' => TRUE,
              '#collapsible' => TRUE,
            );
          }
          $form['email']['email_subject'] = array(
            '#prefix' => '<div id="send_mail_wrapper">',
            '#type' => 'textfield',
            '#title' => t('Subject'),
          );
          $form['email']['email_body'] = array(
            '#type' => 'textarea',
            '#title' => t('Body'),
          );
          $form['email']['email_sendmail'] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="pct_name">',
            '#suffix' => '</div>',
            '#value' => t('Send mail'),
            '#ajax' => array(
              'callback' => 'product_case_tracker_send_email',
              'wrapper' => 'send_mail_wrapper',
              'method' => 'replace',
              'progress' => array(
                'type' => 'throbber',
                'message' => 'Sending mail.....',
              ),
            ),
          );
          $form['email']['email_attach'] = array(
            '#type' => 'file',
            '#description' => t('Upload files of format <b>jpg jpeg gif png</b>'),
            '#title' => t('Attach Document'),
            '#prefix' => '<div class="pct_name">',
            '#suffix' => '</div></div>',
            '#title_display' => 'invisible',
            '#size' => 25,
          );

          $form['phone'] = array(
            '#type' => 'fieldset',
            '#title' => t('Phone Conversations / Notes'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
          );
          $form['phone']['phone_body'] = array(
            '#prefix' => '<div id="submit_phone_wrapper">',
            '#type' => 'textarea',
            '#title' => t('Description'),
          );
          $form['phone']['phone_submit'] = array(
            '#type' => 'submit',
            '#suffix' => '</div>',
            '#value' => t('Submit Record'),
            '#ajax' => array(
              'callback' => 'product_case_tracker_submit_phone',
              'wrapper' => 'submit_phone_wrapper',
              'method' => 'replace',
              'progress' => array(
                'type' => 'throbber',
                'message' => 'Saving record.....',
              ),
            ),
          );
        }

        // Check whether existing collaborators have been deleted.
        $collabo_ids = db_select('product_case_tracker_collabo_details', 'pctcd')
        ->fields('pctcd', array('collabo_id'))
        ->condition('nid', $node->nid, '=')
        ->execute()
        ->fetchCol();

        if (isset($collabo_ids)) {
          foreach ($collabo_ids as $key => $id) {
            if ($id == 0) {
              return drupal_get_form('product_case_tracker_tracker_tampered', $csid);
            }
          }
        }

        // Add/Remove collaborators.
        if (isset($collabs)) {
          $form['collaborator_fieldset'] = array(
            '#type' => 'fieldset',
            '#title' => 'Collaborators',
            '#collapsible' => TRUE,
            '#collapsed' => FALSE,
          );
          $form['collaborator_fieldset']['collab_container'] = array(
            '#prefix' => '<div id="add-collab-wrapper">',
            '#type' => 'container',
          );
          $form['current_status'] = array(
            '#type' => 'value',
            '#value' => $current_status,
          );
          $form['user_type'] = array(
            '#type' => 'value',
            '#value' => $user_type,
          );
          $form['active'] = array(
            '#type' => 'value',
            '#value' => $active[0],
          );

          // If everything seems fine, display collaborators.
          $header = array(
            array(
              'data' => t('Name'),
            ),
            array(
              'data' => t('ID'),
              'sort' => 'ASC',
            ),
            array(
              'data' => t('Option'),
            ),
          );

          $current = $active[0];
          foreach ($collabs as $name => $detail) {
            $collab_exist = user_load_by_name($name);
            if (isset($collab_exist->uid)) {
              if ($collab_exist->uid) {
                $link = array(
                  '#type' => 'link',
                  '#title' => t('Remove !name', array('!name' => $detail->name)),
                  '#href' => 'admin/' . $node->nid . '-' . $current_status . '-' . $user_type . '-' . $current . '-' . $detail->name . '/remove',
                );

                // Populate table.
                $rows[] = array(
                  l(t('!name', array('!name' => $detail->name)), 'user/' . $detail->uid),
                  l(t('!uid', array('!uid' => $detail->uid)), 'user/' . $detail->uid),
                  ($user->uid == 1 || $user_type == 'designer') ? ($current_status != 'case_tracker_closed') ? drupal_render($link) : 'No options available' : 'No options available');

                $form['collaborator_fieldset']['collab_container'][$name]['label_name']['name'] = array(
                  '#type' => 'value',
                  '#title' => l(t('!name', array('!name' => $detail->name)), 'user/' . $detail->uid),
                );
                $form['collaborator_fieldset']['collab_container'][$name]['label_id']['id'] = array(
                  '#type' => 'value',
                  '#title' => l(t('!uid', array('!uid' => $detail->uid)), 'user/' . $detail->uid),
                );

                if ($user->uid == 1 || $user_type == 'designer') {
                  if ($current_status != 'case_tracker_closed') {
                    $form['collaborator_fieldset']['collab_container']['remove']['users'][$name] = array(
                      '#type' => 'value',
                      '#value' => $detail->uid,
                    );
                  }
                }
              }
            }
          }
          $form['collaborator_fieldset']['collab_container']['collab_table'] = array(
            '#theme' => 'table',
            '#prefix' => '</div>',
            '#header' => $header,
            '#rows' => $rows,
            '#empty' => t('No collaborators'),
          );

          if ($user->uid == 1 || $user_type == 'designer') {

            // Can only add collaborator, if the case tracker is not closed.
            $add_or_not = $active[count($active) - 1];
            if ($add_or_not != 'case_tracker_closed') {
              $form['collaborator_fieldset']['name'] = array(
                '#type' => 'textfield',
                '#autocomplete_path' => 'artisan/autocomplete',
              );
              $form['collaborator_fieldset']['op'] = array(
                '#type' => 'submit',
                '#value' => t('Add Collaborator'),
                '#ajax' => array(
                  'callback' => 'product_case_tracker_add_collaborator',
                  'wrapper' => 'add-collab-wrapper',
                  'method' => 'replace',
                  'progress' => 'throbber',
                ),
              );
            }
            else {
              $form['collaborator_fieldset']['no_more_add'] = array(
                '#type' => 'item',
                '#prefix' => '<div class="open_collab">',
                '#title' => t('<p>No collaborators can be added on a closed case tracker.</p>'),
                '#suffix' => '</div>',
              );
            }
          }
        }

        // Summary for customer.
        $customer_exist = user_load_by_name(key($customer));
        if (isset($customer_exist->uid)) {
          if ($customer_exist->uid) {
            $form['customer_fieldset'] = array(
              '#type' => 'fieldset',
              '#title' => 'Customer summary',
              '#collapsible' => TRUE,
              '#collapsed' => FALSE,
            );
            $form['customer_fieldset'][$customer_exist->name]['label_name'] = array(
              '#type' => 'item',
              '#prefix' => '<div class="pct_name">',
              '#suffix' => '</div>',
              '#title' => t('!name', array('!name' => strtoupper('customer name'))),
            );
            $form['customer_fieldset'][$customer_exist->name]['label_name']['name'] = array(
              '#type' => 'item',
              '#prefix' => '<div class="pct_name">',
              '#suffix' => '</div>',
              '#title' => l(t('!name', array('!name' => $customer_exist->name)), 'user/' . $customer_exist->uid),
            );
            $form['customer_fieldset'][$customer_exist->name]['label_id'] = array(
              '#type' => 'item',
              '#prefix' => '<div class="pct_name">',
              '#suffix' => '</div>',
              '#title' => t('!id', array('!id' => strtoupper('customer id'))),
            );
            $form['customer_fieldset'][$customer_exist->name]['label_id']['id'] = array(
              '#type' => 'item',
              '#prefix' => '<div class="pct_name">',
              '#suffix' => '</div>',
              '#title' => l(t('!uid', array('!uid' => $customer_exist->uid)), 'user/' . $customer_exist->uid),
            );
          }
        }
        else {
          return drupal_get_form('product_case_tracker_tracker_tampered', $csid);
        }

        // Timeline, if allowed.
        if ($timeline_allowed) {
          $tline = db_select('product_case_tracker_timeline', 'pctt')
          ->fields('pctt', array('nid'))
          ->condition('nid', $node->nid, '=')
          ->execute()
          ->fetchCol();

          if (!empty($tline)) {
            $form['timeline'] = array(
              '#type' => 'fieldset',
              '#title' => t('Product timeline'),
            );
            $form['timeline']['view'] = array(
              '#type' => 'item',
              '#prefix' => '<iframe src="' . $base_url . '/?q=timeline/' . $node->nid . '" id="pct_iframe">',
              '#suffix' => '</iframe>',
              '#title' => theme('product_timeline', array('node' => $node)),
            );
          }
        }

        // Product Case History table.
        $result = db_select('product_case_tracker_details', 'pctd')
        ->fields('pctd', array(
          'date_of_entry',
          'current_status_name',
          'timeline_step',
          'correspondance_from',
          'email',
          'phone',
          'attachment',
        ))
        ->condition('nid', $node->nid, '=')
        ->execute();

        while ($row = $result->fetchAssoc()) {
          $data[] = $row;
        }
        if (isset($data)) {
          $form['history'] = array(
            '#type' => 'fieldset',
            '#title' => t('Case History'),
            '#description' => t('Refresh page to update the Case history table'),
          );
          $form['history']['case_history'] = array(
            '#type' => 'item',
            '#title' => theme('product_case_history', array('data' => $data)),
          );
        }

        // Other important form elements.
        $form['nid'] = array(
          '#type' => 'hidden',
          '#value' => $node->nid,
        );
        if ($user_type == 'designer' || $user_type == 'collab' || $user->uid == 1) {

          // Only visible if current status is not the last status.
          if ($current_status != 'case_tracker_closed') {
            if ($current_status != $last_status[0]) {
              $form['submit'] = array(
                '#type' => 'submit',
                '#value' => 'Save Case Tracker',
                '#validate' => array('product_case_tracker_track_report_validate'),
                '#submit' => array('product_case_tracker_track_report_submit'),
              );
            }
          }
        }
        if ($user->uid == 1) {
          if ($current_status != 'case_tracker_closed') {
            $form['close'] = array(
              '#type' => 'submit',
              '#value' => t('Close Case Tracker'),
              '#validate' => array('product_case_tracker_validate'),
              '#submit' => array('product_case_tracker_close'),
            );
          }
        }
        return $form;
      }
      else {
        drupal_set_message('Access Denied', 'warning');
      }
    }
    else {
      drupal_set_message(t('<p>Case Tracker may not have been initialized on this node. Following could be the reasons : </p><p><ul><li>The node would have been created when case tracker was not allowed on it. Please refer <i>!link1</i></li><li>Case tracker is not allowed on this node type. To enable case tracker, go to the <i>!link2</i> page.</li><li>The node would have already been created prior to enabling the <i>!link3</i> module.</li><li>Case Tracker must have been removed from this class type, therby deleting all case tracker details related to this node. To re-enable case tracker go the <i>!link4</i> page.</li></ul></p>', array(
        '!link1' => l(t('case tracker settings'), 'admin/store/settings/case_tracker'),
        '!link2' => l(t('case tracker settings'), 'admin/store/settings/case_tracker'),
        '!link3' => l(t('product case tracker'), 'admin/modules'),
        '!link4' => l(t('case tracker settings'), 'admin/store/settings/case_tracker'),
      )), 'warning');
    }
  }
  else {
    drupal_set_message('Looks like <i>Case tracker</i> has not started for any ubercart class type. To start a new case tracker, go to <i> ' . l(t('case tracker settings'), 'admin/store/settings/case_tracker') . '</i> page, select a class type and its associated field, and create a node.', 'warning');
  }
}

/**
 * Function to report an error for case tracker.
 */
function product_case_tracker_tracker_tampered($form, &$form_state, $csid) {
  $form['csid'] = array(
    '#type' => 'item',
    '#title' => t('Case Tracker ID : <i>!csid</i>', array('!csid' => $csid)),
  );
  $form['message'] = array(
    '#type' => 'item',
    '#title' => t('<p class="error"><i>This case has tracker been tampered with and is corrupt.<br /> One or more of the following could be the reasons : <br />- Either the product designer, collaborator or the customer is missing <br /> - Status field has been edited or changed and its contents are no longer the same <br /> - Status entries are not exactly the same as were entered at the time of filling timeline entries ( if timeline allowed ) <br /> - Case tracker was not properly created ( all entries related to timeline - if allowed or credentials were not saved )</i></p>'),
    '#description' => t('<p class="success status" style="margin-top:150px;"><i><b>Create a new case tracker for this product, or contact the Product designer or Administrator.</b></i></p>'),
  );
  return $form;
}


/**
 * Callback to validate close tracker form submit.
 */
function product_case_tracker_validate($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'product_case_tracker') . '/product_case_tracker.css');
}

/**
 * Callback to send an email message.
 */
function product_case_tracker_send_email($form, &$form_state) {
  global $language;
  global $user;
  $username = '';
  $user_type = '';
  drupal_get_messages();
  $to = '';
  $params['subject'] = $form_state['values']['email']['email_subject'];
  $params['body'] = $form_state['values']['email']['email_body'];

  if (empty($params['subject']) || empty($params['body'])) {
    return;
  }

  $file = file_save_upload('email', array(
    'file_validate_is_image' => array(),
    'file_validate_extensions' => array('png gif jpg jpeg'),
  ), 'public://');

  if ($file) {
    if ($file = file_move($file, 'public://')) {
      $form_state['values']['email']['email_attach'] = $file;
    }
    $file = $form_state['values']['email']['email_attach'];
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $params['attachment'] = $form_state['values']['email']['email_attach'];
  }

  // Retrieve product designer.
  $aid = db_select('product_case_tracker_artisan', 'pcta')
  ->fields('pcta', array('aid'))
  ->condition('nid', $form_state['values']['nid'], '=')
  ->condition('owner', 'y', '=')
  ->execute()
  ->fetchCol();

  // Retrieve customer.
  $cid = db_select('product_case_tracker_customer', 'pctc')
  ->fields('pctc', array('cid'))
  ->condition('nid', $form_state['values']['nid'], '=')
  ->execute()
  ->fetchCol();

  // Retrieve collaborator.
  $collabo = db_select('product_case_tracker_collabo_details', 'pctcd')
  ->fields('pctcd', array('collabo_id'))
  ->condition('nid', $form_state['values']['nid'], '=')
  ->execute()
  ->fetchCol();

  if ($user->uid == $aid[0]) {
    $user_type = 'designer';
    $username = $user_type . ' - ' . user_load($aid[0])->name;
  }
  elseif ($user->uid == $cid[0]) {
    $user_type = 'customer';
    $username = $user_type . ' - ' . user_load($cid[0])->name;
  }
  else {
    foreach ($collabo as $key => $id) {
      if ($user->uid == $id) {
        $user_type = 'collab';
        $username = $user_type . ' - ' . user_load($id)->name;
      }
    }
  }
  if (isset($user_type)) {
    if ($user_type == '') {
      if ($user->uid == 1) {
        $user_type = 'admin';
        $username = $user_type . ' - ' . user_load($user->uid)->name;
      }
      else {
        $user_type = 'unknown';
      }
    }
    if ($user_type != 'unknown') {
      if ($user->uid == 1 || $user_type == 'designer' || $user_type == 'collab') {
        $user_detail = user_load($cid);
        $tos[] = $user_detail->mail;
      }
      if ($user_type == 'customer') {
        $user_detail = user_load($aid);
        $tos[] = $user_detail->mail;

        if (isset($collabo)) {
          foreach ($collabo as $key => $uid) {
            $user_details = user_load($uid);
            $tos[] = $user_details->mail;
          }
        }
      }
    }
    else {
      return '<b class="error">Unknown user</b>';
    }
  }

  // Format all 'to' addresses.
  foreach ($tos as $key => $mail) {
    $to .= $mail . ', ';
  }
  $to = rtrim($to, ',');

  // Send mail.
  $result = drupal_mail('product_case_tracker', 'conversation', $to, language_default(), $params, $user->mail, TRUE);
  if ($result['result']) {
    $forms['mail_success'] = array(
      '#type' => 'item',
      '#prefix' => '<div class="success status">',
      '#title' => empty($params['attachment']) ? t('<p>Your message has been sent to <i>!result</i> without any attachment. Reason - Attachment file not supported or not uploaded.</p>', array('!result' => rtrim($result['to'], ','))) : t('<p>Your message has been sent to <i>!results</i> with attachment - <i>!filename</i>.</p>', array(
        '!results' => rtrim($result['to'], ','),
        '!filename' => $params['attachment']->filename)),
    );
    $forms['mail_message'] = array(
      '#type' => 'item',
      '#prefix' => '<div style="color:darkgray;">',
      '#suffix' => '</div></div>',
      '#title' => t('!body', array('!body' => $params['body'])),
    );

    $csid = db_select('product_case_tracker', 'pct')
    ->fields('pct', array('csid'))
    ->condition('nid', $form_state['values']['nid'], '=')
    ->execute()
    ->fetchCol();

    $current_status = db_select('product_case_tracker_details', 'pctd')
    ->fields('pctd', array(
      'current_status_key',
      'current_status_name',
      'timeline_step',
    ))
    ->condition('nid', $form_state['values']['nid'], '=')
    ->execute();

    while ($row = $current_status->fetchAssoc()) {
      $current_key[][$row['current_status_key']] = $row['current_status_name'];
      $timeline_step[] = $row['timeline_step'];
    }

    db_insert('product_case_tracker_details')
    ->fields(array(
      'nid',
      'csid',
      'date_of_entry',
      'current_status_name',
      'current_status_key',
      'timeline_step',
      'correspondance_from',
      'email',
      'phone',
      'attachment',
    ))
    ->values(array(
      'nid' => $form_state['values']['nid'],
      'csid' => $csid[0],
      'date_of_entry' => strtotime('today'),
      'current_status_name' => array_values($current_key[count($current_key) - 1]),
      'current_status_key' => array_keys($current_key[count($current_key) - 1]),
      'timeline_step' => $timeline_step[count($timeline_step) - 1],
      'correspondance_from' => $username,
      'email' => $params['body'],
      'phone' => '',
      'attachment' => !empty($params['attachment']) ? 'Yes' : 'No',
    ))
    ->execute();
  }
  else {
    $forms['mail_no_success'] = array(
      '#type' => 'item',
      '#prefix' => '<div class="error">',
      '#suffix' => '</div>',
      '#title' => t('<i>There was a problem sending your email. Message could not be sent.</i>'),
    );
  }
  drupal_get_messages();
  return $forms;
}

/**
 * Callback to save notes / phone records.
 */
function product_case_tracker_submit_phone($form, &$form_state) {
  global $user;
  $username = '';
  $body = $form_state['values']['phone']['phone_body'];
  $node = node_load($form_state['values']['nid']);
  drupal_get_messages();

  // Check current user type for this case tracker.
  $owner_id = db_select('product_case_tracker_artisan', 'pcta')
  ->fields('pcta', array('aid'))
  ->condition('nid', $node->nid, '=')
  ->condition('owner', 'y', '=')
  ->execute()
  ->fetchCol();

  $cust_id = db_select('product_case_tracker_customer', 'pctc')
  ->fields('pctc', array('cid'))
  ->condition('nid', $node->nid, '=')
  ->execute()
  ->fetchCol();

  $collab_id = db_select('product_case_tracker_collabo_details', 'pctcd')
  ->fields('pctcd', array('collabo_id'))
  ->condition('nid', $node->nid, '=')
  ->execute()
  ->fetchCol();

  if ($user->uid == $owner_id[0]) {
    $user_type = 'designer';
    $username = $user_type . ' - ' . user_load($owner_id[0])->name;
  }
  elseif ($user->uid == $cust_id[0]) {
    $user_type = 'customer';
    $username = $user_type . ' - ' . user_load($cust_id[0])->name;
  }
  else {
    foreach ($collab_id as $key => $id) {
      if ($user->uid == $id) {
        $user_type = 'collab';
        $username = $user_type . ' - ' . user_load($id)->name;
      }
    }
  }
  if ($user->uid == 1) {
    $user_type = 'admin';
    $username = $user_type . ' - ' . user_load($user->uid)->name;
  }

  // If none above is concerned, save this user's name.
  if (isset($user_type)) {
    if ($user_type == '') {
      $user_type = 'unknown';
    }

    if ($user_type != 'unknown') {
      if (isset($node)) {
        if (!empty($body)) {

          // Retrieve case tracker id for this product.
          $csid = db_select('product_case_tracker', 'pct')
          ->fields('pct', array('csid'))
          ->condition('nid', $form_state['values']['nid'], '=')
          ->execute()
          ->fetchCol();

          $current_status = db_select('product_case_tracker_details', 'pctd')
          ->fields('pctd', array(
            'current_status_key',
            'current_status_name',
            'timeline_step',
          ))
          ->condition('nid', $node->nid, '=')
          ->execute();

          while ($row = $current_status->fetchAssoc()) {
            $current_key[][$row['current_status_key']] = $row['current_status_name'];
            $timeline_step[] = $row['timeline_step'];
          }

          db_insert('product_case_tracker_details')
          ->fields(array(
            'nid',
            'csid',
            'date_of_entry',
            'current_status_name',
            'current_status_key',
            'timeline_step',
            'correspondance_from',
            'email',
            'phone',
            'attachment',
           ))
          ->values(array(
            'nid' => $form_state['values']['nid'],
            'csid' => $csid[0],
            'date_of_entry' => strtotime('today'),
            'current_status_name' => array_values($current_key[count($current_key) - 1]),
            'current_status_key' => array_keys($current_key[count($current_key) - 1]),
            'timeline_step' => $timeline_step[count($timeline_step) - 1],
            'correspondance_from' => $username,
            'email' => '',
            'phone' => $body,
            'attachment' => '',
          ))
          ->execute();

          $forms['title'] = array(
            '#type' => 'item',
            '#prefix' => '<div class="success status">',
            '#title' => t('Phone record submitted'),
          );
          $forms['response'] = array(
            '#type' => 'item',
            '#prefix' => '<div style="color:darkgray;">',
            '#suffix' => '</div></div>',
            '#title' => t('!body', array('!body' => $body)),
          );
          return $forms;
        }
        else {
          return;
        }
      }
    }
  }
  else {
    return '<i class="error">unknown user</i>';
  }
}

/**
 * Function to close case tarcker.
 */
function product_case_tracker_close($form, &$form_state) {
  global $base_url;
  global $user;
  $admin = user_load(1);

  // Check if this case tracker is already closed.
  $closed = db_select('product_case_tracker_details', 'pctd')
  ->fields('pctd', array('current_status_key'))
  ->condition('nid', $form_state['values']['nid'], '=')
  ->execute()
  ->fetchCol();

  if ($closed[count($closed) - 1] == 'case_tracker_closed') {
    drupal_set_message('Case tracker closed by the administrator -  ' . $admin->name, 'warning');
  }
  else {
    // Look for case tracker id of this product.
    $csid = db_select('product_case_tracker', 'pct')
    ->fields('pct', array('csid'))
    ->condition('nid', $form_state['values']['nid'], '=')
    ->execute()
    ->fetchCol();

    db_insert('product_case_tracker_details')
    ->fields(array(
      'nid',
      'csid',
      'date_of_entry',
      'current_status_name',
      'current_status_key',
      'timeline_step',
      'correspondance_from',
      'email',
      'phone',
      'attachment',
    ))
    ->values(array(
      'nid' => $form_state['values']['nid'],
      'csid' => $csid[0],
      'date_of_entry' => strtotime('today'),
      'current_status_name' => 'CASE TRACKER CLOSED',
      'current_status_key' => 'case_tracker_closed',
      'timeline_step' => '0',
      'correspondance_from' => $user->name,
      'email' => '',
      'phone' => '',
      'attachment' => '',
    ))
    ->execute();
    drupal_set_message('This Case tracker is closed by ' . $admin->name, 'warning');
  }
}

/**
 * Menu callback to remove a collaborator.
 */
function product_case_tracker_remove_collaborator() {
  global $base_url;

  $info = explode('-', arg(1));
  $nid = $info[0];
  $current_status = $info[1];
  $user_type = $info[2];
  $active = $info[3];
  $name = $info[4];
  $user_details = user_load_by_name($name);

  // Check if this is a valid collaborator.
  $collabo = db_select('product_case_tracker_collabo_details', 'pctcd')
  ->fields('pctcd', array('collabo_id'))
  ->condition('nid', $nid, '=')
  ->condition('collabo_id', $user_details->uid, '=')
  ->execute()
  ->fetchCol();

  if (!empty($collabo)) {
    db_delete('product_case_tracker_collabo_details')
    ->condition('nid', $nid, '=')
    ->condition('collabo_id', $user_details->uid, '=')
    ->execute();

    drupal_goto($base_url . '?q=admin/' . $nid . '/product_case_tracker');
  }
}

/**
 * Function to display list of collaborators.
 */
function product_case_tracker_display_collabs($nid, $current_status, $user_type, $active) {
  global $user;
  $node = node_load($nid);

  // Retrieve collaborators associated with this node.
  $result = db_select('product_case_tracker_collabo_details', 'pctcd')
  ->fields('pctcd', array('collabo_id', 'collabo_name'))
  ->condition('nid', $node->nid, '=')
  ->execute();

  while ($row = $result->fetchAssoc()) {
    $collabs[$row['collabo_name']] = user_load($row['collabo_id']);
  }

  if (isset($collabs)) {

    // If everything seems fine, display collaborators.
    $header = array(
      array(
        'data' => t('Name'),
      ),
      array(
        'data' => t('ID'),
        'sort' => 'ASC',
      ),
      array(
        'data' => t('Option'),
      ),
    );

    $current = $active[0];
    foreach ($collabs as $name => $detail) {
      $collab_exist = user_load_by_name($name);
      if (isset($collab_exist->uid)) {
        if ($collab_exist->uid) {
          $link = array(
            '#type' => 'link',
            '#title' => t('Remove !name', array('!name' => $detail->name)),
            '#href' => 'admin/' . $node->nid . '-' . $current_status . '-' . $user_type . '-' . $current . '-' . $detail->name . '/remove',
          );

          // Populate table.
          $rows[] = array(
            l(t('!name', array('!name' => $detail->name)), 'user/' . $detail->uid),
            l(t('!uid', array('!uid' => $detail->uid)), 'user/' . $detail->uid),
            ($user->uid == 1 || $user_type == 'designer') ? ($current_status != 'case_tracker_closed') ? drupal_render($link) : 'No options available' : 'No options available');

        }
      }
    }
    return theme('table', array('header' => $header, 'rows' => $rows));
  }
}

/**
 * Callback to add a collaborator.
 */
function product_case_tracker_add_collaborator($form, &$form_state) {
  drupal_get_messages('error');
  $nid = $form_state['values']['nid'];
  $current_status = $form_state['values']['current_status'];
  $user_type = $form_state['values']['user_type'];
  $active = $form_state['values']['active'];

  // Retrieve case tracker id for this product.
  $csid = db_select('product_case_tracker', 'pct')
  ->fields('pct', array('csid'))
  ->condition('nid', $form_state['values']['nid'], '=')
  ->execute()
  ->fetchCol();

  // Check if this is a valid user.
  $user_details = user_load_by_name($form_state['values']['collaborator_fieldset']['name']);
  if (!empty($user_details)) {
    if ($user_details->uid == 0) {
      return;
    }

    $aids = db_select('product_case_tracker_artisan_roles', 'pctar')
    ->fields('pctar', array('id'))
    ->execute()
    ->fetchCol();

    // Check if this is a valid artisan.
    if (!in_array($user_details->uid, $aids)) {
      return '<div id="add-collab-wrapper" class="error"><i>' . strtoupper($user_details->name) . ', not a recognised product designer.</i>' . product_case_tracker_display_collabs($nid, $current_status, $user_type, $active) . '</div>';
    }

    // Check if this user is an artisan or customer.
    $aid = db_select('product_case_tracker_artisan', 'pcta')
    ->fields('pcta', array('aid'))
    ->condition('nid', $form_state['values']['nid'], '=')
    ->condition('owner', 'y', '=')
    ->execute()
    ->fetchCol();

    $cid = db_select('product_case_tracker_customer', 'pctc')
    ->fields('pctc', array('cid'))
    ->condition('nid', $form_state['values']['nid'], '=')
    ->execute()
    ->fetchCol();

    $result = db_select('product_case_tracker_collabo_details', 'pctcd')
    ->fields('pctcd', array('collabo_id'))
    ->condition('nid', $form_state['values']['nid'], '=')
    ->execute();

    while ($row = $result->fetchAssoc()) {
      $collabo[] = $row;
    }
    if ($aid[0] == $user_details->uid) {
      return '<div id="add-collab-wrapper" class="error"><i>' . strtoupper($user_details->name) . ' is already a product designer.</i>' . product_case_tracker_display_collabs($nid, $current_status, $user_type, $active) . '</div>';
    }
    if ($cid[0] == $user_details->uid) {
      return '<div id="add-collab-wrapper" class="error"><i>' . strtoupper($user_details->name) . ' is already a customer for this product.</i>' . product_case_tracker_display_collabs($nid, $current_status, $user_type, $active) . '</div>';
    }
    if (isset($collabo)) {
      foreach ($collabo as $key => $id) {
        foreach ($id as $key => $collab_id) {
          if ($collab_id == $user_details->uid) {
            return '<div id="add-collab-wrapper" class="error"><i>' . strtoupper($user_details->name) . ' is already a collaborator for this product.</i>' . product_case_tracker_display_collabs($nid, $current_status, $user_type, $active) . '</div>';
          }
        }
      }
    }

    // Add collaborator if everything seems fine.
    if ($user_details->uid != 0) {
      db_insert('product_case_tracker_collabo_details')
      ->fields(array('csid', 'nid', 'collabo_name', 'collabo_id'))
      ->values(array(
        'csid' => $csid[0],
        'nid' => $form_state['values']['nid'],
        'collabo_name' => $user_details->name,
        'collabo_id' => $user_details->uid,
      ))
      ->execute();
      return '<div id="add-collab-wrapper" class="success status"><i><b>' . strtoupper($form_state['values']['collaborator_fieldset']['name']) . '</b> is added as a collaborator for this product</i>' . product_case_tracker_display_collabs($nid, $current_status, $user_type, $active) . '</div>';
    }
  }
}

/**
 * Function to validate product_case_tracker_track_report form.
 */
function product_case_tracker_track_report_validate(&$elements, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'product_case_tracker') . '/product_case_tracker.css');
  $current_status = '';
  $node = node_load($form_state['values']['nid']);
  $timeline_step = 0;

  if (isset($form_state['values']['product_fieldset']['status'])) {
    $current_status = $form_state['values']['product_fieldset']['status'];
  }

  // Calculate timeline step.
  $field = db_select('product_case_tracker_fields', 'pctf')
  ->fields('pctf', array('field_name'))
  ->condition('content_type_name', $node->type, '=')
  ->execute()
  ->fetchCol();

  $i = 0;
  $field_detail = field_info_field($field[0]);
  foreach ($field_detail['settings']['allowed_values'] as $key => $status) {
    ++$i;
    if ($key == $current_status) {
      $set_status = $status;
      $timeline_step = $i;
    }
  }

  // Check to see if this status entry already exists.
  $result = db_select('product_case_tracker_details', 'pctd')
  ->fields('pctd')
  ->condition('nid', $form_state['values']['nid'], '=')
  ->condition('timeline_step', $timeline_step, '=')
  ->execute()
  ->fetchAssoc();

  if (isset($result)) {
    if ($current_status != '') {
      if ($timeline_step == $result['timeline_step']) {
        form_set_error('product_fieldset][status', t('Entry for this status already exists'));
      }
    }
  }

  // Change designer validation.
  if (isset($form_state['values']['change_artisan_container']['change_artisan'])) {
    if ($form_state['values']['change_artisan_container']['change_artisan'] == 1) {
      if (isset($form_state['values']['change_artisan_container']['artisan'])) {
        $change_user = $form_state['values']['change_artisan_container']['artisan'];

        if (empty($change_user)) {
          form_set_error('change_artisan_container][artisan', t('Enter a valid Product Designer'));
        }
        // Check if designer to change is the same.
        $aid = db_select('product_case_tracker_artisan', 'pcta')
        ->fields('pcta', array('name'))
        ->condition('nid', $form_state['values']['nid'], '=')
        ->condition('owner', 'y', '=')
        ->execute()
        ->fetchAssoc();

        // Check if designer to change is already the customer.
        $cid = db_select('product_case_tracker_customer', 'pctc')
        ->fields('pctc', array('name'))
        ->condition('nid', $form_state['values']['nid'], '=')
        ->execute()
        ->fetchAssoc();

        if ($aid['name'] == $change_user) {
          form_set_error('change_artisan_container][artisan', t('!change_user is already the current product designer.', array('!change_user' => $change_user)));
        }
        if ($cid['name'] == $change_user) {
          form_set_error('change_artisan_container][artisan', t('!change_user is already the customer.', array('!change_user' => $change_user)));
        }
      }
    }
  }
}

/**
 * Function for submit product_case_tracker_track_report form.
 */
function product_case_tracker_track_report_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case 'Save Case Tracker':
      global $user;
      $uid = $user->uid;
      $corresp_from = '';
      $current_status = '';
      $date_of_entry = strtotime("today");
      $node = node_load($form_state['values']['nid']);

      if (isset($form_state['values']['product_fieldset']['status'])) {
        $current_status = $form_state['values']['product_fieldset']['status'];
      }

      // Calculate timeline step.
      $field = db_select('product_case_tracker_fields', 'pctf')
      ->fields('pctf', array('field_name'))
      ->condition('content_type_name', $node->type, '=')
      ->execute()
      ->fetchCol();

      $i = 0;
      $field_detail = field_info_field($field[0]);
      foreach ($field_detail['settings']['allowed_values'] as $key => $status) {
        ++$i;
        if ($key == $current_status) {
          $set_status = $status;
          $timeline_step = $i;
        }
      }

      // Change designer.
      if (isset($form_state['values']['change_artisan_container']['change_artisan'])) {
        if ($form_state['values']['change_artisan_container']['change_artisan'] == 1) {
          if (isset($form_state['values']['change_artisan_container']['artisan'])) {
            $change_user = $form_state['values']['change_artisan_container']['artisan'];
            $new_owner = user_load_by_name($change_user);

            if (isset($new_owner)) {

              // Check if designer to change is the same as collaborator.
              $collabo = db_select('product_case_tracker_collabo_details', 'pctcd')
              ->fields('pctcd', array('collabo_name'))
              ->condition('nid', $form_state['values']['nid'], '=')
              ->execute()
              ->fetchAssoc();

              if (!empty($collabo)) {
                if ($collabo['collabo_name'] == $new_owner->name) {
                  db_delete('product_case_tracker_collabo_details')
                  ->condition('nid', $form_state['values']['nid'], '=')
                  ->condition('collabo_id', $new_owner->uid, '=')
                  ->execute();
                }
              }

              // Change ownership of existing product desinger.
              $update_aid = db_update('product_case_tracker_artisan')
              ->fields(array('owner' => 'n'))
              ->condition('nid', $form_state['values']['nid'], '=')
              ->execute();

              db_insert('product_case_tracker_artisan')
              ->fields(array('aid', 'name', 'nid', 'owner'))
              ->values(array(
                'aid' => $new_owner->uid,
                'name' => $new_owner->name,
                'nid' => $form_state['values']['nid'],
                'owner' => 'y',
              ))
              ->execute();
              drupal_set_message('Ownership of this product changed to <i>' . $new_owner->name . '</i> as the new product designer.');
            }
          }
        }
      }

      // Correspondance from.
      $owner_id = db_select('product_case_tracker_artisan', 'pcta')
      ->fields('pcta', array('aid'))
      ->condition('nid', $node->nid, '=')
      ->condition('owner', 'y', '=')
      ->execute()
      ->fetchCol();

      $cust_id = db_select('product_case_tracker_customer', 'pctc')
      ->fields('pctc', array('cid'))
      ->condition('nid', $node->nid, '=')
      ->execute()
      ->fetchCol();

      $collab_id = db_select('product_case_tracker_collabo_details', 'pctcd')
      ->fields('pctcd', array('collabo_id'))
      ->condition('nid', $node->nid, '=')
      ->execute()
      ->fetchCol();

      if ($uid == $owner_id[0]) {
        $user_name = user_load($owner_id[0]);
        $corresp_from = 'Product Designer - ' . $user_name->name;
      }
      elseif ($uid == $cust_id[0]) {
        $user_name = user_load($id);
        $corresp_from = 'Customer - ' . $user_name->name;
      }
      else {
        foreach ($collab_id as $key => $id) {
          if ($uid == $id) {
            $user_name = user_load($id);
            $corresp_from = 'Product Collaborator - ' . $user_name->name;
          }
        }
      }

      // If none above is concerned, save this user's name.
      if ($corresp_from == '') {
        $corresp_from = $user->name;
      }

      // Check if the case tracker is not closed.
      $closed = db_select('product_case_tracker_details', 'pctd')
      ->fields('pctd')
      ->condition('nid', $node->nid, '=')
      ->execute();

      while ($row = $closed->fetchAssoc()) {
        $details[] = $row;
      }
      $prev_timeline_step = $details[count($details) - 1]['timeline_step'];
      $prev_status = $details[count($details) - 1]['current_status_key'];

      if ($prev_status == 'case_tracker_closed') {
        drupal_set_message('This case tracker has been closed by admin.', 'warning');
      }
      else {
        if ($prev_timeline_step > $timeline_step) {
          drupal_set_message('Previous status entries cannot be changed. They are assumed to be completed', 'warning');
        }
        else {

          // Look for case tracker id of this product.
          $csid = db_select('product_case_tracker', 'pct')
          ->fields('pct', array('csid'))
          ->condition('nid', $node->nid, '=')
          ->execute()
          ->fetchCol();

          // Create a history entry for case tracker on this date.
          db_insert('product_case_tracker_details')
          ->fields(array(
            'nid',
            'csid',
            'date_of_entry',
            'current_status_name',
            'current_status_key',
            'timeline_step',
            'correspondance_from',
            'email',
            'phone',
            'attachment',
          ))
          ->values(array(
            'nid' => $node->nid,
            'csid' => $csid[0],
            'date_of_entry' => $date_of_entry,
            'current_status_key' => $current_status,
            'current_status_name' => $set_status,
            'timeline_step' => $timeline_step,
            'correspondance_from' => $corresp_from,
            'email' => '',
            'phone' => '',
            'attachment' => '',
          ))
          ->execute();
        }
      }
      break;
  }
}

/**
 * Function to implement product_case_tracker_commissioned_orders().
 */
function product_case_tracker_commissioned_orders() {
  global $base_url;
  global $user;
  $form = array();
  $uid = arg(1);
  $user_details = user_load($uid);

  if ($user->uid == 1) {

    // Retrieve all case trackers.
    $result = db_select('product_case_tracker', 'pct')
    ->fields('pct')
    ->execute();

    while ($row = $result->fetchAssoc()) {
      $trackers[] = $row;
    }

    $form['admin'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => t('All Commissioned Orders'),
    );

    if (isset($trackers)) {
      foreach ($trackers as $key => $details) {
        $node = node_load($details['nid']);
        $form['admin']['artisan_for_nid_' . $details['nid']] = array(
          '#prefix' => '<div style="width:500px;margin-top:5px;">',
          '#suffix' => '</div>',
          '#type' => 'link',
          '#title' => t('!title', array('!title' => $node->title)),
          '#href' => $base_url . '?q=admin/' . $node->nid . '/product_case_tracker',
        );
      }
    }
    else {
      $form['admin']['no_trackers'] = array(
        '#type' => 'item',
        '#title' => t('There are currently no case trackers active.'),
      );
    }
  }

  if ($user->uid == $uid || $user->uid == 1) {
    if (!empty($user_details)) {
      $artisan = db_select('product_case_tracker_artisan', 'pcta')
      ->fields('pcta', array('nid'))
      ->condition('aid', $uid, '=')
      ->execute()
      ->fetchCol();

      $customer = db_select('product_case_tracker_customer', 'pctc')
      ->fields('pctc', array('nid'))
      ->condition('cid', $uid, '=')
      ->execute()
      ->fetchCol();

      $collabo = db_select('product_case_tracker_collabo_details', 'pctcd')
      ->fields('pctcd', array('nid'))
      ->condition('collabo_id', $uid, '=')
      ->execute()
      ->fetchCol();

      if (!empty($artisan)) {
        $form['artisan'] = array(
          '#type' => 'fieldset',
          '#title' => t('!link is Product Designer for', array('!link' => l($user_details->name, 'user/' . $user_details->uid))),
          '#collapsible' => TRUE,
        );
        foreach ($artisan as $key => $val) {
          $node = node_load($val);
          $form['artisan']['artisan_for_nid_' . $val] = array(
            '#prefix' => '<div style="width:500px;margin-top:5px;">',
            '#suffix' => '</div>',
            '#type' => 'link',
            '#title' => t('!title', array('!title' => $node->title)),
            '#href' => $base_url . '?q=admin/' . $node->nid . '/product_case_tracker',
          );
          unset($node);
        }
      }
      if (!empty($customer)) {
        $form['customer'] = array(
          '#type' => 'fieldset',
          '#title' => t('!link is customer for', array('!link' => l($user_details->name, 'user/' . $user_details->uid))),
          '#collapsible' => TRUE,
        );
        foreach ($customer as $key => $val) {
          $node = node_load($val);
          $form['customer']['customer_for_nid_' . $val] = array(
            '#prefix' => '<div style="width:500px;margin-top:5px;">',
            '#suffix' => '</div>',
            '#type' => 'link',
            '#title' => t('!title', array('!title' => $node->title)),
            '#href' => $base_url . '?q=admin/' . $node->nid . '/product_case_tracker',
          );
          unset($node);
        }
      }
      if (!empty($collabo)) {
        $form['collabo'] = array(
          '#type' => 'fieldset',
          '#title' => t('!link is collaborator for', array('!link' => l($user_details->name, 'user/' . $user_details->uid))),
          '#collapsible' => TRUE,
        );
        foreach ($collabo as $key => $val) {
          $node = node_load($val);
          $form['collabo']['collabo_for_nid_' . $val] = array(
            '#prefix' => '<div style="width:500px;margin-top:5px;">',
            '#suffix' => '</div>',
            '#type' => 'link',
            '#title' => t('!title', array('!title' => $node->title)),
            '#href' => $base_url . '?q=admin/' . $node->nid . '/product_case_tracker',
          );
          unset($node);
        }
      }
      if (empty($artisan) && empty($customer) && empty($collabo)) {
        $form['nothing'] = array(
          '#type' => 'fieldset',
          '#collapsible' => TRUE,
        );
        $form['nothing']['no'] = array(
          '#type' => 'item',
          '#title' => t('!link has no commissioned orders yet.', array('!link' => l($user_details->name, 'user/' . $user_details->uid))),
        );
      }
      return $form;
    }
    else {
      $form['nothings']['no'] = array(
        '#type' => 'item',
        '#title' => t('Not a valid user.'),
      );
      return $form;
    }
  }
  else {
    $form['nothing']['no'] = array(
      '#type' => 'item',
      '#title' => t('You cannot access other users workspace.'),
    );
    return $form;
  }
}

/**
 * Function to show timeline for a given product.
 */
function product_case_tracker_product_timeline() {
  global $user;
  $node = node_load(arg(1));
  $user_type = 'unknown';

  if (!empty($node)) {

    // Check current user type for this case tracker.
    $owner_id = db_select('product_case_tracker_artisan', 'pcta')
    ->fields('pcta', array('aid'))
    ->condition('nid', $node->nid, '=')
    ->condition('owner', 'y', '=')
    ->execute()
    ->fetchCol();

    $cust_id = db_select('product_case_tracker_customer', 'pctc')
    ->fields('pctc', array('cid'))
    ->condition('nid', $node->nid, '=')
    ->execute()
    ->fetchCol();

    $collab_id = db_select('product_case_tracker_collabo_details', 'pctcd')
    ->fields('pctcd', array('collabo_id'))
    ->condition('nid', $node->nid, '=')
    ->execute()
    ->fetchCol();

    if (isset($owner_id[0])) {
      if ($user->uid == $owner_id[0]) {
        $user_type = 'designer';
      }
    }
    if (isset($cust_id[0])) {
      if ($user->uid == $cust_id[0]) {
        $user_type = 'customer';
      }
    }
    if (isset($collab_id[0])) {
      foreach ($collab_id as $key => $id) {
        if ($user->uid == $id) {
          $user_type = 'collab';
        }
      }
    }

    if ($user->uid == 1 || $user_type == 'designer' || $user_type == 'customer' || $user_type == 'collab') {
      echo theme('product_timeline', array('node' => node_load(arg(1))));
    }
    else {
      echo 'Timeline - Access Denied';
    }
  }
  else {
    echo 'Timeline - Invalid URL';
  }
}

/**
 * Menu callback for artisan autocomplete entries.
 */
function product_case_tracker_artisan_autocomplete($string = '') {
  $matches = array();

  $result = db_select('product_case_tracker_artisan_roles', 'pctar')
  ->fields('pctar', array('name'))
  ->condition('name', db_like($string) . '%', 'LIKE')
  ->range(0, 10)
  ->execute();

  foreach ($result as $artisan) {
    $matches[$artisan->name] = check_plain($artisan->name);
  }
  drupal_json_output($matches);
}

/**
 * Menu callback for customer autocomplete entries.
 */
function product_case_tracker_customer_autocomplete($string = '') {
  $matches = array();

  $result = db_select('product_case_tracker_customer_roles', 'pctcr')
  ->fields('pctcr', array('name'))
  ->condition('name', db_like($string) . '%', 'LIKE')
  ->range(0, 10)
  ->execute();

  foreach ($result as $artisan) {
    $matches[$artisan->name] = check_plain($artisan->name);
  }
  drupal_json_output($matches);
}

/**
 * Function to add/remove new/old roles.
 */
function product_case_tracker_refresh_roles() {
  $users = array();

  $artisan_roles = db_select('product_case_tracker_artisan_selected_roles', 'pctasr')
  ->fields('pctasr', array('role'))
  ->execute()
  ->fetchCol();

  $customer_roles = db_select('product_case_tracker_customer_selected_roles', 'pctcsr')
  ->fields('pctcsr', array('role'))
  ->execute()
  ->fetchCol();

  db_delete('product_case_tracker_artisan_roles')
  ->execute();

  db_delete('product_case_tracker_customer_roles')
  ->execute();

  if (!empty($artisan_roles) && !empty($customer_roles)) {
    $roles = user_roles();

    // Insert all users with selected artisan roles.
    foreach ($artisan_roles as $key => $role_value) {
      $users = db_select('users_roles', 'ur')
      ->fields('ur', array('uid'))
      ->condition('rid', array_search($role_value, $roles), '=')
      ->execute()
      ->fetchCol();

      foreach ($users as $key => $uid) {
        $user_load = user_load($uid);

        db_insert('product_case_tracker_artisan_roles')
        ->fields(array('id', 'name'))
        ->values(array('id' => $uid, 'name' => $user_load->name))
        ->execute();
      }
    }

    // Insert all users with selected customer roles.
    foreach ($customer_roles as $key => $role_value) {

      // Retrieve all users with this role.
      $users = db_select('users_roles', 'ur')
      ->fields('ur', array('uid'))
      ->condition('rid', array_search($role_value, $roles), '=')
      ->execute()
      ->fetchCol();

      foreach ($users as $key => $uid) {
        $user_load = user_load($uid);

        db_insert('product_case_tracker_customer_roles')
        ->fields(array('id', 'name'))
        ->values(array('id' => $uid, 'name' => $user_load->name))
        ->execute();
      }
    }
  }
}

/**
 * Function to refresh users.
 */
function product_case_tracker_refresh_users() {

  // Check whether existing artisan has been deleted.
  $artisan_previous_status = db_select('product_case_tracker_artisan', 'pcta')
  ->fields('pcta', array('aid'))
  ->condition('owner', 'y', '=')
  ->distinct()
  ->execute()
  ->fetchCol();

  $artisan_current_status = db_select('product_case_tracker_artisan_roles', 'pctar')
  ->fields('pctar', array('id'))
  ->execute()
  ->fetchCol();

  if (isset($artisan_previous_status)) {
    $artisans = array_diff($artisan_previous_status, $artisan_current_status);
    foreach ($artisans as $key => $id) {
      db_update('product_case_tracker_artisan')
      ->fields(array('owner' => 'n'))
      ->condition('aid', $id, '=')
      ->execute();
    }
  }

  // Check whether existing customer has been deleted.
  $customer_previous_status = db_select('product_case_tracker_customer', 'pctc')
  ->fields('pctc', array('cid'))
  ->distinct()
  ->execute()
  ->fetchCol();

  $customer_current_status = db_select('product_case_tracker_customer_roles', 'pctcr')
  ->fields('pctcr', array('id'))
  ->execute()
  ->fetchCol();

  if (isset($customer_previous_status)) {
    $customers = array_diff($customer_previous_status, $customer_current_status);
    foreach ($customers as $key => $id) {
      db_delete('product_case_tracker_customer')
      ->condition('cid', $id, '=')
      ->execute();
    }
  }

  // Check whether existing collaborator has been deleted.
  $collabo_previous_status = db_select('product_case_tracker_collabo_details', 'pctcd')
  ->fields('pctcd', array('collabo_id'))
  ->distinct()
  ->execute()
  ->fetchCol();

  if (isset($collabo_previous_status)) {
    $collabos = array_diff($collabo_previous_status, $artisan_current_status);
    foreach ($collabos as $key => $id) {
      db_update('product_case_tracker_collabo_details')
      ->fields(array('collabo_id' => '0'))
      ->condition('collabo_id', $id, '=')
      ->execute();
    }
  }
}
