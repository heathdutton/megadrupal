<?php

/**
 * @file
 * Provides class that expires flag objects' pages.
 */

class ExpireFlag implements ExpireInterface {

  /**
   * Executes expiration actions for user.
   *
   * @param object $flag
   *   Flag object which is acted on an entity.
   *
   * @param int $action
   *   Action that has been executed.
   *
   * @param bool $skip_action_check
   *   Shows whether should we check executed action or just expire node.
   */
  public function expire($flag, $action, $skip_action_check = FALSE) {
    if (empty($flag)) {
      return;
    }
    $expire_urls = array();

    // Expire custom pages.
    $expire_custom = variable_get('expire_flag_custom', FALSE);
    if ($expire_custom) {
      $pages = variable_get('expire_flag_custom_pages');

      // Check whether entity is loaded.
      if (!empty($flag->entity_id)) {
        $flag_action = new stdClass();
        $flag_action->entity_id = $flag->entity_id;
        $flag_action->entity_type = $flag->entity_type;
        $urls = ExpireAPI::expireCustomPages($pages, array('flag-action' => $flag_action));
      }
    }

    // Flush page cache for expired urls.
    ExpireAPI::executeExpiration($urls, 'flag', $flag);
    $cache_flushed = TRUE;
  }
}
