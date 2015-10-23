<?php

/**
 * @file
 * YouTube uploader Zend Gdata implementation.
 */

class MediaYoutubeUploadYtapi {

  protected $yt;
  protected $client;

  /**
   * Set library path.
   */
  public function __construct() {
    $this->includeGoogleLib();
  }

  /**
   * Test if Google API library is available.
   */
  public function mtestGoogleLib() {
    if (!$this->includeGoogleLib()) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Set Google API library path.
   */
  protected function includeGoogleLib() {

    if (file_exists(DRUPAL_ROOT . "/sites/all/libraries/google-api-php-client/src/Google/autoload.php")) {
      $path = DRUPAL_ROOT . "/sites/all/libraries";
    }
    elseif (file_exists(DRUPAL_ROOT . "/sites/" . $_SERVER['SERVER_NAME'] . "/libraries/google-api-php-client/src/Google/autoload.php")) {
      $path = DRUPAL_ROOT . "/sites/" . $_SERVER['SERVER_NAME'] . "/libraries";
    }
    elseif (file_exists('profiles/' . drupal_get_profile() . '/libraries/google-api-php-client/src/Google/autoload.php')) {
      $path = 'profiles/' . drupal_get_profile() . '/libraries';
    }
    else {
      return FALSE;
    }

    set_include_path(get_include_path() . PATH_SEPARATOR . $path);

    require_once $path . '/google-api-php-client/src/Google/autoload.php';

    return TRUE;
  }

  /**
   * Get a authentication url.
   */
  public function getAuthUrl($credentials = array()) {
    $this->authenticate();
    $state = mt_rand();
    $this->client->setState($state);
    $auth_url = $this->client->createAuthUrl();
    return $auth_url;
  }
  
  /**
   * Get categories.
   */
  public function setCategories() {

    try {
      $categories = array();
      $region = _media_youtube_upload_get_country();
      $list_response = $this->yt->videoCategories->listVideoCategories('snippet', array('regionCode' => $region));
      foreach ($list_response as $resp) {
        $categories[$resp['id']] = $resp['snippet']['title'];
      }
      variable_set('media_youtube_upload_youtube_categories', $categories);
    }
    catch (Google_ServiceException $e) {
      drupal_set_message(t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage()))), 'error');
    }
    catch (Google_Exception $e) {
      drupal_set_message(t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage()))), 'error');
    }
  }

  /**
   * Get a token based on returned code.
   */
  public function getTokenFromCode($code) {
    try {
      $this->authenticate();
      $this->client->authenticate($code);
      variable_set('media_youtube_upload_token', drupal_json_decode($this->client->getAccessToken()));
      // Set categories variable again.
      $this->setCategories();
    }
    catch (Google_ServiceException $e) {
      drupal_set_message(t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage()))), 'error');
    }
    catch (Google_Exception $e) {
      drupal_set_message(t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage()))), 'error');
    }
  }

  /**
   * Authenticate to google with account settings.
   */
  public function authenticate($credentials = array()) {

    $error = '';
    try {
      if (!isset($this->client)) {
        $app_name = !empty($credentials['app_name']) ? $credentials['app_name'] : variable_get('media_youtube_upload_app_name', '');
        $client_secret = !empty($credentials['client_secret']) ? $credentials['client_secret'] : variable_get('media_youtube_upload_client_secret', '');
        $client_id = !empty($credentials['client_id']) ? $credentials['client_id'] : variable_get('media_youtube_upload_client_id', '');

        // Authenticate to Google API.
        global $base_url;
        $client = new Google_Client();
        $client->setClientId($client_id);
        $client->setClientSecret($client_secret);
        $client->setScopes('https://www.googleapis.com/auth/youtube');
        $client->setRedirectUri($base_url . '/media_youtube_upload/oauth2callback');
        $client->setApplicationName($app_name);
        $client->setAccessType('offline');
        $client->setApprovalPrompt('force');

        $this->yt = new Google_Service_YouTube($client);
        $this->client = $client;
      }
    }
    catch (Google_ServiceException $e) {
      $error = t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage())));
    }
    catch (Google_Exception $e) {
      $error = t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage())));
    }
    return array('error' => $error);
  }

  /**
   * Get a fresh, valid token.
   */
  public function getFreshToken() {

    $token = variable_get('media_youtube_upload_token', array());

    if (empty($token['refresh_token'])) {
      return array('error' => t('no refresh token'));
    }

    $error = '';
    try {
      $this->authenticate();
      $this->client->setAccessToken(drupal_json_encode($token));

      // Check to see if our access token has expired.
      // If so, get a new one and save it to file for future use.
      if ($this->client->isAccessTokenExpired()) {
        $new_token = json_decode($this->client->getAccessToken());
        $this->client->refreshToken($new_token->refresh_token);
        $token = drupal_json_decode($this->client->getAccessToken());
        variable_set('media_youtube_upload_token', $token);
      }
    }
    catch (Google_ServiceException $e) {
      $error = t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage())));
    }
    catch (Google_Exception $e) {
      $error = t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage())));
    }
    return array('token' => $token['access_token'], 'error' => $error);
  }

  /**
   * Get default thumb and actual title for a video.
   */
  public function getTitleThumbs($video_id) {

    $this->getFreshToken();
    $title = $thumb = $error = '';
    try {
      $list_response = $this->yt->videos->listVideos("snippet", array('id' => $video_id));
    }
    catch (Google_ServiceException $e) {
      $error = t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage())));
    }
    catch (Google_Exception $e) {
      $error = t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage())));
    }
    
    if (empty($list_response)) {
      return array('error' => t('Video %vid not found', array('%vid' => $video_id)));
    }
    $video = $list_response[0]['snippet'];
    $title = $video['title'];
    $thumb = $video['thumbnails']['high']['url'];

    return array(
      'title' => $title,
      'default_thumb' => $thumb,
      'error' => $error
    );
  }

  /**
   * Delete a video from YouTube.
   */
  public function deleteVideo($video_id) {
    $this->getFreshToken();
    $error = '';
    try {
      $result = $this->yt->videos->delete($video_id);
      drupal_set_message(t('Video deleted on YouTube.'));
      return TRUE;
    }
    catch (Google_ServiceException $e) {
      drupal_set_message(t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage()))), 'error');
      return FALSE;
    }
    catch (Google_Exception $e) {
      drupal_set_message(t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage()))), 'error');
      return FALSE;
    }
  }

  /**
   * Update a video on YouTube.
   */
  public function updateVideo($video_id, $field_values) {

    try {
      $this->getFreshToken();
      $list_response = $this->yt->videos->listVideos('status,snippet', array('id' => $video_id));
    }
    catch (Google_ServiceException $e) {
      drupal_set_message(t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage()))), 'error');
    }
    catch (Google_Exception $e) {
      drupal_set_message(t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars($e->getMessage()))), 'error');
    }
    if (empty($list_response)) {
      return array('error' => t('Video %vid not found', array('%vid' => $video_id)));
    }
    $video = $list_response[0];
    // Change the title.
    $video_snippet = $video['snippet'];
    $video_snippet->setTitle($field_values['title']);
    $video_snippet->setDescription($field_values['description']);
    $video_snippet->setCategoryId($field_values['category']);
    $video_snippet->setTags($field_values['tags']);
    $video->setSnippet($video_snippet);

    // Change the privacy setting.
    $video_status = $video['status'];
    $video_status->privacyStatus = $field_values['privacy'];
    $video->setStatus($video_status);

    // Update the video.
    $update_response = $this->yt->videos->update('status,snippet', $video);

  }

}