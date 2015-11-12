<?php
/**
 * @file
 * Ctools access plugin for checking if a view returns results.
 */

/**
 * Plugins are described by creating a $plugin array which will
 * be used by the system that includes the file.
 */
$plugin = array(
  'title' => t('View: A view returns results'),
  'description' => t('Controls access by checking if the specified view returns a result'),
  'callback' => 'ctools_view_access_view_access_ctools_access_check',
  'settings form' => 'ctools_view_access_view_access_ctools_settings',
  'settings form submit' => 'ctools_view_access_view_access_ctools_settings_submit',
  'summary' => 'ctools_view_access_view_access_ctools_summary',
  'all contexts' => TRUE,
);

/**
 * Custom callback defined by 'callback' in the $plugin array.
 *
 * Check for access.
 */
function ctools_view_access_view_access_ctools_access_check($conf, $context) {
  // First parameter is the view's machine name.
  // Second parameter is the view's display name.
  $views_parameters = explode(':', $conf['view_instance']);

  // All next parameters are passed as arguments.
  if (isset($conf['view_arguments'])) {
    $view_arguments = explode('/', (string) $conf['view_arguments']);
    foreach ($view_arguments as $keyword) {
      if ($keyword) {
        // Add keyword as argument.
        $views_parameters[] = _ctools_view_access_get_argument_from_contexts($keyword, $context);
      }
    }
  }

  // Check if View with arguments returns a result.
  $result = call_user_func_array('views_get_view_result', $views_parameters);
  if (!empty($result)) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Settings form for the 'view_access' access plugin.
 */
function ctools_view_access_view_access_ctools_settings($form, &$form_state, $conf) {
  $view_list = views_get_all_views();
  $views_options = array();
  // Loop through each view adding each instance to the view options array
  foreach ($view_list as $view_parent_name => $view_object) {
    foreach ($view_object->display as $view_instance => $view_instance_name) {
      // Fixes Nodequeue Views with empty $view_object->human_name values.
      $name = ($view_object->human_name) ? $view_object->human_name : $view_object->name;
      $views_options[$view_parent_name . ':' . $view_instance_name->id] = $name . ' - ' . $view_instance;
    }
  }
  $view_default = !empty($form_state['values']['settings']['view_instance']) ? $form_state['values']['settings']['view_instance'] : $conf['view_instance'];

  $form['settings']['view_instance'] = array(
    '#type' => 'select',
    '#title' => t('View name - Display id'),
    '#description' => t('If the view selected returns results, access will return TRUE (block/pane will be visible)'),
    '#default_value' => $conf['view_instance'],
    '#options' => $views_options,
  );
  // Pass the labels so we an easily match them upon submission
  $form['settings']['view_instance_labels'] = array(
    '#type' => 'value',
    '#value' => $views_options,
  );
  $form['settings']['view_arguments'] = array(
    '#type' => 'textfield',
    '#title' => t('Arguments'),
    '#description' => t('Pass these arguments to the view. Separate arguments with a /. You can fill in an absolute value or a context by using the argument format %node.') . '<br/><br/>' . t('Available contexts are') . ': ' . _ctools_view_access_get_contexts($form_state),
    '#default_value' => $conf['view_arguments'],
    '#required' => FALSE,
  );

  return $form;
}

/**
 * Custom submit handler to save the pretty label in the conf array
 */
function ctools_view_access_view_access_ctools_settings_submit($form, &$form_state) {
  $form_state['values']['settings']['label'] = $form_state['values']['settings']['view_instance_labels'][$form_state['values']['settings']['view_instance']];
  unset($form_state['values']['settings']['view_instance_labels']);
}

/**
 * Provide a summary description
 */
function ctools_view_access_view_access_ctools_summary($conf, $context) {
  // Here we use the label value so its pretty
  return t('View @label has been populated', array('@label' => $conf['label']));
}

/**
 * Helper function that returns argument when context and keyword are passed.
 */
function _ctools_view_access_get_argument_from_contexts($keyword, $contexts) {
  if (!$keyword) {
    return FALSE;
  }

  // Make it possible to pass a single context.
  if (!is_array($contexts)) {
    $contexts = array($contexts);
  }

  // Loop contexts.
  foreach ($contexts as $context) {
    // Return argument if keyword exists.
    if ($context->keyword == str_replace('%', '', $keyword)) {
      // If the $context is an object (a real context).
      if (is_object($context->data) && isset($context->argument)) {
        // Return ID as argument.
        return $context->argument;
      }
      elseif (empty($context->argument) && isset($context->data)) {
        return $context->data;
      }
    }
  }

  // Return original keyword if no match was found.
  return $keyword;
}

/**
 * Helper function that returns available contexts.
 *
 * @return string
 *   HTML string with available contexts.
 */
function _ctools_view_access_get_contexts($form_state) {

  // Add a select-option for each available context. This is independent
  // of the view. So the context will be blindly passed as argument even if
  // the view cannot handle them.
  if (!empty($form_state['contexts'])) {
    $contexts = $form_state['contexts'];
  }

  // This is a little hacky, but needed for "visibility rules" in Panels.
  // Though we use "'all contexts' => TRUE" in the plugin settings above in this
  // file these contexts are not available if we use this plugin as "visibility
  // rule" in Panels. The Contexts are available in the display form. We test
  // if that is the case and if so we still have our contexts available.
  elseif (empty($contexts) && isset($form_state['display']) && isset($form_state['display']->context)) {
    $contexts = $form_state['display']->context;

    // The global %viewer : "Logged in user" is missing. Add a stub for it.
    $stub_context_loggedin_user = new stdClass();
    $stub_context_loggedin_user->keyword = "viewer";
    $stub_context_loggedin_user->identifier = "Logged in user";

    $contexts[] = $stub_context_loggedin_user;
  }

  // Build up the return value. It will be saved in $form['settings'].
  $context_options = "<ul>";
  foreach ($contexts as $context) {
    $context_options .= "<li>%" . $context->keyword . " (" . $context->identifier . ")" . "</li>";
  }
  $context_options .= "</ul>";

  return $context_options;
}
