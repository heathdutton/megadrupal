<?php

/**
 * @file
 * Settings.
 */

/**
 * Page callback: Form constructor for the settings.
 *
 *
 * @return array
 *   A system_settings_form array.
 *
 * @see role_execution_time_limit_menu()
 */
function role_execution_time_limit_settings_form($form, &$form_state) {
  $form["role_execution_time_limit_status"] = array(
    "#type" => "checkbox",
    "#title" => t("Enable"),
    "#default_value" => variable_get("role_execution_time_limit_status", 0),
  );

  $form['role_execution_time_limit_roles'] = array(
    "#type" => "fieldset",
    "#title" => t("Roles"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("Enter in seconds or 0 for unlimited, minimum is 30 seconds.") . "<br />" . t("Leave empty for default"),
  );

  $form['role_execution_time_limit_roles']['role_execution_time_limit_role_admin'] = array(
    '#type' => "textfield",
    '#title' => t("admin"),
    '#field_suffix' => "seconds",
    '#default_value' => variable_get('role_execution_time_limit_role_admin', ''),
  );

  foreach (user_roles() as $role) {

    $role_variable = role_execution_time_limit_var_name($role);

    $form['role_execution_time_limit_roles'][$role_variable] = array(
      '#type' => "textfield",
      '#title' => check_plain($role),
      '#field_suffix' => "seconds",
      '#default_value' => variable_get($role_variable, ''),
    );

  }

  $form['role_execution_time_limit_role_cli'] = array(
    '#type' => "textfield",
    '#title' => t("command line (drush)"),
    '#field_suffix' => "seconds",
    '#default_value' => variable_get('role_execution_time_limit_role_cli', ''),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for role_execution_time_limit_settings().
 */
function role_execution_time_limit_settings_form_validate($form, &$form_state) {
  $roles = user_roles();
  array_unshift($roles, "admin", "cli");

  foreach ($roles as $role) {
    $role_variable = role_execution_time_limit_var_name($role);

    if ($form_state['values'][$role_variable] || $form_state['values'][$role_variable] === "0") {
      if (!is_numeric($form_state['values'][$role_variable])) {
        form_set_error($role_variable, t("Only numbers is allowed."));
      }
      elseif ($form_state['values'][$role_variable] < 30 && $form_state['values'][$role_variable] !== "0") {
        form_set_error($role_variable, t("Minimum allowed is 30."));
      }
    }
  }
}
