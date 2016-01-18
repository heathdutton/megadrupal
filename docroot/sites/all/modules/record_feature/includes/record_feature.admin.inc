<?php

/**
 * Form to add a new feature for recording
 */
function record_feature_overview_form($form, &$form_state) {
  // War, the user that strongarm is really required when exporting variables!
  if (!module_exists('strongarm')) {
    drupal_set_message(t('You do not have the !link module installed and/or activated. Without it, you will <strong>not</strong> be able to export variable settings to features!', array('!link' => l('Strongarm', 'http://drupal.org/project/strongarm'))), 'warning');
  }

  drupal_set_title(t('Record feature'));

  $form = array();
  $features = _record_feature_get_features();
  $collapsed = FALSE;
  if (!empty($features)) {
    $form['recorded_feature_list']['table'] = array(
      '#type' => 'markup',
      '#markup' => _record_feature_overview_form_table($features),
    );
    $collapsed = TRUE;
  }

  // Add the 'add feature' part.
  _record_feature_add_feature_form($form, $form_state, $collapsed);

  return $form;
}

/**
 * Form to add a new feature for recording
 */
function _record_feature_add_feature_form(&$form, &$form_state, $collapsed = FALSE, $edit = FALSE) {
  // Default values.
  $title = t('Add new feature');
  $description = t('You can add a new feature here.');
  $viewable =  t('above');
  $disabled = FALSE;
  $collapsible = TRUE;
  $default_options = array('variables');
  $machine_description = t('Enter the machine readable name for this feature. This will eventually be the module name for this feature, such as <em>feature_blog</em>.');
  $submit = t('Add feature');

  // Values for the edit version.
  if ($edit) {
    $options = _record_feature_get_options($edit);
    $default_options = array();
    foreach ($options as $key => $value) {
      if ($value)
      $default_options[] = $key;
    }
    $machine_description = '<strong>'.t('The machine readable name can no longer be changed!').'</strong>';
    $title = t('Edit feature');
    $description = t('You can edit a feature here.');
    $viewable =  t('on the overview page');
    $disabled = TRUE;
    $collapsible = FALSE;
    $submit = t('Save changes');
    // Mark as edit form
    $form['action'] = array(
      '#type' => 'hidden',
      '#value' => 'edit',
    );
  }

  $form['record_feature_add'] = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#description' => $description .' '. t('When you record data to this feature, the results will be stored in the database and are viewable') . ' ' . $viewable .'.',
    '#collapsible' => $collapsible,
    '#collapsed' => $collapsed,
  );

  if (!$edit) {
    if (module_exists('features')) {
      $features = features_get_features();
      // Remove used features.
      $used_features = _record_feature_get_features();
      for ($i = 0; $i < count($used_features); $i++) {
        $name = $used_features[$i]['name'];
        unset($features[$name]);
      }
    }
    $options_existing = array();
    foreach ($features as $feature => $object) {
      $options_existing[$feature] = $feature;
    }
    $form['record_feature_add']['ahah'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="custom-feature">',
      '#suffix' => '</div>',
    );
    // Remove when selected!
    if (!isset($form_state['values']) || $form_state['values']['new'] != 1) {
      $form['record_feature_add']['ahah']['existing'] = array(
        '#type' => 'select',
        '#title' => t('Select existing feature'),
        '#options' => $options_existing,
        '#description' => t('Select one of the features you wish to record data for.'),
        '#required' => TRUE,
      );
      $form['record_feature_add']['ahah']['new'] = array(
        '#type' => 'checkbox',
        '#title' =>   t('Create new feature'),
        '#description' => t('Tick this box if you want to add a non-existant feature that you want to record data for.'),
        '#default_value' => isset($form_state['values']) ? $form_state['values']['new'] : 0,
        '#ajax' => array(
          'callback' => 'record_feature_add_custom',
          'wrapper' => 'custom-feature',
          'effect' => 'fade',
        ),
        '#required' => FALSE,
      );
    }
  }
  // Only show when the checkbox above is selected.
  if (isset($form_state['values']) && $form_state['values']['new'] == 1 || $edit) {
    $form['record_feature_add']['ahah']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Feature machine name'),
      '#description' => $machine_description,
      '#size' => 60,
      '#maxlength' => 255,
      '#required' => TRUE,
      '#disabled' => $disabled,
    );
  }

  if ($edit) {
    // Prefill feature name.
    $form['record_feature_add']['ahah']['name']['#value'] = $edit;
  }
  $form['record_feature_add']['options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Options'),
    '#options' => array(
      'variables' => t('Record changes to variables'),
      'permissions' => t('Record changes to permissions'),
    ),
    '#default_value' => $default_options,
    '#description' => t('Indicate what exactly you wish to record for this feature.'),
  );
  $form['record_feature_add']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start recording immediately.'),
    '#description' => '<strong>' . t('Warning: this will abort any recording that might be in progress! All data recorded up to this point will be saved.') .'</strong>',
    '#required' => FALSE,
  );

  if ($edit) {
    // Markup field for proper styling.
    $form['buttons'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="form-actions">',
      '#suffix' => '</div>',
    );
    // Save
    $form['buttons']['feature_submit'] = array(
      '#type' => 'submit',
      '#value' => $submit,
    );
    // Add cancel option.
    $form['buttons']['feature_cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    );
  }
  else {
    $form['record_feature_add']['feature_submit'] = array(
      '#type' => 'submit',
      '#value' => $submit,
    );
  }
}

/**
 * Callback to add custom text field.
 */
function record_feature_add_custom($form, &$form_state) {
  return $form['record_feature_add']['ahah'];
}

/**
 * Helper function to theme all featues into a table
 */
function _record_feature_overview_form_table($features) {
  // Set headers.
  $headers = array(
    t('Name'),
    t('Data'),
    t('Status'),
    array('data' => t('Actions'), 'colspan' => 5),
  );

  // Compose rows.
  $rows = array();
  for ($i = 0; $i < count($features); $i++) {
    // Set status action and message based on current status.
    switch ($features[$i]['status']) {
      case 0: $record = t('Start recording');
              $status = t('Idle');
              break;
      case 1: $record = t('Stop recording');
              $status = '<strong>' . t('Recording') . '</strong>';
              break;
    }

    // Set data message.
    $data = unserialize($features[$i]['data']);
    $data_msg = array();
    $data_stored = FALSE;
    // $data is not an array if it's a blank newly created feature!
    if (is_array($data)) {
      foreach ($data as $key => $value) {
        if (is_array($value) && count($value) > 0) {
          $data_msg[] = '<strong>' . t('Changes to @name stored', array('@name' => $key)) . '</strong>';
          $data_stored = TRUE;
        }
        else if (is_array($value) && count($value) == 0) {
          $data_msg[] = '<em>'.t('No changes to @name detected during recording', array('@name' => $key)).'</em>';
        }
      }
    }
    // No data stored.
    if (empty($data_msg)) {
      $data_msg = t('Empty');
      $view = t('View');
    }
    else {
      $data_msg = implode('<br />', $data_msg);
    }

    // Set view and flush links.
    $view = t('View');
    $flush = t('Flush');
    if ($data_stored) {
      $view = l(t('View'), RECORD_FEATURE_BASE_PATH . '/'.$features[$i]['name']);
      $flush = l(t('Flush'), RECORD_FEATURE_BASE_PATH . '/'.$features[$i]['name'].'/flush');
    }

    // Actually compose the rows.
    $row = array(
      $features[$i]['name'],
      $data_msg,
      $status,
      /* Compose links. */
      $view,
      l(t('Edit'), RECORD_FEATURE_BASE_PATH . '/'.$features[$i]['name'].'/edit'),
      $flush,
      l(t('Delete'), RECORD_FEATURE_BASE_PATH . '/'.$features[$i]['name'].'/delete'),
      l($record, RECORD_FEATURE_BASE_PATH . '/'.$features[$i]['name'].'/togglerec'),
    );
    $rows[] = $row;
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Validation function to check for duplicate feature names
 */
function record_feature_overview_form_validate($form, &$form_state) {
  if (!isset($form_state['values']['action']) || $form_state['values']['action'] != 'edit') {
    if (isset($form_state['values']['name'])) {
      $feature = check_plain($form_state['values']['name']);
    }
    else if (isset($form_state['values']['existing']) && $form_state['values']['existing'] != '') {
      $feature = $form_state['values']['existing'];
    }
    else {
      form_set_error('existing', t('You must select an existing feature!'));
    }
    if ($feature != 'record_feature_temp_data') {
      $sql = "SELECT name FROM {recorded_features} WHERE name = :name";
      $result = db_query($sql, array(':name' => $feature))->fetchField();
      if (!empty($result)) {
        form_set_error('name', t('The feature name must be unique. A feature named %name already exists.', array('%name' => $feature)));
      }
    }
    else {
      form_set_error('name', t('<em>record_feature_temp_data</em> is a reserved name and cannot be used. Please choose a different name.'));
    }
  }
  // At least one option needs to be checked.
  $count = 0;
  foreach ($form_state['values']['options'] as $key => $value) {
    if ($value) {
      $count++;
    }
  }
  if ($count == 0) {
    form_set_error('options', t('At least one option needs to be checked.'));
  }
}

/**
 * Submit function to save new features to the database.
 */
function record_feature_overview_form_submit($form, &$form_state) {
  if (!empty($form_state['ahah_submission'])) {
    return;
  }

  $feature = new stdClass();
  $options = array();

  switch($form_state['values']['op']) {
    case t('Add feature'):
    case t('Save changes'):
      // Make an array of options.
      foreach ($form_state['values']['options'] as $key => $value) {
        if ($value) {
          $options[$key] = 1;
        }
      }
      // Fill the object and save.
      if (isset($form_state['values']['name'])) {
        $feature->name = check_plain($form_state['values']['name']);
      }
      else {
        $feature->name = $form_state['values']['existing'];
      }
      $feature->options = serialize($options);
      $feature->status = $form_state['values']['status'];
      if (isset($form_state['values']['action']) && $form_state['values']['action'] == 'edit') {
        $update = array('name');
      }
      else {
        $update = array();
        $feature->data = NULL;
      }
      $result = drupal_write_record('recorded_features', $feature, $update);
      if ($result) {
        drupal_set_message(t('Feature %name correctly saved.', array('%name' => $feature->name)));
        // Start recording immediately if necessary.
        if ($feature->status == 1) {
          _record_feature_start_recording($feature->name, TRUE);
        }
      }
      break;
  }
  drupal_goto(RECORD_FEATURE_BASE_PATH);
}

/**
 * Form to view a feature.
 * The way this was built doesn't feel right. Can't think of anything better at
 * the moment though :(
 */
function record_feature_view_feature_form($form, &$form_state, $feature) {
  $options = _record_feature_get_options($feature);
  $form = array();
  if (!isset($form_state['storage']['values'])) {
    // Display overview.
    drupal_set_title(t('View recorded feature'));
    $data = _record_feature_get_data($feature);
    // Prevent warning messages when users try to access the url directly when
    // they should not.
    if ($data == NULL) {
      $data = array();
    }
    foreach ($data as $type => $array) {
      if (is_array($array) && count($array) > 0) {
        $form['feature'] = array(
          '#type' => 'hidden',
          '#value' => $feature,
        );
        // Store data for theming to table later on.
        switch ($type) {
          case 'variables': _record_feature_view_feature_render_variables($form, $type, $array);
                           break;
          case 'permissions': _record_feature_view_feature_render_permissions($form, $type, $array);
                           break;
        }
        // Markup field for proper styling.
        $form['buttons'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="form-actions">',
          '#suffix' => '</div>',
        );
        $form['buttons']['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Export feature'),
          '#weight' => 49,
        );
      }
      else if ((is_array($array) && count($array) == 0)) {
        $form['empty'][$type] = array(
          '#type' => 'markup',
          '#markup' => '<h2>'.ucfirst($type).'</h2><p>'.t('No changes detected during recording.').'</p><p>&nbsp;</p>',
        );
      }
      else if ($options[$type]) {
        $form['empty'][$type] = array(
          '#type' => 'markup',
          '#markup' => '<h2>'.ucfirst($type).'</h2><p>'.t('No data stored yet.').'<br /><br /></p>',
        );
      }
    }
  }
  else {
    drupal_set_title(t('Export recorded feature'));
    // Display export results
    $form['results']['info'] = array(
      '#title' => t('Put this in @module.info in your modules/features/@module directory', array('@module' => $form_state['values']['feature'])),
      '#type' => 'textarea',
      '#rows' => 9,
      '#cols' => 60,
      '#resizable' => TRUE,
      '#value' => $form_state['storage']['values']['info'],
      '#required' => FALSE,
    );
    $form['results']['drush'] = array(
      '#type' => 'markup',
      '#markup' => '<p>'
                  .t('When you have added the above to <strong>@module.info</strong>, you can use', array('@module' => $form_state['values']['feature']))
                  .'<br /><br />&nbsp;<code>$ '.l('drush', 'http://www.drupal.org/project/drush').' features-update '.$form_state['values']['feature'].'</code><br /><br />'
                  .'or'
                  .'<br /><br />&nbsp;<code>$ '.l('drush', 'http://www.drupal.org/project/drush').' fu '.$form_state['values']['feature'].'</code><br /><br />'
                  .t('to update your feature.')
                  .'<br /><br /></p>',
    );
  }
  // Markup field for proper styling.
  if (!isset($form['buttons'])) {
    $form['buttons'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="form-actions">',
      '#suffix' => '</div>',
    );
  }
  $form['buttons']['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back to overview'),
    '#weight' => 50,
  );

  $form['#redirect'] = FALSE;
  return $form;
}

/**
 * Helper function to render variable data in form for later theming
 */
function _record_feature_view_feature_render_variables(&$form, $type, $array) {
  foreach ($array as $var => $value) {
    $options[$var] = '';
    // Activate all by default.
    $status[] = $var;
    if ($array[$var]['original'] != NULL) {
      $original = $array[$var]['original'];
    }
    else {
      $original = '<em>'.t('n/a').'</em>';
    }
    $form[$type][$var]['original'] = array(
      '#type' => 'value',
      '#value' => $original,
    );
    $form[$type][$var]['modified'] = array(
      '#type' => 'value',
      '#value' => $array[$var]['modified'],
    );
  }
  $form['export'][$type] = array(
   '#type' => 'checkboxes',
   '#options' => $options,
   '#default_value' => $status,
  );
}

/**
 * Helper function to render permission data in form for later theming
 */
function _record_feature_view_feature_render_permissions(&$form, $type, $array) {
  // $array contains roles and permissions
  foreach ($array as $role => $permissions) {
    // $permissions contains 'granted' and 'revoked' which in turn contain
    // arrays of permissions.
    foreach ($permissions as $state => $perms) {
      $options = $status = array();
      for ($i = 0; $i < count($perms); $i++) {
        // Only go on if there are perms for this state. If not, this state
        // contains an empty array.
        if ($perms[$i]) {
          $options[$perms[$i]] = '';
          // Activate all by default.
          $status[] = $perms[$i];
          $form[$type][$role][$state][$i] = array(
            '#type' => 'value',
            '#value' => $perms[$i],
          );
        }
      }
      // Prepend 'perm-' so we can easlily find these when parsing submitted
      // data.
      // Append role and state to ensure a unique name for each set of
      // checkboxes.
      $form['export'][$type]['perm-'.str_replace(' ', '_',$role).'-'.$state] = array(
       '#type' => 'checkboxes',
       '#options' => $options,
       '#default_value' => $status,
      );
    }
  }
}

/**
 * Export a feature
 */
function record_feature_view_feature_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case t('Export feature'):
      // Create module.info contents
      $info = '';
      $roles = $permissions = array();

      // Process variables & permissions.
      foreach ($form_state['values'] as $checkboxes => $values) {
        if (is_array($values) && ($checkboxes == 'variables' || strpos($checkboxes, 'perm-') === 0)) {
          foreach ($values as $key => $val) {
            // We had to cast $val to string in order for the if to work. Probably
            // because $val is int (0) when you untick a checkbox?
            if ((string)$val == $key) {
              if ($checkboxes == 'variables') {
                $info .= 'features[variable][] = "'.$key.'"'."\r\n";
              }
              else if (strpos($checkboxes, 'perm-') === 0) {
                // We prevent duplicates this way (we just override them ;-) )
                $permissions[$key] = 'features[user_permission][] = "'.$key.'"';
                $role = explode('-', $checkboxes);
                $role = str_replace('_', ' ', $role[1]);
                $roles[$role] = 'features[user_role][] = "'.$role.'"';
              }
            }
          }
        }
      }
      if (!empty($permissions)) {
        // Sort alphabetically.
        ksort($permissions, SORT_STRING);
        foreach ($permissions as $key => $value) {
          $info .= $value."\r\n";
        }
      }
      if (!empty($roles)) {
        // Sort alphabetically.
        ksort($roles, SORT_STRING);
        foreach ($roles as $key => $value) {
          $info .= $value."\r\n";
        }
      }
      // Store data to be displayed in 'step 2' of the form.
      $form_state['storage']['values']['info'] = $info;
      $form_state['rebuild'] = TRUE;
      break;
    case t('Back to overview'):
      drupal_goto(RECORD_FEATURE_BASE_PATH);
      break;
  }
}

/**
 * Helper function to theme all feature data into a table.
 */
function theme_record_feature_view_feature_form($variables) {
  $form = $variables['form'];
  $output = '';
  if (!isset($form['results'])) {
    // Variables table.
    if (isset($form['variables'])) {
      $output .= '<h2>'.t('Variables').'</h2>';
      // Set headers.
      $header = array(
        /* The 'checkbox' class is for centering */
        array('data' => t('Export'), 'class' => 'checkbox'),
        t('Variable name'),
        t('Original value'),
        t('New value'),
      );

      // Compose rows.
      $rows = array();
      foreach (element_children($form['variables']) as $key) {
        $row = array(
          array(
            'data' => drupal_render($form['export']['variables'][$key]),
            'class' => 'checkbox', /* This is for centering */
          ),
          $key,
          $form['variables'][$key]['original']['#value'],
          $form['variables'][$key]['modified']['#value'],
        );
        $rows[] = $row;
      }
      $output .= theme('table', array('header' => $header, 'rows' => $rows));
    }
    // Permissions table.
    if (isset($form['permissions'])) {
      // Add separator
      if (!empty($output)) {
        $output .= '<br />';
      }
      $output .= '<h2>'.t('Permissions').'</h2>';
      // Set headers.
      $header = array(
        t('Role'),
        /* The 'checkbox' class is for centering */
        array('data' => t('Export'), 'class' => 'checkbox'),
        t('Granted'),
        array('data' => t('Export'), 'class' => 'checkbox'),
        t('Revoked'),
      );

      // Compose rows.
      $rows = array();
      foreach ($form['permissions'] as $role => $permissions) {
        if (strpos($role, '#') !== 0) {
          $row = array(
            array('data' => '<strong>'.$role.'</strong>'),
            array('data' => '', 'colspan' => 4),
          );
          $rows[] = $row;
          $total_add = isset($permissions['granted']) ? count(element_children($permissions['granted'])) : 0;
          $total_rev = isset($permissions['revoked']) ? count(element_children($permissions['revoked'])) : 0;
          $total = $total_add;
          if ($total_add < $total_rev) {
            $total = $total_rev;
          }
          for ($i = 0; $i < $total; $i++) {
            $check_add = $check_rev = '';
            if ($i < $total_add) {
              $perm_add = $form['permissions'][$role]['granted'][$i]['#value'];
              $check_add = array(
                'data' => drupal_render($form['export']['permissions']['perm-'.str_replace(' ', '_',$role).'-granted'][$perm_add]),
                'class' => 'checkbox', /* This is for centering */
              );
            }
            if ($i < $total_rev) {
              $perm_rev = $form['permissions'][$role]['revoked'][$i]['#value'];
              $check_rev = array(
                'data' => drupal_render($form['export']['permissions']['perm-'.str_replace(' ', '_',$role).'-revoked'][$perm_rev]),
                'class' => 'checkbox', /* This is for centering */
              );
            }
            // Build rows.
            $row = array(
              '',
              $check_add,
              isset($form['permissions'][$role]['granted']) ? $form['permissions'][$role]['granted'][$i]['#value'] : '',
              $check_rev,
              isset($form['permissions'][$role]['revoked']) ? $form['permissions'][$role]['revoked'][$i]['#value'] : '',
            );
            $rows[] = $row;
          }
        }
      }

      $output .= theme('table', array('header' => $header, 'rows' => $rows));
    }
  }
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Form to edit a feature
 */
function record_feature_edit_feature_form($form, &$form_state, $feature) {
  drupal_set_title(t('Edit recorded feature') . ' <em>' . check_plain($feature) . '</em>', PASS_THROUGH);
  $form = array();

    // Add the 'add feature' part.
  _record_feature_add_feature_form($form, $form_state, FALSE, $feature);

  $form['#validate'] = array('record_feature_overview_form_validate');
  $form['#submit'] = array('record_feature_overview_form_submit');

  return $form;
}

/**
 * Form to delete a feature's data
 */
function record_feature_flush_feature_form($form, &$form_state, $feature) {
  $form = array();
  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => $feature,
  );

  return confirm_form($form, t('Are you sure you wish to delete all recorded data for %name?', array('%name' => $feature)), RECORD_FEATURE_BASE_PATH);
}

/**
 * Form to delete a feature's recorded data
 */
function record_feature_flush_feature_form_submit($form, &$form_state) {
  $feature = $form_state['values']['action'];
  if ($feature) {
    $result = _record_feature_flush_data($feature);
    if ($result === FALSE) {
      drupal_set_message(t('Unable to flush data for feature %name!', array('%name' => $feature)), 'error');
    }
    else {
      drupal_set_message(t('Recorded data for %name successfully flushed.', array('%name' => $feature)));
    }
  }
  drupal_goto(RECORD_FEATURE_BASE_PATH);
}

/**
 * Form to delete a feature
 */
function record_feature_delete_feature_form($form, &$form_state, $feature) {
  $form = array();
  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => $feature,
  );

  return confirm_form($form, t('Are you sure you wish to delete the feature %name?', array('%name' => $feature)), RECORD_FEATURE_BASE_PATH);
}

/**
 * Delete a feature
 */
function record_feature_delete_feature_form_submit($form, &$form_state) {
  $feature = $form_state['values']['action'];
  if ($feature) {
    $sql = "DELETE FROM {recorded_features} WHERE name = :name";
    $result = db_query($sql, array(':name' => $feature));
    if ($result === FALSE) {
      drupal_set_message(t('Unable to delete feature %name!', array('%name' => $feature)), 'error');
    }
    else {
      $recording = variable_get('record_feature_recording', FALSE);
      if ($recording['name'] == $feature) {
        $sql = "DELETE FROM {recorded_features} WHERE name = 'record_feature_temp_data'";
        $result = db_query($sql);
        if ($result === FALSE) {
          drupal_set_message(t('Unable to delete data that was being recorded for feature %name!', array('%name' => $feature)), 'error');
        }
        else {
          variable_del('record_feature_recording');
        }
      }
      drupal_set_message(t('Feature %name successfully deleted.', array('%name' => $feature)));
    }
  }
  drupal_goto(RECORD_FEATURE_BASE_PATH);
}

/**
 * Form to toggle recording
 */
function record_feature_toggle_recording_form($form, &$form_state, $feature) {
  $form = array();

  $form['feature'] = array(
    '#type' => 'hidden',
    '#value' => $feature,
  );

  if (_record_feature_get_status($feature) == 0) {
    $form['action'] = array(
      '#type' => 'hidden',
      '#value' => 'start',
    );
    $output = confirm_form($form, t('Are you sure you wish to start recording data for %name?', array('%name' => $feature)), RECORD_FEATURE_BASE_PATH, t('You can stop recording later on.'));
  }
  else {
    $form['action'] = array(
      '#type' => 'hidden',
      '#value' => 'stop',
    );
    $output = confirm_form($form, t('Are you sure you wish to stop recording data for %name?', array('%name' => $feature)), RECORD_FEATURE_BASE_PATH, t('You can continue recording later on without loss of data.'));
  }
  return $output;
}

/**
 * Toggle recording
 */
function record_feature_toggle_recording_form_submit($form, &$form_state) {
  if ($form_state['values']['feature'] && $form_state['values']['action']) {
    switch ($form_state['values']['action']) {
      case 'start':
        _record_feature_start_recording($form_state['values']['feature']);
        break;
      case 'stop':
        _record_feature_stop_recording($form_state['values']['feature']);
        break;
    }
  }
  drupal_goto(RECORD_FEATURE_BASE_PATH);
}

