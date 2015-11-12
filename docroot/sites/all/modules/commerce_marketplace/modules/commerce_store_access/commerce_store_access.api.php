<?php
/**
 * @file
 * This file contains hooks provided by commerce_store_access module.
 */

/**
 * Use hook_commerce_store_access_permission() to define new store-level permissions.
 * @see hook_permission()
 */
function hook_commerce_store_access_permission() {
  return array(
    'administer commerce_store members' => array(
      'title' => t('Administer store members'),
      'restrict' => TRUE,
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE),
    ),
    'edit store information' => array(
      'title' => t('Edit store'),
      'restrict' => TRUE,
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE),
    ),
    'view store page' => array(
      'title' => t('View store page'),
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE, COMMERCE_STORE_ANONYMOUS_ROLE, COMMERCE_STORE_AUTHENTICATED_ROLE),
    ),
    'add new members to commerce_store' => array(
      'title' => t('Add new members to the store'),
      'restrict' => TRUE,
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE),
    ),
    'change commerce_store role permissions' => array(
      'title' => t('Change permissions for store roles'),
      'restrict' => TRUE,
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE),
    ),
    'view commerce_store roles overview page' => array(
      'title' => t('View store roles list'),
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE),
    ),
    'add new role to commerce_store' => array(
      'title' => t('Create new store roles'),
      'restrict' => TRUE,
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE),
    ),
    'edit commerce_store roles' => array(
      'title' => t('Edit store roles'),
      'restrict' => TRUE,
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE),
    ),
    'add products to the store' => array(
      'title' => t('Create products for the store'),
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE, COMMERCE_STORE_AUTHENTICATED_ROLE),
    ),
    'view store orders' => array(
      'title' => t('View store orders'),
      'roles' => array(COMMERCE_STORE_AUTHENTICATED_ROLE, COMMERCE_STORE_ADMINISTRATOR_ROLE),
    ),
    'edit store orders' => array(
      'title' => t('Edit store orders'),
      'roles' => array(COMMERCE_STORE_ADMINISTRATOR_ROLE, COMMERCE_STORE_AUTHENTICATED_ROLE),
    ),
  );
}

/**
 * Use hook_commerce_store_Access_permission_alter() to change store permissions defined by other modules.
 */
function hook_commerce_store_access_permission_alter(&$permissions) {

}
