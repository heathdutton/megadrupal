<?php

/**
 * @file
 * Plugin to provide access control based upon currently active step.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Route: step name"),
  'description' => t('Controls access using currently active step.'),
  'callback' => 'route_step_name_ctools_access_check',
  'settings form' => 'route_step_name_ctools_access_settings',
  'summary' => 'route_step_name_ctools_access_summary',
  'required context' => new ctools_context_required(t('Route'), 'route'),
);

/**
 * Settings form for the 'step_name' access plugin
 */
function route_step_name_ctools_access_settings($form, &$form_state, $conf) {
  $form['settings']['step_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Step name'),
    '#default_value' => $conf['step_name'],
    '#description' => t('Step name that activates this rule, i.e. inbound-train. This is machine readable string to be passed as argument in URL to active this rule.'),
    '#required' => TRUE,
    '#size' => 10,
  );
  
  return $form;
}

/**
 * Check for access.
 */
function route_step_name_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this is safe.  
  if (empty($context) || empty($context->data) || !isset($context->data->active_step)) {
    return FALSE;
  }

  // Please make sure here that the step we're trying to go to actually exists. If not, return FALSE.
  if(!$context->data->step_exists()){
    return FALSE;
  }

  // Please make sure here that the step we're trying to go to is actually processed. If not, return FALSE.
  if(!$context->data->is_processed()){
    return FALSE;
  }
  
  return $context->data->active_step == $conf['step_name'];
}

/**
 * Provide a summary description based upon the checked roles.
 */
function route_step_name_ctools_access_summary($conf, $context) {
  return "Current step name is: " . $conf['step_name'];
}

