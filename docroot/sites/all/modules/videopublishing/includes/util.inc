<?php
/**
 * @file
 * Provides generic functionality for the Videopublishing module.
 */


/**
 * Generic functionality for the Videopublishing module.
 */
class VideopublishingUtil {


  // The type of options self::getOptions() may get.
  const OPTIONS_VIDEO  = 'Video';
  const OPTIONS_PLAYER = 'Player';
  const OPTIONS_FORMAT = 'Format';


  /**
   * Returns an associative options array with all the available videos.
   *
   * @example
   *   array(
   *     '59dc073b1d07a13a8d542e1ec69d64af' => 'Awesome video',
   *     '523940eee5752efe785fd28b2268b813' => 'Epic video 42',
   *   );
   *
   * @return array
   */
  private static function getOptionsVideo() {
    $options = array();
    $vp = VideopublishingApi::getInstance();
    $videos = json_decode($vp->getVideos(), TRUE);
    foreach ($videos['videos'] as $v) {
      $options[$v['item']] = $v['title'];
    }
    return $options;
  }


  /**
   * Returns an associative options array with all the VideoPublishing
   * video players available to the user.
   *
   * @return array
   */
  private static function getOptionsPlayer() {
    $options = array();
    $vp = VideopublishingApi::getInstance();
    $players = json_decode($vp->getPlayers(), TRUE);
    foreach ($players['players'] as $player) {
      $options[$player['item']] = $player['title'] . ' (' . $player['width'] . 'x' . $player['height'] . ')';
    }
    return $options;
  }


  /**
   * Returns an associative options array with all the video formats.
   *
   * @return array
   */
  private static function getOptionsFormat() {
    $options = array();
    $vp = VideopublishingApi::getInstance();
    $formats = json_decode($vp->getFormats(), TRUE);
    foreach ($formats['formats'] as $format) {
      $options[$format['item']] = $format['title'] . ' (' . $format['width'] . 'x' . $format['height'] . ')';
    }
    return $options;
  }


  /**
   * Returns an array of options as expected by Drupal form elements.
   *
   * It's an abstractization of getOptions<Type>();
   *
   * @param string $type Can be Video or Player
   * @param bool $show_blank
   * @param bool $reset_cache
   *
   * @return array
   */
  public static function getOptions($type, $show_blank = TRUE, $reset_cache = FALSE) {
    static $options;
    $key = $type . '_' . (int) $show_blank;
    if (isset($options[$key]) && !$reset_cache) {
      return $options[$key];
    }

    $options[$key] = $show_blank ? array('' => t('- None -')) : array();
    $vp = VideopublishingApi::getInstance();
    $options[$key] += call_user_func(array(__CLASS__, 'getOptions' . $type));

    return $options[$key];
  }

  /**
   * Returns a render array of installation instructions.
   *
   * @return array
   */
  public static function getInstallInstructions($config_only = FALSE) {
    $config = array(
      '#theme' => 'html_tag',
      '#tag' => 'p',
      '#value' => t('To configure the Videopublishing module, !link.', array(
        '!link' => l(t('visit this page'), VideopublishingHooks::PATH_ADMIN, array(
          'attributes' => array('target' => '_blank'),
        )),
      )),
      '#attributes' => array(),
    );
    if ($config_only) {
      return $config;
    }

    $items = array(
      "Create a 'VideoPublishing' subdirectory in the videopublishing module directory.",
      'Download the VideoPublishing API (PHP Library).',
      "Copy the 'library' directory in the 'VideoPublishing' subdirectory (result: path-to-module/videopublishing/VideoPublishing/library).",
      'Install as usual.'
    );
    return array(
      'install' => array(
        '#theme' => 'item_list',
        '#title' => t('Installation instructions'),
        '#type'  => 'ol',
        '#items' => $items,
        '#attributes' => array(),
      ),
      'config' => $config,
    );
  }

}