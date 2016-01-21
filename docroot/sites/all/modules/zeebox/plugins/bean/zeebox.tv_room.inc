<?php
/**
 * @file
 * TV Room block functionality for the zeebox Everywhere module.
 */

class ZeeboxTVRoomBlockBean extends ZeeboxTVRoomIDBeanPlugin {
  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    return array(
      '#theme' => 'zeebox_tv_room',
      '#settings' => $bean->settings,
    );
  }
}
