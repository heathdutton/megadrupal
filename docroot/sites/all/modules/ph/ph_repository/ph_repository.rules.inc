<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function ph_repository_rules_action_info() {
  return array(
    'ph_repository_sync' => array(
      'label' => t('Synchronize a repository'),
      'group' => t('Project Hosting'),
      'parameter' => array(
        'repository' => array(
          'label' => t('Repository'),
          'type' => 'ph_repository',
        ),
      ),
      'provides' => array(
        'successful' => array(
          'label' => t('Repository sync successful'),
          'type' => 'boolean',
        ),
      ),
      'base' => 'ph_repository_rules_sync',
    ),
    'ph_repository_list_versions' => array(
      'label' => t('List repository versions'),
      'group' => t('Project Hosting'),
      'parameter' => array(
        'repository' => array(
          'label' => t('Repository'),
          'type' => 'ph_repository',
        ),
      ),
      'provides' => array(
        'versions' => array(
          'label' => t('List of versions'),
          'type' => 'list<ph_version>',
        ),
      ),
      'base' => 'ph_repository_rules_list_versions',
    ),
  );
}

/**
 * Synchronizes a repository.
 */
function ph_repository_rules_sync(PHRepository $repository, $element) {
  try {
    ph_repository_synchronize($repository);
    return array('successful' => TRUE);
  }
  catch (PHRepositoryBackendException $ex) {
    watchdog_exception('project hosting', $ex);
    throw new RulesEvaluationException('Unable to synchronize repository (ID: @rid).', array('@rid' => $repository->rid), $element);
  }
}

/**
 * Lists versions for a repository.
 */
function ph_repository_rules_list_versions(PHRepository $repository, $element) {
  try {
    $versions = ph_repository_list_versions($repository);
    return array('versions' => $versions);
  }
  catch (PHRepositoryBackendException $ex) {
    watchdog_exception('project hosting', $ex);
    throw new RulesEvaluationException('Unable to list versions from repository (ID: @rid).', array('@rid' => $repository->rid), $element);
  }
}
