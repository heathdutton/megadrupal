<?php

/**
 * @file
 * Handles all FB integration to Authorize and to Post
 */

_load_facebook_sdk();

/**
 * API class to handle common actions when autoposting
 * This class uses FBAutopostException for error handling. Severity is
 * passed resusing watchdog severity (See: http://api.drupal.org/api/drupal/includes%21bootstrap.inc/function/watchdog/7)
 * @see https://developers.facebook.com/docs/howtos/login/server-side-login
 */
class FBAutopost extends Facebook {
  /**
   * Constant indicating error code for a missing parameter
   */
  const missing_param = 0;
  /**
   * Constant indicating error code for an icorrect parameter
   */
  const incorrect_param = 1;
  /**
   * Constant indicating error code for a missing dependency
   */
  const missing_dependency = 2;
  /**
   * Constant indicating error code for a SDK thrown error
   */
  const sdk_error = 3;

  /**
   * Publication types as defined in Facebook documentation
   * Contains the name of the publication and the endpoint keyed by the machine name of the publication.
   * @see https://developers.facebook.com/docs/reference/api/page
   */
  private $publication_types = array(
    'event' => array(
      'name' => 'Event',
      'endpoint' => 'events',
    ),
    'link' => array(
      'name' => 'Links',
      'endpoint' => 'feed',
    ),
    'note' => array(
      'name' => 'Note',
      'endpoint' => 'notes',
    ),
    'photo' => array(
      'name' => 'Photo',
      'endpoint' => 'photos',
    ),
    'album' => array(
      'name' => 'Album',
      'endpoint' => 'albums',
    ),
    'post' => array(
      'name' => 'Post',
      'endpoint' => 'feed',
    ),
    'question' => array(
      'name' => 'Question',
      'endpoint' => 'questions',
    ),
    'milestone' => array(
      'name' => 'Milestone',
      'endpoint' => 'milestones',
    ),
    'offer' => array(
      'name' => 'Offer',
      'endpoint' => 'offers',
    ),
    'message' => array(
      'name' => 'Message',
      'endpoint' => 'conversations',
    ),
    'status' => array(
      'name' => 'Status',
      'endpoint' => 'feed',
    ),
    'video' => array(
      'name' => 'Video',
      'endpoint' => 'videos',
    ),
    'tab' => array(
      'name' => 'Tab',
      'endpoint' => 'tabs',
    ),
  );

  /**
   * Gets the endpoint of a publication
   *
   * @return string
   *   String containing the endpoint
   */
  protected function getEndpoint() {
    return $this->publication_types[$this->getType()]['endpoint'];
  }

  /**
   * Destination for the publication. An string with the page ID or 'me'.
   */
  private $destination;

  /**
   * Privacy setting for the publications. This property is made public because
   * the getter method will give some formatting along with the property.
   */
  public $privacy;

  /**
   * Stored publication type.
   */
  private $type;

  /**
   * Boolean indicating wether to retry the publication or not when publishing
   * on the user's timeline if the acting user does not have a valid access
   * token.
   */
  private $retry;

  function __construct($conf = array()) {
    if (!isset($conf['appId'])) {
      $conf['appId'] = variable_get('fb_autopost_app_id', '');
    }
    if (!isset($conf['secret'])) {
      $conf['secret'] = variable_get('fb_autopost_app_secret', '');
    }
    $this->destination = NULL;
    $this->retry = TRUE;
    // Delete any previously saved publication in the session.
    $this->getSession()->removePublication();
    parent::__construct($conf);
  }

  /**
   * Publishes content in the selected pages
   * @param $publication
   *   Keyed array containing the info for the publication. Must contain the
   *   keys:
   *     - 'type': The publication type as defined in $publication_types
   *     - 'params': Associative array with the necessary params for a
   *                 successful FB publication
   * @param $page_id
   *   Page id (among those already selected via UI).
   *   If this is present it will override the parameter destination.
   *   Use 'me' to publish to the user's timeline
   * @return string
   *   Facebook id string for the publication. Needed to edit, or delete the publication.
   * @throws FBAutopostException
   */
  function publish($publication, $page_id = NULL) {
    // $page_id parameter for backwards compatibility
    if (!is_null($page_id)) {
      $this->setDestination($page_id);
    }
    $page_id = $this->getDestination();
    // Override publication type if it's present in the publication array.
    if (!empty($publication['type'])) {
      try {
        $this->setType($publication['type']);
      }
      catch (FBAutopostException $e) {}
    }
    if (empty($this->type)) {
      throw new FBAutopostException(t('The publication array must contain publication type.'), FBAutopost::missing_param, WATCHDOG_ERROR);
    }
    // Error generation
    if (!isset($publication['params'])) {
      throw new FBAutopostException(t('The publication array must contain publication parameters.'), FBAutopost::missing_param, WATCHDOG_ERROR);
    }
    $this->checkPagesAvailability();
    // Now we can start the publication.
    try {
      return $this->publishOn($publication);
    }
    catch (FacebookApiException $e) {
      if ($this->getRetry()) {
        $session = $this->getSession();
        // If trying to publish on timeline for the first time
        if ($page_id == 'me' && !$session->isStored()) {
          // Here we can handle the unauthorized user error
          // To do so we save the relevant data in $_SESSION, then redirect to
          // Facebook athorization URL. This URL redirects back to a fixed
          // location that handles the publishing and redirects back to the
          // original location
          $publication['type'] = $this->getType();
          $session->storePublication($publication);
          $login_url = $this->getLoginUrl(array(
            'scope' => fb_permissions_get_facebook_permissions(),
            'redirect_uri' => url('fbautopost/authorization/retry', array('absolute' => TRUE)),
          ));
          // Redirect the user token the login URL that will redirect back to
          // the retry URI.
          // Remove the destination parameter, this is bad for redirections
          // outside Drupal.
          if (isset($_GET['destination'])) {
            unset($_GET['destination']);
          }
          drupal_goto($login_url);
          // Since there is a redirection the return statement is never
          // executed. Its presence is only for code clarity.
          return NULL;
        }
      }
      // Throw an exception
      throw new FBAutopostException(t('Facebook SDK threw an error: %error', array('%error' => $e)), FBAutopost::sdk_error, WATCHDOG_ERROR);
    }
  }

  /**
   * Publishes on a single destination.
   *
   * @param string $publication
   *   The publication as described in FBAutopost::publish()
   *
   * @return string
   *   The publication ID.
   * @see FBAutopost::publish()
   * @throws FacebookApiException
   */
  protected function publishOn($publication) {
    $this->publishParameterPrepare($publication);

    // Call api method from ancestor.
    $result = $this->api(
      // Post to destination on the selected endpoint.
      '/' . $this->getDestination() . '/' . $this->getEndpoint(),
      // This is fixed.
      'POST',
      // Add access token to the params.
      $publication['params']
    );

    module_invoke_all('fb_autopost_publication_publish', $publication, $result);

    return $result;
  }

  /**
   * Prepares the parameters to publish to Facebook, this means settings any
   * field or destination dependent configuration.
   */
  protected function publishParameterPrepare(&$publication) {
    $destination = $this->getDestination();
    if ($destination != 'me') {
      $pages = self::getPagesAccessTokens(variable_get('fb_autopost_account_id', 'me'), variable_get('fb_autopost_token', ''));
      $publication['params'] += array('access_token' => $pages[$destination]['access_token']);
    }
    else {
      // Privacy only makes sense when publishing to timeline
      if ($p = $this->getPrivacy()) {
        $publication['params'] += array('privacy' => $p);
      }
    }
  }

  /**
   * Get access tokens for publishing to several Facebook Pages
   *
   * @param $account_id
   *   User account asked for
   * @param $account_access_token
   *   Server side access token stored from the admin form.
   * @return
   *   A keyed array indexed by page id and containing the values:
   *     - 'id': Facebook page id
   *     - 'access_token': Access token for publishing to the page
   * @throws FBAutopostException
   */
  private static function getPagesAccessTokens($account_id, $account_access_token) {
    $pages = array();
    foreach (variable_get('fb_autopost_pages_access_tokens', array()) as $page_id => $page_access_token) {
      $pages[$page_id] = array(
        'id' => $page_id,
        'access_token' => $page_access_token,
      );
    }
    return $pages;
  }

  /**
   * Gets the reply given from Facebook when asking for user account.
   * @param $account_id
   *   User account asked for
   * @param $account_access_token
   *   Server side access token stored from the admin form.
   * @return
   *   The array from the Graph API call
   * @throws FBAutopostException
   */
  public function getPagesData($account_id, $account_access_token) {
    // Check if there is an access_token available.
    if (empty($account_access_token)) {
      throw new FBAutopostException(t('Cannot ask for user accounts without an access token.'), FBAutopost::missing_param, WATCHDOG_ERROR);
    }
    try {
      return $this->api('/' . $account_id . '/accounts', 'GET', array(
        'limit' => 999,
      ));

    } catch (FacebookApiException $e) {
      // Get the FacebookApiException and throw an ordinary
      // FBAutopostException
      throw new FBAutopostException(t('Facebook SDK threw an error: %error It is possible that your Facebook account cannot access the configured pages, if so please log in again in !url.', array('%error' => $e, '!url' => l(t('Facebook autopost configuration page'), 'admin/config/services/fbautopost'))), FBAutopost::sdk_error, WATCHDOG_ERROR);
    }
  }

  /**
   * Set the destination to publish to
   *
   * @param $destination
   * @return
   *   Returns itsef for chainable code.
   */
  public function setDestination($destination) {
    $this->destination = $destination;
    return $this;
  }

  /**
   * Gets the destination to publish to
   *
   * @return
   *   The destination parameter
   */
  public function getDestination() {
    return $this->destination;
  }

  /**
   * Deletes a publication from Facebook
   *
   * @param $publication_id
   *   The id that identifies the publication in Facebook
   * @throws FacebookApiException
   */
  public function remoteDelete($publication_id) {
    // TODO: Add a remoetDelete callback to the entity delete (optional)
    $this->api('/' . $publication_id, 'DELETE');
    // If request fails then an exception is thrown and there is no return value
    return TRUE;
  }

  /**
   * Edits a publication from Facebook
   *
   * @param $publication
   *   Keyed array containing the info for the publication. Must contain the
   *   keys:
   *     - 'type': The publication type as defined in $publication_types
   *     - 'params': Associative array with the necessary params for a
   *                 successful FB publication
   * @param $publication_id
   *   The id that identifies the publication in Facebook
   */
  public function remoteEdit($publication, $publication_id) {
    // In this case, edit means delete + publish. This has the side effect that
    // the ID is not preserved.
    if ($this->remoteDelete($publication)) {
      return $this->publish($publication);
    }
  }

  /**
   * Gets the session object for sugar syntax. Factory method.
   *
   * @return FBSession
   */
  public function getSession() {
    return new FBSession();
  }

  /**
   * Sets the privacy.
   *
   * @param string $privacy_code
   *   Privacy string as defined in Facebook documentation. Allowed values are:
   *     - EVERYONE
   *     - ALL_FRIENDS
   *     - FRIENDS_OF_FRIENDS
   *     - SELF
   * @return $this
   *   Returns itself.
   * @throws FBAutopostException
   * @see https://developers.facebook.com/docs/reference/api/privacy-parameter
   */
  public function setPrivacy($privacy_code) {
    if (!in_array($privacy_code, array('ALL_FRIENDS', 'EVERYONE', 'SELF', 'FRIENDS_OF_FRIENDS'))) {
      throw new FBAutopostException(t('The privacy code you specified is invalid.'), FBAutopost::incorrect_param, WATCHDOG_ERROR);
    }
    $this->privacy = $privacy_code;
    return $this;
  }

  /**
   * Gets the privacy value.
   *
   * @return string
   *   The privacy array formatted as array('value' => 'ALL_FRIENDS') …
   * @see https://developers.facebook.com/docs/reference/api/privacy-parameter/
   * @todo Better handling of the privacy
   */
  public function getPrivacy() {
    return isset($this->privacy) ? array('value' => $this->privacy) : NULL;
  }

  /**
   * Sets the type value.
   *
   * @param string $type
   *   The type as defined in FBAutopost::types.
   *
   * @throws FBAutopostException
   *
   * @return FBAutopost
   *   Returns itself
   */
  public function setType($type) {
    if (!in_array($type, array_keys($this->publication_types))) {
      throw new FBAutopostException(t('The publication type you specified is invalid.'), FBAutopost::incorrect_param, WATCHDOG_ERROR);
    }
    $this->type = $type;
    return $this;
  }

  /**
   * Gets the publication type.
   *
   * @return string
   *   The publication type.
   */
  protected function getType() {
    return $this->type;
  }

  /**
   * Throws an exception if the selected page id is not among the authorized
   * ones.
   */
  private function checkPagesAvailability() {
    $page_id = $this->getDestination();
    // Get fresh access tokens for the pages. We use the server side access
    // token and the account ID to retrieve them
    $pages = self::getPagesAccessTokens(variable_get('fb_autopost_account_id', 'me'), variable_get('fb_autopost_token', ''));
    // Check that the selected page is in the available list.
    if (!in_array($page_id, array_keys($pages)) && $page_id != 'me') {
      throw new FBAutopostException(t('Insufficient permissions to publish on page with id @id. Please check !config.', array('@id' => $page_id, '!config' => l(t('your configuration'), 'admin/config/services/fbautopost'))), FBAutopost::incorrect_param, WATCHDOG_ERROR);
    }
  }

  /**
   * Checks if a user has an active access token.
   *
   * @return boolean
   *   TRUE if the user may publish to the timeline.
   */
  public function hasActiveAccessToken() {
    $access_token = FALSE;
    foreach ($_SESSION as $key => $value) {
      if (preg_match("/fb_\d+_access_token/", $key)) {
        $access_token = $value;
        break;
      }
    }
    // This is to make sure the access token is not gotten from the app.
    return $this->getAccessToken() == $access_token;
  }

  /**
   * Sets the retry value.
   *
   * @param boolean $retry
   *   Boolean indicating if the publication in the timeline must be retried.
   *
   * @return FBAutopost
   *   Returns itself
   */
  public function setRetry($retry) {
    $this->retry = $retry;
    return $this;
  }

  /**
   * Gets the publication type.
   *
   * @return boolean
   *   The retry settings.
   */
  protected function getRetry() {
    return $this->retry;
  }
}
