<?php

namespace Drupal\moip\Controllers\PageControllers;

class MoipConfig implements \Drupal\cool\Controllers\PageController {

  /**
   * Path to be used by hook_menu().
   */
  static public function getPath() {
    return 'admin/config/services/moip';
  }

  /**
   * Passed to hook_menu()
   */
  static public function getDefinition() {
    return array(
      'title' => 'MoIP',
      'description' => t('Basic configuration to integrate Drupal with MoIP'),
    );
  }

  public static function pageCallback() {
    return drupal_get_form('moip_settings_form');
  }

  public static function accessCallback() {
    return user_access('administer moip');
  }

}
