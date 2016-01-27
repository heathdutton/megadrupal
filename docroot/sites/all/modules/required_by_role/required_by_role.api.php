<?php
/**
 * @file
 * Hooks provided by the Required by role module.
 */

/**
 * Implements hook_required_by_role_callback_alter.
 *
 * Returns a string this with name of the function to be used to work out
 * the #required property of a field.
 *
 * The function should return a boolean.
 */
function hook_required_by_role_callback_alter(&$callback) {
  $callback = 'example_required_callback';
}

/**
 * Example callback for hook_required_by_role_callback_alter.
 *
 * This callcack is intended to the actual work on working whether or not the
 * #required property of a field is TRUE or FALSE
 *
 * The callback takes as arguments, the account to be used (by default the
 * current user), the roles to matched against, the context provided by
 * hook_field_widget_form_alter and the $form_state
 *
 * This is an example on how to alter the account that is going to be tested
 * based on the form_id. Sometime would be useful to use $context to get extra
 * information about the environment.
 */
function example_required_callback($account, $roles, $context, $form_state) {

  if ($form_state['build_info']['form_id'] == 'user_profile_form') {
    $account = !empty($context['form']['#user']) ? $context['form']['#user'] : $account;
  }

  return _required_by_role_roles_intersect($account, $roles, $context, $form_state);

}
