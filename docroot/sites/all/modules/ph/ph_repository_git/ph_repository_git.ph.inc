<?php
/**
 * @file
 * Git repository backend declaration.
 */

/**
 * Implements hook_ph_repository_info().
 */
function ph_repository_git_ph_repository_info() {
  return array(
    'git' => array(
      'label' => t('Git', array(), array('context' => 'repository backend')),
      'backend class' => variable_get('ph_repository_git_backend_class', 'PHRepositoryGitBackend'),
      'field_config' => array(
        array(
          'field_name' => 'ph_repository_git_url',
          'type' => 'ph_repository_git_url',
        ),
      ),
      'field_instance' => array(
        array(
          'field_name' => 'ph_repository_git_url',
          'label' => t('Git repository URL'),
          'required' => TRUE,
          'description' => t('Specify the repository URL suitable for clone and sync.'),
        ),
      ),
    ),
  );
}
