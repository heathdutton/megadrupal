<?php
/**
 * @file
 * Provides a Drupal specific wrapper over the Videopublishing PHP API.
 */


/**
 * Config required by the base Videopublishing API.
 */
define('CRLF', "\r\n");
define('API_ADDRESS', 'api.videopublishing.com');

/**
 * A Drupal specific wrapper over the Videopublishing PHP API.
 */
class VideopublishingApi {

  private static $instance;
  private static $api_user;
  private static $api_key;
  private static $api_base_dir;

  /**
   * Returns an instance of VideopublishingApi.
   *
   * @return VideopublishingApi
   */
  public static function getInstance() {
    if (!self::$instance) {
      self::$instance = new VideopublishingApi();
    }
    return self::$instance;
  }

  /**
   * Initializes the api instance.
   */
  private function __construct() {
    spl_autoload_register(array(__CLASS__, 'autoload'));
    self::$api_base_dir = drupal_get_path('module', 'videopublishing') . '/VideoPublishing';

    $settings = LazyVars::get('videopublishing_settings', array());
    self::$api_user = isset($settings['api_user']) ? $settings['api_user'] : '';
    self::$api_key  = isset($settings['api_key'])  ? $settings['api_key']  : '';
  }

  /**
   * Prevents clone and unserialize.
   */
  private function __clone() {}
  private function __weakup() {}


  /**
   * Custom autoload function for the Videopublishign API.
   */
  public static function autoload($class_name) {
    $class = self::$api_base_dir . DIRECTORY_SEPARATOR . 'library' . DIRECTORY_SEPARATOR . str_replace('_', DIRECTORY_SEPARATOR, $class_name) . '.php';
    if (is_file($class)) {
      include_once $class;
    }
  }

  /**
   * Checks if the main library directory exists.
   *
   * @return bool
   */
  public function libraryExists() {
    return file_exists(self::$api_base_dir . '/library/Api');
  }


  /**
   * An internal abstractization of the various requests methods of this API.
   *
   * @param string $api_class_name
   * @param string $api_method_name
   * @param array $params
   * @param string $response_type
   *
   * @return string The API response
   */
  private function makeRequest($api_class_name, $api_method_name, $response_type, array $params = array()) {
    try {
      $factory = new Api_Methods_Factory(self::$api_user, self::$api_key, $response_type);
      $object = $factory->factory($api_class_name);

      return call_user_func_array(array($object, $api_method_name), $params);
    }
    catch (Api_Methods_Exception $api_exception) {
      watchdog_exception('videopublishing_api', $api_exception);
      if (user_access(VideopublishingHooks::PERMISSION_ERRORS)) {
        drupal_set_message(t('Video Publishing API error: @message', array('@message' => $api_exception->getMessage())), 'error');
      }
    }
    catch (Api_Http_RequestException $http_exception) {
      watchdog_exception('videopublishing_api', $http_exception);
      if (user_access(VideopublishingHooks::PERMISSION_ERRORS)) {
        drupal_set_message(t('Video Publishing Connection error: @message', array('@message' => $http_exception->getMessage())), 'error');
      }
    }

    throw new VideopublishingApiException();
  }


  /**
   * Get all defined video players.
   *
   * @param string $response_type
   *
   * @return string The API response
   */
  public function getPlayers($response_type = Api_Methods_Factory::JSON_RESPONSE) {
    return $this->makeRequest(Api_Methods_Factory::PLAYERS, 'getAll', $response_type);
  }


  /**
   * Returns information about all the user's videos.
   *
   * @return string The API response
   */
  public function getVideos($response_type = Api_Methods_Factory::JSON_RESPONSE) {
    return $this->makeRequest(Api_Methods_Factory::VIDEOS, 'getAll', $response_type);
  }


  /**
   * Returns information about all the user's videos.
   *
   * @param string $playlist_id
   * @param string $response_type
   *
   * @return string The API response
   */
  public function getPlaylistVideos($playlist_id, $response_type = Api_Methods_Factory::JSON_RESPONSE) {
    if (!isset($playlist_id) || !is_string($playlist_id)) {
      throw new VideopublishingApiException('No playlist id given or the given playlist id is not a string.');
    }
    return $this->makeRequest(Api_Methods_Factory::PLAYLISTS, 'getInfoExtended', $response_type, array($playlist_id));
  }


  /**
   * Get all streams for a given video.
   *
   * @param string $video_id
   * @param string $response_type
   *
   * @return string The API response
   */
  public function getStreams($video_id, $response_type = Api_Methods_Factory::JSON_RESPONSE) {
    if (!isset($video_id) || !is_string($video_id)) {
      throw new VideopublishingApiException('No video id given or the given video id is not a string.');
    }
    return $this->makeRequest(Api_Methods_Factory::STREAMS, 'getAll', $response_type, array($video_id));
  }


  /**
   * Returns all available Video Publishing formats.
   *
   * @param string $response_type
   *
   * @return SimpleXMLElement
   */
  public function getFormats($response_type = Api_Methods_Factory::JSON_RESPONSE) {
    return $this->makeRequest(Api_Methods_Factory::FORMATS, 'getAll', $response_type);
  }


  /**
   * Uploads a new video and returns the video_id as a string.
   *
   * @param string $video_path
   * @param string $video_name
   * @param int $timelimit
   * @param string $response_type
   *
   * @return string The API response
   */
  public function uploadVideo($video_path, $video_name, $timelimit = 600, $response_type = Api_Methods_Factory::JSON_RESPONSE) {
    set_time_limit($timelimit);
    return $this->makeRequest(Api_Methods_Factory::VIDEOS, 'upload', $response_type, array($video_path, $video_name));
  }


  /**
   * Returns extensive video information.
   *
   * @param string $video_id
   * @param string $response_type
   *
   * @return string The API response
   */
  public function getVideoInfo($video_id, $response_type = Api_Methods_Factory::JSON_RESPONSE) {
    return $this->makeRequest(Api_Methods_Factory::VIDEOS, 'getInfo', $response_type, array($video_id));
  }


  /**
   * Creates a stream (a playable format) for the given video.
   *
   * @param string $video_id
   * @param int $format_id
   * @param string $response_type
   *
   * @return string The API response
   */
  function setStream($video_id, $format_id = 1, $response_type = Api_Methods_Factory::JSON_RESPONSE) {
    return $this->makeRequest(Api_Methods_Factory::STREAMS, 'setStream', $response_type, array($video_id, $format_id));
  }


  /**
   * Publishes a steam.
   *
   * @param string $player_id
   * @param string $element_id (stream ID or playlist ID)
   * @param string $element_type ('stream' or 'playlist')
   * @param array $data
   * @param string $response_type
   *
   * Note that the stream id is actually a <videoId>_<formatId> string.
   *
   * The $data array can contain the following keys:
   * - links (array),
   * - acudeo (string)
   *
   * @return string The API response
   */
  function publish($player_id, $element_id, $element_type, array $data = array(), $response_type = Api_Methods_Factory::JSON_RESPONSE) {
    return $this->makeRequest(Api_Methods_Factory::PUBLISH, 'publish', $response_type, array($player_id, $element_id, $element_type, $data));
  }


  /**
   * Retrieves the embed code for specified player association.
   *
   * @param string $publish_id
   * @param string $response_type
   *
   * @return string
   */
  function getPublishEmbedCode($publish_id, $response_type = Api_Methods_Factory::JSON_RESPONSE) {
    return $this->makeRequest(Api_Methods_Factory::PUBLISH, 'getEmbedJS', $response_type, array($publish_id));
  }


  /**
   * Retrieves an element's associations (element = video or playlist).
   *
   * @param string $element_id
   * @param string $player_type (single or playlist)
   * @param string $response_type
   *
   * @return string The API response
   */
  public function getElementAssociations($element_id, $player_type = 'single', $response_type = Api_Methods_Factory::JSON_RESPONSE) {
    return $this->makeRequest(Api_Methods_Factory::PUBLISH, 'getElementAssociations', $response_type, array($element_id, $player_type));
  }

}