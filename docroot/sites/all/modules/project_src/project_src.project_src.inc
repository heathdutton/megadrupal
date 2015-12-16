<?php

/**
 * @file
 * Hooks invoked by the Project Source API.
 */


/**
 * Implements hook_project_src_terms().
 *
 * Ensures that the project type term is populated for the given project (this
 * is the value used by drush to determine installation location).
 */
function project_src_project_src_terms($term_type, $short_name, $api_version, $project) {
  $terms = array();

  if ($term_type == 'project') {
    $settings = project_src_get_project_settings($short_name);
    if (isset($settings['type'])) {
      $terms[] = array(
        'Projects',
        _project_src_type_to_string($settings['type'], 'term'),
      );
    }
  }

  return $terms;
}


/**
 * Implements hook_project_src_settings().
 */
function project_src_project_src_settings() {
  // Builds the options array for project type.
  $type_options = _project_src_type_to_string();
  foreach ($type_options as $key => &$option) {
    $type_options[$key] = $option['title'];
  }

  // We can't know what major versions exist, so we provide sane defaults.
  $major_version_options = array(
    '1' => 'N.x-1.x',
    '2' => 'N.x-2.x',
    '3' => 'N.x-3.x',
    '4' => 'N.x-4.x',
    '5' => 'N.x-5.x',
    '6' => 'N.x-6.x',
    '7' => 'N.x-7.x',
    '8' => 'N.x-8.x',
    '9' => 'N.x-9.x',
  );

  // Configuration for the type of Drupal extension of a given project.
  $settings['type'] = array(
    'scope' => 'project',
    'element' => array(
      '#title' => t('Type'),
      '#type' => 'select',
      '#options' => $type_options,
      '#required' => TRUE,
      '#default_value' => 'project_module',
    ),
    '#weight' => -10,
  );

  // Configuration for whether or not the project should be listed/available.
  $settings['hidden'] = array(
    'scope' => 'project',
    'element' => array(
      '#title' => t('Hidden'),
      '#type' => 'checkbox',
      '#default_value' => FALSE,
    ),
    '#weight' => 0,
  );

  // Configuration for project status (e.g. unsupported, revoked, etc).
  $settings['status'] = array(
    'scope' => 'api_version',
    'element' => array(
      '#title' => t('Status'),
      '#type' => 'select',
      '#options' => array(
        'published' => t('Published'),
        'unpublished' => t('Unpublished'),
        'revoked' => t('Revoked'),
        'unsupported' => t('Unsupported'),
        'insecure' => t('Insecure'),
      ),
    ),
    '#weight' => -1,
  );

  // Configuration for project supported major versions.
  $settings['supported_majors'] = array(
    'scope' => 'api_version',
    'element' => array(
      '#title' => t('Supported major versions'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $major_version_options,
    ),
  );

  // Configuration for project recommended major version.
  $settings['recommended_major'] = array(
    'scope' => 'api_version',
    'element' => array(
      '#title' => t('Recommended major version'),
      '#type' => 'select',
      '#options' => $major_version_options,
      '#empty_option' => t('- None -'),
    ),
  );

  return $settings;
}
