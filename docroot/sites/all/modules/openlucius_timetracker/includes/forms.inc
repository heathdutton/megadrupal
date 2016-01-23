<?php
/**
 * @file
 * This file contains the forms and their validation / submit methods.
 */

define("TIME_TRACKER_START_BUTTON", '<span class="glyphicon glyphicon-play"></span>');
define("TIME_TRACKER_STOP_BUTTON", '<span class="glyphicon glyphicon-stop"></span>');
define("TIMETRACKER_VOCABULARY_NAME", 'timetracker_type');

/**
 * Form constructor for the configuration form of the time-tracker.
 * @ingroup forms
 */
function openlucius_timetracker_config_form($form, $form_state) {

  $form['openlucius_timetracker_max_message'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Message to display when a case has reached its maximum.'),
    '#default_value' => variable_get('openlucius_timetracker_max_message', t('Clocking is not possible, this case has exceeded 100%.')),
  );

  // Fieldset for adding taxonomy terms.
  $form['openlucius_timetracker_vocabulary_terms'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Timetracker type'),
  );

  $form['openlucius_timetracker_vocabulary_terms']['taxonomy_overview'] = array(
    '#type'         => 'markup',
    '#markup'       => openlucius_timetracker_build_vocabulary_terms_table(),
  );

  // Textfield for the term.
  $form['openlucius_timetracker_vocabulary_terms']['timetracker_term'] = array(
    '#type'         => 'textfield',
    '#title'        => t('Add term'),
  );

  // Extra submit for adding terms.
  $form['submit'] = array(
    '#type'         => 'submit',
    '#value'        => t('Add term'),
  );

  $form['#submit'][] = 'openlucius_timetracker_add_taxonomy_term_submit';

  return system_settings_form($form);
}

/**
 * Custom form submit for the config form.
 */
function openlucius_timetracker_add_taxonomy_term_submit($form, &$form_state) {

  // Check if there was a term filled in.
  if (!empty($form_state['values']['timetracker_term'])) {

    // Check the term.
    $name = check_plain($form_state['values']['timetracker_term']);

    // Load the timetracker type vocabulary.
    $vocabulary = taxonomy_vocabulary_machine_name_load(TIMETRACKER_VOCABULARY_NAME);

    // Create a new taxonomy term.
    $term = new stdClass();
    $term->name = $name;
    $term->vid = $vocabulary->vid;
    taxonomy_term_save($term);

    drupal_set_message(t('Taxonomy term added.'));
  }
}

/**
 * Form constructor for the time-tracker form.
 * @ingroup forms
 */
function openlucius_timetracker_timer_form($form, &$form_state) {
  global $user;

  // Load the functions for the timetracker.
  module_load_include('inc', 'openlucius_timetracker', 'includes/functions');

  // Only show if the user has rights to create time entries.
  if (!user_access('create time entries') || $user->uid == 1) {
    return FALSE;
  }

  // Add javascript required for displaying and using the timetracker.
  drupal_add_js(drupal_get_path('module', 'openlucius_timetracker') . '/js/openlucius_timetracker.js');

  $form = array();

  // Load the active timer for this user.
  $active = _openlucius_timetracker_timer_get_active($user->uid);

  $node = menu_get_object();
  if (!empty($node) && $node->type == 'ol_todo') {
    $nid = $node->nid;

    // If we're on a node and there's no active timer hide the form using css.
    if (empty($active)) {
      $form['#attributes']['style'] = array('display:none;');
    }
    // Load other node for active timer.
    elseif ($active->nid != $nid) {
      $node2 = node_load($active->nid);
    }
  }
  // We're on a different page so use the active time entry for loading.
  elseif (!empty($active)) {
    $node = node_load($active->nid);
    $nid  = $active->nid;
  }
  else {
    // Nothing to do here.
    return FALSE;
  }

  // If there is an active timer and that timer
  // is on the current node it is in a "start" state.
  if ($active && $active->nid == $nid) {

    // So the label should be stop.
    $label = TIME_TRACKER_STOP_BUTTON;
  }
  // There is an active timer, but not for this node.
  // Or no timer at all, "stopped" state.
  elseif (($active && $active != $nid) || !$active) {

    // So the label should be start.
    $label = TIME_TRACKER_START_BUTTON;
  }

  $form['#prefix'] = '<div id="timetracker-form">';
  $form['#suffix'] = '</div>';

  // Store the node id.
  $form['nid'] = array(
    '#value' => $nid,
  );
  // Store the user id.
  $form['uid']   = array(
    '#value' => $user->uid,
  );
  $form['title'] = array(
    '#markup' => '<h2 class="block-title">' . t('Time tracking') . '</h2>',
  );

  // Initialize the array.
  $form_variables = array();

  // Append the node.
  openlucius_timetracker_form_process_node($node, $active, 1, $form_variables);

  // If we're clocked in another node fetch that time as well.
  if (isset($node2)) {
    openlucius_timetracker_form_process_node($node2, $active, 2, $form_variables);
  }

  // Append variables.
  drupal_add_js(array(
    'openlucius_timetracker' => $form_variables,
  ), 'setting');

  // Add start of header wrapper.
  $form['wrapper']['#markup'] = '<span class="time-header collapsed">';

  // Add the submit button .
  $form['submit'] = array(
    '#type'       => 'submit',
    // Dynamic label.
    '#value'      => $label,
    // For the ajax replace.
    '#prefix'     => '<span class="button">',
    '#suffix'     => '</span>',
    '#ajax'       => array(
      'callback' => 'openlucius_timetracker_timer_form_ajax_submit',
      // Id the ajax replaces.
      'wrapper'  => 'timetracker-form',
    ),
    '#attributes' => array(
      'class' => array(
        'btn-xs',
        'main-ajax-submit',
      ),
    ),
    '#id'         => 'openlucius_timetracker_submit',
  );

  // Add class to hide item initially.
  if (isset($node2)) {
    $form['submit']['#prefix'] = '<span class="button hide-initially">';
  }

  // Append data for secondary node.
  if (isset($node2) && isset($form_variables['current_time2'])) {

    // Add the secondary "submit" button.
    $button                    = array(
      '#type'       => 'button',
      '#value'      => TIME_TRACKER_STOP_BUTTON,
      '#prefix'     => '<span class="button">',
      '#suffix'     => '</span>',
      '#attributes' => array(
        'class'      => array(
          'btn-xs',
          'secondary-submit',
        ),
        'data-token' => drupal_get_token(),
      ),
    );
    $form_variables['submit2'] = drupal_render($button);
  }

  // Add themed body.
  $form['markup']['#markup'] = theme('openlucius_timetracker_body', $form_variables);

  // Return the form.
  return $form;
}

/**
 * Method for processing a todo and appending the results to an array.
 *
 * @param \stdClass $node
 *   The node to be processed.
 * @param mixed $active
 *   The active node or FALSE.
 * @param int $num
 *   The current entry (prefix).
 * @param array $form_variables
 *   The array the values are merged into.
 */
function openlucius_timetracker_form_process_node($node, $active = FALSE, $num = 1, array &$form_variables) {
  // Load the time for this node.
  $clocked = openlucius_timetracker_get_time_for_node($node);

  // Check if empty if it is set to 0.
  if (empty($clocked)) {
    $clocked = array(
      'user'  => 0,
      'total' => 0,
    );
  }

  // Fetch the maximum amount of time.
  $max = 0;
  if (!empty($node->field_shared_time_maximum)) {
    $max = $node->field_shared_time_maximum[LANGUAGE_NONE][0]['value'] * 3600;
  }

  $is_active = FALSE;
  // Check if this node is the active node.
  if ($node->nid === $active->nid) {
    $is_active = TRUE;
    $diff = time() - $active->start;

    // Also update the totals with the active timer.
    $clocked['user'] += $diff;
    $clocked['total'] += $diff;
  }

  // Fetch the progress for the node.
  $progress         = openlucius_timetracker_calculate_percent($clocked['total'], $max);
  $progress_display = $progress . '%';

  // Fetch the current time.
  $current_time = openlucius_timetracker_format_time($clocked['user']);
  $current_time .= sprintf(":%02d", $clocked['user'] % 60);

  // Prepare variables for merge with $form_variables.
  $variables = array(
    'active' . $num           => $is_active,
    'title' . $num            => l($node->title, 'node/' . $node->nid, array(
        'attributes' => array(
          'onclick' => 'document.location.href = this.href',
        ),
      )),
    'current_time' . $num     => $current_time,
    'your_time' . $num        => t('Your tracked time') . ': ' . openlucius_timetracker_format_time($clocked['user']),
    'total_progress' . $num   => t('Total tracked time') . ': ' . openlucius_timetracker_format_time($clocked['total']),
    'current_progress' . $num => t('Current progress') . ': ' . $progress_display,
    'maximum' . $num          => t('Time limit') . ': ' . openlucius_timetracker_format_time($max),
    'max' . $num              => $max,
    'start_time' . $num       => $clocked['user'],
  );

  // Add new entries.
  $form_variables += $variables;
}

/**
 * Ajax form submission handler for openlucius_timetracker_timer_form().
 */
function openlucius_timetracker_timer_form_ajax_submit($form, &$form_state) {
  $uid = $form['uid']['#value'];
  $nid = $form['nid']['#value'];

  // Load the functions for the timetracker.
  module_load_include('inc', 'openlucius_timetracker', 'includes/functions');

  // Get the active timer.
  $active = _openlucius_timetracker_timer_get_active($uid);

  // If there is an active timer and that timer
  // is on the current node it is in a "start" state.
  if ($active && $active->nid == $nid) {
    // Pressing the button means stop.
    _openlucius_timetracker_timer_stop($uid);
  }
  // There is an active timer, but not for this node,
  // or no timer at all, "stopped" state.
  elseif (($active && $active != $nid) || !$active) {
    // Pressing the button means start.
    _openlucius_timetracker_timer_start($uid, $nid);
  }
}

/**
 * Form constructor for the time-tracker extra time form.
 * @see openlucius_timetracker_extra_time_form_validate()
 * @see openlucius_timetracker_extra_time_form_submit()
 * @ingroup forms
 */
function openlucius_timetracker_extra_time_form($form, &$form_state) {
  global $user;

  // Load processing functions.
  module_load_include('inc', 'openlucius_timetracker', 'includes/functions');

  $uid  = $user->uid;
  $node = menu_get_object();

  $form['uid'] = array(
    '#type'  => 'hidden',
    '#value' => $uid,
  );

  // Default you can only change your own entries.
  $users = array($user->uid => check_plain($user->name));

  // Allow other modules to changes the users.
  drupal_alter('openlucius_timetracker_extra_users', $users);

  $form['user'] = array(
    '#type'          => 'select',
    '#title'         => t('User'),
    '#options'       => $users,
    '#required'      => TRUE,
    '#default_value' => $user->uid,
  );
  $form['date'] = array(
    '#type'  => 'date',
    '#title' => t('Provide a date for the time correction.'),
  );

  // Check if there is a maximum, if there is none there is nothing to do.
  if (!empty($node->field_shared_time_maximum)) {
    $max      = $node->field_shared_time_maximum[LANGUAGE_NONE][0]['value'] * 3600;
    $clocked  = openlucius_timetracker_get_time_for_node($node);
    $progress = openlucius_timetracker_calculate_percent($clocked['total'], $max);
  }

  // If the todo has progressed over the max,
  // only allow for negative corrections.
  if ($progress >= 100) {
    $options = array(1 => t('-'));
  }
  // Default, allow positive and negative corrections.
  else {
    $options = array(0 => t('+'), 1 => t('-'));
  }

  $form['negative']      = array(
    '#type'    => 'select',
    '#title'   => t('Type'),
    '#options' => $options,
    '#prefix'  => '<div style="float: left;width: 50px;">',
    '#suffix'  => '</div>',
  );
  $form['extra_hours']   = array(
    '#type'        => 'textfield',
    '#title'       => t('Extra hours'),
    '#description' => t('Provide the time in hours.'),
    '#required'    => TRUE,
    '#prefix'      => '<div style="margin-left:15px;float: left;width: 100px;">',
    '#suffix'      => '</div>',
  );
  $form['extra_minutes'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Extra minutes'),
    '#description' => t('Provide the time in minutes.'),
    '#required'    => TRUE,
    '#prefix'      => '<div style="margin-left:15px;float: left;width: 100px;">',
    '#suffix'      => '</div>',
  );
  $form['message']       = array(
    '#type'        => 'textarea',
    '#title'       => t('Description'),
    '#description' => t('Provide a description for the extra time.'),
    '#required'    => TRUE,
  );
  $form['submit']        = array(
    '#type'       => 'submit',
    '#value'      => t('Add time'),
    '#attributes' => array(
      'class' => array('btn-sm'),
    ),
  );

  // Return the form.
  return $form;
}

/**
 * Form validation handler for openlucius_timetracker_extra_time_form().
 * @see openlucius_timetracker_extra_time_form_submit()
 */
function openlucius_timetracker_extra_time_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Verify that the extra_hours value is numeric.
  if (!is_numeric($values['extra_hours'])) {
    form_set_error('extra_hours', t('Time value can only be numeric') . '.');
  }

  // Verify that the extra_minutes value is numeric.
  if (!is_numeric($values['extra_minutes'])) {
    form_set_error('extra_minutes', t('Time value can only be numeric') . '.');
  }

  // Verify that a user has set a message for an extra time enty.
  if (!isset($values['message']) || empty($values['message'])) {
    form_set_error('message', t('A description for the extra time is required') . '.');
  }
}

/**
 * Form submission handler for openlucius_timetracker_extra_time_form().
 * @see openlucius_timetracker_extra_time_form_validate()
 */
function openlucius_timetracker_extra_time_form_submit($form, &$form_state) {
  $node   = menu_get_object();
  $nid    = $node->nid;
  $values = $form_state['values'];

  $extra_time = ($values['extra_hours'] * 3600) + ($values['extra_minutes'] * 60);
  if ($values['negative']) {
    $extra_time = '-' . $extra_time;
  }

  // Cleanup string before insert.
  $message = check_plain($values['message']);

  // Create new timestamp for a given moment.
  $time = mktime(0, 0, 0, $values['date']['month'], $values['date']['day'], $values['date']['year']);

  // Create new time entry with message.
  db_insert('openlucius_timetracker_entry')->fields(array(
    'uid'     => $values['user'],
    'nid'     => $nid,
    'start'   => $time,
    'stop'    => $time + $extra_time,
    'message' => $message,
  ))->execute();
}

/**
 * Form constructor for the time-tracker filter form.
 * @ingroup forms
 */
function openlucius_timetracker_filter_form($form, &$form_state) {

  $menu_item = menu_get_item();
  // If a extra type is loaded, store it in the form.
  if (isset($menu_item['page_arguments'][1])) {
    $form['type'] = array(
      '#type'  => 'hidden',
      '#value' => $menu_item['page_arguments'][1],
    );
  }

  // Get all the terms that are needed to display.
  $terms = openlucius_core_get_terms('labels_todo_s');

  // Get all the timetrackers types that are needed to display.
  $users    = openlucius_timetracker_get_users(TRUE);
  $users[0] = t('All users');

  drupal_alter('openlucius_timetracker_filter_users', $users);

  $form['status']          = array(
    '#type'  => 'fieldset',
    '#title' => t('Status'),
  );
  $form['status']['state'] = array(
    '#type'          => 'select',
    '#options'       => $terms,
    '#default_value' => explode(',', $_GET['state']),
    '#multiple'      => TRUE,
  );

  $form['user-filter'] = array(
    '#type'  => 'fieldset',
    '#title' => t('User'),
  );

  $form['user-filter']['user'] = array(
    '#type'          => 'select',
    '#options'       => $users,
    '#default_value' => $_GET['user'],
  );

  $form['filter']['time'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Last activity'),
  );

  $default = NULL;
  if ($_GET['start']) {
    $default = date('Y-m-d', $_GET['start']);
  }
  $form['filter']['time']['timestart'] = array(
    '#type'          => 'date_popup',
    '#title'         => t('From'),
    '#default_value' => $default,
    '#date_format'   => 'd/m/Y',
  );

  $default_end = NULL;
  if ($_GET['end']) {
    $default_end = date('Y-m-d', $_GET['end']);
  }

  $form['filter']['time']['timeend'] = array(
    '#type'          => 'date_popup',
    '#title'         => t('until'),
    '#default_value' => $default_end,
    '#date_format'   => 'd/m/Y',
  );
  $form['filter']['time']['submit']  = array(
    '#type'  => 'submit',
    '#value' => 'submit',
  );
  return $form;
}

/**
 * Form submission handler for openlucius_timetracker_filter_form().
 */
function openlucius_timetracker_filter_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $query  = array();

  if ($values['state']) {
    $query['state'] = implode(',', $values['state']);
  }

  if ($values['timestart']) {
    $start          = $values['timestart'];
    $year           = substr($start, 0, 4);
    $month          = substr($start, 5, 2);
    $day            = substr($start, 8, 2);
    $query['start'] = mktime(0, 0, 0, $month, $day, $year);
  }
  if ($values['timeend']) {
    $end          = $values['timeend'];
    $year         = substr($end, 0, 4);
    $month        = substr($end, 5, 2);
    $day          = substr($end, 8, 2);
    $query['end'] = mktime(0, 0, 0, $month, $day, $year);
  }

  if (is_numeric($values['user'])) {
    $query['user'] = $values['user'];
  }

  $url = $_GET['q'];
  drupal_goto($url, array('query' => $query));
}

/**
 * Function to build the taxonomy term overview table for timetracker types.
 */
function openlucius_timetracker_build_vocabulary_terms_table() {

  // Set the table header.
  $header = array(t('Name'), t('Operations'));

  // Load the timetracker type vocabulary.
  $vocabulary = taxonomy_vocabulary_machine_name_load(TIMETRACKER_VOCABULARY_NAME);
  $terms      = taxonomy_get_tree($vocabulary->vid);
  $rows       = array();

  // Loop through the taxonomy terms.
  foreach ($terms as $term) {
    $rows[] = array(
      $term->name,
      '<a href="timetracker/term/' . $term->tid . '/edit">Edit</a>',
    );
  }

  // Return the table.
  return theme('table', array(
    'header'     => $header,
    'rows'       => $rows,
    'attributes' => array('id' => 'taxonomy'),
  ));
}

/**
 * Form constructor for editing timetracker type taxonomy terms.
 * @see openlucius_timetracker_taxonomy_term_edit_submit()
 * @ingroup forms.
 */
function openlucius_timetracker_taxonomy_term_edit($form, &$form_state, $term) {

  // Check term.
  if (!empty($term)) {

    // Check the vocabulary.
    if ($term->vocabulary_machine_name == TIMETRACKER_VOCABULARY_NAME) {

      // The hidden term id.
      $form['tid'] = array(
        '#type' => 'hidden',
        '#value' => $term->tid,
      );

      // The term.
      $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Term name'),
        '#default_value' => $term->name,
      );

      // The submit.
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Update'),
      );

      // Return the form.
      return $form;
    }
    else {
      drupal_access_denied();
      drupal_exit();
    }
  }

  // Return false if no term.
  return FALSE;
}

/**
 * Form submit constructor for editing timetracker type taxonomy term.
 * @see openlucius_timetracker_taxonomy_term_edit()
 * @ingroup forms.
 */
function openlucius_timetracker_taxonomy_term_edit_submit($form, &$form_state) {

  // Check the name.
  if (!empty($form_state['values']['name'])) {

    // Load the taxonomy term.
    $term = taxonomy_term_load($form_state['values']['tid']);

    // Set the name.
    $term->name = $form_state['values']['name'];

    // Save the taxonomy term.
    taxonomy_term_save($term);

    // Redirect to the timetracker configuration screen.
    $form_state['redirect'] = 'admin/config/openlucius/timetracker';
  }
  else {
    // Don't allow empty taxonomy terms.
    form_set_error('name', t('The name can not be empty.'));
  }
}
