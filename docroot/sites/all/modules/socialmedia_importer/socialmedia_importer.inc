<?php
/**
 * @file
 * Hold all common functions for Application handling.
 */

class SocialMediaImporterApplication {
  const NO_VALID_APP_ERROR = 'The Application can not be authenticated. Check your Application Settings';
  public $id = NULL;
  public $appName = NULL;
  public $appId = NULL;
  public $appSecret = NULL;
  public $accessToken = NULL;
  public $accessTokenSecret = NULL;
  public $isAuthorized = FALSE;
  public $sha1Method = NULL;
  public $provider = NULL;
  public $redirectUrl = NULL;

  /**
   * Init Social Media Application.
   */
  public function __construct($id = NULL, $app_id = NULL, $app_secret = NULL) {
    $this->sha1Method = new OAuthSignatureMethod_HMAC_SHA1();
    $this->redirectUrl = $GLOBALS['base_url'] . "/socialmedia_importer/response_handler";
    if (!is_null($id)) {
      $app = $this->loadApplication($id);
      if (is_object($app)) {
        $this->id = $id;
        $this->appName = $app->app_name;
        $this->appId = $app->app_id;
        $this->appSecret = $app->app_secret;
        $this->accessToken = $app->access_token;
        $this->accessTokenSecret = $app->access_token_secret;
        $this->isAuthorized = $app->is_authorized;
        $this->provider = $app->provider;
      }
    }
    else {
      if (!is_null($app_id) && !is_null($app_secret)) {
        $this->appId = $app_id;
        $this->appSecret = $app_secret;
      }
    }
  }

  /**
   * Load an Application by id.
   */
  public function loadApplication($id = NULL) {
    if (is_null($id)) {
      $id = $this->id;
    }
    try {
      if ($id) {
        $id = (int) $id;
        $result = db_query('SELECT * FROM {socialmedia_applications} smia WHERE smia.id = :id', array(':id' => $id))->fetchObject();
        return $result;
      }
    }
    catch (Exception $e) {
      watchdog_exception('socialmedia_importer', $e, WATCHDOG_ERROR);
    }
  }

  /**
   * Get Platform for an Application.
   */
  public function getProvider() {
    if (is_null($this->provider)) {
      try {
        if ($this->id) {
          $result = db_query('SELECT provider FROM {socialmedia_applications} sma WHERE sma.id = :id', array(':id' => $this->id))->fetchObject();
          return $result;
        }
      }
      catch (Exception $e) {
        watchdog_exception('socialmedia_importer', $e, WATCHDOG_ERROR);
      }
    }
    else {
      return $this->provider;
    }
  }

  /**
   * Authorize Application.
   */
  public function authorizeApplication() {}

  /**
   * Deauthorize an Application.
   * 
   * @return array
   *   An associative array with keys:
   *   - success: TRUE or FALSE
   *   - (optional) error_message: The error message in case of error.
   */
  public function deauthorizeApplication() {
    $is_deauthorize = array('success' => FALSE);
    return $is_deauthorize;
  }

  /**
   * Get data from a Social Media Application.
   */
  public function getApplicationData() {
    return json_encode('null');
  }
}
