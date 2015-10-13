<?php

/**
 * @file
 * Administrative settings for the Git Wrapper module.
 */

/**
 * Returns the path to the Git binary.
 *
 * If the path to the Git binary is not set, it is auto-discovered via the
 * \Symfony\Component\Process\ExecutableFinder class.
 *
 * @return string
 *   The path to the Git binary.
 */
function git_wrapper_get_binary() {
  $git_binary = variable_get('git_wrapper_binary', '');
  if (!$git_binary) {
    try {
      $wrapper = git_wrapper();
      $git_binary = $wrapper->getGitBinary();
    }
    catch (Exception $e) {
      $git_binary = '';
    }
  }
  return $git_binary;
}

/**
 * Provides a GUI for all the options provided by the Git Wrapper library.
 *
 * @see git_wrapper_git_settings_form_validate()
 *
 * @ingroup forms
 */
function git_wrapper_git_settings_form($form, &$form_state) {

  $form['git_wrapper_binary'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Git binary'),
    '#description' => t('The absolute or relative path to the Git binary.'),
    '#default_value' => git_wrapper_get_binary(),
  );

  $form['git_wrapper_version']['version'] = array();
  $form['git_wrapper_version']['#after_build'] = array('git_wrapper_version_build');

  $form['git_wrapper_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#description' => t('The timeout of the Git command in seconds.'),
    '#default_value' => variable_get('git_wrapper_timeout', 60),
    '#size' => 4,
  );

  $form['git_wrapper_connect_via_ssh'] = array(
    '#type' => 'checkbox',
    '#title' => t('Connect via SSH'),
    '#default_value' => variable_get('git_wrapper_connect_via_ssh', 0),
    '#description' => t('Connect to the Git repository via SSH.'),
  );

  $form['git_wrapper_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to private key'),
    '#default_value' => variable_get('git_wrapper_private_key', ''),
    '#description' => t('Optionally specify a path to a private key used to authenticate against the remote repository.'),
    '#size' => 90,
    '#states' => array(
      'visible' => array(
        ':input[name="git_wrapper_connect_via_ssh"]' => array('checked' => TRUE),
      )
    ),
  );

  $form['git_wrapper_ssh_port'] = array(
    '#type' => 'textfield',
    '#title' => t('SSH Port'),
    '#default_value' => variable_get('git_wrapper_ssh_port', 22),
    '#description' => t('The port that the SSH server hosting the Git repository listens on.'),
    '#size' => 6,
    '#states' => array(
      'visible' => array(
        ':input[name="git_wrapper_connect_via_ssh"]' => array('checked' => TRUE),
      )
    ),
  );

  $form['git_wrapper_ssh_wrapper'] = array(
    '#type' => 'textfield',
    '#title' => t('SSH wrapper script'),
    '#default_value' => variable_get('git_wrapper_ssh_wrapper', '<default>'),
    '#description' => t('The absolute or relative path to the Git SSH wrapper script.<br>A value of <code>&lt;default&gt;</code> resolves to the default SSH wrapper script at <code>@path</code>', array('@path' => git_wrapper_default_ssh_wrapper())),
    '#size' => 90,
    '#states' => array(
      'visible' => array(
        ':input[name="git_wrapper_connect_via_ssh"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['git_wrapper_set_envvars'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set environment variables'),
    '#default_value' => variable_get('git_wrapper_set_envvars', 0),
    '#description' => t('Set environment variables that are defined for the scope of the Git process.'),
  );

  $form['git_wrapper_envvars'] = array(
    '#type' => 'textarea',
    '#title' => t('Environment variables'),
    '#default_value' => variable_get('git_wrapper_envvars', ''),
    '#description' => t('Enter one variable per line in the format variable|value, e.g. HOME|/var/www.'),
    '#states' => array(
      'visible' => array(
        ':input[name="git_wrapper_set_envvars"]' => array('checked' => TRUE),
      )
    ),
  );

  $form['#validate'][] = 'git_wrapper_git_settings_form_validate';

  return system_settings_form($form);
}

/**
 * Form validation handler for git_wrapper_git_settings_form().
 */
function git_wrapper_git_settings_form_validate($form, &$form_state) {
  $values = &$form_state['values'];
  if ($values['git_wrapper_connect_via_ssh'] && !$values['git_wrapper_private_key']) {
    form_set_error('git_wrapper_private_key', t('Private key is required'));
  }
}

/**
 * #after_build callback to output the Git version.
 *
 * @see http://drupal.org/project/imagemagick
 * @see _imagemagick_build_version()
 */
function git_wrapper_version_build($element, &$form_state) {
  // Do not attempt to output version information when the form is submitted.
  if ($form_state['process_input']) {
    return $element;
  }

  try {
    $version = git_wrapper()->version();
    $markup = '<pre>' . check_plain($version) . '</pre>';
  }
  catch (Exception $e) {
    $error = $e->getMessage();
    $markup = '<p class="error">' . check_plain($error) . '</p>';
  }

  $element['version'] = array(
    '#type' => 'item',
    '#title' => t('Git version'),
    '#markup' => $markup,
  );

  return $element;
}
