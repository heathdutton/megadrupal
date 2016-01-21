<?php
/**
 * @file
 * Synchronised play-along zone block functionality for the zeebox Everywhere module.
 */

class ZeeboxPlayAlongBlockBean extends ZeeboxBrandIDBeanPlugin {
  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    return array(
      '#theme' => 'zeebox_play_along',
      '#settings' => $bean->settings,
    );
  }
}
