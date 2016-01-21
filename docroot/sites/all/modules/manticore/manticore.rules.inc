<?php

/**
 * Implements hook_rules_action_info().
 */
function manticore_rules_action_info() {
  // Action to add a contact to Manticore.
  $actions['manticore_rules_contact'] = array(
    'label' => t('Add a contact to Manticore'),
    'group' => t('Manticore'),
    'access callback' => 'manticore_rules_access',
  );
  // Provide the parameters to the Manticore action.
  foreach (manticore_availablefields() as $name => $type) {
    $actions['manticore_rules_contact']['parameter'][$name] = array(
      'type' => $type,
      'label' => $name,
      'allow null' => TRUE,
      'optional' => TRUE,
    );
  }
  return $actions;
}

/**
 * Rules Action callback; Adds a contact to Manticore.
 */
function manticore_rules_contact() {
  // Match the parameters to what was passed in through the Rules.
  $parameters = manticore_availablefields();
  $args = func_get_args();
  $i = 0;
  foreach ($parameters as $name => $type) {
    if (isset($args[$i])) {
      $parameters[$name] = trim($args[$i]);
    }
    else {
      unset($parameters[$name]);
    }
    $i++;
  }

  // Filter out any empty arguments.
  $parameters = array_filter($parameters);

  // Make sure at least the email was provided.
  if (isset($parameters['EmailAddress'])) {
    manticore_call($parameters);
  }
}

/**
 * Rules callback; Determines whether or not the user has access to use the rule.
 */
function manticore_rules_access() {
  return user_access('administer manticore');
}

