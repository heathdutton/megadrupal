<?php

/**
 * Callback for drupal_get_form
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function mail_debugger_callback($form, &$form_state) {

  // basic form setup
  $form = array(
    '#tree' => TRUE,
    'tab_group' => array(
      '#type' => 'vertical_tabs',
      '#default_tab' => user_variable_get('mail_debugger_default_tab'),
    ),
  );

  $default_values = user_variable_get('mail_debugger_default_values');
  
  $pages = module_invoke_all('mail_debugger_info');
  
  drupal_alter('mail_debugger_info', $pages);
  
  foreach ($pages as $page => $info) {
    // prevent array type errors
    if (!is_array($info)) {
      continue;
    }

    // ensure default values
    $info = $info + array(
      'include' => array(),
      'title' => NULL,
      'form' => NULL,
      'validate' => NULL,
      'submit' => NULL,
      'weight' => 0,
    );

    // there must be a title, form and submit.
    if (empty($info['title']) || empty($info['form']) || empty($info['submit'])) {
      continue;
    }

    // Do we need to load an include file?
    if (is_array($info['include']) && count($info['include']) >= 3) {
      list($type, $module, $name) = $info['include'];
      module_load_include($type, $module, $name);
    }

    if (!function_exists($info['form'])) {
      continue;
    }
    
    $page_form = call_user_func($info['form'], $default_values[$page]);

    if (!$page_form) {
      continue;
    }

    // basic page setup
    $form[$page] = array(
      '#type' => 'fieldset',
      '#group' => 'tab_group',
      '#title' => $info['title'],
      '#weight' => $info['weight'],
    );

    $form_state['mail_debugger']['submit_callback']["submit-$page"] = $info['submit'];
    $form_state['mail_debugger']['validate_callback']["submit-$page"] = $info['validate'];
    $form_state['mail_debugger']['active_element']["submit-$page"] = $page;

    // add fields
    foreach ($page_form as $var => $data) {
      $form[$page][$var] = $data;
      
      $form_state['mail_debugger']['elements'][$page][$var] = "$page][$var";
    }


    // add submit button
    $form[$page]['submit'] = array(
      '#type' => 'submit',
      '#name' => "submit-$page",
      '#value' => t('Send'),
      '#submit' => array(
        'mail_debugger_callback_submit',
      ),
      '#validate' => array(
        'mail_debugger_callback_validate',
      ),
    );
  }

  return $form;
}

/**
 * Form submit handler. Store form data to variables.
 * 
 * @param array $form
 * @param array $form_state
 */
function mail_debugger_callback_submit($form, &$form_state) {
  // find the active page
  $trigger = $form_state['triggering_element']['#name'];
  $page = $form_state['mail_debugger']['active_element'][$trigger];
  
  // return to this page after submit
  user_variable_set('mail_debugger_default_tab', "edit-$page");
  
  // store the parameters
  user_variable_set('mail_debugger_default_values', $form_state['values']);

  // validate submit handler
  $function_name = $form_state['mail_debugger']['submit_callback'][$trigger];
  if (function_exists($function_name)) {
    // execute the callback
    call_user_func($function_name, $form_state['values'][$page], $form_state);
  }
}

/**
 * Form validate handler. Store form data to variables.
 * 
 * @param array $form
 * @param array $form_state
 */
function mail_debugger_callback_validate($form, &$form_state) {
//  $form_state['rebuild'] = TRUE;
//  dpm($form_state);
  // find the active page
  $trigger = $form_state['triggering_element']['#name'];
  $page = $form_state['mail_debugger']['active_element'][$trigger];

  // Inventorize validate handlers
  if (!is_array($form_state['mail_debugger']['validate_callback'][$trigger])) {
    $callbacks = array($form_state['mail_debugger']['validate_callback'][$trigger]);
  } else {
    $callbacks = $form_state['mail_debugger']['validate_callback'][$trigger];
  }
  
  // Execute the submit handlers
  foreach($callbacks as $function_name) {
    if (!function_exists($function_name)) {
      continue;
    }
    
    $param = array(
      $form_state['values'][$page],
      $form_state['mail_debugger']['elements'][$page],
      &$form_state,
    );
    
    call_user_func_array($function_name, $param);
  }
}
