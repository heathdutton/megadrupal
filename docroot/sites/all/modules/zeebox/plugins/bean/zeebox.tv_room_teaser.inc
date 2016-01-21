<?php
/**
 * @file
 * TV Room Teaser block functionality for the zeebox Everywhere module.
 */

class ZeeboxTVRoomTeaserBlockBean extends ZeeboxTVRoomIDBeanPlugin {
  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    return array(
      '#theme' => 'zeebox_tv_room_teaser',
      '#settings' => $bean->settings,
    );
  }
}
