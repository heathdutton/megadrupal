<?php
/**
 * @file
 * Provides hook implementations for the forms module.
 */

/**
 * Provies hook implementations for the Table Size module.
 */
class TableSizeHooks {

  /**
   * The hook_menu paths.
   */
  const MENU_ADMIN = 'admin/config/development/table-size';
  const MENU_ADMIN_DEFAULT = 'admin/config/development/table-size/list';
  const MENU_ADMIN_SETTINGS = 'admin/config/development/table-size/settings';

  /**
   * Provides an implementation for hook_menu().
   */
  public static function menu() {
    $items = array();
    // All the pages we provide render a form as their content.
    $path = self::getPath('/forms');

    $items[self::MENU_ADMIN] = array(
      'title' => 'Table Sizes',
      'type' => MENU_NORMAL_ITEM,
      'description' => 'Manage table size quotas and alerts.',
      'access arguments' => array('manage table size'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('table_size_admin_form'),
      'file path' => $path,
      'file' => 'admin.inc',
      'weight' => 0,
    );

    $items[self::MENU_ADMIN_DEFAULT] = array(
      'title' => 'Table Sizes',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'access arguments' => array('manage table size'),
    );

    $items[self::MENU_ADMIN_SETTINGS] = array(
      'title' => 'Alert Settings',
      'type' => MENU_LOCAL_TASK,
      'description' => 'Manage table size alerts.',
      'access arguments' => array('manage table size'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('table_size_admin_settings_form'),
      'file path' => self::getPath('/forms'),
      'file' => 'admin_settings.inc',
      'weight' => 1,
    );

    return $items;
  }

  /**
   * Provides an implementation for hook_permission.
   */
  public static function permission() {
    $items = array();

    $items['manage table size'] = array(
      'title' => t('Manage table size quotas and alerts'),
      'description' => t('Set arbitrary table size quotas and configure alerts to be sent if the quota is exceeded.'),
    );

    return $items;
  }

  /**
   * Provides an implementation for hook_theme().
   */
  public static function theme() {
    $items = array();
    $path = self::getPath('/forms');

    $items['table_size_admin_form'] = array(
      'render element' => 'form',
      'file' => 'admin.inc',
      'path' => $path,
    );

    return $items;
  }

  /**
   * Provides an implementation for hook_mail().
   */
  public static function mail($key, &$message, $params) {
    switch ($key) {
      case 'table_size_notification':
        $message['subject'] = variable_get('site_name') . ' - Table Size Notification';
        if (empty($params)) {
          $message['body'][] = t('There are no tables that exceeded their quota.');
        }
        else {
          $message['body'][] = t('The following table(s) exceeded their quota:');
          foreach ($params as $table => $info) {
            $message['body'][] = ' - ' . $table;
          }
          $message['body'][] = t('The Table Size script.');
        }
        break;
    }
  }


  /**
   * Returns the path to a module's directory and optionally sets a suffix.
   * The suffix may be used to get the path to certain subdirectories.
   *
   * @param string $suffix
   * @param string $module
   *
   * @return string
   */
  private static function getPath($suffix = '', $module = 'table_size') {
    return drupal_get_path('module', $module) . $suffix;
  }

}