<?php
/**
 * @file
 * Provides generic hook implementations for the videopublishing module.
 */

/**
 * Generic hook implementations for the videopublishing module.
 */
class VideopublishingHooks {

  const PATH_ADMIN = 'admin/config/media/videopublishing';
  const PERMISSION_ADMIN = 'administer videopublishing';
  const PERMISSION_ERRORS = 'view videopublishing api error messages';
  const THEME_VIDEO_DEFAULT_FORMATTER = 'videopublishing_video_default_formatter';
  const QUEUE_VIDEO_EMBED_CODE = 'videopublishing_video_embed_codes';
  const WORKER_CALLBACK_VIDEO_EMBED_CODE = 'videopublishing_video_embed_codes_worker';

  /**
   * Provides an implementation for hook_menu().
   */
  public static function menu() {
    $items = array();

    $items[self::PATH_ADMIN] = array(
      'title' => 'Video Publishing Settings',
      'description' => 'Administer Video Publishing',
      'type' => MENU_NORMAL_ITEM,
      'page callback' => 'drupal_get_form',
      'page arguments' => array('videopublishing_admin_form'),
      'access arguments' => array(self::PERMISSION_ADMIN),
      'file' => 'admin.inc',
      'file path' => drupal_get_path('module', 'videopublishing') . '/forms',
    );

    return $items;
  }

  /**
   * Provides an implementation for hook_permission().
   */
  public static function permission() {
    $item = array();
    $item[self::PERMISSION_ADMIN] = array(
      'title' => t('Administer videopublishing'),
      'description' => t('Manage videopublishing settings.'),
    );
    $item[self::PERMISSION_ERRORS] = array(
      'title' => t('View videopublishing errors'),
      'description' => t('View the Videopublishing API error messages.'),
    );
    return $item;
  }

  /**
   * Provides an implementation for hook_permission().
   */
  public static function theme() {
    $item = array();
    $path = drupal_get_path('module', 'videopublishing') . '/templates';

    $item[self::THEME_VIDEO_DEFAULT_FORMATTER] = array(
      'variables' => array('item' => array()),
      'template' => 'videopublishing-video-default-formatter',
      'path' => $path,
    );

    return $item;
  }

  /**
   * Provides an implementation for hook_cron_queue_info().
   */
  public static function cronQueueInfo() {
    $items = array();
    $items[self::QUEUE_VIDEO_EMBED_CODE] = array(
      'worker callback' => self::WORKER_CALLBACK_VIDEO_EMBED_CODE,
      'time' => 30,
    );
    return $items;
  }

}