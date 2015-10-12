<?php

/**
 * @file
 * Admin hooks and functions for the Project Source: GitHub module.
 */


/**
 * Admin form for the Project Source: GitHub module.
 */
function project_src_github_settings($form_state) {
  $form = array();

  // This
  $form['project_src_github_org'] = array(
    '#type' => 'textfield',
    '#title' => t('GitHub Organization'),
    '#description' => t('The name of the organization under which your custom Drupal extensions are released.'),
    '#default_value' => variable_get('project_src_github_org', ''),
  );

  $form['actions']['cache_clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear project cache'),
    '#weight' => 10,
    '#submit' => array('project_src_github_clear_cache_submit'),
  );

  return system_settings_form($form);
}


/**
 * Submit handler to clear Project Source: GitLab's cached projects/releases.
 */
function project_src_github_clear_cache_submit($form, &$form_state) {
  drupal_set_message(t('GitHub project cache data cleared successfully.'));
  project_src_github_clear_cache();
}
