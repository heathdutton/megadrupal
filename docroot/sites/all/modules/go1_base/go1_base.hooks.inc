<?php
/**
 * @file go1_base.hooks.inc
 *
 * Drupal hook implementations.
 */

use \Drupal\go1_base\Route\Importer as RouteImporter;
use \Drupal\go1_base\Hook\Menu as HookMenu;
use \Drupal\go1_base\Hook\PageBuild as HookPageBuild;
use \Drupal\go1_base\Hook\Entity\ViewAlter as HookEntityViewAlter;

/**
 * Implements hook_menu()
 */
function go1_base_menu() {
  require_once dirname(__FILE__) . '/lib/Hook/Menu.php';
  return go1_id(new HookMenu())->getMenuItems();
}

/**
 * Implements hook_flush_caches()
 *
 * @tag cache
 * @tag autoload
 */
function go1_base_flush_caches() {
  go1_id(new \Drupal\go1_base\Hook\FlushCache())->execute();
}

/**
 * Implements hook_modules_enabled()
 *
 * @tag autoload
 */
function go1_base_modules_enabled($modules) {
  // Refresh the cached-modules
  go1_modules('go1_base', TRUE);

  // Rebuild module weight
  go1_id(new Drupal\go1_base\Hook\FlushCache())->fixModuleWeight();
}

/**
 * Implements hook_block_info()
 */
function go1_base_block_info() {
  return go1_id(new \Drupal\go1_base\Hook\BlockInfo())->import();
}

/**
 * Implements hook_block_view
 */
function go1_base_block_view($delta) {
  return go1_id(new \Drupal\go1_base\Hook\BlockView($delta))->view();
}

/**
 * Implements hook_admin_paths()
 */
function go1_base_admin_paths() {
  return array('at/twig' => TRUE);
}

###############################################################
# Cache warming
###############################################################

/**
 * Implements hook_entity_view()
 */
function go1_base_entity_view($entity, $type, $view_mode, $langcode) {
  go1_container('breadcrumb_api')->checkEntityConfig($entity, $type, $view_mode, $langcode);
}

if (defined('GO1_BASE_ENTITY_TEMPLATE') && constant('GO1_BASE_ENTITY_TEMPLATE')) {
  /**
   * Implements hook_entity_view_alter()
   */
  function go1_base_entity_view_alter(&$build, $entity_type) {
    go1_id(new HookEntityViewAlter($build, $entity_type))->execute();
  }
}

/**
 * Implements hook_entity_insert()
 */
function go1_base_entity_update($entity, $type) {
  go1_container('cache.warmer')
    ->setEventName('entity_update')
    ->setContext(array('entity_type' => $type, 'entity' => $entity))
    ->warm()
  ;
}

/**
 * Implements hook_entity_insert()
 */
function go1_base_entity_insert($entity, $type) {
  go1_container('cache.warmer')
    ->setEventName('entity_insert')
    ->setContext(array('entity_type' => $type, 'entity' => $entity))
    ->warm()
  ;
}

/**
 * Implements hook_entity_insert()
 */
function go1_base_entity_delete($entity, $type) {
  go1_container('cache.warmer')
    ->setEventName('entity_delete')
    ->setContext(array('entity_type' => $type, 'entity' => $entity))
    ->warm()
  ;
}

/**
 * Implements hook_user_login()
 */
function go1_base_user_login(&$edit, $account) {
  go1_container('cache.warmer')
    ->setEventName('user_login')
    ->setContext(array('entity_type' => 'user', 'entity' => $account))
    ->warm()
  ;
}

/**
 * Implements hook_user_login()
 */
function go1_base_user_logout($account) {
  go1_container('cache.warmer')
    ->setEventName('user_logout')
    ->setContext(array('entity_type' => 'user', 'entity' => $account))
    ->warm()
  ;
}

/**
 * Implements hook_page_build().
 *
 * Renders blocks into their regions.
 *
 * @see Drupal\go1_base\Route\Controller::prepareContextBlocks()
 */
function go1_base_page_build(&$page) {
  if (go1_container('container')->offsetExists('page.blocks')) {
    go1_id(new HookPageBuild($page, go1_container('page.blocks')))->execute();
  }

  go1_container('breadcrumb_api')->pageBuild();
}
