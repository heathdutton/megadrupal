<?php
/**
 * @file
 * Git administrative UI implementation.
 */

/**
 * Alters repository settings form.
 */
function _ph_repository_git_form_ph_repository_admin_settings_alter(&$form, &$form_state) {
  $form['git'] = array(
    '#type' => 'fieldset',
    '#title' => t('Git repository'),
  );
  $form['git']['ph_repository_git_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Git executable'),
    '#default_value' => variable_get('ph_repository_git_path', 'git'),
    '#description' => t('Specify the path to the Git binary executable, e.g. %git. If this is left blank, the Git backend will try to find a git executable based on environment executable search paths.', array('%git' => '/usr/bin/git')),
    '#element_validate' => array('ph_repository_git_admin_validate_git_path'),
  );
  $form['git']['active_executable'] = array(
    '#type' => 'item',
    '#title' => t('Active Git executable')
  );
  if ($path = ph_repository_git_find_executable()) {
    $form['git']['active_executable']['#markup'] = check_plain($path);
  }
  else {
    $form['git']['active_executable']['#markup'] = '<em>' . t('There is no currently active Git executable. Please set the path above manually or configure environment executable search paths to include the Git executable.') . '</em>';
  }

  $form['#submit'][] = 'ph_repository_git_admin_reset_executable';
}

/**
 * Validates the Git executable path.
 */
function ph_repository_git_admin_validate_git_path($element, &$form_state) {
  $path = $element['#value'];
  if ($path && !(file_exists($path) && is_executable($path))) {
    form_error($element, t('The specified path does not exist or is not executable.'));
  }
}

/**
 * Resets the cache for the executable path.
 */
function ph_repository_git_admin_reset_executable() {
  ph_repository_git_find_executable(TRUE);
}
