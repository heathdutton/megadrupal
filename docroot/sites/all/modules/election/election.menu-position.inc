<?php
/**
 * @file
 * Provides the Election rule plugin for the Menu Position module.
 */

/**
 * Checks if the election is of a certain type.
 *
 * @param array $variables
 *   An array containing each of the variables saved in the database necessary
 *   to evaluate this condition of the rule.
 *
 * @return bool
 *   TRUE if condition applies successfully. Otherwise FALSE.
 */
function election_menu_position_condition_election($variables) {
  $args = arg();
  if ($args[0] == 'election' && !empty($args[1]) && is_numeric($args[1])) {
    $election = election_load($args[1]);
    $type = $election->type;
  }
  if (empty($election)) {
    return FALSE;
  }
  return (in_array($type, $variables['election_type']));
}

/**
 * Adds form elements for the "election" plugin to the rule configuration form.
 *
 * @param array $form
 *   A reference to the "add/edit rule" form array. New form elements should be
 *   added directly to this array.
 * @param array $form_state
 *   A reference to the current form state.
 */
function election_menu_position_rule_election_form(&$form, &$form_state) {
  $variables = array();

  $form['conditions']['election'] = array(
    '#type' => 'fieldset',
    '#title' => t('Elections'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $type_options = array();
  $types = election_types();
  foreach ($types as $type_machine_name => $type) {
    $type_options[$type_machine_name] = drupal_ucfirst($type['name']);
  }

  $form['conditions']['election']['election_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Election types'),
    '#default_value' => !empty($variables['election_type']) ? $variables['election_type'] : array(),
    '#options' => $type_options,
    '#description' => t('Apply this rule only for elections of the given type(s).'),
    '#weight' => -20,
  );

  // Add a submit handler.
  $form['#submit'][] = 'election_menu_position_rule_election_form_submit';
}

/**
 * Prepares the "election_type" variables to be stored in the rule.
 *
 * @param array $form
 *   A reference to the "add/edit rule" form array.
 * @param array $form_state
 *   A reference to the current form state, including submitted values.
 */
function election_menu_position_rule_election_form_submit(&$form, &$form_state) {
  // The user has added our plugin's form elements as a condition for the rule.
  if (!empty($form_state['values']['election_type'])) {
    // Remove any 0 valued options.
    foreach ($form_state['values']['election_type'] as $key => $value) {
      if ($value === 0) {
        unset($form_state['values']['election_type'][$key]);
      }
    }
    // Determine if any checkboxes were on.
    if (!empty($form_state['values']['election_type'])) {
      // Add this plugin's variables to the rule.
      $variables = array(
        'election_type' => $form_state['values']['election_type'],
      );
      $form_state['values']['conditions']['election'] = $variables;
    }
  }
}
