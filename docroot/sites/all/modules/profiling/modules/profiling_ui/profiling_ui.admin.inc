<?php

/**
 * @file
 * Profiling UI settings.
 */

function profiling_ui_core_settings($form, &$form_state) {
  $form = array();

  $form['profiling_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t("Enable profiling"),
    '#description' => t("Check this option to start profiling! By unchecking this option, you disable profiling data collection, the profiling module will reduce his impact on performances but won't do any task anymore. This is like uninstalling the profiling module except you still get access to old profiling tracks for analysis."),
    '#default_value' => variable_get('profiling_enabled', FALSE), 
  );

  $form['profiling_query_log'] = array(
    '#type' => 'checkbox',
    '#title' => t("Core query logging"),
    '#description' => t("By checking this option, you activate the core query logging facilities. Notice that this could have a non neglictable performance impact, but it will allow the profiling module to log number of queries ran during each timer lifetime."),
    '#default_value' => variable_get('profiling_query_log', 1), 
    '#disabled' => TRUE,
  );

  $options = module_invoke_all('profiling_backend');
  $form['profiling_backend'] = array(
    '#title' => t("Backend"),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('profiling_backend', 'Profiling_Backend_Default'),
    '#description' => t("Modules can provide environment specific backend, such as Pinba based measures or any other external tool related backend. Backends can provide more or less information depending on their implementation.")
      . ' ' . t("Default backend has a low performance impact itself, and uses basic PHP API for time and memory measure and will work smoothly within all environments."),
  );

  $form = system_settings_form($form);

  $form['buttons']['clear'] = array(
    '#type' => 'submit',
    '#value' => t("Wipe out collected data"),
    '#validate' => array(),
    '#submit' => array('profiling_ui_core_settings_submit_clear'),
  );

  return $form;
}

/**
 * Submit handler for data wipe out button.
 */
function profiling_ui_core_settings_submit_clear($form, &$form_state) {
  db_query("DELETE FROM {profiling_timers}");
  db_query("DELETE FROM {profiling_request}");
  drupal_set_message(t("Emptied the collected timers table"));
}

/**
 * Hook configuration.
 */
function profiling_ui_core_settings_hooks($form, &$form_state) {
  $form = array('#tree' => TRUE);

  // Append hook settings.
  $hooks = variable_get('profiling_hooks', array());

  // For AJAX rebuild.
  if (isset($form_state['add_hook'])) {
    foreach ($form_state['add_hook'] as $hook_name) {
      $hooks[$hook_name] = array();
    }
    unset($form_state['add_hook']);
  }
  if (isset($form_state['remove_hook'])) {
    foreach ($form_state['remove_hook'] as $hook_name) {
      unset($hooks[$hook_name]);
    }
    unset($form_state['remove_hook']);
  }

  $form['code'] = array(
    '#type' => 'fieldset',
    '#title' => t('<em>settings.php</em> file code'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['code']['export'] = array(
    '#prefix' => '<pre>',
    '#suffix' => '</pre>',
    '#markup' => "\$conf['profiling_hooks'] = " . var_export($hooks, TRUE),
  );

  $form['hooks'] = array(
    '#prefix' => '<div class="clearfix" id="hook-wrapper">',
    '#suffix' => '</div>',
    '#theme' => 'profiling_ui_core_settings_hooks_table',
  );
  foreach ($hooks as $hook => $options) {
    $form['hooks'][$hook]['name'] = array('#markup' => 'hook_' . $hook . '()');
    $form['hooks'][$hook]['array'] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($options['array']) ? $options['array'] : FALSE,
    );
    $form['hooks'][$hook]['remove-' . $hook] = array(
      '#type' => 'submit',
      '#value' => t("Remove"),
      '#attributes' => array('class' => array('use-ajax-submit')),
      '#submit' => array('profiling_ui_core_settings_hooks_submit_remove'),
      '#limit_validation_errors' => array(array('hooks')),
      '#ajax' => array(
        'callback' => 'profiling_ui_core_settings_hooks_js',
        'wrapper' => 'hook-wrapper',
      ),
    );
    
    $modules = module_implements($hook);
    $modules = array_flip($modules);
    unset($modules['profiling'], $modules['profiling_late']);
    $form['hooks'][$hook]['modules'] = array('#markup' => implode(', ', array_keys($modules)));
  }

  $form['add'] = array(
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['add']['hook_name'] = array(
    '#type' => 'textfield',
    '#size' => 32,
  );
  $form['add']['add_submit'] = array(
    '#type' => 'submit',
    '#value' => t("Add hook"),
    '#submit' => array('profiling_ui_core_settings_hooks_submit_add'),
    '#validate' => array('profiling_ui_core_settings_hooks_validate_add'),
    '#limit_validation_errors' => array(array('add')),
  	'#ajax' => array(
      'callback' => 'profiling_ui_core_settings_hooks_js',
      'wrapper' => 'hook-wrapper',
    ),
  );

  $form['save_submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save hook selection"),
    '#submit' => array('profiling_ui_core_settings_hooks_submit_save'),
    '#validate' => array(),
  );

  return $form;
}

/**
 * Return hooks part of form JS handler.
 */
function profiling_ui_core_settings_hooks_js($form, $form_state) {
  return $form['hooks'];
}

/**
 * Remove hook AJAX submit.
 */
function profiling_ui_core_settings_hooks_submit_remove($form, &$form_state) {
  // Fetch item delta to remove.
  // Proceed to variable recopy to avoid modifications.
  $parents = $form_state['clicked_button']['#parents'];
  list(, $hook_name) = explode('-', array_pop($parents), 2);
  $form_state['remove_hook'][] = $hook_name;
  // Mark the form for rebuild. The object will give the new decorator list to
  // build state.
  $form_state['rebuild'] = TRUE;
}

function profiling_ui_core_settings_hooks_validate_add($form, &$form_state) {
  $hook_name = $form_state['values']['add']['hook_name'];
  // Proceed to any alteration first, name could be empty after removing the
  // hook_ prefix.
  if (substr($hook_name, 0, 5) == 'hook_') {
    $hook_name = substr($hook_name, 5);
  }
  // Check hook name is valid and not empty.
  if (!preg_match('/^[a-zA-Z_0-9]+$/', $hook_name)) {
    form_set_error('add][hook_name', t("Hook name is invalid or empty"));
  }
  // Check if hook already exists.
  if (isset($form['hooks'][$hook_name])) {
    form_set_error('add][hook_name', t());
  }
  // Reset the input with the cleanup name.
  $form_state['values']['add']['hook_name'] = $hook_name;
}

/**
 * Add hook submit handler.
 */
function profiling_ui_core_settings_hooks_submit_add($form, &$form_state) {
  $form_state['add_hook'][] = $form_state['values']['add']['hook_name'];
  // Rebuild form and ensure it keeps our $form_state.
  $form_state['rebuild'] = TRUE;
}

/**
 * Save user settings.
 */
function profiling_ui_core_settings_hooks_submit_save($form, &$form_state) {
  $hooks = array();
  foreach ($form_state['values']['hooks'] as $hook => $options) {
    $hooks[$hook] = array('array' => $options['array']);
  }
  variable_set('profiling_hooks', $hooks);
  $form_state['rebuild'] = FALSE;
  drupal_set_message(t("Hook selection has been updated."));
}

/**
 * Render a table in the hook form.
 */
function theme_profiling_ui_core_settings_hooks_table(&$variables) {
  $form = &$variables['form'];
  $headers = array(t("Hook"), t("Return array"), t("Modules"), t("Operations"));
  $rows = array();
  foreach (element_children($form) as $key) {
    $row = array();
    $row[] = drupal_render($form[$key]['name']);
    $row[] = drupal_render($form[$key]['array']);
    $row[] = drupal_render($form[$key]['modules']);
    $row[] = drupal_render_children($form[$key]);
    $rows[] = $row;
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}
