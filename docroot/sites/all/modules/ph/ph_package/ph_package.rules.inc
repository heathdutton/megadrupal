<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_data_info().
 */
function ph_package_rules_data_info() {
  return array(
    'ph_package_directory' => array(
      'label' => t('package directory'),
      'parent' => 'text',
    ),
    'ph_package_metadata' => array(
      'label' => t('package metadata'),
      'property info' => array(
        'version' => array(
          'label' => t('Version'),
          'type' => 'ph_version',
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'project' => array(
          'label' => t('Project machine name'),
          'type' => 'text',
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'project_status_url' => array(
          'label' => t('Project update status URL'),
          'type' => 'uri',
          'setter callback' => 'entity_property_verbatim_set',
        ),
        'date' => array(
          'label' => t('Timestamp'),
          'type' => 'date',
          'setter callback' => 'entity_property_verbatim_set',
        )
      ),
      'creation callback' => 'rules_action_data_create_array',
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function ph_package_rules_action_info() {
  return array(
    'ph_package_export' => array(
      'label' => t('Export a repository version'),
      'group' => t('Project Hosting'),
      'parameter' => array(
        'repository' => array(
          'label' => t('Repository'),
          'type' => 'ph_repository',
        ),
        'version' => array(
          'label' => t('Version'),
          'type' => 'ph_version',
        ),
        'version_since' => array(
          'label' => t('Base version to compute detailed version on'),
          'type' => 'ph_version',
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
      ),
      'provides' => array(
        'directory' => array(
          'label' => t('Package directory'),
          'type' => 'ph_package_directory',
        ),
        'detailed_version' => array(
          'label' => t('Detailed version'),
          'type' => 'ph_version',
        ),
      ),
      'base' => 'ph_package_rules_export',
    ),
    'ph_package_stamp' => array(
      'label' => t('Stamp a package directory'),
      'group' => t('Project Hosting'),
      'parameter' => array(
        'repository' => array(
          'label' => t('Directory'),
          'type' => 'ph_package_directory',
        ),
        'metadata' => array(
          'label' => t('Package metadata'),
          'type' => 'ph_package_metadata',
        ),
      ),
    ),
    'ph_package_archive' => array(
      'label' => t('Archive a package directory'),
      'group' => t('Project Hosting'),
      'parameter' => array(
        'repository' => array(
          'label' => t('Directory'),
          'type' => 'ph_package_directory',
        ),
        'metadata' => array(
          'label' => t('Package metadata'),
          'type' => 'ph_package_metadata',
        ),
        'extension' => array(
          'label' => t('Archive extension'),
          'type' => 'text',
          'restriction' => 'input',
          'options list' => 'ph_package_extension_options',
        ),
      ),
      'provides' => array(
        'archive' => array(
          'label' => t('Package archive'),
          'type' => 'file',
        ),
      ),
      'base' => 'ph_package_rules_archive',
    ),
  );
}

/**
 * Exports a package for Rules.
 */
function ph_package_rules_export($repository, $version, $version_since = NULL, $element) {
  try {
    $detailed_version = $version;
    $directory = ph_package_export($repository, $detailed_version, $version_since);
    return compact('directory', 'detailed_version');
  }
  catch (PHRepositoryBackendException $ex) {
    watchdog_exception('project hosting', $ex);
    throw new RulesEvaluationException('Unable to export repository (ID: @rid, version: @version).', array('@rid' => $repository->rid, '@version' => ph_version_format($version)), $element);
  }
}

/**
 * Archives a package directory for Rules.
 */
function ph_package_rules_archive($directory, $metadata, $extension) {
  $file = ph_package_archive($directory, $metadata, $extension);
  return array('archive' => $file);
}
