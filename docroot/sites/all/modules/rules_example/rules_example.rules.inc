<?php

/**
 * @file
 * Example Rules code: actions, conditions and events.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function rules_example_rules_action_info() {
  $actions = array(
    // The base-level keys in the array are the machine names for the actions,
    // and by default also the function name for the action callback. Prefix
    // with your module name to avoid name conflicts.
    'rules_example_action_hello_world' => array(
      'label' => t('Print Hello world on the page'), // Name displayed to admins
      'group' => t('Rules examples'), // Used for grouping actions in select lists
    ),

    // Further actions are just declared by appending the associative array with
    // more entries.
    'rules_example_action_hello_user' => array(
      'label' => t('Say hello to a selected user'),
      'group' => t('Rules examples'),
      // When declaring parameters, site builders will by default get a form to
      // select fitting data. In this case, we require the site builder to
      // select a user object. The 'type' could be any data type known to Rules,
      // which means any entity type known to Entity API or any additional data
      // type declared by hook_rules_data_info – by default including things
      // like 'integer', 'text' and more. See
      'parameter' => array(
        // Each parameter is keyed with an internally used machine name.
        'account' => array(
          'type' => 'user',
          'label' => t('User to say hello to'),
          // If we were to make changes to the parameter object, whe should set
          // the 'save' property to TRUE. This makes Rules handle the saving in
          // a more optimized way than if each action saved the objects.
          // 'save' => TRUE,
        ),
      ),
    ),
  );

  // Here additional actions are provided, assuming that the Views module is
  // enabled. You can of course implement other logic in your own module.
  if (module_exists('views')) {
    $actions['rules_example_action_get_view_results'] = array(
      'label' => t('Get the number of results in a view'),
      'group' => t('Rules examples'),
      'parameter' => array(
        'view_name' => array(
          'type' => 'text',
          'label' => t('The view to use to load the number of results'),
          // If the options list property is set, its value will be used for
          // building a select list to show to the site builder.
          'options list' => 'rules_example_views_list',
          // Restricting input method to 'input' (and not 'selector') hides
          // the 'data selection' button.
          // 'restriction' => 'input',
        ),
      ),
      // The 'provides' property tells Rules that this action provides new data.
      // The data is described almost identically to how parameters are
      // described, with the exception of input properties.
      'provides' => array(
        'number_of_results' => array(
          'type' => 'integer',
          'label' => t('Number of view result'),
        ),
      ),
    );
  }

  // Don't forget to return the array of action information!
  return $actions;
}

/**
 * Implements hook_rules_condition_info().
 *
 * Declares any meta-data about conditions for Rules in an associative, nested
 * array. Works very similar to the action declarations, but conditions cannot
 * provide any new parameters and only return TRUE or FALSE. See also
 * hook_rules_condition_info in the rules.api.php file, and the online
 * documentation at http://drupal.org/node/878928.
 */
function rules_example_rules_condition_info() {
  $conditions = array();

  if (module_exists('views')) {
    // Conditions, like actions, are described by an array with keys
    // representing the machine name of the condition (which also by default is
    // the name of the callback function).
    $conditions['rules_example_condition_view_base_table'] = array(
      'label' => t('View has base table'),
      'group' => t('Rules examples'),
      // Parameters are described identically to how they work for actions.
      'parameter' => array(
        'view_name' => array(
          'type' => 'text',
          'label' => t('View to check'),
          'options list' => 'rules_example_views_list',
        ),
        // You can provide multiple parameters, no worries.
        'base_table' => array(
          'type' => 'text',
          'label' => t('The machine name for the base table'),
          'restriction' => 'input',
        ),
      ),
    );
  }

  return $conditions;
}

/**
 * Implements hook_rules_event_info().
 *
 * Declares any meta-data about events for Rules in an associative nested
 * array. See also hook_rules_event_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/884554.
 */
function rules_example_rules_event_info() {
  $events = array();

  if (module_exists('views')) {
    // Events are, like the other Rules plugins in this example, keyed by their
    // machine name – by default also the callback function. Prefix with your
    // module name to avoid name conflicts.
    $events['rules_example_event_view_render'] = array(
      'label' => t('A view is going to be rendered'),
      'group' => t('Rules examples'),
      // If the event provides any parameters to Rules, these are described with
      // the 'variables' property.
      'variables' => array(
        // Each provided variable is keyed by a given machine name, and work
        // very similar to how parameters and provided variables are desribed in
        // conditions and actions.
        'view_name' => array(
          'type' => 'text',
          'label' => t('view name'),
        ),
      ),
    );
  }

  return $events;
}

/**
 * The action function for 'rules_example_action_hello_world'.
 */
function rules_example_action_hello_world() {
  drupal_set_message(t('Hello world!'));
}

/**
 * The action function for the 'rules_example_action_hello_user'.
 *
 * The $accout parameter is the user object sent into this action, selected in
 * the Rules configuration interface.
 * @param <object> $account
 *   The user object to work with. Selected by the site administrator when the
 *   action is configured.
 */
function rules_example_action_hello_user($account) {
  drupal_set_message(t('Hello @username!',
    array('@username' => $account->name)));
}

/**
 * The action function for the 'rules_example_action_get_view_results'.
 * @param <string> $view_name
 *   The view machine name. Selected by the site administrator when the action
 *   is configured.
 * @return <array>
 *   An array of all data objects that Rules should store – in this case the new
 *   data object 'number_of_results'. Could also include parameters that the
 *   action has changed, and should be saved by Rules.
 */
function rules_example_action_get_view_results($view_name) {
  $results = views_get_view_result($view_name);

  // Each returned value in the array must be keyed with the exact machine name
  // specified in the action's description in hook_rules_action_info.
  return array(
    'number_of_results' => count($results),
  );
}

/**
 * Helper function to build a select list over all views on the site.
 *
 * @return <array>
 *   An array with all selectable options, keyed with the 'actual' value (as
 *   used by Rules) and with values being the names displayed in the select
 *   list.
 */
function rules_example_views_list() {
  $selectable_views = array();
  foreach (views_get_all_views() as $view_name => $view_object) {
    // If the array has double indeces – here both the $view_object->base_table
    // and the $view_name – then Rules will use the first index to group the
    // list content.
    $selectable_views[$view_object->base_table][$view_name] =
      (isset($view_object->human_name))
      ? $view_object->human_name
      : $view_name;
  }

  // Don't forget to return the array.
  return $selectable_views;
}

/**
 * The callback for the condition 'rules_example_condition_view_base_table'.
 * @param <string> $view_name
 * @param <string> $base_table
 * @return <boolean>
 *   TRUE or FALSE depending on if the given view has the given base table or
 *   not.
 */
function rules_example_condition_view_base_table($view_name, $base_table) {
  // Note that the order of the parameters is the same as in the declaration of
  // the condition. Use the same variable names as in the declaration to avoid
  // confusion.

  $view = views_get_view($view_name);

  // All conditions return TRUE or FALSE.
  return ($view->base_table == $base_table);
}
