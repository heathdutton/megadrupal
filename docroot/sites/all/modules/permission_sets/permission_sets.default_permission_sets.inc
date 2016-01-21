<?php
/**
 * @file
 * permission_sets.default_permission_sets.inc
 */

/**
 * Implements hook_default_permission_sets().
 */
function permission_sets_default_permission_set() {
  $export = array();

  $permission_set = new stdClass();
  $permission_set->disabled = FALSE; /* Edit this to true to make a default permission_set disabled initially */
  $permission_set->api_version = 1;
  $permission_set->machinename = 'empty_permissions_set';
  $permission_set->name = 'Empty Permissions Set';
  $permission_set->permissions = array();
  $export['empty_permissions_set'] = $permission_set;

  $permission_set = new stdClass();
  $permission_set->disabled = FALSE; /* Edit this to true to make a default permission_set disabled initially */
  $permission_set->api_version = 1;
  $permission_set->machinename = 'anonymous_user_permissions_set';
  $permission_set->name = 'Anonymous User Permissions Set';
  $permission_set->permissions = array(
    'access comments' => 'access comments',
    'access content' => 'access content',
    'use text format filtered_html' => 'use text format filtered_html',
  );
  $export['anonymous_user_permissions_set'] = $permission_set;

  $permission_set = new stdClass();
  $permission_set->disabled = FALSE; /* Edit this to true to make a default permission_set disabled initially */
  $permission_set->api_version = 1;
  $permission_set->machinename = 'authenticated_user_permissions_set';
  $permission_set->name = 'Authenticated User Permissions Set';
  $permission_set->permissions = array(
    'access comments' => 'access comments',
    'access comments' => 'access comments',
    'post comments' => 'post comments',
    'skip comment approval' => 'skip comment approval',
    'use text format filtered_html' => 'use text format filtered_html',
  );
  $export['authenticated_user_permissions_set'] = $permission_set;

  $permission_set = new stdClass();
  $permission_set->disabled = FALSE; /* Edit this to true to make a default permission_set disabled initially */
  $permission_set->api_version = 1;
  $permission_set->machinename = 'admin_permissions_set';
  $permission_set->name = 'Admin Permissions Set';
  $permission_set->permissions = array(
    'access administration pages' => 'access administration pages',
    'access comments' => 'access comments',
    'access content' => 'access content',
    'access content overview' => 'access content overview',
    'access contextual links' => 'access contextual links',
    'access dashboard' => 'access dashboard',
    'access overlay' => 'access overlay',
    'access site in maintenance mode' => 'access site in maintenance mode',
    'access site reports' => 'access site reports',
    'access toolbar' => 'access toolbar',
    'access user profiles' => 'access user profiles',
    'administer actions' => 'administer actions',
    'administer blocks' => 'administer blocks',
    'administer comments' => 'administer comments',
    'administer content types' => 'administer content types',
    'administer filters' => 'administer filters',
    'administer image styles' => 'administer image styles',
    'administer menu' => 'administer menu',
    'administer modules' => 'administer modules',
    'administer nodes' => 'administer nodes',
    'administer permissions' => 'administer permissions',
    'administer search' => 'administer search',
    'administer shortcuts' => 'administer shortcuts',
    'administer site configuration' => 'administer site configuration',
    'administer software updates' => 'administer software updates',
    'administer taxonomy' => 'administer taxonomy',
    'administer themes' => 'administer themes',
    'administer url aliases' => 'administer url aliases',
    'administer users' => 'administer users',
    'block IP addresses' => 'block IP addresses',
    'bypass node access' => 'bypass node access',
    'cancel account' => 'cancel account',
    'change own username' => 'change own username',
    'create article content' => 'create article content',
    'create page content' => 'create page content',
    'create url aliases' => 'create url aliases',
    'customize shortcut links' => 'customize shortcut links',
    'delete any article content' => 'delete any article content',
    'delete any page content' => 'delete any page content',
    'delete own article content' => 'delete own article content',
    'delete own page content' => 'delete own page content',
    'delete revisions' => 'delete revisions',
    'delete terms in 1' => 'delete terms in 1',
    'edit any article content' => 'edit any article content',
    'edit any page content' => 'edit any page content',
    'edit own article content' => 'edit own article content',
    'edit own comments' => 'edit own comments',
    'edit own page content' => 'edit own page content',
    'edit terms in 1' => 'edit terms in 1',
    'post comments' => 'post comments',
    'revert revisions' => 'revert revisions',
    'search content' => 'search content',
    'select account cancellation method' => 'select account cancellation method',
    'skip comment approval' => 'skip comment approval',
    'switch shortcut sets' => 'switch shortcut sets',
    'use advanced search' => 'use advanced search',
    'use text format filtered_html' => 'use text format filtered_html',
    'use text format full_html' => 'use text format full_html',
    'view own unpublished content' => 'view own unpublished content',
    'view revisions' => 'view revisions',
    'view the administration theme' => 'view the administration theme',
  );
  $export['admin_permissions_set'] = $permission_set;

  return $export;
}
