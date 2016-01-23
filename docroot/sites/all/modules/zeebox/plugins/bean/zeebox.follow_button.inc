<?php
/**
 * @file
 * Follow Button block functionality for the zeebox Everywhere module.
 */

class ZeeboxFollowButtonBlockBean extends ZeeboxBrandIDBeanPlugin {
  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    return array(
      '#theme' => 'zeebox_follow_button',
      '#settings' => $bean->settings,
    );
  }
}
