<?php

/**
 * @file
 * comstack_pm_report.entity.inc
 */

/**
 * Implements hook_entity_info().
 */
function comstack_pm_report_entity_info() {
  $entities = array();

  $entities['comstack_pm_report'] = array(
    'label' => t('Comstack PM Report'),
    'plural label' => t('Comstack PM Reports'),
    // Don't use the default DrupalDefaultEntityController class.
    'controller class' => 'EntityAPIController',
    'entity class' => 'Entity',
    'base table' => 'comstack_pm_report',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'cspmr_id',
    ),
    'module' => 'comstack_pm_report',
    'entity cache' => module_exists('entitycache'),
    'metatags' => FALSE,
    'access callback' => 'comstack_pm_report_entity_access',
  );

  return $entities;
}

/**
 * Implements hook_entity_property_info_alter().
 */
function comstack_pm_report_entity_property_info_alter(&$info) {
  $report_properties = &$info['comstack_pm_report']['properties'];

  $report_properties['cspmr_id'] = array(
    'label' => t("Report ID"),
    'type' => 'integer',
    'description' => t("The unique ID of the report."),
    'schema field' => 'cspmr_id',
  );
  $report_properties['uid'] = array(
    'label' => t('Reporter'),
    'type' => 'user',
    'description' => t('The creator of the report.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer comstack content',
    'required' => TRUE,
    'schema field' => 'uid',
  );
  $report_properties['created'] = array(
    'label' => t('Report created'),
    'type' => 'date',
    'description' => t('The date the report was created.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer comstack content',
    'schema field' => 'created',
  );
  $report_properties['updated'] = array(
    'label' => t('Report updated'),
    'type' => 'date',
    'description' => t('The date the report was most recently updated.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer comstack content',
    'schema field' => 'updated',
  );
  $report_properties['dismissed'] = array(
    'label' => t('Dismissed'),
    'type' => 'boolean',
    'description' => t('Whether or not the report has been dismissed.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer comstack content',
    'schema field' => 'dismissed',
  );
}

/**
 * Load a report.
 */
function comstack_pm_report_load($report_id, $uid = NULL, $reset = FALSE) {
  $conversations = comstack_pm_report_load_multiple(array($report_id), $uid, array(), $reset);
  return reset($conversations);
}

/**
 * Load a series of reports.
 */
function comstack_pm_report_load_multiple(array $report_ids = array(), $uid = NULL, array $conditions = array(), $reset = FALSE) {
  if (!$uid) {
    global $user;
    $uid = $user->uid;
  }

  $controller = entity_get_controller('comstack_pm_report');
  if ($reset) {
    $controller->resetCache();
  }

  return $controller->load($report_ids, $conditions);
}

/**
 * Access callback.
 *
 * @throws Exception
 */
function comstack_pm_report_entity_access($op, $entity, $account, $entity_type) {
  switch ($op) {
    case 'view':
      return user_access('view comstack_pm_report entries', $account) || $entity->uid == $account->uid && user_access('view own comstack_pm_report entries', $account);
      break;
    case 'create':
      return user_access('create comstack_pm_report entries', $account);
      break;
    case 'update':
      return user_access('update comstack_pm_report entries', $account) || $entity->uid == $account->uid && user_access('update own comstack_pm_report entries', $account);
      break;
    case 'delete':
      return user_access('delete comstack_pm_report entries', $account) || $entity->uid == $account->uid && user_access('delete own comstack_pm_report entries', $account);
      break;
    default:
      throw new Exception(t("An invalid operation string has been passed to the Entity access check function for Comstack PM Reports."));
      break;
  }
}
