<?php
/**
 * @file
 * Provides a class that manages the videopublishing embed codes.
 */


/**
 * Handles the videopublishing embed codes.
 */
class VideopublishingEmbedCode {

  const TABLE = 'videopublishing_embed_code';
  const FIELD_EMBED_CODE = 'embed_code';
  const FIELD_PUBLISH_ID = 'publish_id';

  /**
   * Returns the embed code for the given publish id.
   *
   * If the embed code for the given publish id was generated and pulled
   * from videopublishing this method will return a string. Otherwise it will
   * return FALSE.
   *
   * @param string $vpid
   * @param int $format_id
   * @param string $player_id
   * @param string $field
   *
   * @return mixed
   *   The embed code as a string or FALSE.
   */
  public static function get($vpid, $format_id, $player_id, $field = self::FIELD_EMBED_CODE) {
    $select = db_select(self::TABLE, 'vpec')
      ->condition('vpid', $vpid)
      ->condition('format_id', $format_id)
      ->condition('player_id', $player_id);
    $select->addField('vpec', $field);

    return $select->execute()->fetchColumn();
  }

  /**
   * Sets or updates an embed code entry.
   *
   * @param string $vpid
   * @param int $format_id
   * @param string $player_id
   * @param string $publish_id
   * @package string $embed_code
   */
  public static function set($vpid, $format_id, $player_id, $publish_id, $embed_code) {
    $merge = db_merge(self::TABLE)
      ->key(array(
        'vpid' => $vpid,
        'format_id' => $format_id,
        'player_id' => $player_id,
      ))
      ->fields(array(
        'publish_id' => $publish_id,
        'embed_code' => $embed_code,
      ))
      ->execute();
  }

  /**
   * Provides an implementation for the hook_cron_queue_info worker callback.
   *
   * @param array $queue_item
   *
   * @see videopublishing_video_embed_codes_worker()
   */
  public static function embedCodeWork(array $queue_item) {
    $vp = VideopublishingApi::getInstance();
    try {
      $embed_code = json_decode($vp->getPublishEmbedCode($queue_item['publish_id']));
      VideopublishingEmbedCode::set($queue_item['vpid'], $queue_item['format_id'], $queue_item['player_id'], $queue_item['publish_id'], $embed_code->embed);
    }
    catch (Api_Methods_Exception $e) {
      watchdog_exception('videopublishing method', $e);
    }
    catch (Api_Http_RequestException $e) {
      // Requeues the item.
      $queue = DrupalQueue::get(VideopublishingHooks::QUEUE_VIDEO_EMBED_CODE, TRUE);
      $queue->createItem($queue_item);
    }
    catch (VideopublishingApiException $e) {
      watchdog_exception('videopublishing api', $e);
    }
    catch (Exception $e) {
      watchdog_exception('videopublishing', $e);
    }
  }

}