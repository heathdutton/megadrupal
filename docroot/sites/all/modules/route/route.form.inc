<?php

/**
 * @file
 * Implements number of related form building functions and validate / submit hanlders.
 */

/**
 * Base route form.
 *
 * @param Array $form
 * @param Array $form_state
 * @param Array $sub_form_id
 * @return Array
 */
function route_form($form, &$form_state) {
  $form_id = $form_state['build_info']['form_id'];

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#validate' => array(),
    '#submit' => array('route_control_form_set_cache'),
  );

  // Inject subform
  if ($form_id && function_exists($form_id)) {
    // Wire up any validate and submit handlers.
    // In order for this to work, developers will have to stick to naming convention
    // {$form_id}_validate, {$form_id}_submit
    if (isset($form['buttons']['submit']) && function_exists($form_id . '_validate')) {
      array_unshift($form['buttons']['submit']['#validate'], $form_id . '_validate');
    }

    if (isset($form['buttons']['submit']) && function_exists($form_id . '_submit')) {
      array_unshift($form['buttons']['submit']['#submit'], $form_id . '_submit');
    }
  }

  return $form;
}

/**
 * Base route control form.
 *
 * @param Array $form
 * @param Array $form_state
 * @return Array
 */
function route_control_form($form, &$form_state) {
  $route = $form_state['route'];
  $form_id = $form_state['build_info']['form_id'];

  // Set next and previous steps. this has to be attached to the form, so multiple clicks on Next and Previous buttons won't progress to steps that are further than next.
  $form['previous_step'] = array(
    '#type' => 'value',
    '#value' => $route->get_previous_step()
  );

  $form['next_step'] = array(
    '#type' => 'value',
    '#value' => $route->get_next_step()
  );

  $form['buttons']['#weight'] = 999;

  // Show only, if there's actually previous step available
  if ($route->get_previous_step()) {
    $form['buttons']['back'] = array(
      '#type' => 'submit',
      '#value' => t('Back'),
      '#validate' => array(),
      '#attributes' => array('class' => array('route-back')),
      '#submit' => array('route_control_form_back_submit', 'route_control_form_set_cache'),
    );
  }

  // Show only, if there's actually next step available
  if ($route->get_next_step()) {
    $form['buttons']['continue'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
      '#validate' => array(),
      '#attributes' => array('class' => array('route-continue')),
      '#submit' => array('route_control_form_continue_submit', 'route_control_form_set_cache'),
    );
  }

  // Show only, if there's no next step available. This means we're finishing the journey
  else {
    $form['buttons']['continue'] = array(
      '#type' => 'submit',
      '#value' => t('Start again'),
      '#validate' => array(),
      '#submit' => array('route_control_form_finish_submit', 'route_control_form_set_cache'),
    );
  }

  // Inject subform
  if ($form_id && function_exists($form_id)) {
    // Wire up any validate and submit handlers.
    // In order for this to work, developers will have to stick to naming convention
    // {$form_id}_validate, {$form_id}_submit
    if (isset($form['buttons']['continue']) && function_exists($form_id . '_validate')) {
      array_unshift($form['buttons']['continue']['#validate'], $form_id . '_validate');
    }

    if (isset($form['buttons']['continue']) && function_exists($form_id . '_submit')) {
      array_unshift($form['buttons']['continue']['#submit'], $form_id . '_submit');
    }
  }

  // Inject handlers that reload the route object. This is for all forms that do AJAX based processing. We want context stored in form_state to always be fresh
  if (isset($form['buttons']['continue']['#submit']) && is_array($form['buttons']['continue']['#submit'])) {
    array_unshift($form['buttons']['continue']['#submit'], 'route_control_form_reload_cache');
  }

  if (isset($form['buttons']['back']['#submit']) && is_array($form['buttons']['back']['#submit'])) {
    array_unshift($form['buttons']['back']['#submit'], 'route_control_form_reload_cache');
  }

  return $form;
}

/**
 * Base route control form submit handler
 * @param Array $form
 * @param Array $form_state
 */
function route_control_form_back_submit($form, &$form_state) {
  $route = $form_state['route'];

  // Call back hook
  module_invoke_all('route_back', $route);

  // Regress to the previous step.
  if (!$form_state['redirect'] && !empty($form_state['values']['previous_step'])) {
    $form_state['redirect'] = route_build_path(route_get_current_page_subtask_name(), $form_state['values']['previous_step']);
  }
}

/**
 * Base route control form submit handler
 * @param Array $form
 * @param Array $form_state
 */
function route_control_form_continue_submit($form, &$form_state) {
  $route = $form_state['route'];

  // Call next hook
  module_invoke_all('route_next', $route);

  // Progress to the next step and process
  if (!$form_state['redirect'] && !$form_state['rebuild'] && !empty($form_state['values']['next_step'])) {
    $route->process_step($form_state['values']['next_step']);
    $form_state['redirect'] = route_build_path(route_get_current_page_subtask_name(), $form_state['values']['next_step']);
  }
}

/**
 * Base route control form finish handler
 * @param Array $form
 * @param Array $form_state
 */
function route_control_form_finish_submit($form, &$form_state) {
  $route = $form_state['route'];

  // Call finish hook
  module_invoke_all('route_finish', $route);

  // Reset the Route object
  $form_state['route'] = $route = route_new(route_get_current_page_subtask_name());

  // Set the status of the journey as successful and redirect to special step 'finish'.
  if (!$form_state['redirect'] && !$form_state['rebuild'] && $next_step = $route->get_next_step()) {

    // TODO: Go to the begining OR to the path specified by the user.
    $form_state['redirect'] = route_build_path(route_get_current_page_subtask_name(), $next_step);
  }
}

/**
 * Submit handler that saves route object
 * @param Array $form
 * @param Array $form_state
 */
function route_control_form_set_cache($form, &$form_state) {
  route_set(route_token(), $form_state['route']);
}

/**
 * Submit handler that reloads route object
 * @param Array $form
 * @param Array $form_state
 */
function route_control_form_reload_cache($form, &$form_state) {
  // It's necessary to realod context here. This is due to the fact that we're using lot of ajax calls and one of them could modify context object in due course.
  $context = route_get(route_token());

  $form_state['route'] = $context;
}