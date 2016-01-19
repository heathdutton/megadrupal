<?php

function openpublic_workflow_access_apps_app_info() {
  return array(
    'configure form' => 'openpublic_workflow_access_configure_form',
    'status callback' => 'openpublic_workflow_access_status',
  );
}

/**
 * Provides configuration form. Needed to show hook_status below.
 */
function openpublic_workflow_access_configure_form() {
  $form = array();
  return system_settings_form($form);
}

/**
 * Implements pseudo hook_status().
 */
function openpublic_workflow_access_status() {
  // Setup the status table
  $status = array(
    'title' => t('Status'),
    'headers' => array(t('severity'), t('title'), t('description'), t('action')),
    'items' => array(),
  );

  $status['items']['workbench_access'] = array(
    'title' => 'Workbench Access',
    'action' => array(
      l(t("Administer settings"), "admin/config/workbench/access/settings",  array('query'  => drupal_get_destination())),
    ),
    'description' => t("Workbench access seems to be configured."),
    'severity' => REQUIREMENT_OK,
  );

  // Get the user options.
  $options = workbench_access_active_options();

  if (empty($options) && !variable_get('workbench_access', FALSE)) {
    $status['items']['workbench_access']['description'] = workbench_access_configuration_needed_message();
    $status['items']['workbench_access']['severity'] = REQUIREMENT_ERROR;
  }
  else {
    $status['items']['users'] = array(
      'title' => 'Users',
      'action' => array(l("Change users for sections", "admin/config/workbench/access",  array('query'  => drupal_get_destination()))),
      'description' => t("Administer which users can edit which sections."),
      'severity' => REQUIREMENT_INFO,
    );
    $status['items']['roles'] = array(
      'title' => 'Roles',
      'action' => array(
        l(t("Change roles for sections"), "admin/config/workbench/access/roles",  array('query'  => drupal_get_destination()))
      ),
      'description' => t("Administer which roles can edit which sections."),
      'severity' => REQUIREMENT_INFO,
    );
    $status['items']['sections'] = array(
      'title' => 'Sections',
      'action' => array(
        l(t("Change editors for sections"), "admin/config/workbench/access/sections",  array('query'  => drupal_get_destination())),
      ),
      'description' => t("Administer your sections."),
      'severity' => REQUIREMENT_INFO,
    );
  }

  return $status;
}
