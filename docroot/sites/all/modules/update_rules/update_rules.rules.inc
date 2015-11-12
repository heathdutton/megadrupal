<?php
/**
 * @file
 * update_rules.rules.inc
 */

/**
 * Implements hook_rules_event_info().
 */
function update_rules_rules_event_info() {
  $info = array();

  $defaults = array(
    'group'     => t('Update Rules'),
    'variables' => array(
      'project_name' => array(
        'type'  => 'text',
        'label' => t('Machine name of the project'),
        'description' => t('Example: ctools'),
      ),
      'project_title' => array(
        'type'  => 'text',
        'label' => t('Full title of the project'),
        'description' => t('Example: Chaos tool suite (ctools)'),
      ),
      'current_release' => array(
        'type'  => 'text',
        'label' => t('Current release'),
        'description' => t('Example: 7.x-1.2'),
      ),
      'recommended_release' => array(
        'type'  => 'text',
        'label' => t('Recommended release'),
        'description' => t('Example: 7.x-1.3'),
      ),
      'release_link' => array(
        'type'  => 'text',
        'label' => t('Release link for recommended release'),
        'description' => t('Example: https://www.drupal.org/node/1960424'),
      ),
      'release_type' => array(
        'type'  => 'list<text>',
        'label' => t('Release type terms for recommended release'),
        'description' => t('Example: "Security update", "Bug fixes"'),
        'options list' => 'update_rules_release_types',
      ),
      'status' => array(
        'type'  => 'integer',
        'label' => t('Status of the project'),
        'options list' => 'update_rules_status_labels',
      ),
      'host' => array(
        'type'  => 'text',
        'label' => t('Host name'),
        'description' => t('As determined by gethostname() or php_uname(\'n\') - Example: web123.myserver.com'),
      ),
      'guid' => array(
        'type'  => 'text',
        'label' => t('Unique identifier for this release'),
        'description' => t('A unique identifier for this project+release combination - Example: 4dc384ba-9df1-4d65-b594-5836498005ce'),
      ),
    ),
  );

  $info['update_rules_after_checking_all'] = $defaults;
  $info['update_rules_after_checking_all']['label'] = t('After checking a project\'s update status');
  $info['update_rules_after_checking_all']['help'] = t('Update module has finished checking a for project updates.');

  // Sure the user could just add a condition to the rule to exclude projects
  // with a current status, but rather than invoke the rule all of those times
  // it'd be better to let the user choose to only process projects that need updates
  $info['update_rules_after_checking_exclude_current'] = $defaults;
  $info['update_rules_after_checking_exclude_current']['label'] = t('After detecting that a project is not current');
  $info['update_rules_after_checking_exclude_current']['help'] = t('Update module has finished checking a project updates, and the project is not current');

  return $info;
}

/**
 * Implements hook_rules_action_info().
 */
function update_rules_rules_action_info() {
  $info = array();

  $info['update_rules_process_once'] = array(
    'group'           => t('Update Rules'),
    'label'           => t('Only process this release once (must use with "Release has already been processed" condition)'),
    'named parameter' => FALSE,
    'parameter'       => array(
      'group_identifier' => array(
        'type'          => 'integer',
        'label'         => t('Group identifier'),
        'description'   => t('The "Release has already been processed" condition will check if a release has been processed based on this identifier. This allows you to have multiple rules that process updates, where each rule can store its own "already processed" status for each project. Rules may also share the same "already processed" data by using the same identifier.'),
        'optional'      => FALSE,
        'options list'  => 'update_rules_already_processed_group_ids',
        'default value' => 1,
        'restriction'   => 'input',
      ),
      'project_name' => array(
        'type'          => 'text',
        'label'         => t('Machine name of the project'),
        'description'   => t('Example: ctools'),
        'optional'      => FALSE,
        'default value' => '[project-name:value]',
      ),
      'recommended_release' => array(
        'type'          => 'text',
        'label'         => t('Recommended release'),
        'description'   => t('Example: 7.x-1.3'),
        'optional'      => FALSE,
        'default value' => '[recommended-release:value]',
      ),
    ),
  );

  return $info;
}

/**
 * Implements hook_rules_condition_info().
 */
function update_rules_rules_condition_info() {
  $info = array();

  $info['update_rules_already_processed'] = array(
    'group'           => t('Update Rules'),
    'label'           => t('Release has already been processed'),
    'parameter'       => array(
      'group_identifier' => array(
        'type'          => 'integer',
        'label'         => t('Group identifier'),
        'description'   => t('Which group identifier would you like to check for already-processed statuses?'),
        'optional'      => FALSE,
        'options list'  => 'update_rules_already_processed_group_ids',
        'default value' => 1,
        'restriction'   => 'input',
      ),
      'project_name' => array(
        'type'          => 'text',
        'label'         => t('Machine name of the project'),
        'description'   => t('Example: ctools'),
        'optional'      => FALSE,
        'default value' => '[project-name:value]',
      ),
      'recommended_release' => array(
        'type'          => 'text',
        'label'         => t('Recommended release'),
        'description'   => t('Example: 7.x-1.3'),
        'optional'      => FALSE,
        'default value' => '[recommended-release:value]',
      ),
    ),
  );

  return $info;
}
