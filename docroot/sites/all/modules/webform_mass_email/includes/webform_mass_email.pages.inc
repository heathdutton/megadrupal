<?php

/**
 * @file
 *   Webform Mass Email sending page.
 *
 * @ingroup webform_mass_email
 */

/**
 * Implements hook_forms().
 */
function webform_mass_email_forms($form_id, $args) {
  $forms['webform_mass_email_form'] = array(
    'callback' => 'webform_mass_email_form',
  );
  return $forms;
}

/**
 * Create a table containing all submitted values for a webform node.
 */
function webform_mass_email_form($form, $form_state, $node) {
  // Load the Webform module functions for our use.
  module_load_include('inc', 'webform', 'includes/webform.report');
  module_load_include('inc', 'webform', 'includes/webform.submissions');

  // Build the form, output the webform results as a suffix.
  $form = array(
    '#suffix' => webform_results_table($node, 50),
  );
  $form['mass_mail'] = array(
    '#type' => 'fieldset',
    '#title' => t('Webform Mass Email'),
  );

  // Store the webform submissions for later use (submit function processing).
  $form['mass_mail']['submissions'] = array(
    '#type' => 'value',
    '#value' => webform_get_submissions($node->nid),
  );
  // Store info about the webform node.
  $form['mass_mail']['node'] = array(
    '#type' => 'value',
    '#value' => array(
      'nid' => $node->nid,
      'title' => $node->title,
    ),
  );

  // Get all email components for the select list.
  $email_components = array();
  $default = NULL;
  foreach ($node->webform['components'] as $cid => $component) {
    if ($component['type'] === 'email') {
      if (empty($default)) $default = $cid;
      $email_components[$cid] = $component['name'];
    }
  }

  // Build rest of the form components.
  $form['mass_mail']['email_component'] = array(
    '#type' => 'select',
    '#title' => t('Email field'),
    '#description' => t('Select the email field to be used as the recipient address.'),
    '#default_value' => $default,
    '#options' => $email_components,
    '#required' => TRUE,
  );
  $form['mass_mail']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('Enter the email subject.'),
    '#required' => TRUE,
  );
  $form['mass_mail']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#description' => t('Enter the email body text.'),
    '#required' => TRUE,
  );
  $form['mass_mail']['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Create a table containing all submitted values for a webform node.
 */
function webform_mass_email_form_submit($form, $form_state) {
  // Retrieve values from the form.
  $submissions = $form_state['values']['submissions'];
  $email_component_cid = $form_state['values']['email_component'];
  $subject = $form_state['values']['subject'];
  $body = $form_state['values']['body'];
  $node_nid = $form_state['values']['node']['nid'];
  $node_title = $form_state['values']['node']['title'];

  // Prepare the values that we're going to put into the queue.
  $queue_values = array(
    'nid' => $node_nid,
    'node_title' => $node_title,
    'cid' => $email_component_cid,
    'subject' => $subject,
    'body' => $body,
  );

  // Store all emails here for to prevent any duplicates.
  $emails = array();
  // Loop through the submissions, pick up submission id + email and
  // enqueue the request for the cron to fetch.
  $count = 0;
  foreach ($submissions as $sid => $submission) {
    // Reset, store submission id.
    $queue_values['sid'] = $sid;
    $queue_values['email'] = '';

    // There's email for this submission and it's not already queued before.
    if (isset($submission->data[$email_component_cid]['value'][0])
        && !empty($submission->data[$email_component_cid]['value'][0])
        && !in_array($submission->data[$email_component_cid]['value'][0], $emails)) {

      // Store the email.
      $emails[] = $submission->data[$email_component_cid]['value'][0];
      $queue_values['email'] = $submission->data[$email_component_cid]['value'][0];

      // Queue the values into the 'queue' table.
      webform_mass_email_enqueue_request($queue_values);
      $count++;
    }
  }

  // Set message with the count.
  drupal_set_message(t('%count items queued for sending.', array('%count' => $count)));
}
