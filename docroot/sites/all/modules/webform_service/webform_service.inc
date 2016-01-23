<?php

/**
 * Determine whether the current user can access a submission resource.
 *
 * @param $op
 *   One of create, view, edit, delete per webform_submission_access().
 * @param $args
 *   Resource arguments passed through from the original request.
 * @return bool
 *
 * @see webform_submission_access() and webform_results_access()
 */
function _submission_resource_access($op = 'view', $args = array()) {
  // Adds backwards compatability with regression fixed in #1083242
  if (isset($args[1])) {
    $args[1] = _services_access_value($args[1], 'webform_submission');
  }

  // Make sure we have an object or this all fails, some servers can
  // mess up the types.
  if (is_array($args[0])) {
    $node = (object) $args[0];
  }
  elseif (!is_array($args[0]) && !is_object($args[0])) {  //This is to determine if it is just a string happens on node/%NID
    $node = node_load($args[0]);
  }

  if (is_array($args[1])) {
    $args[1] = (object) $args[1];
  }
  elseif (!is_array($args[0]) && !is_object($args[1])) {  //This is to determine if it is just a string happens on webform_submission/%SID
    $args[1] = (object)array('sid' => $args[1]);
  }

  if ($op != 'create') {    
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    if ($op == 'view' && empty($args[1]->sid)) {
      return webform_results_access($node);
    }
    else {
      $submission = webform_get_submission($node->nid, $args[1]->sid);
      return webform_submission_access($node, $submission, $op);
    }
  }
  elseif ($op == 'create') {
    return node_access($op, $node);
  }
}


/**
 * Returns the results of a webform_get_submission() for the specified nid and sid.
 *
 * @param $nid
 *   NID of the submitted node.
 * @param $sid
 *   SID of the submission we want to return.
 * @return
 *   Submission object or FALSE if not found.
 *
 * @see webform_get_submission()
 */
function _submission_resource_retrieve($nid, $sid) {
  return webform_get_submission($nid, $sid);
}


/**
 * Creates a new webform submission based on submitted values.
 *
 * Note that this function uses drupal_form_submit() to create new submissions,
 * which may require very specific formatting. The full implications of this
 * are beyond the scope of this comment block. The Googles are your friend.
 *
 * @param $nid
 *   Node id to which we make submission.
 * @param $node
 *   Array representing the attributes a submission add form would submit.
 * @return
 *   An associative array contained the new submission id.
 *
 * @see drupal_form_submit()
 */
function _submission_resource_create($nid, $submission) {
  $node = node_load($nid);
  // Adds backwards compatability with regression fixed in #1083242
  $submission = _services_arg_value($submission, 'webform_submission');

  // Load the required includes for drupal_execute
  module_load_include('module', 'webform');

  // Prepare form state
  $form_state = array();
  $form_state['values']['submitted'] = $submission;
  $form_state['values']['op'] = variable_get('services_webform_submit_button_resource_create', t('Submit'));

  // Submit form programmatically
  drupal_form_submit('webform_client_form_' . $node->nid, $form_state, $node, array());

  if ($errors = form_get_errors()) {
    return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  }
  // Fetch $sid out of $form_state
  $sid = $form_state['values']['details']['sid'];
  // Only add the URI for servers that support it.
  $submission = array('nid' => $nid, 'sid' => $sid);
  return $submission;
}


/**
 * Updates an existing submission based on submitted values.
 *
 * Note that this function uses drupal_execute() to create new submissions,
 * which may require very specific formatting. The full implications of this
 * are beyond the scope of this comment block. The Googles are your friend.
 *
 * @param $nid
 *   Node ID of the node the submission belongs to.
 * @param $sid
 *   Submission ID of the submission we're editing.
 * @param $submission
 *   Array representing the attributes a submission add form would submit.
 * @return
 *   The submission's sid.
 *
 * @see drupal_execute()
 */
function _submission_resource_update($nid, $sid, $submission) {
  $node = node_load($nid);
  // Adds backwards compatability with regression fixed in #1083242
  $submission = _services_arg_value($submission, 'webform_submission');

  // Load the required includes for drupal_execute
  module_load_include('module', 'webform');

  // Prepare form state
  $form_state = array();
  $form_state['values']['submitted'] = $submission;
  $form_state['values']['details']['sid'] = $sid;
  $form_state['values']['op'] = variable_get('services_webform_submit_button_resource_create', t('Submit'));

  // Submit form programmatically
  drupal_form_submit('webform_client_form_' . $node->nid, $form_state, $node, array());

  if ($errors = form_get_errors()) {
    return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  }
  // Fetch $sid out of $form_state
  $sid = $form_state['values']['details']['sid'];
  // Only add the URI for servers that support it.
  $submission = array('nid' => $nid, 'sid' => $sid);
  return $submission;
}


/**
 * Delete a submission given its nid ans sid.
 *
 * @param $nid
 *   NID of the submitted node.
 * @param $sid
 *   SID of the submission we're deleting.
 * @return
 *   Boolean value.
 */
function _submission_resource_delete($nid, $sid) {
  $node = node_load($nid);
  $submission = webform_get_submission($nid, $sid);
  if ($node && $submission) {
    webform_submission_delete($node, $submission);
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
 * Return an array of optionally paged sids baed on a set of criteria.
 *
 * An example request might look like
 *
 * http://domain/endpoint/submissions?fields=nid,sid&parameters[nid]=7&parameters[uid]=1
 *
 * This would return an array of objects with only nid and vid defined, where
 * nid = 7 and uid = 1.
 *
 * @param $page
 *   Page number of results to return (in pages of 20).
 * @param $fields
 *   The fields you want returned.
 * @param $parameters
 *   An array containing fields and values used to build a sql WHERE clause
 *   indicating items to retrieve.
 * @param $page_size
 *   Integer number of items to be returned.
 * @return
 *   An array of submissions objects.
 *
 * @todo
 *   Evaluate the functionality here in general. Particularly around
 *     - Do we need fields at all? Should this just return full submissions?
 *     - Is there an easier syntax we can define which can make the urls
 *       for index requests more straightforward?
 */
function _submission_resource_index($page, $fields, $parameters, $page_size) {
  module_load_include('inc', 'services', 'services.module');
  $submission_select = db_select('webform_submissions', 't')
    ->orderBy('submitted', 'DESC');

  services_resource_build_index_query($submission_select, $page, $fields, $parameters, $page_size, 'webform_submission');

  $results = services_resource_execute_index_query($submission_select);

  return services_resource_build_index_list($results, 'webform_submission', 'sid');
}


/**
 * Returns the submissions of a specified node.
 *
 * @param $nid
 *   Unique identifier for the node.
 * @param $count
 *   Number of submissions to return.
 * @return
 *   An array of submission objects.
 */
function _submission_resource_load_webform_submissions($nid, $count = 0) {
  return webform_get_submissions(array('nid' => $nid), array(), $count);
}
