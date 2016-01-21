<?php
/**
 * @file
 * sprout_users.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function sprout_users_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access administration pages'.
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access all views'.
  $permissions['access all views'] = array(
    'name' => 'access all views',
    'roles' => array(
      'site manager' => 'site manager',
    ),
    'module' => 'views',
  );

  // Exported permission: 'access all webform results'.
  $permissions['access all webform results'] = array(
    'name' => 'access all webform results',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'access comments'.
  $permissions['access comments'] = array(
    'name' => 'access comments',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'access contextual links'.
  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'contextual',
  );

  // Exported permission: 'access overlay'.
  $permissions['access overlay'] = array(
    'name' => 'access overlay',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'overlay',
  );

  // Exported permission: 'access own webform results'.
  $permissions['access own webform results'] = array(
    'name' => 'access own webform results',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'access own webform submissions'.
  $permissions['access own webform submissions'] = array(
    'name' => 'access own webform submissions',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'access site in maintenance mode'.
  $permissions['access site in maintenance mode'] = array(
    'name' => 'access site in maintenance mode',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access toolbar'.
  $permissions['access toolbar'] = array(
    'name' => 'access toolbar',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'toolbar',
  );

  // Exported permission: 'access user profiles'.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer comments'.
  $permissions['administer comments'] = array(
    'name' => 'administer comments',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'administer main-menu menu items'.
  $permissions['administer main-menu menu items'] = array(
    'name' => 'administer main-menu menu items',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'menu_admin_per_menu',
  );

  // Exported permission: 'administer users'.
  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(
      'site manager' => 'site manager',
    ),
    'module' => 'user',
  );

  // Exported permission: 'change own username'.
  $permissions['change own username'] = array(
    'name' => 'change own username',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'user',
  );

  // Exported permission: 'create page content'.
  $permissions['create page content'] = array(
    'name' => 'create page content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create url aliases'.
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'path',
  );

  // Exported permission: 'create webform content'.
  $permissions['create webform content'] = array(
    'name' => 'create webform content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete all webform submissions'.
  $permissions['delete all webform submissions'] = array(
    'name' => 'delete all webform submissions',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'delete any page content'.
  $permissions['delete any page content'] = array(
    'name' => 'delete any page content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any webform content'.
  $permissions['delete any webform content'] = array(
    'name' => 'delete any webform content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own page content'.
  $permissions['delete own page content'] = array(
    'name' => 'delete own page content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own webform content'.
  $permissions['delete own webform content'] = array(
    'name' => 'delete own webform content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own webform submissions'.
  $permissions['delete own webform submissions'] = array(
    'name' => 'delete own webform submissions',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'edit all webform submissions'.
  $permissions['edit all webform submissions'] = array(
    'name' => 'edit all webform submissions',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'edit any page content'.
  $permissions['edit any page content'] = array(
    'name' => 'edit any page content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any webform content'.
  $permissions['edit any webform content'] = array(
    'name' => 'edit any webform content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit menu title'.
  $permissions['edit menu title'] = array(
    'name' => 'edit menu title',
    'roles' => array(
      'site manager' => 'site manager',
    ),
    'module' => 'simplified_menu_admin',
  );

  // Exported permission: 'edit meta tags'.
  $permissions['edit meta tags'] = array(
    'name' => 'edit meta tags',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'metatag',
  );

  // Exported permission: 'edit own comments'.
  $permissions['edit own comments'] = array(
    'name' => 'edit own comments',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'edit own page content'.
  $permissions['edit own page content'] = array(
    'name' => 'edit own page content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own webform content'.
  $permissions['edit own webform content'] = array(
    'name' => 'edit own webform content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own webform submissions'.
  $permissions['edit own webform submissions'] = array(
    'name' => 'edit own webform submissions',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'edit views basic settings'.
  $permissions['edit views basic settings'] = array(
    'name' => 'edit views basic settings',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'views_ui_basic',
  );

  // Exported permission: 'post comments'.
  $permissions['post comments'] = array(
    'name' => 'post comments',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'save draft'.
  $permissions['save draft'] = array(
    'name' => 'save draft',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'save_draft',
  );

  // Exported permission: 'schedule (un)publishing of nodes'.
  $permissions['schedule (un)publishing of nodes'] = array(
    'name' => 'schedule (un)publishing of nodes',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'scheduler',
  );

  // Exported permission: 'skip comment approval'.
  $permissions['skip comment approval'] = array(
    'name' => 'skip comment approval',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'use text format html'.
  $permissions['use text format html'] = array(
    'name' => 'use text format html',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format wysiwyg'.
  $permissions['use text format wysiwyg'] = array(
    'name' => 'use text format wysiwyg',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'view advanced help index'.
  $permissions['view advanced help index'] = array(
    'name' => 'view advanced help index',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'advanced_help',
  );

  // Exported permission: 'view advanced help popup'.
  $permissions['view advanced help popup'] = array(
    'name' => 'view advanced help popup',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'advanced_help',
  );

  // Exported permission: 'view advanced help topic'.
  $permissions['view advanced help topic'] = array(
    'name' => 'view advanced help topic',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'advanced_help',
  );

  // Exported permission: 'view any unpublished page content'.
  $permissions['view any unpublished page content'] = array(
    'name' => 'view any unpublished page content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished webform content'.
  $permissions['view any unpublished webform content'] = array(
    'name' => 'view any unpublished webform content',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view the administration theme'.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'content editor' => 'content editor',
      'site manager' => 'site manager',
    ),
    'module' => 'system',
  );

  return $permissions;
}
