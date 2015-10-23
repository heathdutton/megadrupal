<?php
/**
 * @file
 * Creates forms for the ghpages module
 * Returns an administration form
 */

/**
 * Form constructor for the admin settings form.
 */
function ghpages_admin_settings($form, &$form_state) {
  $form['form_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Git Repo Details'),
    '#description' => t('Fill in these details to enable users to push to a public or private repo.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['form_defaults']['ghpages_username'] = array(
    '#type' => 'textfield',
    '#title' => t('GitHub Username'),
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => variable_get('ghpages_username', ''),
  );
  $form['form_defaults']['ghpages_pass'] = array(
    '#type' => 'password',
    '#title' => t('GitHub Password'),
    '#description' => t('Caution: This will be stored in plain text in the database'),
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => variable_get('ghpages_pass', ''),
  );
  $form['form_defaults']['ghpages_repo'] = array(
    '#type' => 'textfield',
    '#title' => t('Git Repository'),
    '#description' => t("This is the bare repo you'll be pushing to. Please ensure it has a gh-pages branch."),
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => variable_get('ghpages_repo', ''),
  );
  return system_settings_form($form);
}


/**
 * Validation for the admin form.
 */
function ghpages_admin_settings_validate($form, &$form_state) {
  if (!preg_match('/^http(s)?:\/\//i',
      $form_state['values']['ghpages_repo'])) {
    form_set_error('ghpages_repo', t('Please use the https:// URL form.'));
  }
}
