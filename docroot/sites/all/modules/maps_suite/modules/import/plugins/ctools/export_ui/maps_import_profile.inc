<?php

/**
 * @file
 * Plugin definition for CTools Export UI integration.
 */

$menu_prefix = 'admin/maps-suite';
$prefix_count = count(explode('/', $menu_prefix));
$module_path = drupal_get_path('module', 'maps_import');
$include_path = $module_path . '/admin';

$plugin = array(
  'handler' => array(
    'class' => 'Drupal\\maps_import\\Plugins\\CTools\\ExportUI\\Profile',
    'file' => 'Profile.php',
    'path' => $module_path . '/lib/Drupal/maps_import/Plugins/CTools/ExportUI',
  ),
  'schema' => 'maps_import_profile',
  'access' => 'administer maps suite',

  'allowed operations' => array(
    'edit'    => array('title' => t('Edit')),
    'enable'  => array('title' => t('Enable'), 'ajax' => TRUE, 'token' => TRUE),
    'disable' => array('title' => t('Disable'), 'ajax' => TRUE, 'token' => TRUE),
    'revert'  => array('title' => t('Revert')),
    'delete'  => array('title' => t('Delete')),
    'clone'   => FALSE,
    'import'  => array('title' => t('Import')),
    'export'  => array('title' => t('Export')),
  ),

  'menu' => array(
    'menu item' => 'profiles',
    'menu prefix' => $menu_prefix,
    'menu title' => 'Profiles',
    'menu description' => 'Administer MaPS Profiles: Web Services settings, import configuration, entities mapping, etc.',
    'items' => array(
      'list callback' => array(
        'position' => 'right',
      ),
      'add' => array(
        'title' => 'New profile',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('maps_import_admin_profile_form'),
        'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
        'file path' => $include_path,
        'file' => 'maps_import.ws.inc',
      ),
      'edit callback' => array(
        'path' => '%maps_import_profile',
        'title callback' => 'maps_import_profile_title',
        'title arguments' => array($prefix_count + 1),
        'page callback' => 'drupal_get_form',
        'page arguments' => array('maps_import_admin_profile_form', $prefix_count + 1),
        'access callback' => 'user_access',
        'access arguments' => array('administer maps suite'),
        'type' => MENU_NORMAL_ITEM,
        'file path' => $include_path,
        'file' => 'maps_import.ws.inc',
      ),
      'edit' => array(
        'path' => '%maps_import_profile/edit',
        'title' => 'Settings',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('maps_import_admin_profile_form', $prefix_count + 1),
        'access callback' => 'user_access',
        'access arguments' => array('administer maps suite'),
        'file path' => $include_path,
        'file' => 'maps_import.ws.inc',
      ),
      'export' => array(
        'path' => '%ctools_export_ui/export',
        'page arguments' => array(
          'maps_import_profile',
          'export',
          $prefix_count + 1,
        ),
        'access arguments' => array(
          'maps_import_profile',
          'export',
          $prefix_count + 1,
        ),
        'type' => MENU_LOCAL_ACTION,
      ),
      'revert' => array(
        'path' => '%ctools_export_ui/revert',
        // Note: Yes, 'delete' op is correct.
        'page arguments' => array(
          'maps_import_profile',
          'delete',
          $prefix_count + 1,
        ),
        'access arguments' => array(
          'maps_import_profile',
          'revert',
          $prefix_count + 1,
        ),
      ),
      'delete' => array(
        'path' => '%ctools_export_ui/delete',
        'title' => 'Delete profile',
        'page callback' => 'drupal_get_form',
        'page arguments' => array(
          'maps_import_admin_profile_delete_confirm_form',
          $prefix_count + 1,
        ),
        'access arguments' => array(
          'maps_import_profile',
          'delete',
          $prefix_count + 1,
        ),
        'file path' => $include_path,
        'file' => 'maps_import.ws.inc',
      ),
      'enable' => array(
        'path' => '%ctools_export_ui/enable',
        'page arguments' => array(
          'maps_import_profile',
          'enable',
          $prefix_count + 1,
        ),
        'access arguments' => array(
          'maps_import_profile',
          'enable',
          $prefix_count + 1,
        ),
      ),
      'disable' => array(
        'path' => '%ctools_export_ui/disable',
        'page arguments' => array(
          'maps_import_profile',
          'disable',
          $prefix_count + 1,
        ),
        'access arguments' => array(
          'maps_import_profile',
          'disable',
          $prefix_count + 1,
        ),
      ),
    ),
  ),

  'title singular' => t('profile'),
  'title singular proper' => t('Profile'),
  'title plural' => t('profiles'),
  'title plural proper' => t('Profiles'),

  'strings' => array(
    'confirmation' => array(
      'revert' => array(
        'information' => t('This action will permanently remove any customizations made to this profile.'),
        'success' => t('The profile has been reverted.'),
      ),
      'delete' => array(
        'information' => t('This action will permanently remove the profile from your database.'),
        'success' => t('The profile has been deleted.'),
      ),
    ),
    'message' => array(
      'no items' => t('There is no existing profile yet. Add a !link now.', array('!link' => l(t('new profile'), 'admin/maps-suite/profiles/add'))),
    ),
  ),
);
