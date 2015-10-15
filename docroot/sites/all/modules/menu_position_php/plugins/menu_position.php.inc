<?php
/**
 * @file
 * Provides the PHP rule plugin for the Menu Position module.
 */

/**
 * Checks value returning by user supplied PHP code.
 *
 * This is the condition callback that will be used when a rule is evaluated. If
 * the condition was not added to the rule, this callback will not be called.
 *
 * @param $variables
 *   An array containing each of the variables saved in the database necessary
 *   to evaluate this condition of the rule.
 * @return
 *   TRUE if condition applies successfully. Otherwise FALSE.
 */
function menu_position_php_menu_position_condition_php($variables) {
  // Grab the variables stored statically in the rule.
  $php = $variables['php'];

  if (!empty($php)) {
    // Use PHP code to check the visibility.
    ob_start();
    $php = trim($php);
    $apply_rule = eval($php);
    ob_end_clean();
  }

  return ($php && $apply_rule) ? TRUE : FALSE;
}

/**
 * Adds form elements for the PHP plugin to the rule configuration form.
 *
 * If this condition was active in the current rule, the plug-in variables will
 * be available in $form_state['#menu-position-rule']['conditions']['php'].
 *
 * It is the resposibility of this hook to add any necessary form validation and
 * submission handlers.
 *
 * @param $form
 *   A reference to the "add/edit rule" form array. New form elements should be
 *   added directly to this array.
 * @param $form_state
 *   A reference to the current form state.
 */
function menu_position_php_menu_position_rule_php_form(&$form, &$form_state) {
  // If this is an existing rule, load the variables stored in the rule for this plugin.
  $variables = !empty($form_state['#menu-position-rule']['conditions']['php']) ? $form_state['#menu-position-rule']['conditions']['php'] : array();

  // To ensure that the plugin's form elements are placed inside vertical tabs,
  // all elements should be placed inside a collapsed fielset inside the
  // $form['conditions'] array.
  $form['conditions']['php'] = array(
    '#type' => 'fieldset',
    '#title' => t('PHP code'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attached' => array(
        // Ensures a proper summary is added to its vertical tab.
        'js' => array(drupal_get_path('module', 'menu_position_php') . '/plugins/menu_position.php.js'),
    ),
  );
  $form['conditions']['php']['php'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP code'),
    '#default_value' => !empty($variables['php']) ? $variables['php'] : '',
    '#description' => t('Pages on which this PHP code returns <code>TRUE</code> (experts only). Do not use <?php ?> tags.'),
    '#weight' => -20,
  );

  // Add a submit handler.
  $form['#submit'][] = 'menu_position_php_menu_position_rule_php_form_submit';
}

/**
 * Prepares the plugin's variables to be stored in the rule.
 *
 * If the plugin's form elements indicate that the condition needs to be
 * included with the rule, the submit handler must, at the very least, set:
 * $form_state['conditions']['php'] = array(). Optionally, the plugin can add
 * to this array any static variables to be stored in the database with the rule
 * configuration.
 *
 * If, after this submit handler is run, the $form_state['conditions']['php']
 * variables array is not set, this plugin will not be added as a condition for
 * this rule.
 *
 * @param $form
 *   A reference to the "add/edit rule" form array.
 * @param $form_state
 *   A reference to the current form state, including submitted values.
 */
function menu_position_php_menu_position_rule_php_form_submit(&$form, &$form_state) {
  if (!empty($form_state['values']['php'])) {
    // Add this plugin's variables to the rule.
    $variables = array(
      'php' => $form_state['values']['php'],
    );
    $form_state['values']['conditions']['php'] = $variables;
  }
}
