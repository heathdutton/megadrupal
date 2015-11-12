<?php
/**
 * @file
 * Webform A/B Testing admin functions
 */

/**
 * Menu callback, admin form.
 */
function webform_ab_admin_settings($form, &$form_state) {
  $form = array();
  $form['webform_ab_ignore_hits_from_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles to ignore hits from'),
    '#description' => t('Hits and conversions will be ignored from users of the chosen roles.'),
    '#options' => user_roles(),
    '#default_value' => variable_get('webform_ab_ignore_hits_from_roles', array()),
  );
  $form['webform_ab_ignore_hits_from_ips'] = array(
    '#type' => 'textarea',
    '#title' => t('IPs to ignore hits from'),
    '#description' => t('Ignore webform hits and conversions from users in the listed IPs. List one IP per row.'),
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' => variable_get('webform_ab_ignore_hits_from_ips', ''),
  );
  $form['webform_ab_ignore_repeat_hits'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log only one hit per user per form'),
    '#description' => t('If checked, hits on a webform will only be logged once per user.'),
    '#default_value' => variable_get('webform_ab_ignore_repeat_hits', '1'),
  );
  return system_settings_form($form);
}

/**
 * Menu callback for the Reports and Monitoring tab on Test A/B
 *
 * @param $node Node object
 */
function webform_ab_test_reports($node) {
  $return = '';

  // Get total hit counts for all webforms
  $hit_counts_result = db_query('SELECT webform_nid, count(hit_id) hits FROM {webform_ab_hits} ' .
    'WHERE test_nid = :test_nid ' .
    'GROUP BY test_nid, webform_nid',
    array(':test_nid' => $node->nid));
  $hit_counts = array();
  foreach ($hit_counts_result as $row) {
    $hit_counts[$row->webform_nid] = $row->hits;
  }

  // Get total conversions for all webforms
  $conversion_counts_result = db_query('SELECT webform_nid, count(sid) conversions FROM {webform_ab_conversion} ' .
    'WHERE test_nid = :test_nid ' .
    'GROUP BY test_nid, webform_nid',
    array(':test_nid' => $node->nid));
  $conversion_counts = array();
  foreach ($conversion_counts_result as $row) {
    $conversion_counts[$row->webform_nid] = $row->conversions;
  }

  // If one of the webforms won, determine pre-win and post-win conversions
  if (($winning_webform_nid = _webform_ab_has_winner_already($node)) > 0) {
    // Get pre- and post-win hit counts
    $post_win_hit_data = db_query('SELECT SUM(IF(hits.time > f.win_time, 1, 0)) AS post_win, SUM(IF(hits.time <= f.win_time, 1, 0)) AS pre_win ' .
      'FROM {webform_ab_forms} f ' .
      'LEFT JOIN {webform_ab_hits} hits ON hits.test_nid = f.test_nid AND hits.webform_nid = f.webform_nid ' .
      'WHERE f.test_nid = :test_nid AND f.webform_nid = :webform_nid ' .
      'GROUP BY f.test_nid, f.webform_nid',
      array(':test_nid' => $node->nid, ':webform_nid' => $winning_webform_nid))->fetchAssoc();

    // Get pre- and post-win conversions counts
    $post_win_conversion_data = db_query('SELECT SUM(IF(sub.submitted > f.win_time, 1, 0)) AS post_win, SUM(IF(sub.submitted <= f.win_time, 1, 0)) AS pre_win ' .
      'FROM {webform_ab_forms} f ' .
      'LEFT JOIN {webform_ab_conversion} con ON con.test_nid = f.test_nid AND con.webform_nid = f.webform_nid ' .
      'LEFT JOIN {webform_submissions} sub ON con.sid = sub.sid ' .
      'WHERE f.test_nid = :test_nid AND f.webform_nid = :webform_nid ' .
      'GROUP BY f.test_nid, f.webform_nid',
      array(':test_nid' => $node->nid, ':webform_nid' => $winning_webform_nid))->fetchAssoc();
  }

  // Status labels
  $webform_ab_status_labels = array(
    WEBFORM_AB_WEBFORM_STATUS_ACTIVE => t('Active'),
    WEBFORM_AB_WEBFORM_STATUS_WINNER => t('Winner'),
    WEBFORM_AB_WEBFORM_STATUS_RETIRED => t('Retired'),
  );
  // Show the list of currently-included forms
  $headers = array(
    t('Status'),
    t('Name'),
    t('Hits'),
    t('Conversions'),
    t('Conversion %'),
    t('Pre-win Hits'),
    t('Pre-win Conversions'),
    t('Pre-win Conversion %'),
    t('Post-win Hits'),
    t('Post-win Conversions'),
    t('Post-win Conversion %'),
  );
  $rows = array();

  // Loop over the webforms and add a row to the report table
  if (!empty($node->included_webforms) && is_array($node->included_webforms)) {
    foreach ($node->included_webforms as $webform_nid => $webform_details) {
      $this_hit_count = isset($hit_counts[$webform_nid]) ? $hit_counts[$webform_nid] : 0;
      $this_conversion_count = isset($conversion_counts[$webform_nid]) ? $conversion_counts[$webform_nid] : 0;
      $line = array(
        $webform_ab_status_labels[$webform_details['status']],
        $webform_details['title'],
        isset($hit_counts[$webform_nid]) ? $hit_counts[$webform_nid] : 0,
        isset($conversion_counts[$webform_nid]) ? $conversion_counts[$webform_nid] : 0,
        ($this_hit_count > 0) ? round(100 * $this_conversion_count / $this_hit_count) . '%' : '0%',
      );

      // If this webform is the winner, show pre- and post-win conversion percentages
      if ($winning_webform_nid == $webform_nid) {
        $line[] = isset($post_win_hit_data['pre_win']) ? $post_win_hit_data['pre_win'] : 0;
        $line[] = isset($post_win_conversion_data['pre_win']) ? $post_win_conversion_data['pre_win'] : 0;
        // Only divide if greater than 0
        if (isset($post_win_hit_data['pre_win']) && $post_win_hit_data['pre_win'] > 0) {
          $line[] = round(100 * $post_win_conversion_data['pre_win'] / $post_win_hit_data['pre_win']) . '%';
        }
        else {
          $line[] = '0%';
        }
        $line[] = isset($post_win_hit_data['post_win']) ? $post_win_hit_data['post_win'] : 0;
        $line[] = isset($post_win_conversion_data['post_win']) ? $post_win_conversion_data['post_win'] : 0;
        $post_win_hit_data = isset($post_win_hit_data['post_win']) ? $post_win_hit_data['post_win'] : 0;
        $post_win_conversion_data = isset($post_win_conversion_data['post_win']) ? $post_win_conversion_data['post_win'] : 0;
        $line[] = ($post_win_hit_data > 0) ? round(100 * $post_win_conversion_data / $post_win_hit_data) . '%' : '0%';
      }
      else {
        // Not the winning webform. Pad out into the columns for win data
        while (count($line) < count($headers)) {
          $line[] = '';
        }
      }
      $rows[] = $line;
    }
  }

  // If there were no rows, show an empty message
  if (count($rows) == 0) {
    $rows[] = array(array(
        'data' => t('No webforms have been added to this A/B test. Visit the !forms_tab tab to add webforms.',
          array('!forms_tab' => l(t('Included Webforms'), 'node/' . $node->nid . '/webforms'))),
        'colspan' => count($headers),
      ));
  }
  $return .= theme('table', array('header' => $headers, 'rows' => $rows));
  return $return;
}

/**
 * Menu callback for the Included Forms tab on A/B Test nodes
 *
 * @param $node Node object
 */
function webform_ab_test_webforms($node) {
  $return = '';

  // Status labels
  $webform_ab_status_labels = array(
    WEBFORM_AB_WEBFORM_STATUS_ACTIVE => t('Active'),
    WEBFORM_AB_WEBFORM_STATUS_WINNER => t('Winner'),
    WEBFORM_AB_WEBFORM_STATUS_RETIRED => t('Retired'),
  );

  // Show the list of currently-included forms
  $headers = array(
    t('Name'),
    t('Status'),
    t('Operations'),
  );
  $rows = array();
  if (!empty($node->included_webforms) && is_array($node->included_webforms)) {
    foreach ($node->included_webforms as $webform_nid => $webform_details) {
      $rows[] = array(
        $webform_details['title'],
        $webform_ab_status_labels[$webform_details['status']],
        l(t('view'), 'node/' . $webform_nid) . ' ' .
        l(t('edit'), 'node/' . $webform_nid . '/edit') . ' ' .
        l(t('remove from test'), 'node/' . $node->nid . '/webforms/remove/' . $webform_nid),
      );
    }
  }

  // If there are no webforms included, show an empty message
  if (count($rows) == 0) {
    $rows[] = array(array(
        'data' => t('No webforms have been added to this A/B test. ' .
          'Select a webform in the dropdown below and click the Add button.'),
        'colspan' => count($headers),
      ));
  }
  $return .= theme('table', array('header' => $headers, 'rows' => $rows));
  // Show a form to add more
  $form = drupal_render(drupal_get_form('webform_ab_webform_list_add', $node));
  $return .= $form;
  return $return;
}

/**
 * Form to add webforms to the A/B Test
 *
 * @param $form_state
 * @param $node   Node object
 */
function webform_ab_webform_list_add($form, &$form_state, $node) {
  // Get all valid forms for the form type from other modules
  $webform_types = isset($node->webform_types) ? $node->webform_types : array();
  $flat_forms = module_invoke_all('webform_ab_valid_webforms', $webform_types);
  // Since module_invoke_all() will wipe out numeric array indices, the
  // implementations of hook_webform_ab_valid_webforms() have to return the nid
  // and title within a sub-array, rather than using the nid as an array key.
  // Re-index the array to use the nid as the key and title as the value.
  $indexed_forms = array();
  foreach ($flat_forms as $form) {
    $form = (array) $form;
    // Don't add a webform in the list if it's already included in the test
    if (!isset($node->included_webforms[$form['nid']])) {
      $indexed_forms[$form['nid']] = $form['title'];
    }
  }
  // Allow modules to alter the list of forms
  drupal_alter('webform_ab_valid_webforms', $indexed_forms, $node->webform_types);

  $form = array();
  $form['add_form_id'] = array(
    '#type' => 'select',
    '#title' => t('Add webform'),
    '#description' => t('Choose a webform to add to this test'),
    '#options' => array(0 => t('-- Select one --')) + $indexed_forms,
    '#default_value' => -1,
  );
  $form['test_nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  // If there are no more webforms that may be added, disable the form
  if (count($indexed_forms) == 0) {
    $form['add_form_id']['#disabled'] = TRUE;
    $form['add_form_id']['#description'] = 'There are no more webforms available to be added to this A/B test.';
    unset($form['add']);
  }
  return $form;
}

/**
 * Handle submission of the form to add a webform to an A/B Test
 */
function webform_ab_webform_list_add_submit($form, &$form_state) {
  // Only continue if they actually selected a webform
  if ($form_state['values']['add_form_id'] > 0) {
    // Make sure the chosen webform isn't already in the test
    // It wouldn't have been an option if it was already in the test, but be on
    // the safe side
    $exists = (bool) db_query('SELECT 1 FROM {webform_ab_forms} ' .
      'WHERE test_nid = :test_nid AND webform_nid = :webform_nid',
      array(
        ':test_nid' => $form_state['values']['test_nid'],
        ':webform_nid' => $form_state['values']['add_form_id']
      ))->fetchField() > 0;
    if (!$exists) {
      $record = array(
        'test_nid' => $form_state['values']['test_nid'],
        'webform_nid' => $form_state['values']['add_form_id'],
        'status' => WEBFORM_AB_WEBFORM_STATUS_ACTIVE,
      );
      drupal_write_record('webform_ab_forms', $record);
    }
  }
}

/**
 * Show a confirmation form when a user tries to remove a webform from a test
 */
function webform_ab_test_remove_webform($node, $webform_nid) {
  // Make sure the webform they want to remove is actually in the test
  $form_is_included = (bool) db_query('SELECT 1 FROM {webform_ab_forms} ' .
    'WHERE test_nid = :test_nid AND webform_nid = :webform_nid',
    array(':test_nid' => $node->nid, ':webform_nid' => $webform_nid))->fetchField() > 0;
  if ($form_is_included) {
    // Show a confirmation form
    drupal_set_message(t('The form has been removed from the A/B test.'));
    return drupal_render(drupal_get_form('webform_ab_test_remove_webform_confirm', $node->nid, $webform_nid));
  }
  else {
    // Somehow tried to remove a webform that isn't in the test
    drupal_set_message(t('That form is not in this A/B test.'), 'error');
    drupal_goto('node/' . $node->nid . '/webforms');
  }
}

/**
 * Confirmation form when removing a webform from a test
 */
function webform_ab_test_remove_webform_confirm($form, &$form_state, $test_nid, $webform_nid) {
  $form = array();
  $form['#test_nid'] = $test_nid;
  $form['#webform_nid'] = $webform_nid;
  $title = db_query('SELECT n.title from {node} n WHERE n.nid = :nid',
    array(':nid' => $webform_nid))->fetchField();
  return confirm_form(
    $form,
    t('Are you sure you want to remove webform %title from the A/B test?', array('%title' => $title)),
    'node/' . $test_nid . '/webforms',
    t('This action cannot be undone.'),
    t('Remove'),
    t('Cancel'),
    'webform_ab_test_remove_webform_confirm');
}

/**
 * Remove a webform from a test
 */
function webform_ab_test_remove_webform_confirm_submit($form, &$form_state) {
  db_delete('webform_ab_forms')
    ->condition('test_nid', $form['#test_nid'])
    ->condition('webform_nid', $form['#webform_nid'])
    ->execute();
  // Notify other modules that the webform is being removed
  module_invoke_all('webform_ab_remove_form', $form['#test_nid'], $form['#webform_nid']);
  drupal_set_message(t('The webform has been removed.'));
  $form_state['redirect'] = 'node/' . $form['#test_nid'] . '/webforms';
}

/**
 * Show a confirmation form when a user tries to clear data on a test
 */
function webform_ab_clear_test_data_confirm($form, &$form_state, $node) {
  $form = array();
  $form['#test_nid'] = $node->nid;
  return confirm_form(
    $form,
    t('Are you sure you want to clear all hit and conversion data on the test %title?', array('%title' => $node->title)),
      'node/' . $node->nid . '/reports',
    t('This action will remove all hit and conversion records saved for this test, return all included webforms to ' .
      'the Active status, and it cannot be undone. Note that any data submitted to the included webforms will remain ' .
      'intact, it just won\'t be associated with this A/B Test or used to declare a winning webform.'),
    t('Reset'),
    t('Cancel'));
}

/**
 * Clear the data in the test. Removes hits, conversions, and returns all webforms to
 * the active status
 */
function webform_ab_clear_test_data_confirm_submit($form, &$form_state) {
  db_delete('webform_ab_conversion')
  ->condition('test_nid', $form['#test_nid'])
  ->execute();
  db_delete('webform_ab_hits')
  ->condition('test_nid', $form['#test_nid'])
  ->execute();
  db_update('webform_ab_forms')
  ->fields(array(
    'status' => WEBFORM_AB_WEBFORM_STATUS_ACTIVE,
    'win_time' => 0,
  ))
  ->condition('test_nid', $form['#test_nid'])
  ->execute();
  // Notify other modules that may have test data to wipe
  module_invoke_all('webform_ab_reset_data', $form['#test_nid']);
  drupal_set_message(t('The data has been cleared.'));
  $form_state['redirect'] = 'node/' . $form['#test_nid'] . '/reports';
}
