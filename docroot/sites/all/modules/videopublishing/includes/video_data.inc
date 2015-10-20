<?php
/**
 * @file
 * Provides a class that manages the videopublishing video data received from
 * videopublishing.com. This consists of extensive information about the videos
 * like: thumbnail images, video length and size, etc..
 */


/**
 * Handles the videopublishing video data.
 */
class VideopublishingData {

  const TABLE = 'videopublishing_data';

  /**
   * Returns the video information for the given video id.
   *
   * @param string $vpid
   * @param int $format_id
   * @param string $player_id
   * @param string $field
   *
   * @return array.
   */
  public static function get($vpid) {
    $select = db_select(self::TABLE, 'vpd')
      ->condition('vpid', $vpid);
    $select->addField('vpd', 'data');

    $result = $select->execute()->fetchColumn();
    return $result ? json_decode($result, TRUE) : array();
  }

  /**
   * Inserts or updates a vpid - data pair (information about a video).
   *
   * @param string $vpid
   * @param array $data
   */
  public static function set($vpid, array $data) {
    $merge = db_merge(self::TABLE)
      ->key(array('vpid' => $vpid))
      ->fields(array('data' => json_encode($data)))
      ->execute();
  }

}