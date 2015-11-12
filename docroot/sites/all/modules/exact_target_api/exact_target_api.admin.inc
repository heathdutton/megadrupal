<?php
/**
 * @file
 * This file contains the admin and GUI callbacks for the module that are not needed in day-to-day operations
 */

/**
 * Implements callback for form construction
 */
function exact_target_api_configure_form($form, &$form_state) {
  $form = array();
  $cfg = _exact_target_api_get_config();

  $form[ET_CFG_ENDPOINT] = array(
    '#type' => 'select',
    '#title' => t('ExactTarget API endpoint'),
    '#description' => t('The URL that acts as the endpoint for the XML API calls, described <a href="http://wiki.memberlandingpages.com/030_Developer_Documentation/040_XML_API/ExactTarget_XML_API_Technical_Reference" target="_blank">here</a>.  If you are not sure what your endpoint type is, refer to <a href="http://wiki.memberlandingpages.com/The_Getting_Started_Guide/Set_Up_Your_Account" target="_blank">this page</a>.'),
    '#options' => array(
      ET_XML_ENDPOINT_NORMAL => t('Normal Endpoint'),
      ET_XML_ENDPOINT_S4 => t('S4 Instance Endpoint'),
    ),
    '#default_value' => (!empty($cfg[ET_CFG_ENDPOINT])) ? $cfg[ET_CFG_ENDPOINT] : ET_XML_ENDPOINT_NORMAL,
  );
  $form[ET_CFG_API_USER] = array(
    '#type' => 'textfield',
    '#title' => t('ExactTarget API Username'),
    '#default_value' => (!empty($cfg[ET_CFG_API_USER])) ? $cfg[ET_CFG_API_USER] : '',
    '#required' => TRUE,
  );
  $form[ET_CFG_API_PASS] = array(
    '#type' => 'password',
    '#title' => t('ExactTarget API Password'),
    '#required' => TRUE,
  );
  /* Not sure if we need to add this at this time...
   $form[ET_CFG_CHANNEL_MEMBER_ID] = array(
   '#type' => 'textfield',
   '#title' => t('ExactTarget Channel Member ID'),
   '#default_value' =>  (!empty($cfg[ET_CFG_CHANNEL_MEMBER_ID])) ? $cfg[ET_CFG_CHANNEL_MEMBER_ID] : '',
   '#required' => FALSE,
   );
   */
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Implements callback for validation of form 'exact_target_api_configure_form'
 */
function exact_target_api_configure_form_validate($form, &$form_state) {
  // Go ahead and store the values as the test API call won't work otherwise: 
  _exact_target_api_set_config($form_state['values']);

  // Test the new creds using the values we were just given:
  $lists = exact_target_api_get_lists();
  if (empty($lists)) {
    // We should ALWAYS get something back as the "global" list is always present
    form_set_error('', t('Failed to retrieve subscription lists in test call to the ExactTarget server.  Please check your credentials and try again.'));
  }
  else {
    $form_state['storage']['et_lists'] = $lists;
  }
}

/**
 * Implements callback for submit handling of form 'exact_target_api_configure_form'
 */
function exact_target_api_configure_form_submit($form, &$form_state) {
  drupal_set_message(t('ExactTarget API settings successfully saved.'), 'status');

  // Now initialize/refresh the subscription list cache so it doesn't bog down any future API calls
  $batch = array(
    'operations' => array(
      array('_exact_target_process_lists', array($form_state['storage']['et_lists'])),
    ),
    'finished' => '_exact_target_process_lists_finished',
    'title' => t('Processing initial ExactTarget subscription lists'),
    'init_message' => t('Batch is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Batch processing of ExactTarget subscription lists has encountered an error.'),
    'file' => drupal_get_path('module', 'exact_target_api') . '/exact_target_api.admin.inc',
  );
  batch_set($batch);
}

/**
 * Implements callback for batch processing run
 *
 * Bits and pieces shamelessly copied from http://drupal.org/node/180528
 */
function _exact_target_process_lists($lists, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($lists);
  }

  // Retrieve the next list's information and cache it

  $listid = $lists[$context['sandbox']['progress']];
  $listinfo = exact_target_api_get_list_info($listid);

  // Update our progress information.

  $context['sandbox']['progress']++;
  $context['results'][] = t('Processed ExactTarget list ID %id', array('%id' => $listid));
  $context['message'] = t('Now processing list %count of %max', array('%count' => $context['sandbox']['progress'], '%max' => $context['sandbox']['max']));

  // Inform the batch engine that we are not finished,

  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Implements callback for completion of batch processing run
 *
 * Shamelessly copied from http://drupal.org/node/180528
 */
function _exact_target_process_lists_finished($success, $results, $operations) {
  if ($success) {
    $message = count($results) . ' ExactTarget subscription lists processed.';
  }
  else {
    // An error occurred.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
  }
  drupal_set_message($message);
}
