<?php

/**
 * amplify.admin
 *
 * @file
 */

/**
 * Administrative form for QP services.
 */
function amplify_admin_form() {
  
  $form['amplify_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Amplify API Key'),
    '#description' => t('Enter your developer API key for the OpenAmplify web service'),
    '#default_value' => variable_get('amplify_api_key', ''),
    '#size' => 60,
    '#maxlength' => 256,
    '#required' => FALSE,
  );
  
  
  // Get node types that OpenAmplify should submit.
  $types = node_type_get_types();
  $type_list = array();
  foreach ($types as $key => $type) {
    $type_list[$key] = $type->name;
  }
  $form['amplify_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Amplified Node Types'),
    '#description' => t('List of node types that will be amplified by OpenAmplify.'),
    '#options' => $type_list,
    '#default_value' => variable_get('amplify_node_types', array()),
    '#required' => FALSE,
  );
  
  $form['test'] = array(
    '#markup' => t('Once you have saved, you can test !here.', array('!here' => l(t('here'), 'admin/config/search/amplify/test'))) . '<br/>',
  );
  
  
  return system_settings_form($form);
}

function amplify_test_url_form() {
  $form['help'] = array(
    '#type' => 'markup',
    '#value' => t('Enter either a URL or a Node ID (nid) to test the OpenAmplify service.'),
  );
  
  $form['amplify_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of resource to amplify'),
    '#description' => t('Enter a full URL which will then be submitted to OpenAmplify for analysis.'),
    '#default_value' => 'http://drupal.org/about',
    '#size' => 60,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  $form['amplify_node_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Node ID (nid)'),
    '#description' => t('Enter a Node ID here to submit a node to OpenAmplify for analysis.'),
    '#default_value' => t(''),
    '#size' => 60,
    '#maxlength' => 256,
    '#required' => FALSE,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}

function amplify_test_url_form_submit(&$form, &$form_state) {
  $nid = $form_state['values']['amplify_node_id'];
  $url = $form_state['values']['amplify_url'];
  
  if (!empty($url)) {
    drupal_set_message(t('Using URL.'), 'status');
    $qp = amplify_amplify_url($url);
  }
  elseif (!empty($nid)) {
    drupal_set_message(t('Using Node'), 'status');
    $content = strip_tags(node_view(node_load($nid)));
    $qp = _amplify_do_request(array('analysis' => 'topics'), $content);
  }
  else {
    drupal_set_message(t('You must enter either a URL or a Node ID'), 'error');
    return;
  }
  
  if (empty($qp) || $qp->size() == 0) {
    drupal_set_message(t('No content was returned by the remote server or the URL request for content failed (check log for details).'), 'error');
    return;
  }
  
  drupal_set_message(t('Output: <pre>@xml</pre>', array('@xml' => htmlentities($qp->xml()))), 'status');
  
}