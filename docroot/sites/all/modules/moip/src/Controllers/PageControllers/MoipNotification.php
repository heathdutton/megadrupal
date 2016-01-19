<?php

namespace Drupal\moip\Controllers\PageControllers;

/**
 * Process the $POST data that MoIP sends in the NASP integration
 * See more about hte integration at https://www.moip.com.br/pdf/Notificar.pdf
 */
class MoipNotification implements \Drupal\cool\Controllers\PageController {

  /**
   * Path to be used by hook_menu().
   */
  static public function getPath() {
    return 'moip/notification';
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
    try {
      $nasp = \Drupal\moip\Entity\MoipNasp::create($_POST);
      $nasp->processMoipData();
    }
    catch (Exception $e) {
      print $e->getMessage();
      drupal_exit();
    }
  }

  public static function accessCallback() {
    return TRUE;
  }

}
