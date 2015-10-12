<?php

/**
 * @file
 * Youtube uploader Zend Gdata implementation
 */

class YoutubeUploaderYtapi {

  protected $yt;

  /**
   * Set library path.
   */
  public function __construct() {
    $this->includeZendLib();
  }

  /**
   * Test if Zend library is available.
   */
  public function mtestZendLib() {
    if (!$this->includeZendLib()) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Set Zend library path.
   */
  protected function includeZendLib() {
    if (file_exists(DRUPAL_ROOT . "/sites/all/libraries/Zend/Loader.php")) {
      $path = DRUPAL_ROOT . "/sites/all/libraries";
    }
    elseif (file_exists(DRUPAL_ROOT . "/sites/" . $_SERVER['SERVER_NAME'] . "/libraries/Zend/Loader.php")) {
      $path = DRUPAL_ROOT . "/sites/" . $_SERVER['SERVER_NAME'] . "/libraries";
    }
    else {
      return FALSE;
    }

    set_include_path(get_include_path() . PATH_SEPARATOR . $path);

    require_once $path . '/Zend/Loader.php';
    Zend_Loader::loadClass('Zend_Gdata_ClientLogin');
    Zend_Loader::loadClass('Zend_Gdata_YouTube');
    Zend_Loader::loadClass('Zend_Gdata_App_Exception');
    Zend_Loader::loadClass('Zend_Gdata_App_HttpException');

    return TRUE;
  }

  /**
   * Authenticate to youtube with account settings.
   */
  public function authenticate($credentials = array()) {
    $username = !empty($credentials['username']) ? $credentials['username'] : variable_get('youtube_uploader_username', '');
    $upasswd = !empty($credentials['passwd']) ? $credentials['passwd'] : variable_get('youtube_uploader_passwd', '');
    $devkey = !empty($credentials['devkey']) ? $credentials['devkey'] : variable_get('youtube_uploader_devkey', '');
    // Authenticate to Youtube API.
    // Passing a Developer Key and ClientID to Zend_Gdata_YouTube.
    try {
      $httpclient = Zend_Gdata_ClientLogin::getHttpClient($username, $upasswd, 'youtube');
      $this->yt = new Zend_Gdata_YouTube($httpclient, NULL, NULL, $devkey);
    }
    catch (Zend_Gdata_App_Exception $ex) {
      watchdog('youtube_uploader', 'Error: %error could not authenticate to Youtube', array('%error' => $ex->getMessage()), WATCHDOG_ERROR);
      return array('error' => $ex->getMessage());
    }

  }

  /**
   * Get default thumb and actual title for a video.
   */
  public function getTitleThumbs($video_id) {
    $this->authenticate();
    try {
      $video_entry = $this->yt->getVideoEntry($video_id);
      $title = $video_entry->getVideoTitle();
      $video_thumbs = $video_entry->getVideoThumbnails();
    }
    catch (Zend_Gdata_App_Exception $ex) {
      watchdog('youtube_uploader', 'Error: %error could not get info for video @id', array('%error' => $ex->getMessage(), '@id' => $video_id), WATCHDOG_ERROR);
      return array('error' => $ex->getMessage());
    }

    return array('title' => $title, 'default_thumb' => $video_thumbs[0]['url']);

  }

  /**
   * Get a token required by the upload form.
   */
  public function getYoutubeToken($video_title, $field_settings) {
    if (!$video_title) {
      return array('error' => t('You must provide a title'));
    }

    $this->authenticate();

    // Uploading a video.
    $my_entry = new Zend_Gdata_YouTube_VideoEntry();
    $my_entry->setVideoTitle($video_title);
    $my_entry->setVideoDescription($field_settings['youtube_uploader_description']);
    // Note that category must be a valid YouTube category !
    $my_entry->setVideoCategory($field_settings['youtube_uploader_category']);
    $my_entry->SetVideoTags($field_settings['youtube_uploader_tags']);

    // Make video unlisted.
    if ($field_settings['youtube_uploader_publishing_options'] == 'Unlisted') {
      $unlisted = new Zend_Gdata_App_Extension_Element('yt:accessControl', 'yt', 'http://gdata.youtube.com/schemas/2007', '');
      $unlisted->setExtensionAttributes(array(
        array('namespaceUri' => '', 'name' => 'action', 'value' => 'list'),
        array('namespaceUri' => '', 'name' => 'permission', 'value' => 'denied'),
      ));
      $my_entry->setExtensionElements(array($unlisted));
    }

    $token_handler = 'http://gdata.youtube.com/action/GetUploadToken';
    try {
      $token_array = $this->yt->getFormUploadToken($my_entry, $token_handler);
      $token_value = $token_array['token'];
      $post_url = $token_array['url'];
    }
    catch (Zend_Gdata_App_Exception $ex) {
      watchdog('youtube_uploader', 'Error: %error could not get form token', array('%error' => $ex->getMessage()), WATCHDOG_ERROR);
      return array('error' => $ex->getMessage());
    }

    return array('token' => $token_value, 'posturl' => $post_url);
  }

  /**
   * Delete a video from YouTube.
   */
  public function deleteVideo($video_id) {
    $this->authenticate();
    try {
      $video_entry = $this->yt->getFullVideoEntry($video_id);
      $result = $this->yt->delete($video_entry);
    }
    catch (Zend_Gdata_App_HttpException $ex) {
      watchdog('youtube_uploader', 'Error: %error could not delete video @id', array('%error' => $ex->getMessage(), '@id' => $video_id), WATCHDOG_ERROR);
    }
  }
}
