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

    $lib_path = 'libraries/google-api-php-client/vendor/autoload.php';
    if (file_exists(DRUPAL_ROOT . "/sites/all/" . $lib_path)) {
      $path = DRUPAL_ROOT . "/sites/all/";
    }
    elseif (file_exists(DRUPAL_ROOT . "/sites/" . $_SERVER['SERVER_NAME'] . $lib_path)) {
      $path = DRUPAL_ROOT . "/sites/" . $_SERVER['SERVER_NAME'] . "/";
    }
    elseif (file_exists('profiles/' . drupal_get_profile() . $lib_path)) {
      $path = 'profiles/' . drupal_get_profile() . '/';
    }
    else {
      return FALSE;
    }

    require_once $path . $lib_path;

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
      $this->getFreshToken();
      // Get language to display categories in.
      if (module_exists('i18n')) {
        $language = i18n_language_interface();
      }
      else {
        global $language;
      }
      $categories = array();
      $region = _media_youtube_upload_get_country();
      $list_response = $this->yt->videoCategories->listVideoCategories('snippet', array('regionCode' => $region, 'hl' => $language->language));
      foreach ($list_response as $resp) {
        $categories[$resp['id']] = $resp['snippet']['title'];
      }
      return $categories;
    }
    catch (Google_ServiceException $e) {
      $error = t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage())));
      return array('error' => $error);
    }
    catch (Google_Exception $e) {
      $error = t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage())));
      return array('error' => $error);
    }

  }

  /**
   * Get a token based on returned code.
   */
  public function getTokenFromCode($code) {
    try {
      $this->authenticate();
      $this->client->authenticate($code);
      $token = $this->client->getAccessToken();
      variable_set('media_youtube_upload_token', $token);
    }
    catch (Google_ServiceException $e) {
      drupal_set_message(t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage()))), 'error');
    }
    catch (Google_Exception $e) {
      drupal_set_message(t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage()))), 'error');
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
        // Could be used to bypass SSL security for testing purposes.
        // $client->getHttpClient()->setDefaultOption('verify', false);
        // If there is a root CA certificate provided add it to the verification
        // process.
        $ca_certificate = variable_get('media_youtube_upload_root_ca_certificate', '');
        if (!empty($ca_certificate)) {
          $client->getHttpClient()->setDefaultOption('verify', variable_get('media_youtube_upload_root_ca_certificate'));
        }
        $this->yt = new Google_Service_YouTube($client);
        $this->client = $client;
      }
    }
    catch (Google_ServiceException $e) {
      $error = t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage())));
    }
    catch (Google_Exception $e) {
      $error = t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage())));
    }
    return array('error' => $error);
  }

  /**
   * Get a fresh, valid token.
   */
  public function getFreshToken() {

    $token = variable_get('media_youtube_upload_token', array());

    if (!isset($token['refresh_token'])) {
      return array('error' => t('no refresh token'));
    }

    $error = '';
    try {
      $this->authenticate();
      $this->client->setAccessToken($token);

      // Check to see if our access token has expired.
      // If so, get a new one and save it to file for future use.
      if ($this->client->isAccessTokenExpired()) {
        // Hold refresh token for later saving.
        $refresh_token = $token['refresh_token'];
        // Refresh the token.
        $this->client->refreshToken($refresh_token);
        // Get a new access token.
        $token = $this->client->getAccessToken();
        // Re-set the old refresh token.
        $token['refresh_token'] = $refresh_token;
        // Save our token to the variable.
        variable_set('media_youtube_upload_token', $token);
      }
    }
    catch (Google_ServiceException $e) {
      $error = t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage())));
    }
    catch (Google_Exception $e) {
      $error = t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage())));
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
      $error = t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage())));
    }
    catch (Google_Exception $e) {
      $error = t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage())));
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
      drupal_set_message(t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage()))), 'error');
      return FALSE;
    }
    catch (Google_Exception $e) {
      drupal_set_message(t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage()))), 'error');
      return FALSE;
    }
  }

  /**
   * Update a video on YouTube.
   */
  public function updateVideo($video_id, $field_values, $translations) {

    try {
      $this->getFreshToken();
      $list_response = $this->yt->videos->listVideos('status,snippet,localizations', array('id' => $video_id));
    }
    catch (Google_ServiceException $e) {
      drupal_set_message(t('A service error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage()))), 'error');
    }
    catch (Google_Exception $e) {
      drupal_set_message(t('A client error occurred: <code>@error</code></p>', array('@error' => htmlspecialchars_decode($e->getMessage()))), 'error');
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
    
    // Set the translations if any.
    if (!empty($translations)) {
      // Modify video's default language and localizations properties.
      // Ensure that a value is set for the resource's snippet.defaultLanguage property.
      $video['snippet']['defaultLanguage'] = $translations['original'];
      $localizations = $video['localizations'];
      if (is_null($localizations)) {
        $localizations = array();
      }
      if (isset($translations['translations'])) {
        foreach($translations['translations'] as $lang_code => $translation) {
          $localizations[$lang_code] = array('title' => $translation['title'], 'description' => $translation['description']);
        }
      }
      $video['localizations'] = $localizations;
    }

    // Update the video.
    $update_response = $this->yt->videos->update('status,snippet,localizations', $video);
  }
}
