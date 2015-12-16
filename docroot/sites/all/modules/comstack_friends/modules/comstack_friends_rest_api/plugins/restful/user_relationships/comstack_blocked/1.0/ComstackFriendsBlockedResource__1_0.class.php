<?php

/**
 * @file
 * Contains ComstackFriendsBlockedResource__1_0.
 */

class ComstackFriendsBlockedResource__1_0 extends \ComstackFriendsRestfulBase {
  /**
   * Overrides \ComstackFriendsRestfulBase::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '' => array(
        \RestfulInterface::GET => 'getList',
        \RestfulInterface::POST => 'newRequest',
      ),
      '^([\d]+)' => array(
        \RestfulInterface::GET => 'viewEntity',
        \RestfulInterface::DELETE => 'deleteEntity',
      ),
    );
  }
}
