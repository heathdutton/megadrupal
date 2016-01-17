<?php
/**
 * @file
 * faktura.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function faktura_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function faktura_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function faktura_node_info() {
  $items = array(
    'company' => array(
      'name' => t('Company'),
      'base' => 'node_content',
      'description' => t('Create a new company'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'customer' => array(
      'name' => t('Customer'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dunning' => array(
      'name' => t('Dunning'),
      'base' => 'node_content',
      'description' => t('Create a dunning.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dunning_level' => array(
      'name' => t('Dunning level'),
      'base' => 'node_content',
      'description' => t('Create a dunning level'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'invoice' => array(
      'name' => t('Invoice'),
      'base' => 'node_content',
      'description' => t('Create an invoice'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'project' => array(
      'name' => t('Project'),
      'base' => 'node_content',
      'description' => t('Create a project to manage to do'),
      'has_title' => '1',
      'title_label' => t('Project title'),
      'help' => '',
    ),
    'scheduled_invoice' => array(
      'name' => t('Scheduled invoice'),
      'base' => 'node_content',
      'description' => t('Create a scheduled invoice. Also recurring invoices are possible.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'task' => array(
      'name' => t('Task'),
      'base' => 'node_content',
      'description' => t('Add a new task.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'tax' => array(
      'name' => t('Tax'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'time_report' => array(
      'name' => t('Time report'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
