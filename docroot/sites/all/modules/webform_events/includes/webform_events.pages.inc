<?php
/**
 * @file
 * Event management page.
 */

/**
 * Implements hook_form().
 */
function webform_events_form($form, $form_state, $node) {
  // Data building.
  $headers = array();
  $subs = array();

  // Load the Webform module functions for our use.
  module_load_include('inc', 'webform', 'includes/webform.report');
  module_load_include('inc', 'webform', 'includes/webform.submissions');

  // Load the Webform module functions for our use.
  module_load_include('inc', 'webform', 'includes/webform.components');

  // Set nid.
  $nid = $node->nid;

  // Load submissions.
  $submissions = webform_get_submissions($nid);

  // Get component names.
  $component_names = webform_component_list($node);

  // Initiate headers.
  $manage_headers['username'] = t('User name');

  // Build components as header.
  foreach ($component_names as $header) {
    $manage_headers[] = $header;
  }

  // Add manage headers.
  $headers = array_merge($manage_headers, array(
    'edit'      => t('Edit'),
    'delete'    => t('Delete'),
    'view'      => t('View'),
  ));

  // Loop through submissions.
  foreach ($submissions as $submission) {
    // Get submission id.
    $sid = $submission->sid;

    // Get uid for the submission.
    $uid = $submission->uid;
    $user = user_load($uid);

    // Get components.
    $result = $submission->data;

    // Is anonymous?.
    $name = t("Anonymous");
    if ($user->name != "") {
      $name = $user->name;
    }

    // Submission data.
    $temp_subs = array(
      'uid'         => $uid,
      'nid'         => $nid,
      'sid'         => $submission->sid,
      'fields'      => array(
        'username'  => $name,
      ),
    );

    $comp = array();
    // Loop through components.
    foreach ($result as $component) {
      if (isset($component[0])) {
        $comp[] = ucfirst($component[0]);
      }
      else {
        $comp[] = ucfirst($component['value'][0]);
      }
    }

    // Manage links for submission.
    $links = array(
      'edit'    => l(t('Edit'), 'node/' . $nid . '/submission/' . $sid . '/edit', array('query' => drupal_get_destination())),
      'delete'  => l(t('Delete'), 'node/' . $nid . '/submission/' . $sid . '/delete', array('query' => drupal_get_destination())),
      'view'    => l(t('View'), 'node/' . $nid . '/submission/' . $sid),
    );

    $fields = array_merge($comp, $links);
    $temp_subs['fields'] = array_merge($temp_subs['fields'], $fields);
    $subs[] = $temp_subs;
  }

  // Load queue.
  $queue = db_select('webform_submitted_data', 'subs')
          ->fields('subs', array('sid'))
          ->condition('data', 'queue', '=')
          ->condition('nid', $nid, '=')
          ->execute()
          ->fetchAll();

  // Get first one in the queue.
  if (!empty($queue)) {
    $first_queued = min($queue)->sid;
  }

  // Create link for easy access.
  $queuelink = "";
  if (!empty($first_queued)) {
    $queuelink = l(t('View first submission in the queue'), 'node/' . $nid . '/submission/' . $first_queued . '/edit', array('query' => drupal_get_destination()));
  }

  // Users that have cancelled their participation.
  // Check users that have also cancelled status submission(s).
  $user_cancel = db_select('webform_submitted_data', 'subs')
                ->fields('subs', array('sid'))
                ->condition('data', 'cancel', '=')
                ->condition('nid', $nid, '=')
                ->execute()
                ->fetchAll();

  // Loop cancellations.
  $cancels = array();
  foreach ($submissions as $submission) {
    $status = $submission->data[1];
    $uid = $submission->uid;

    if (isset($status[0])) {
      $state = $status[0];
    }
    else {
      $state = $status['value'][0];
    }

    if ($state == "cancel") {
      $user_cancel                                  = user_load($uid);
      $cancels[$user_cancel->name]['submissions']   = _webform_events_cancel_submissions($nid, $uid);
      $cancels[$user_cancel->name]['uid']           = $uid;
    }
  }

  // Slot data.
  $participants = 0;
  $participant_field = field_view_field('node', $node, 'field_webform_events_participant');
  if (isset($participant_field['#items'][0]['value'])) {
    $participants = $participant_field['#items'][0]['value'];
  }

  $slot_data = _webform_events_slot_info($nid, $participants);

  // Build data array.
  $data = array(
    'headers'       => $headers,
    'submissions'   => $subs,
    'queue'         => $queuelink,
    'cancel'        => $cancels,
    'slots'         => $slot_data,
  );

  // Data building Ends.
  // Markup building.
  // Main manage table.
  $rows = array();
  $i = 0;
  $rows_data = array_reverse($data['submissions']);

  foreach ($rows_data as $row) {
    $rows[$i] = $row['fields'];
    $i++;
  }

  // Let's build a pager, first how many per page.
  $per_page = variable_get('webform_events_default_manage_pager');
  $header = $data['headers'];

  // Get current page.
  $current_page = pager_default_initialize(count($rows), $per_page);

  // Break row data to chunks defined by $per_page.
  $chunks = array_chunk($rows, $per_page, TRUE);
  if (!empty($chunks)) {
    $chunks = $chunks[$current_page];
  }

  // Theme table actual data by chunks.
  $markup = theme('table', array(
      'header' => $header,
      'rows' => $chunks,
      'attributes' => array('width' => '100%'),
  ));

  // Add pager to markuo.
  $markup .= theme('pager', array('quantity', count($rows)));

  // Form markup.
  $form['markup-manage-table'] = array(
    '#markup' => $markup,
    '#type'   => 'markup',
  );

  // Cancellation tables.
  $manage_markup = '<div class="slot-data">';
  // Total participants.
  $manage_markup .= t("Participating currently:") . " " . $data['slots']['used'] . "<br/>";
  // Free slots.
  $manage_markup .= t("Free slots:") . " " . $data['slots']['free'] . "<br/>";
  // Total slots.
  $manage_markup .= "<strong>" . t("Total slots:") . " " . ($data['slots']['used'] + $data['slots']['free']) . "</strong>";

  // Notify if event is overbooked.
  if ($data['slots']['used'] > ($data['slots']['used'] + $data['slots']['free'])) {
    $manage_markup .= "<br/><strong>" . t("Event is overbooked!") . "</strong>";
  }

  $manage_markup .= "</div>";

  // Build cancellation queue link and header for cancellations.
  $manage_markup .= '<div class="queue-link">' .
    $data['queue'] .
    '</div><h2 class="cancellations-header">' .
    t('Cancelled users') .
    '</h2><p>' .
    t('<strong>Notice:</strong> Delete cancellation last.') .
    '</p>';

  // Form markup.
  $form['markup-manage-table-below'] = array(
    '#markup' => $manage_markup,
    '#type'   => 'markup',
  );

  // Loop users that have cancellations.
  // Setup data array for for canceled user.
  $submissions = $cancel_array = array();
  $cancel_form = $cancel_markup = "";
  $i = 0;
  $cancel_array['headers'] = array(
    t('Action'),
    t('Edit'),
    t('Delete'),
    t('View'),
  );

  // Loop canceled users.
  foreach ($data['cancel'] as $name => $cancel_user) {
    $uid = $cancel_user['uid'];
    $anon = FALSE;

    // Name for anonymous.
    if ($name == "") {
      $name = t("Anonymous");
      $anon = TRUE;
    }

    // Is anonymous, don't output moderation links.
    if ($anon == TRUE) {
      $cancel_form .= '<div class="submissions-container"><div class="cancel-user-subheader"><strong>' .
            $name .
            ' ' .
            t('users') .
            ', </strong></div><div class="cancel-submissions"> ' .
            t('have canceled and have the following submissions:') .
            '<br/></div>';
    }

    // Registered users.
    if ($anon == FALSE) {
      $cancel_form .= '<div class="submissions-container"><div class="cancel-user-subheader"><strong>' .
            t('User') .
            ' ' .
            l($name, 'user/' . $uid) .
            ',</strong></div><div class="cancel-submissions"> ' .
            t('has canceled and have the following submissions:') .
            '<br/></div>';
    }

    // Form markup.
    $cuid = "markup-cancel-" . $uid;
    $form[$cuid] = array(
      '#markup'  => $cancel_form . "</div>",
      '#type'    => 'markup',
    );

    $cuid = "delete-subs-" . $uid;
    if ($uid != 0) {
      $form[$cuid] = array(
        '#title'            => t('Delete all submissions'),
        '#description'      => t('Check to delete all submissions for this event for this user.'),
        '#default_value'    => 0,
        '#on_value'         => 1,
        '#off_value'        => 0,
        '#value_key'        => 'value',
        '#type'             => 'checkbox',
        '#required'         => FALSE,
      );
    }
    $cancel_form = "";

    $submissions = $cancel_user['submissions'];

    // Build data.
    foreach ($cancel_user['submissions'] as $sub => $submission_data) {
      $key = 0;
      $sid = $submission_data['sid'];
      $nid = $submission_data['nid'];
      $cancel_array['data'][$i][$key] = ucfirst($submission_data['action']);
      $key++;
      $cancel_array['data'][$i][$key] = l(t('Edit'), 'node/' . $nid . '/submission/' . $sid . '/edit', array('query' => drupal_get_destination()));
      $key++;
      $cancel_array['data'][$i][$key] = l(t('Delete'), 'node/' . $nid . '/submission/' . $sid . '/delete', array('query' => drupal_get_destination()));
      $key++;
      $cancel_array['data'][$i][$key] = l(t('Delete'), 'node/' . $nid . '/submission/' . $sid . '/delete', array('query' => drupal_get_destination()));
      $cancel_array['data'][$i][$key] = l(t('View'), 'node/' . $nid . '/submission/' . $sid);
      $i++;
    }

    // Create table out of the data.
    $cancel_form .= theme('table', array(
      'header' => $cancel_array['headers'],
      'rows' => $cancel_array['data'],
      'attributes' => array('width' => '100%'),
    ));

    // Form markup.
    $cuid = $cuid . "-table";
    $form[$cuid] = array(
      '#markup' => $cancel_form,
      '#type'   => 'markup',
    );

    $markup = "";
    // Close wrapper.
    $cancel_markup .= $cancel_form . "</div>";

    // Clear array and markup.
    $cancel_array['data'] = array();
    $cancel_form = "";
  }

  // Output markup.
  $markup .= $cancel_markup;

  // Change title.
  drupal_set_title(t("Manage:") . " " . $node->title);

  // Submit button.
  $form['submit']   = array(
    '#type'  => 'submit',
    '#value' => t('Mass delete submissions'),
  );

  $form['#validate'] = array('_webform_events_admin_page_validate');

  return $form;
}

/**
 * This custom function deletes submissions on submit.
 */
function _webform_events_admin_page_validate($form, $form_state) {
  $array = $form_state['values'];
  $nid = arg(1);

  // Go through input.
  while ($check = current($array)) {
    if ($check == 1) {
      $uid = str_replace("delete-subs-", "", key($array));

      // Delete submissions.
      if (!empty($uid) && $uid != 0) {
        // Get matching submissions.
        $delsubs = db_select('webform_submissions')
          ->condition('nid', $nid, '=')
          ->condition('uid', $uid, '=')
          ->fields('webform_submissions')
          ->execute()
          ->fetchAll();

        // Delete matching submissions.
        foreach ($delsubs as $delsub) {
          $sid = $delsub->sid;
          db_delete('webform_submitted_data')
            ->condition('nid', $nid)
            ->condition('sid', $sid)
            ->execute();
          db_delete('webform_submissions')
            ->condition('nid', $nid)
            ->condition('uid', $uid)
            ->execute();
        }
        $cancelled_user = user_load($uid);
        drupal_set_message(t('Submissions deleted for user :name', array(':name' => $cancelled_user->name)), 'status');
      }
      // If someone tries to delete anonymous user subs, this shouldn't happen.
      else {
        drupal_set_message(t("Can't mass delete anonymous user submissions. You must remove them manually."), 'warning');
      }
    }
    next($array);
  }
}

/**
 * This custom function gets submissions for user that has cancellations.
 */
function _webform_events_cancel_submissions($nid, $uid) {
  // Get other submissions for canceled user.
  $return = array();
  $other_submissions = db_select('webform_submissions', 'subs')
                        ->fields('subs', array('sid'))
                        ->condition('uid', $uid, '=')
                        ->condition('nid', $nid, '=')
                        ->execute()
                        ->fetchAll();

  // Loop them.
  $i = 0;
  foreach ($other_submissions as $submission_data) {
    // Load submission data.
    $data = webform_get_submission($nid, $submission_data->sid);

    // Loop data.
    if (isset($data->data[1][0])) {
      $return[$i]['action'] = $data->data[1][0];
    }
    else {
      $return[$i]['action'] = $data->data[1]['value'][0];
    }
    $return[$i]['nid'] = $nid;
    $return[$i]['sid'] = $submission_data->sid;
    $i++;
  }
  return $return;
}
