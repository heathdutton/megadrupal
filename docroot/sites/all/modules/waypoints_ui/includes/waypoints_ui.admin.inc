<?php
/**
 * @file
 * waypoints_ui.admin.inc
 *
 * Replace waypoints_ui_entity with the actual entity name.
 */


/**
 * Settings form.
 */
function waypoints_ui_settings_form($form, $form_state) {
  // Get the current form waypoints.
  if ($form_state['rebuild']) {
    $waypoints = $form_state['values']['waypoints'];
  }
  else {
    $waypoints = variable_get('waypoints_ui_waypoints', array());
  }

  // Calculate the number of waypoint subforms.
  if (!empty($form_state['subform_count'])) {
    $subform_count = $form_state['subform_count'];
  }
  else {
    $subform_count = max(1, empty($waypoints) ? 1 : count($waypoints));
  }

  // Ajax replaced wrapper.
  $form['waypoints_wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div class="clearfix waypoints-wrapper">',
    '#suffix' => '</div>',
  );
  // Main data-tree element.
  $form['waypoints_wrapper']['waypoints'] = array(
    '#prefix' => '<div id="waypoint-definitions">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  $i = 1;
  foreach($waypoints as $key => $waypoint) {
    $form['waypoints_wrapper']['waypoints'][$key] = array(
      '#type' => 'fieldset',
      '#title' => t("Waypoint #$i"),
    );
    $i++;
    $form['waypoints_wrapper']['waypoints'][$key] += waypoints_ui_admin_subform($key, $waypoint);
  }

  // Add a new subform if this is an ajax build of the main form.
  $num_waypoints = count($waypoints);
  if ($subform_count > $num_waypoints) {
    // Get the last key to determine the next one.
    $keys = array_keys($waypoints);
    if (!empty($keys)) {
      $key = $keys[($num_waypoints-1)] + 1;
    }
    else {
      $key = 0;
    }
    // Wrap the new subform in a fieldset.
    $form['waypoints_wrapper']['waypoints'][$key] = array(
      '#type' => 'fieldset',
      '#title' => t('Waypoint #' . $subform_count),
    );
    // Add the new subform.
    $form['waypoints_wrapper']['waypoints'][$key] += waypoints_ui_admin_subform($key);
  }

  // Sort the waypoint subforms.
  asort($form['waypoints_wrapper']['waypoints']);

  // We name our button 'waypoints_more' to avoid conflicts with other modules using
  // Ajax-enabled buttons with the id 'more'.
  $form['waypoints_wrapper']['waypoints_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add another'),
    '#limit_validation' => array('waypoints'),
    '#attributes' => array(
      'title' => t("If you need another waypoint, click here to add another!"),
    ),
    '#weight' => 1,
    '#submit' => array('waypoints_ui_more_waypoints_submit'),
    '#ajax' => array(
      'callback' => 'waypoints_ui_js',
      'wrapper' => 'waypoint-definitions',
      'effect' => 'fade',
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Administration sub-form.
 */
function waypoints_ui_admin_subform($key, $values=array()) {
  $form['attach_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Waypoint Element Selector'),
    '#description' => t('Enter the DOM selector on which a waypoint will be set.'),
    '#default_value' => (isset($values['attach_element'])) ? $values['attach_element'] : '',
    //'#value' => (isset($values['attach_element'])) ? $values['attach_element'] : '',
    '#attributes' => array(
      'placeholder' => t("e.g. 'body', or 'div.whatever'"),
    ),
  );
  $form['toggle_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Toggle Class'),
    '#description' => t('Enter the name of a class that will toggle on the toggle element.'),
    '#default_value' => (isset($values['toggle_class'])) ? $values['toggle_class'] : '',
    //'#value' => (isset($values['toggle_class'])) ? $values['toggle_class'] : '',
    '#attributes' => array(
      'placeholder' => t("e.g. 'myclass', 'goo', or 'whatever'"),
    ),
  );
  $form['same_element'] = array(
    '#type' => 'checkbox',
    '#title' => t('Toggle on same waypoint element?'),
    '#description' => t('If checked then the configured class will toggle on the same element the waypoint is attached to.'),
    '#default_value' => (isset($values['same_element'])) ? $values['same_element'] : 1,
    //'#value' => (isset($values['same_element'])) ? $values['same_element'] : 1,
  );
  $form['toggle_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Toggle Element Selector'),
    '#description' => t('Enter the DOM selector that will toggle on the toggle class.'),
    '#default_value' => (isset($values['toggle_element'])) ? $values['toggle_element'] : '',
    //'#value' => (isset($values['toggle_element'])) ? $values['toggle_element'] : '',
    '#attributes' => array(
      'placeholder' => t("e.g. 'body', or 'div.whatever'"),
    ),
    '#states' => array(
      'invisible' => array(
        ':input[name="waypoints[' . $key . '][same_element]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['toggle_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Toggle Offset'),
    '#description' => t('Enter the percentage of the viewport that the toggle element must pass to trigger the waypoint. Defaults to 0% (top of the view port).'),
    '#default_value' => (isset($values['toggle_offset'])) ? $values['toggle_offset'] : 0,
    //'#value' => (isset($values['toggle_offset'])) ? $values['toggle_offset'] : 0,
    '#element_validate' => array('element_validate_number'),
    '#field_suffix' => '%',
  );
  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Remove'),
    '#name' => $key,
    '#limit_validation' => array('waypoints'),
    '#attributes' => array(
      'title' => t("Delete this waypoint"),
    ),
    '#weight' => 1,
    '#submit' => array('waypoints_ui_delete_waypoint_submit'),
    '#ajax' => array(
      'callback' => 'waypoints_ui_js',
      'wrapper' => 'waypoint-definitions',
      'effect' => 'fade',
    ),
  );
  return $form;
}

/**
 * Submit handler for the admin form add waypoints button.
 */
function waypoints_ui_more_waypoints_submit($form, &$form_state) {
  if ($form_state['values']['waypoints_more']) {
    $form_state['subform_count'] = count($form_state['values']['waypoints']) + 1;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove" button.
 */
function waypoints_ui_delete_waypoint_submit($form, &$form_state) {
  $form_state['subform_count'] = count($form_state['values']['waypoints']) - 1;
  $form_state['values']['waypoints'][$form_state['triggering_element']['#parents'][1]] = FALSE;
  $form_state['values']['waypoints'] = array_filter($form_state['values']['waypoints']);
  /*
  // Refactor the waypoints array to just a serial list. Uncomplicates form build.
  $i = 1;
  $new_waypoints = array();
  foreach($form_state['values']['waypoints'] as $waypoint) {
    $new_waypoints['waypoint-' .  $i++] = $waypoint;
  }
  $form_state['values']['waypoints'] = $new_waypoints;
  */
  waypoints_ui_settings_form_submit($form, $form_state);
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for the admin form add waypoints button.
 */
function waypoints_ui_js($form, $form_state) {
  return $form['waypoints_wrapper']['waypoints'];
}

/**
 * Submit handler for the admin form.
 */
function waypoints_ui_settings_form_submit($form, $form_state) {
  $waypoints = array();
  foreach($form_state['values']['waypoints'] as $key => $waypoint) {
    if (!is_array($waypoint) || ($waypoint['attach_element'] == null)) {
      continue;
    }
    $waypoints[$key] = $waypoint;
  }
  variable_set('waypoints_ui_waypoints', $waypoints);
  drupal_set_message(t('Waypoints saved!'), 'status', FALSE);
}
