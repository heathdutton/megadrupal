<?php

/**
 * @file
 * This file is loaded when handling submissions, either submitting new,
 * editing, or viewing. It also contains all CRUD functions for submissions.
 */

/**
 * Given an array of submitted values, flatten it into data for a submission.
 *
 * @param object $node
 *   The node object containing the current dquarks.
 * @param array $submitted
 *   The submitted user values from the dquarks.
 *
 * @return array
 *   An array suitable for use in the 'data' property of a $submission object.
 */
function dquarks_submission_data($node, $submitted) {
  $data = array();

  foreach ($submitted as $cid => $values) {
    if (is_array($values)) {
      $data[$cid]['value'] = $values;
    }
    else {
      $data[$cid]['value'][0] = $values;
    }
  }

  return $data;
}

/**
 * Update a dquarks submission entry in the database.
 *
 * @param object $node
 *   The node object containing the current dquarks.
 * @param int $sid
 *   The id of submission.
 * @param array $submitted
 *   The already submitted submisson.
 * @param boolean $is_draft
 *   True if the submission is draft.
 *
 * @return int
 *   The existing submission SID.
 */
function dquarks_submission_update($node, $sid, $submitted, $is_draft = 0) {
  global $user;
  // Update the submission data by first removing all this submissions data.
  db_query("DELETE FROM {dquarks_submissions} WHERE sid = %d", $sid);
  db_delete('dquarks_submissions')
  ->condition('sid', $sid)
  ->execute();

  db_insert('dquarks_submissions')
  ->fields(array(
    'nid' => $node->nid,
    'sid' => $sid,
    'uid' => $user->uid,
    'submitted' => time(),
    'remote_addr' => ip_address(),
    'is_draft' => $is_draft,
  ))
  ->execute();
  // If is draft, only delete data for components submitted, to preserve any
  // data from form pages not visited in this submission.
  if ($is_draft) {
    $_submitted_cids = array();
    foreach ($submitted as $cid => $value) {
      $_submitted_cids[] = $cid;
    }
    db_delete('dquarks_submitted_data')
    ->condition('sid', $sid)
    ->condition('cid', array_keys($submitted), 'IN')
    ->execute();
  }
  else {
    db_delete('dquarks_submitted_data')
    ->condition('sid', $sid)
    ->execute();
  }
  // Then re-add it to the database.
  foreach ($submitted as $cid => $value) {
    // Don't save directions as submitted data.
    if ($node->dquarks['components'][$cid]['type'] == 'direction') {
      continue;
    }

    if (is_array($value)) {
      $delta = 0;
      foreach ($value as $k => $v) {
        db_insert('dquarks_submitted_data')
        ->fields(array(
          'nid' => $node->nid,
          'sid' => $sid,
          'cid' => $cid,
          'no' => $delta,
          'data' => $v,
        ))
        ->execute();
        $delta++;
      }
    }
    else {
      db_insert('dquarks_submitted_data')
      ->fields(array(
        'nid' => $node->nid,
        'sid' => $sid,
        'cid' => $cid,
        'no' => 0,
        'data' => $value,
      ))
      ->execute();
    }
  }

  return $sid;
}

/**
 * Insert a dquarks submission entry in the database.
 *
 * @param object $node
 *   The node object containing the current dquarks.
 * @param object $submission
 *   The dquarks submission object to be saved into the database.
 *
 * @return int
 *   The new submission SID.
 */
function dquarks_submission_insert($node, $submission, $is_draft = 0) {

  global $user;

  // The submission ID may already be set if being called as an update.
  // Allow other modules to modify the submission before saving.
  foreach (module_implements('dquarks_submission_presave') as $module) {
    $function = $module . '_dquarks_submission_presave';
    $function($node, $submission);
  }

  $sid = db_insert('dquarks_submissions')
  ->fields(array(
    'nid' => $node->nid,
    'uid' => $user->uid,
    'submitted' => time(),
    'remote_addr' => ip_address(),
    'is_draft' => $is_draft,
  ))
  ->execute();

  foreach ($submission as $cid => $value) {

    // Don't save directions as submitted data.
    if ($node->dquarks['components'][$cid]['type'] == 'direction') {
      continue;
    }
    $delta = 0;
    if (is_array($value)) {

      foreach ($value as $k => $v) {
        db_insert('dquarks_submitted_data')
        ->fields(array(
          'nid' => $node->nid,
          'sid' => $sid,
          'cid' => $cid,
          'no' => $delta,
          'data' => is_null($v) ? '' : $v,
        ))
        ->execute();
        $delta++;
      }
    }
    else {
      db_insert('dquarks_submitted_data')
      ->fields(array(
        'nid' => $node->nid,
        'sid' => $sid,
        'cid' => $cid,
        'no' => $delta,
        'data' => is_null($value) ? '' : $value,
      ))
      ->execute();
    }
  }

  return $sid;
}

/**
 * Delete a single submission.
 */
function dquarks_submission_delete($node, $submission) {
  // Iterate through all components and let each do cleanup if necessary.
  foreach ($node->dquarks['components'] as $cid => $component) {
    if (isset($submission->data[$cid])) {
      dquarks_component_invoke($component['type'], 'delete', $component, $submission->data[$cid]['value']);
    }
  }

  // Delete any anonymous session information.
  if (isset($_SESSION['dquarks_submission'][$submission->sid])) {
    unset($_SESSION['dquarks_submission'][$submission->sid]);
  }

  db_delete('dquarks_submitted_data')
  ->condition('nid', $node->nid)
  ->condition('sid', $submission->sid)
  ->execute();
  db_delete('dquarks_submissions')
  ->condition('nid', $node->nid)
  ->condition('sid', $submission->sid)
  ->execute();

  module_invoke_all('dquarks_submission_delete', $node, $submission);
}

/**
 * Send related e-mails related to a submission.
 *
 * This function is usually invoked when a submission is completed, but may be
 * called any time e-mails should be redelivered.
 *
 * @param object $node
 *   The node object containing the current dquarks.
 * @param object $submission
 *   The dquarks submission object to be used in sending e-mails.
 * @param array $emails
 *   (optional) An array of specific e-mail settings to be used. If omitted, all
 *   e-mails in $node->dquarks['emails'] will be sent.
 */
function dquarks_submission_send_mail($node, $submission, $emails = NULL) {
  global $user;

  // Get the list of e-mails we'll be sending.
  $emails = isset($emails) ? $emails : $node->dquarks['emails'];

  // Create a themed message for mailing.
  $send_count = 0;
  foreach ($emails as $eid => $email) {
    // Set the HTML property based on availablity of MIME Mail.
    $email['html'] = ($email['html'] && dquarks_email_html_capable());

    // Pass through the theme layer if using the default template.
    if ($email['template'] == 'default') {
      $email['message'] = theme(array(
        'dquarks_mail_' . $node->nid,
        'dquarks_mail',
        'dquarks_mail_message
          '), array(
            'node' => $node,
            'submission' => $submission,
            'email' => $email,
          )
      );
    }
    else {
      $email['message'] = $email['template'];
    }

    // Replace tokens in the message.
    $email['message'] = _dquarks_filter_values($email['message'], $node, $submission, $email, FALSE, TRUE);

    // Build the e-mail headers.
    $email['headers'] = theme(
    array(
      'dquarks_mail_headers_' . $node->nid,
      'dquarks_mail_headers',
    ), array(
      'node' => $node,
      'submission' => $submission,
      'email' => $email,
    )
    );

    // Assemble the From string.
    if (isset($email['headers']['From'])) {
      // If a header From is already set, don't override it.
      $email['from'] = $email['headers']['From'];
      unset($email['headers']['From']);
    }
    else {
      $email['from'] = dquarks_format_email_address($email['from_address'], $email['from_name'], $node, $submission);
    }

    // Update the subject if set in the themed headers.
    if (isset($email['headers']['Subject'])) {
      $email['subject'] = $email['headers']['Subject'];
      unset($email['headers']['Subject']);
    }
    else {
      $email['subject'] = dquarks_format_email_subject($email['subject'], $node, $submission);
    }

    // Update the to e-mail if set in the themed headers.
    if (isset($email['headers']['To'])) {
      $email['email'] = $email['headers']['To'];
      unset($email['headers']['To']);
    }

    // Generate the list of addresses that this e-mail will be sent to.
    $addresses = array_filter(explode(',', $email['email']));
    $addresses_final = array();
    foreach ($addresses as $address) {
      $address = trim($address);

      // After filtering e-mail addresses with component values, a single value
      // might contain multiple addresses (such as from checkboxes or selects).
      $address = dquarks_format_email_address($address, NULL, $node, $submission, TRUE, FALSE, 'short');

      if (is_array($address)) {
        foreach ($address as $new_address) {
          $new_address = trim($new_address);
          if (valid_email_address($new_address)) {
            $addresses_final[] = $new_address;
          }
        }
      }
      elseif (valid_email_address($address)) {
        $addresses_final[] = $address;
      }
    }

    // Mail the dquarks results.
    foreach ($addresses_final as $address) {
      // Verify that this submission is not attempting to send any spam hacks.
      if (_dquarks_submission_spam_check($address, $email['subject'], $email['from'], $email['headers'])) {
        watchdog('dquarks', 'Possible spam attempt from @remote_addr' . "<br />\n" . nl2br(htmlentities($email['message'])), array('@remote_add' => ip_address()));
        drupal_set_message(t('Illegal information. Data not submitted.'), 'error');
        return FALSE;
      }

      $language = $user->uid ? user_preferred_language($user) : language_default();
      $mail_params = array(
        'message' => $email['message'],
        'subject' => $email['subject'],
        'headers' => $email['headers'],
        'node' => $node,
        'submission' => $submission,
      );

      if (dquarks_email_html_capable()) {
        // Load attachments for the e-mail.
        $attachments = array();
        if ($email['attachments']) {
          dquarks_component_include('file');
          foreach ($node->dquarks['components'] as $component) {
            if (dquarks_component_feature($component['type'], 'attachment') && !empty($submission->data[$component['cid']]['value'][0])) {
              $file = dquarks_get_file($submission->data[$component['cid']]['value'][0]);
              if ($file && ($file->filepath = drupal_realpath($file->uri))) {
                $attachments[] = $file;
              }
            }
          }
        }

        // Add the attachments to the mail parameters.
        $mail_params['attachments'] = $attachments;

        // Set all other properties for HTML e-mail handling.
        $mail_params['plain'] = !$email['html'];
        $mail_params['plaintext'] = $email['html'] ? NULL : $email['message'];
        $mail_params['headers'] = $email['headers'];
        if ($email['html']) {
          // MIME Mail requires this header or it will filter all text.
          $mail_params['headers']['Content-Type'] = 'text/html; charset=UTF-8';
        }
      }

      // Mail the submission.
      $message = drupal_mail('dquarks', 'submission', $address, $language, $mail_params, $email['from']);
      if ($message['result']) {
        $send_count++;
      }
    }
  }

  return $send_count;
}

/**
 * Confirm form to delete a single form submission.
 *
 * @param array $form
 *   The new form array.
 * @param array $form_state
 *   The current form state.
 * @param object $node
 *   The node for which this dquarks was submitted.
 * @param object $submission
 *   The submission to be deleted (from dquarks_submitted_data).
 */
function dquarks_submission_delete_form($form, $form_state, $node, $submission) {
  dquarks_set_breadcrumb($node, $submission);

  // Set the correct page title.
  drupal_set_title(dquarks_submission_title($node, $submission));

  // Keep the NID and SID in the same location as the dquarks_client_form().
  // This helps mollom identify the same fields when deleting a submission.
  $form['#tree'] = TRUE;
  $form['details']['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['details']['sid'] = array(
    '#type' => 'value',
    '#value' => $submission->sid,
  );

  $question = t('Are you sure you want to delete this submission?');

  if (isset($_GET['destination'])) {
    $destination = $_GET['destination'];
  }
  elseif (dquarks_results_access($node)) {
    $destination = 'node/' . $node->nid . '/dquarks-results';
  }
  else {
    $destination = 'node/' . $node->nid . '/submissions';
  }

  return confirm_form($form, NULL, check_plain($destination), check_plain($question), t('Delete'), t('Cancel'));
}

/**
 * Delete selected submission.
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form values items.
 */
function dquarks_submission_delete_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['details']['nid']);
  $submission = dquarks_get_submission($form_state['values']['details']['nid'], $form_state['values']['details']['sid']);
  dquarks_submission_delete($node, $submission);
  drupal_set_message(t('Submission deleted.'));

  $form_state['redirect'] = 'node/' . $node->nid . '/dquarks-results';
}

/**
 * Menu title callback; Return the submission number as a title.
 */
function dquarks_submission_title($node, $submission) {
  return t('Submission #@sid', array('@sid' => $submission->sid));
}

/**
 * Menu callback; Present a dquarks submission page for display or editing.
 */
function dquarks_submission_page($node, $submission, $format) {
  global $user;

  // Render the admin UI breadcrumb.
  dquarks_set_breadcrumb($node, $submission);

  // Set the correct page title.
  drupal_set_title(dquarks_submission_title($node, $submission));

  if ($format == 'form') {
    $output = drupal_get_form('dquarks_client_form_' . $node->nid, $node, $submission, TRUE, FALSE);
  }
  else {
    $output = drupal_get_form('dquarks_client_form_' . $node->nid, $node, $submission, FALSE, FALSE);
  }

  // Determine the mode in which we're displaying this submission.
  $mode = ($format != 'form') ? 'display' : 'form';
  if (strpos(request_uri(), 'print/') !== FALSE) {
    $mode = 'print';
  }
  if (strpos(request_uri(), 'printpdf/') !== FALSE) {
    $mode = 'pdf';
  }

  // Add navigation for administrators.
  if (dquarks_results_access($node)) {
    $navigation = theme('dquarks_submission_navigation', array(
      'node' => $node,
      'submission' => $submission,
      'mode' => $mode,
    )
    );
    $information = theme('dquarks_submission_information', array(
      'node' => $node,
      'submission' => $submission,
      'mode' => $mode,
    )
    );
  }
  else {
    $navigation = NULL;
    $information = NULL;
  }

  // Actions may be shown to all users.
  $actions = theme('links', array(
    'links' => module_invoke_all('dquarks_submission_actions', $node, $submission),
    'attributes' => array(
      'class' => array('links', 'inline', 'dquarks-submission-actions'),
    ),
  )
  );

  // Disable the page cache for anonymous users viewing or editing submissions.
  if (!$user->uid) {
    dquarks_disable_page_cache();
  }

  $page = array(
    '#theme' => 'dquarks_submission_page',
    '#node' => $node,
    '#mode' => $mode,
    '#submission' => $submission,
    '#submission_content' => $output,
    '#submission_navigation' => $navigation,
    '#submission_information' => $information,
    '#submission_actions' => $actions,
  );
  $page['#attached']['library'][] = array('dquarks', 'admin');
  return $page;
}

/**
 * Print a dquarks submission for display on a page or in an e-mail.
 */
function dquarks_submission_render($node, $submission, $email, $format) {
  $component_tree = array();
  $renderable = array();
  $page_count = 1;
  $excluded_components = isset($email) ? $email['excluded_components'] : array();

  // Meta data that may be useful for modules implementing
  // hook_dquarks_submission_render_alter().
  $renderable['#node'] = $node;
  $renderable['#submission'] = $submission;
  $renderable['#email'] = $email;
  $renderable['#format'] = $format;

  // Set the theme function for submissions.
  $renderable['#theme'] = array('dquarks_submission_' . $node->nid, 'dquarks_submission');

  // Remove excluded components.
  $components = $node->dquarks['components'];
  foreach ($excluded_components as $cid) {
    unset($components[$cid]);
  }

  _dquarks_components_tree_build($components, $component_tree, 0, $page_count);

  // Make sure at least one field is available.
  if (isset($component_tree['children'])) {
    // Recursively add components to the form.
    foreach ($component_tree['children'] as $cid => $component) {
      if (_dquarks_client_form_rule_check($node, $component, $component['page_num'], NULL, $submission)) {
        _dquarks_client_form_add_component($node, $component, NULL, $renderable, $renderable, NULL, $submission, $format);
      }
    }
  }

  drupal_alter('dquarks_submission_render', $renderable);
  return drupal_render($renderable);
}

/**
 * Return all the submissions for a particular node.
 *
 * @param array $filters
 *   An array of filters to apply to this query. Usually in the format
 *   array('nid' => $nid, 'uid' => $uid). A single integer may also be passed
 *   in, which will be equivalent to specifying a $nid filter.
 * @param array $header
 *   If the results of this fetch will be used in a sortable
 *   table, pass the array header of the table.
 * @param int $pager_count
 *   Optional. The number of submissions to include in the results.
 */
function dquarks_get_submissions($filters = array(), $header = NULL, $pager_count = 0) {
  $submissions = array();

  if (!is_array($filters)) {
    $filters = array('nid' => $filters);
  }

  // UID filters need to be against a specific table.
  if (isset($filters['uid'])) {
    $filters['u.uid'] = $filters['uid'];
    unset($filters['uid']);
  }

  // No need to find SIDs if it was given to us.
  if (isset($filters['sid'])) {
    $sids = array($filters['sid']);
  }
  // Build the list of SIDs that need to be retrieved.
  else {
    $pager_query = db_select('dquarks_submissions', 'ws')->fields('ws', array('sid'));
    foreach ($filters as $column => $value) {
      $pager_query->condition($column, $value);
    }

    if (isset($filters['u.uid'])) {
      // Join to the users table for sorting by user name.
      $pager_query->leftJoin('users', 'u', 'u.uid = ws.uid');
    }

    if (isset($filters['u.uid']) && $filters['u.uid'] === 0) {
      if (!empty($_SESSION['dquarks_submission'])) {
        $anonymous_sids = array_keys($_SESSION['dquarks_submission']);
        $pager_query->condition('sid', $anonymous_sids, 'IN');
      }
      else {
        $pager_query->condition('sid', 0);
      }
    }

    if (is_array($header)) {
      // Extending the query instatiates a new query object.
      $pager_query = $pager_query->extend('TableSort');
      $pager_query->orderByHeader($header);
    }
    else {
      $pager_query->orderBy('sid', 'ASC');
    }

    if ($pager_count) {
      // Extending the query instatiates a new query object.
      $pager_query = $pager_query->extend('PagerDefault');
      $pager_query->limit($pager_count);
    }
    $result = $pager_query->execute();

    $sids = array();
    foreach ($result as $row) {
      $sids[] = $row->sid;
      $submissions[$row->sid] = FALSE;
    }
  }

  // If there are no submissions being retrieved, return an empty array.
  if (empty($sids)) {
    return $submissions;
  }

  // Query the required submission data.
  $query = db_select('dquarks_submissions', 's');
  $query->leftJoin('dquarks_submitted_data', 'sd', 'sd.sid = s.sid');
  $query->leftJoin('users', 'u', 'u.uid = s.uid');
  $query
  ->fields('s')
  ->fields('sd', array('cid', 'no', 'data'))
  ->fields('u', array('name'))
  ->condition('s.sid', $sids, 'IN')
  ->orderBy('sd.sid', 'ASC')
  ->orderBy('sd.cid', 'ASC')
  ->orderBy('sd.no', 'ASC');
  $result = $query->execute();

  // Convert the queried rows into submissions.
  $previous = 0;
  foreach ($result as $row) {
    if ($row->sid != $previous) {
      $submissions[$row->sid] = new stdClass();
      $submissions[$row->sid]->sid = $row->sid;
      $submissions[$row->sid]->nid = $row->nid;
      $submissions[$row->sid]->submitted = $row->submitted;
      $submissions[$row->sid]->remote_addr = $row->remote_addr;
      $submissions[$row->sid]->uid = $row->uid;
      $submissions[$row->sid]->name = $row->name;
      $submissions[$row->sid]->is_draft = $row->is_draft;
      $submissions[$row->sid]->score = $row->score;
      $submissions[$row->sid]->max_score = $row->max_score;
      $submissions[$row->sid]->data = array();
    }
    // CID may be NULL if this submission does not actually contain any data.
    if ($row->cid) {
      $submissions[$row->sid]->data[$row->cid]['value'][$row->no] = $row->data;
    }
    $previous = $row->sid;
  }

  foreach (module_implements('dquarks_submission_load') as $module) {
    $function = $module . '_dquarks_submission_load';
    $function($submissions);
  }

  return $submissions;
}

/**
 * Return a count of the total number of submissions for a node.
 *
 * @param int $nid
 *   The node ID for which submissions are being fetched.
 * @param int $uid
 *   Optional; the user ID to filter the submissions by.
 * @param int $reset
 *   reset submission.
 *
 * @return int
 *   An integer value of the number of submissions.
 */
function dquarks_get_submission_count($nid, $uid = NULL, $reset = FALSE) {
  static $counts;

  if (!isset($counts[$nid][$uid]) || $reset) {
    $query = db_select('dquarks_submissions')
    ->condition('nid', $nid);
    if ($uid !== NULL) {
      $query->condition('uid', $uid);
    }
    if ($uid === 0) {
      $submissions = isset($_SESSION['dquarks_submission']) ? $_SESSION['dquarks_submission'] : NULL;
      if ($submissions) {
        $query->condition('sid', $submissions, 'IN');
      }
      else {
        // Intentionally never match anything if the anonymous user has no
        // submissions.
        $query->condition('sid', 0);
      }
    }

    $counts[$nid][$uid] = $query->countQuery()->execute()->fetchField();
  }
  return $counts[$nid][$uid];
}

/**
 * Fetch a specified submission for a dquarks node.
 */
function dquarks_get_submission($nid, $sid, $reset = FALSE) {

  static $submissions = array();

  if ($reset) {
    $submissions = array();
    if (!isset($sid)) {
      return;
    }
  }

  // Load the submission if needed.
  if (!isset($submissions[$sid])) {
    $new_submissions = dquarks_get_submissions(array('sid' => $sid));
    $submissions[$sid] = $new_submissions[$sid];
  }

  // Ensure that the requested NID matches the submission NID.
  if ($submissions[$sid]->nid != $nid) {
    return FALSE;
  }

  return $submissions[$sid];
}

/**
 * Check if they are attempting to spam.
 *
 * @param string $to
 *   Destination e-mail.
 * @param string $subject
 *   Subject of e-mail.
 * @param array $from
 *   Source of e-mail.
 * @param array $headers
 *   Header of e-mail.
 *
 * @return boolean
 *   return false if not spam.
 */
function _dquarks_submission_spam_check($to, $subject, $from, $headers = array()) {
  $headers = implode('\n', (array) $headers);
  // Check if they are attempting to spam using a bcc or content type hack.
  if (preg_match('/(b?cc\s?:)|(content\-type:)/i', $to . "\n" . $subject . "\n" . $from . "\n" . $headers)) {
    // Possible spam attempt.
    return TRUE;
  }
  // Not spam.
  return FALSE;
}

/**
 * Check if the current user has exceeded the limit on this form.
 *
 * @param object $node
 *   The dquarks node to be checked.
 *
 * @return bool
 *   Boolean TRUE if the user has exceeded their limit. FALSE otherwise.
 */
function _dquarks_submission_user_limit_check($node) {
  global $user;

  // Check if submission limiting is enabled.
  if ($node->dquarks['submit_limit'] == '-1') {
    // No check enabled.
    return FALSE;
  }

  // Retrieve submission data for this IP address or username from the database.
  $query = db_select('dquarks_submissions')
  ->condition('nid', $node->nid)
  ->condition('is_draft', 0);

  if ($node->dquarks['submit_interval'] != -1) {
    $query->condition('submitted', REQUEST_TIME - $node->dquarks['submit_interval'], '>');
  }

  if ($user->uid) {
    $query->condition('uid', $user->uid);
  }
  else {
    $query->condition('remote_addr', ip_address());
  }

  // Fetch all the entries from the database within the submit interval with
  // this username and IP.
  $num_submissions_database = $query->countQuery()->execute()->fetchField();

  // Double check the submission history from the users machine using cookies.
  $num_submissions_cookie = 0;
  if ($user->uid == 0 && variable_get('dquarks_use_cookies', 0)) {
    $cookie_name = 'dquarks-' . $node->nid;

    if (isset($_COOKIE[$cookie_name]) && is_array($_COOKIE[$cookie_name])) {
      foreach ($_COOKIE[$cookie_name] as $key => $timestamp) {
        if ($node->dquarks['submit_interval'] != -1 && $timestamp <= REQUEST_TIME - $node->dquarks['submit_interval']) {
          // Remove the cookie if past the required time interval.
          setcookie($cookie_name . '[' . $key . ']', '', 0);
        }
      }
      // Count the number of submissions recorded in cookies.
      $num_submissions_cookie = count($_COOKIE[$cookie_name]);
    }
    else {
      $num_submissions_cookie = 0;
    }
  }

  if ($num_submissions_database >= $node->dquarks['submit_limit'] || $num_submissions_cookie >= $node->dquarks['submit_limit']) {
    // Limit exceeded.
    return TRUE;
  }

  // Limit not exceeded.
  return FALSE;
}

/**
 * Check if the total number of submissions has exceeded the limit on this form.
 *
 * @param object $node
 *   The dquarks node to be checked.
 *
 * @return bool
 *   Boolean TRUE if the form has exceeded it's limit. FALSE otherwise.
 */
function _dquarks_submission_total_limit_check($node) {

  // Check if submission limiting is enabled.
  if ($node->dquarks['total_submit_limit'] == '-1') {
    // No check enabled.
    return FALSE;
  }

  // Retrieve submission data from the database.
  $query = db_select('dquarks_submissions')
  ->condition('nid', $node->nid)
  ->condition('is_draft', 0);

  if ($node->dquarks['total_submit_interval'] != -1) {
    $query->condition('submitted', REQUEST_TIME - $node->dquarks['total_submit_interval'], '>');
  }

  // Fetch all the entries from the database within the submit interval.
  $num_submissions_database = $query->countQuery()->execute()->fetchField();

  if ($num_submissions_database >= $node->dquarks['total_submit_limit']) {
    // Limit exceeded.
    return TRUE;
  }

  // Limit not exceeded.
  return FALSE;
}

/**
 * Preprocess function for dquarks-submission.tpl.php.
 */
function template_preprocess_dquarks_submission(&$vars) {
  $vars['node'] = $vars['renderable']['#node'];
  $vars['submission'] = $vars['renderable']['#submission'];
  $vars['email'] = $vars['renderable']['#email'];
  $vars['format'] = $vars['renderable']['#format'];
}

/**
 * Preprocess function for dquarks-submission-navigation.tpl.php.
 */
function template_preprocess_dquarks_submission_navigation(&$vars) {
  $start_path = ($vars['mode'] == 'print') ? 'print/' : 'node/';

  $previous_query = db_select('dquarks_submissions')
  ->condition('nid', $vars['node']->nid)
  ->condition('sid', $vars['submission']->sid, '<');
  $previous_query->addExpression('MAX(sid)');

  $next_query = db_select('dquarks_submissions')
  ->condition('nid', $vars['node']->nid)
  ->condition('sid', $vars['submission']->sid, '>');
  $next_query->addExpression('MIN(sid)');

  $vars['previous'] = $previous_query->execute()->fetchField();
  $vars['next'] = $next_query->execute()->fetchField();
  $vars['previous_url'] = $start_path . $vars['node']->nid . '/submission/' . $vars['previous'] . ($vars['mode'] == 'form' ? '/edit' : '');
  $vars['next_url'] = $start_path . $vars['node']->nid . '/submission/' . $vars['next'] . ($vars['mode'] == 'form' ? '/edit' : '');
}

/**
 * Preprocess function for dquarks-submission-navigation.tpl.php.
 */
function template_preprocess_dquarks_submission_information(&$vars) {
  $vars['account'] = user_load($vars['submission']->uid);
  $vars['actions'] = theme('links', module_invoke_all('dquarks_submission_actions', $vars['node'], $vars['submission']));
}
