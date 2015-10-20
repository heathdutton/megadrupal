<?php
/**
 * @file
 * Configuration pages for the emergency module.
 */

/**
 * Form builder to manage emergency levels.
 */
function emergency_overview_levels($form, &$form_state) {
  $form = array('#tree' => TRUE);

  // Management links and weights for emergency levels.
  $levels = emergency_level_load_all();
  foreach ($levels as $level) {
    $form[$level->type]['#emergency_level'] = $level;
    $form[$level->type]['name'] = array('#markup' => check_plain($level->name));
    if (module_exists('ctools') && isset($level->export_type)) {
      switch ($level->export_type) {
        case EXPORT_IN_CODE:
          $status = t('In code');
          break;
        case EXPORT_IN_DATABASE:
          $status = t('In database');
          break;
        case EXPORT_IN_CODE && EXPORT_IN_DATABASE:
          $status = t('Database overriding code');
          break;
        default:
          $status = t('Unknown');
      }
      $form[$level->type]['name']['#markup'] .= '<div class="description">' . t('Status: @status', array('@status' => $status)) . '</div>';
    }
    $form[$level->type]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @name', array('@title' => $level->name)),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#default_value' => $level->weight,
    );
    $form[$level->type]['configure'] = array(
      '#type' => 'link',
      '#title' => t('configure response'),
      '#href' => 'admin/structure/emergency/' . $level->type,
    );
    $form[$level->type]['edit'] = array(
      '#type' => 'link',
      '#title' => t('edit'),
      '#href' => 'admin/structure/emergency/' . $level->type . '/edit',
      '#access' => emergency_level_edit_access($level),
    );
    if (empty($level->in_code_only)) {
      $form[$level->type]['delete'] = array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/structure/emergency/' . $level->type . '/delete',
        '#access' => emergency_level_edit_access($level),
      );
    }
  }

  // Only include the weight field if more than one emergency level exists.
  if (count($levels) == 1) {
    unset($form[$level->type]['weight']);
  }

  // Link path for the summary block.
  $form['link_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Link the summary block to'),
    '#default_value' => variable_get('emergency_link_path', '<front>'),
    '#maxlength' => 255,
    '#description' => t('Optionally, specify the path to which the emergency summary block should link for more details. This can be an internal Drupal path such as %add-node or an external URL such as %drupal. Enter %front to link to the front page. Typically, this would be set to a page where the emergency details block is set to appear, usually the front page. If left blank, the summary block will not include a link.', array('%front' => '<front>', '%add-node' => 'node/add', '%drupal' => 'http://drupal.org')),
  );

  // Option to clear the cache.
  $form['cache_clear'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear cache when emergency status or information changes'),
    '#description' => t('By default, the block and page caches are cleared when an emergency is declared or canceled, as well as anytime the information for an active emergency is updated. If disabled, visitors may see stale information, depending on your <a href="@url">cache settings</a>.', array('@url' => url('admin/config/development/performance'))),
    '#default_value' => variable_get('emergency_cache_clear', TRUE),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation handler for managing emergency levels.
 */
function emergency_overview_levels_validate($form, &$form_state) {
  $link_path = &$form_state['values']['link_path'];
  $link_path = trim($link_path);
  if (!empty($link_path) && !drupal_valid_path($link_path, TRUE)) {
    form_set_error('link_path', t("The path '@link_path' is either invalid or you do not have access to it.", array('@link_path' => $link_path)));
  }
}

/**
 * Submit handler for managing emergency levels.
 */
function emergency_overview_levels_submit($form, &$form_state) {
  foreach ($form_state['values'] as $type => $level) {
    if (isset($form[$type]['#emergency_level']) && isset($level['weight']) && $form[$type]['#emergency_level']->weight != $level['weight']) {
      $form[$type]['#emergency_level']->weight = $level['weight'];
      emergency_level_save($form[$type]['#emergency_level']);
    }
  }
  variable_set('emergency_link_path', $form_state['values']['link_path']);
  variable_set('emergency_cache_clear', $form_state['values']['cache_clear']);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Returns HTML for the emergency levels management form.
 */
function theme_emergency_overview_levels($variables) {
  $form = $variables['form'];
  $rows = array();
  $weight = FALSE;

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $level = &$form[$key];

      $row = array();
      $row[] = drupal_render($level['name']);
      if (isset($level['weight'])) {
        $weight = TRUE;
        $level['weight']['#attributes']['class'] = array('emergency-level-weight');
        $row[] = drupal_render($level['weight']);
      }
      $row[] = drupal_render($level['configure']);
      $row[] = drupal_render($level['edit']);
      $row[] = drupal_render($level['delete']);

      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
      );
    }
  }

  $header = array(t('Emergency level'));
  if ($weight) {
    $header[] = t('Weight');
    drupal_add_tabledrag('emergency-levels', 'order', 'sibling', 'emergency-level-weight');
  }
  $header[] = array(
    'data' => t('Operations'),
    'colspan' => '3',
  );

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No emergency levels available. <a href="@link">Add emergency level</a>.', array('@link' => url('admin/structure/emergency/add'))),
    'attributes' => array('id' => 'emergency-levels'),
  )) . drupal_render_children($form);
}

/**
 * Form builder to add or edit an emergency level.
 */
function emergency_form_level($form, &$form_state, $level = NULL) {
  // Ensure that the level object exists and contains no NULL values.
  $defaults = array(
    'type' => '',
    'name' => '',
    'weight' => 0,
  );
  if (!isset($level)) {
    $level = (object) $defaults;
  }
  else {
    foreach ($defaults as $key => $value) {
      if (!isset($level->$key)) {
        $level->$key = $value;
      }
    }
  }

  // Save the original level values for later reference.
  if (!empty($level->type)) {
    $level->old_type = $level->type;
  }
  $form_state['level'] = $level;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $level->name,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => $level->type,
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'emergency_level_load',
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (!empty($level->type)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  return $form;
}

/**
 * Validation handler for adding or editing an emergency level.
 */
function emergency_form_level_validate($form, &$form_state) {
  // Disallow machine names that evaluate to FALSE (reserved for the built-in
  // "Normal" level) or that conflict with path arguments.
  $type = empty($form_state['values']['type']) ? FALSE : $form_state['values']['type'];
  $disallowed = array('add', 'list');
  if (!$type || in_array($type, $disallowed)) {
    form_set_error('type', t('The machine-readable name cannot be "add" or "list," and cannot evaluate to FALSE (i.e. "0").'));
  }
}

/**
 * Submit handler for adding or editing an emergency level.
 */
function emergency_form_level_submit($form, &$form_state) {
  $level = $form_state['level'];

  // If the Delete button was clicked, redirect to the delete callback.
  if ($form_state['triggering_element']['#value'] == t('Delete')) {
    $form_state['redirect'] = 'admin/structure/emergency/' . $level->old_type . '/delete';
      return;
  }

  // Update the level object with the corresponding form values.
  foreach ($form_state['values'] as $key => $value) {
    if (isset($level->$key)) {
      $level->$key = $value;
    }
  }

  switch (emergency_level_save($level)) {
    case SAVED_NEW:
      $message = 'Created new emergency level %name.';
      break;
    case SAVED_UPDATED:
      $message = 'Updated emergency level %name.';
      break;
  }
  if (isset($message)) {
    $args = array('%name' => $level->name);
    drupal_set_message(t($message, $args));
    watchdog('emergency', $message, $args, WATCHDOG_NOTICE, l(t('edit'), 'admin/structure/emergency/' . $level->type . '/edit'));
  }

  $form_state['redirect'] = 'admin/structure/emergency';
}

/**
 * Form builder to confirm deletion of an emergency level.
 */
function emergency_level_confirm_delete($form, &$form_state, $level) {
  $form['type'] = array('#type' => 'value', '#value' => $level->type);
  $form['name'] = array('#type' => 'value', '#value' => $level->name);
  return confirm_form($form,
    t('Are you sure you want to delete the emergency level %name?', array('%name' => $level->name)),
    'admin/structure/emergency',
    t('Deleting an emergency level will delete its associated layout response and notification message.  This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * Submit handler for confirming deletion of an emergency level.
 */
function emergency_level_confirm_delete_submit($form, &$form_state) {
  emergency_level_delete($form_state['values']['type']);
  drupal_set_message(t('Deleted emergency level %name.', array('%name' => $form_state['values']['name'])));
  watchdog('emergency', 'Deleted emergency level %name.', array('%name' => $form_state['values']['name']), WATCHDOG_NOTICE);
  $form_state['redirect'] = 'admin/structure/emergency';
}

/**
 * Form builder to configure an emergency level response.
 */
function emergency_form_response($form, &$form_state, $level, $theme = NULL) {
  // Get the available regions for the theme.
  if (empty($theme)) {
    $theme = variable_get('theme_default', 'bartik');
  }
  $regions = system_region_list($theme, REGIONS_VISIBLE);

  // Save the arguments for later reference.
  $form_state['level'] = $level;
  $form_state['theme'] = $theme;

  // Configure visibility of block regions.
  $region_variable = variable_get('emergency_' . $level->type . '_regions_' . $theme, array());
  $form['regions'] = array('#tree' => TRUE);
  foreach ($regions as $key => $name) {
    $form['regions'][$key] = array(
      '#type' => 'select',
      '#title' => check_plain($name),
      '#options' => array(
        EMERGENCY_REGION_ENABLED => t('Enabled'),
        EMERGENCY_REGION_DISABLED_FRONTPAGE => t('Disabled on front page'),
        EMERGENCY_REGION_DISABLED => t('Disabled'),
      ),
      '#default_value' => empty($region_variable[$key]) ? EMERGENCY_REGION_ENABLED : $region_variable[$key],
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for configuring an emergency level response.
 */
function emergency_form_response_submit($form, &$form_state) {
  $level = $form_state['level'];
  $theme = $form_state['theme'];
  $region_variable = 'emergency_' . $level->type . '_regions_' . $theme;

  variable_set($region_variable, $form_state['values']['regions']);
}

/**
 * The emergency management form.
 */
function emergency_form_manage($form, &$form_state) {
  $form = array();
  $levels = emergency_levels();
  $current = variable_get('emergency', '0');

  if (isset($form_state['emergency_level'])) {
    // Use the emergency level set by previous form builds, if available.
    $type = $form_state['emergency_level'];
  }
  else {
    // Otherwise, use the current state of emergency.
    $type = $current;
    $form_state['emergency_level'] = $type;
  }

  if (!$type) {
    // No current emergency; show options to declare one.
    $form['emergency_level'] = array(
      '#type' => 'radios',
      '#title' => t('Emergency level'),
      '#default_value' => $type,
      '#options' => $levels,
      '#required' => TRUE,
    );
  }
  else {
    // An emergency is active; show the declaration form.
    $form['emergency_level'] = array('#type' => 'value', '#value' => $type);
    $emergency = emergency_load($type);

    // Ensure that the emergency object exists and is populated.
    $defaults = array(
      'title' => '',
      'details' => '',
      'format' => NULL,
      'start' => 0,
      'end' => 0,
      'additional' => '',
    );
    if (empty($emergency)) {
      $emergency = (object) $defaults;
    }
    else {
      foreach ($defaults as $key => $value) {
        if (!isset($emergency->$key)) {
          $emergency->$key = $value;
        }
      }
    }
    // Save the emergency object for later use.
    $form_state['emergency'] = $emergency;

    // If the start time was cleared, reset to current time.
    $start = empty($emergency->start) ? time() : $emergency->start;
    $form['start'] = array('#type' => 'value', '#value' => $start);

    if ($type == $current) {
      $form['current']['level'] = array(
        '#prefix' => '<h2>',
        '#markup' => t('Current emergency level is %name', array('%name' => $levels[$type])),
        '#suffix' => '</h2>',
      );
      $form['current']['start'] = array(
        '#markup' => t('Emergency was declared at %time.', array('%time' => format_date($start, 'short'))),
      );
    }
    $form['current']['actions'] = array('#type' => 'actions');
    $form['current']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel emergency'),
    );

    $form['heading'] = array(
      '#prefix' => '<h2>',
      '#markup' => t('Emergency information'),
      '#suffix' => '</h2>',
    );
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title or brief summary'),
      '#default_value' => $emergency->title,
      '#size' => 80,
      '#maxlength' => 255,
    );
    $form['details'] = array(
      '#type' => 'text_format',
      '#title' => t('Details'),
      '#default_value' => $emergency->details,
      '#format' => $emergency->format,
    );
    // Though it would be better to name this field 'additional' instead of
    // 'details][summary', doing it this way should allow WYSIWYG editors to
    // treat the additional details field the same way as a Body field summary,
    // and thus react properly to changes in text format selection.
    // Better solution: emergency becomes a fieldable entity (possibly in 2.x).
    $form['details']['summary'] = array(
      '#type' => 'textarea',
      '#title' => t('Additional details'),
      '#default_value' => $emergency->additional,
      '#weight' => 10,
      '#description' => t('Inherits the text format setting from the Details field.'),
    );
    $form['end'] = array(
      '#type' => 'textfield',
      '#title' => t('End at'),
      '#maxlength' => 25,
      '#default_value' => $emergency->end ? format_date($emergency->end, 'custom', 'Y-m-d H:i:s') : '',
      '#description' => t('If set, the emergency will be automatically cancelled after this time. Leave blank to require manual cancellation.<br />Format: %time. Enter as local time, according to the site default time zone.<br /><strong>Note that this depends on <a href="@cron">cron</a>, which is currently set to run every @interval.</strong>', array('%time' => format_date(time(), 'custom', 'Y-m-d H:i:s'), '@cron' => url('admin/config/system/cron'), '@interval' => format_interval(variable_get('cron_safe_threshold', DRUPAL_CRON_DEFAULT_THRESHOLD)))),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $type ? t('Save') : t('Declare emergency'),
  );

  return $form;
}

/**
 * Validation handler for managing emergencies.
 */
function emergency_form_manage_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == t('Cancel emergency')) {
    $form_state['values']['emergency_level'] = '0';
  }
}

/**
 * Submit handler for managing emergencies.
 */
function emergency_form_manage_submit($form, &$form_state) {
  $old_type = $form_state['emergency_level'];
  $new_type = $form_state['values']['emergency_level'];

  // Check whether the emergency level has changed.
  if ($old_type != $new_type) {
    if ($new_type) {
      // If we're declaring an emergency, set the form state to act as though
      // the new emergency level has already been saved and rebuild for details.
      $form_state['emergency_level'] = $new_type;
      $form_state['rebuild'] = TRUE;
    }
    else {
      // If we're setting back to Normal, there's no need to rebuild to show the
      // details form, so just save the new value.
      emergency_cancel();
    }
    return;
  }

  // Save the emergency details.
  if ($new_type) {
    emergency_declare(
      $form_state['values']['emergency_level'],
      $form_state['values']['title'],
      $form_state['values']['details']['value'],
      $form_state['values']['details']['format'],
      $form_state['values']['end'],
      $form_state['values']['details']['summary']);
  }
}
