<?php

/**
 * @file
 * This file contains functions using the Redmine REST API.
 */

/**
 * Retrieves the Redmine user information for the currently configured
 * API key and returns the Redmine UID.
 *
 * @return
 * Redmine user ID based on API key.
 */
function feedmine_rmuid(){
  // Retrieve Redmine API settings
  $rm_api_key  = variable_get('feedmine_rmapikey', NULL);
  $rm_base_url = variable_get('feedmine_rmurl', NULL);

  // Verify settings
  if(!isset($rm_api_key)){
    $msg = t('Unable to retrieve Redmine project list because no API key is present.');
    watchdog('feedmine', $msg);
    return;
  }

  // Prepare request
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Redmine-API-Key' => $rm_api_key,
    ),
    'method' => 'GET',
  );
  $rm_endpoint = '/users/current.json';
  $request_url = $rm_base_url.$rm_endpoint;

  // Get response
  $response = drupal_http_request($request_url, $options);

  // Verify response and retrieve results
  if($response->status_message == 'OK') {
    $data = json_decode($response->data);
    return $data->user->id;
  }
  else {
    $msg = t('Unable to retreve user ID from Redmine.<br><strong>Response:</strong><br><code>!response</code>');
    $vars = array('!response' => json_encode($response));
    watchdog('feedmine', $msg, $vars);
  }
}

/**
 * Retrieve a list of projects from the specified Redmine instance.
 *
 * @return
 * Returns a key / value pair array for projects currently available
 * within Redmine with project id as key and name as value.
 */
function feedmine_getrmprojects(){
  // Retrieve Redmine API settings
  $rm_api_key  = variable_get('feedmine_rmapikey', NULL);
  $rm_base_url = variable_get('feedmine_rmurl', NULL);

  // Verify settings
  if(!isset($rm_api_key)){
    $msg = t('Unable to retrieve Redmine project list because no API key is present.');
    watchdog('feedmine', $msg);
    return;
  }

  // Prepare request
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Redmine-API-Key' => $rm_api_key,
    ),
    'method' => 'GET',
  );
  $rm_endpoint = '/projects.json';
  $rm_options = array(
    'limit' => '100',
  );
  $request_url = $rm_base_url.$rm_endpoint.'?'.drupal_http_build_query($rm_options);

  // Get response
  $response = drupal_http_request($request_url, $options);

  // Verify response and retrieve results
  if($response->status_message == 'OK') {
    $results = json_decode($response->data);
    // Verify atleast a single project was retrieved.
    if(count($results->projects) < 1){
      return;
    }
  }
  else {
    $msg = t('Recieved a bad response while attempting to retrieve Redmine projects.<br><strong>Response as JSON:</strong><br><code>!response</code>');
    $vars = array('!response' => json_encode($response));
    watchdog('feedmine', $msg, $vars);
    return;
  }

  // Parse results and return list of project names keyed by project id.
  foreach($results->projects as $key => $project){
    $rm_projects[$project->id] = $project->name;
  };
  return $rm_projects;
}

/**
 * This is the submit handler for the feedback form to report user
 * feedback using the Redmine REST API to create a new issue in the
 * configured Redmine project.
 */
function feedmine_issue_report($form, &$form_state) {
  // Retrieve Redmine API settings
  $rm_base_url = variable_get('feedmine_rmurl', NULL);
  $rm_api_key  = variable_get('feedmine_rmapikey', NULL);
  $rm_uid      = variable_get('feedmine_rmuid', NULL);
  $rm_proj_id  = variable_get('feedmine_rmprojectid', NULL);

  // Collect new issue data
  global $user;
  $rmi_data = array();
  $rmi_data['user']             = $user;
  $rmi_data['destination']      = drupal_get_destination();
  $rmi_data['server_variable']  = $_SERVER;
  $rmi_data['feedback_tracker'] = $form_state['values']['feedmine_tracker'];
  $rmi_data['feedback_subject'] = $form_state['values']['feedmine_subject'];
  $rmi_data['feedback']         = $form_state['values']['feedmine_feedback'];

  // Attach a file to the issue.
  $uploads = array();
  // Retrieve a token for the file and refrence with the new issue
  $token = feedmine_issue_attach($rmi_data);
  if(isset($token)){
    $uploads['upload'] = array (
      'token' => $token,
      'filename' => 'fmuser_'.$user->uid.'_on_'.time().'.json',
      'content_type' => 'application/json',
    );
  }

  // Format new issue data.
  $issue = array();
  $issue['issue'] = array(
    'project_id'     => $rm_proj_id,
    'subject'        => $rmi_data['feedback_subject'],
    'description'    => $rmi_data['feedback'],
    'assigned_to_id' => $rm_uid,
    'uploads'        => $uploads,
  );

  // Prepare request.
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Redmine-API-Key' => $rm_api_key,
    ),
    'method' => 'POST',
    'data' => json_encode($issue),
  );
  $rm_endpoint = '/issues.json';
  $request_url = $rm_base_url.$rm_endpoint;

  // Submit request and get response.
  $response = drupal_http_request($request_url, $options);

  // Check response,log any errors and notify user with confirmation.
  if($response->status_message  != 'Created') {
    $msg = t("Unable to create a new Redmine ticket.<br><br><strong>Data:</strong><br><code>!data</code><br><strong>Request URL:</strong><br><code>!request</code><br><strong>Headers:</strong><br><code>!headers</code>");
    $vars = array('!data' => $options['data'], '!request' => $request_url, '!headers' => print_r($options['headers'], 1));
    watchdog('feedmine', $msg, $vars);
  }
  else {
    drupal_set_message('Thanks for your feedback, an issue has been created.', 'status');
  }
}

/**
 * Upload file contents to Redmine and retrieve a token used to attach
 * the file to a Redmine issue later when we create the issue.
 *
 * @param1
 * Contents of the file.
 * @return
 * A token used to attach the file to a Redmine issues.
 */
function feedmine_issue_attach($file_contents){
  // Retrieve Redmine API settings
  $rm_base_url = variable_get('feedmine_rmurl', 0);
  $rm_api_key  = variable_get('feedmine_rmapikey', 0);
  $rm_proj_id  = variable_get('feedmine_rmprojectid', 0);

  // Prepare request.
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/octet-stream',
      'X-Redmine-API-Key' => $rm_api_key,
    ),
    'method' => 'POST',
    'data' => json_encode($file_contents),
  );
  $rm_endpoint = '/uploads.json';
  $request_url = $rm_base_url.$rm_endpoint;

  // Submit request and get response.
  $response = drupal_http_request($request_url, $options);

  // Verify response.
  if($response->error  != 'Created') {
    $msg = t("Unable to retrieve a upload token.<br><br><strong>Data:</strong><br><code>!data</code><br><strong>Request URL:</strong><br><code>!request</code><br><strong>Headers:</strong><br><code>!headers</code><br><strong>Response as JSON:</strong><br><code>!response</code>");
    $vars = array('!data' => $options['data'], '!request' => $request_url, '!headers' => print_r($options['headers'], 1), '!response' => json_encode($response));
    watchdog('feedmine', $msg, $vars);
    return;
  }
  else {
    // Retrieve results.
    $results = json_decode($response->data);
  }

  // Verify and return results.
  if(isset($results->upload->token)){
    $token = $results->upload->token;
    return $token;
  }
  else {
    $msg = t("Unable to locate file upload token in response data.<br><br><strong>JSON Response results:</strong><br><code>!results</code>");
    $vars = array('!results' => json_encode($response->data));
    watchdog('feedmine', $msg, $vars);
    return;
  }
}
