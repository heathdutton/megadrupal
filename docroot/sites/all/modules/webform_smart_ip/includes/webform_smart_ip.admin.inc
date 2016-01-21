<?php

/**
 * @file
 * Admin interface callbacks/handlers to configure Webform Smart IP.
 */

// First we need to load the sumissions functions from the webform module
module_load_include('inc', 'webform', 'includes/webform.submissions');

/**
 * Webform Smart IP administration settings.
 *
 * @return
 *   Forms for store administrator to set configuration options.
 */
function webform_smart_ip_admin_update_records_form($form, &$form_state) {
  // Define submit handler function

  // Container for manual lookup
  $form['webform_smart_ip_manual_lookup'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Update existing form entries'),
    '#description' => t('Update forms submitted prior to Webform Smart IP installation'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  );

  $allWebforms = webform_smart_ip_get_webforms_with_smart_ip_fields();

  $form['webform_smart_ip_manual_lookup']['form_to_process'] = array(
    '#type' => 'select',
    '#title' => t('Which Form would you like to process?'),
    '#options' => $allWebforms,
    '#description' => t('Select the webform you would like to process.'),
    '#required' => TRUE,
    '#empty_option' => '--Webforms--',
  );

  // Form for manual updating of the IP-Country database
  $form['webform_smart_ip_manual_lookup']['webform_smart_ip_lookup_button'] = array(
    '#type'   => 'submit',
    '#value'  => t('Process Form'),
    '#submit' => array('_webform_smart_ip_lookup_submit'),
    '#ajax'   => array(
      'callback' => '_webform_smart_ip_lookup_js',
    ),
    '#suffix' => isset($form_state['storage']['webform_smart_ip_message']) ? '<div id="lookup-message" class="messages">' . $form_state['storage']['webform_smart_ip_message'] . '</div>' : '',
  );

  return $form;
}


/**
 * Get all webform containing smartIP fields
 * @param  string $bundle Type of node to gather
 * @return array       array of webforms and node ids.
 */
function webform_smart_ip_get_webforms_with_smart_ip_fields() {
  $webform_types = webform_variable_get('webform_node_types');

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $webform_types)
    ->propertyCondition('status', 1);
  $results = $query->execute();

  $smart_ip_nids = array();

  if (!empty($results['node'])) {
    $nids = array_keys($results['node']);
    $nodes = node_load_multiple($nids);
    foreach ($nodes as $node) {
      $components = $node->webform['components'];
      foreach ($components as $component) {
        if ($component['type'] == 'smart_ip') {
          $smart_ip_nids[$node->nid] = $node->title;
          break;
        }
      }
    }
  }

  return $smart_ip_nids;
}

/**
 * Submit handler to queue form submissions for update.
 *
 * @see _webform_smart_ip_lookup_js()
 */
function _webform_smart_ip_lookup_submit($form, &$form_state) {
  $nid = $form_state['values']['form_to_process'];
  $node = node_load($nid);

  $all_sumbissions = webform_get_submissions($nid);

  $queue = DrupalQueue::get('webform_smart_ip_form_updates');
  $queue->createQueue();

  foreach ($all_sumbissions as $submission) {
    if (!webform_smart_ip_check_for_existing_value($node, $submission)) {
      $queue->createItem($submission);
    }
  }

  $numItems = $queue->numberOfItems();

  $message = $numItems . " items will be updated at time next cron run.";

  $form_state['storage']['webform_smart_ip_message'] = $message;
  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX callback to let user know forms are being processed
 *
 * @see _webform_smart_ip_lookup_submit()
 */
function _webform_smart_ip_lookup_js($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_remove('#lookup-message');
  $commands[] = ajax_command_after('#edit-webform-smart-ip-lookup-button', '<div id="lookup-message" class="messages"></div>');
  $commands[] = ajax_command_html('#lookup-message', $form_state['storage']['webform_smart_ip_message']);
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Populate array with existing smart_ip fields used in a form
 * @param  object $nodeEntity Webform Object
 * @return array             array of existing Smart IP Fields.
 */
function webform_smart_ip_get_component_ids($node) {
  $component_ids = array();
  foreach ($node->webform['components'] as $cid => $component) {
    // Find Smart IP fields
    if ($component['type'] == 'smart_ip') {
      $component_ids[$cid] = $component['extra']['smart_ip_type'];
    }
  }

  return $component_ids;
}


/**
 * Checks to see if the existing record has already been geocoded
 * @param  object $form_object Form object
 * @param  object $submission  form submission object
 * @return bool              Whether a smart ip field has a value or not
 */
function webform_smart_ip_check_for_existing_value($node, $submission) {
  $smart_ip_components = webform_smart_ip_get_component_ids($node);
  $form_has_values = false;
  foreach ($smart_ip_components as $cid => $value) {
    if (array_key_exists($cid, $submission->data) && $submission->data[$cid][0] ) {
      $form_has_values = true;
    }
  }

  return $form_has_values;
}
