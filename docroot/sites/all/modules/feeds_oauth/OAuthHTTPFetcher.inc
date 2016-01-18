<?php

/**
 * @file
 * Definition of the import batch object created on the fetching stage by
 * OAuthHTTPFetcher.
 */

class OAuthHTTPFetcherResult extends FeedsFetcherResult {
  protected $url;
  protected $authenticator;
  protected $consumer_key;
  protected $consumer_secret;
  protected $id;
  protected $site_id;
  protected $method;
  protected $uid;
  protected $two; // OAuth2 ?

  /**
   * Constructor.
   */
  public function __construct($url, $authenticator, $consumer_key, $consumer_secret, $id, $site_id, $method, $uid, $two = FALSE) {
    $this->url = $url;
    $this->authenticator = $authenticator;
    $this->consumer_key = $consumer_key;
    $this->consumer_secret = $consumer_secret;
    $this->id = $id;
    $this->site_id = $site_id;
    $this->method = $method;
    $this->uid = $uid;
    $this->two = $two;
    parent::__construct('');
  }

  /**
   * Implementation of FeedsImportBatch::getRaw().
   */
  public function getRaw() {
    // Get access token.
    $access_token = call_user_func($this->authenticator, $this->uid, $this->site_id, $this->id);
    if (empty($access_token)) {
      watchdog('feeds_oauth', 'Authenticator %authenticator returned empty access token for uid %uid, site %site. Aborting.', array(
        '%authenticator' => $this->authenticator,
        '%uid' => $this->uid,
        '%site' => $this->site_id,
      ), WATCHDOG_WARNING);
      return array();
    }

    // Instantiate OAuth client.
    $path = variable_get('feeds_oauth_library_path', FEEDS_OAUTH_LIBRARY_PATH_DEFAULT); 
    if ($this->two) {
      require_once $path . '/lib/oauth/OAuth2Client.php';
      $oauth = new proauth\OAuth2CurlClient(new proauth\OAuth2AccessToken($access_token['oauth_token']));
    }
    else {
      require_once $path . '/lib/oauth/OAuthClient.php';
      $oauth = new proauth\OAuthCurlClient(
        new proauth\OAuthConsumer($this->consumer_key, $this->consumer_secret),
        new proauth\OAuthSignatureHMACSHA1(),
        new proauth\OAuthToken($access_token['oauth_token'], $access_token['oauth_token_secret'])
      );
    }

    // Make the call.
    $parsed_url = parse_url($this->url);
    $query = array();
    if (!empty($parsed_url['query'])) {
      parse_str($parsed_url['query'], $query);
    }
    if ($this->two) {
      $query += array('access_token' => $oauth->getAccessToken()->getToken());
    }
    try {
      if ($this->method == 'post') {
        $request = $oauth->createPostRequest($this->url, $query);
      }
      else {
        $request = $oauth->createGetRequest($this->url, $query);
      }
      $response = $oauth->executeRequest($request);
      if ($response->getStatusCode() == 200) {
        return $response->getBody();
      }
      else {
        watchdog('feeds_oauth', print_r($response->getBody(), TRUE), array(), WATCHDOG_ERROR);
      }
    }
    catch (Exception $e) {
      watchdog('feeds_oauth', $e->getMessage(), array(), WATCHDOG_ERROR);
    }
    return array();
  }
}

/**
 * Support OAuth authentication.
 */
class OAuthHTTPFetcher extends FeedsHTTPFetcher {
  protected $two; // OAuth2 ?

  public function __construct($url = NULL) {
    parent::__construct($url);
    $this->two = $this instanceof OAuth2HTTPSFetcher;
  }

  /**
   * Use signed URL to fetch content.
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    global $user;
    return new OAuthHTTPFetcherResult(
      trim($source_config['source']),
      $this->config['authenticator'],
      $this->config['consumer_key'],
      $this->config['consumer_secret'],
      $this->id,
      $this->config['site_id'],
      $this->config['method'],
      $this->getAuthenticatedUser($source),
      $this->two
    );
  }

  /**
   * Declare defaults.
   */
  public function configDefaults() {
    global $user;
    return array(
      'authenticator' => 'feeds_oauth_get_tokens',
      'site_id' => '',
      'consumer_key' => '',
      'consumer_secret' => '',
      'request_token_url' => '',
      'access_token_url' => '',
      'authorize_url' => '',
      'method' => 'post',
      'uid' => $user->uid,
    ) + parent::configDefaults();
  }

  /**
   * Add form options.
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['use_pubsubhubbub'] = array('#type' => 'value', '#value' => FALSE);
    $form['authenticator'] = array(
      '#type' => 'select',
      '#title' => t('OAuth authenticator'),
      '#default_value' => $this->config['authenticator'],
      '#options' => module_invoke_all('feeds_oauth_authenticator'),
      '#description' => t('Choose the authentication module that provides the needed OAuth tokens.'),
    );
    $form['uid'] = array(
      '#type' => 'select',
      '#title' => t('Authenticating user'),
      '#description' => t('OAuth access tokens will be retrieved for this user when fetching the feed. 
                           Select "- None -" to use the source node\'s owner (in the case of using a source node)
                           or the current user (in the case of standalone source form).'
      ),
      '#default_value' => $this->config['uid'],
      '#options' => array(NULL => t('- None -')) + db_query("SELECT uid, name FROM {users} WHERE status = 1 and uid != 0")->fetchAllKeyed(),
    );
    $form['site_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Site identifier'),
      '#default_value' => $this->config['site_id'],
      '#description' => t('Internal identifier for this connection. Should only contain alphanumeric characters and hyphens. For the current configuration, callback URL will be: <code class="site-id">%url</code>',
        array('%url' => url('feeds/oauth' . ($this->two ? '2' : '') .'/callback/' . $this->config['site_id'], array('absolute' => TRUE)))
      ),
      '#required' => TRUE,
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'feeds_oauth') . '/feeds_oauth.js',
        ),
      ),
    );
    $form['consumer_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Consumer key'),
      '#default_value' => $this->config['consumer_key'],
      '#required' => TRUE,
    );
    $form['consumer_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Consumer secret'),
      '#default_value' => $this->config['consumer_secret'],
      '#required' => TRUE,
    );
    if (!$this->two) {
      $form['request_token_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Request token URL'),
        '#default_value' => $this->config['request_token_url'],
        '#required' => TRUE,
      );
    }
    $form['access_token_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Access token URL'),
      '#default_value' => $this->config['access_token_url'],
      '#required' => TRUE,
    );
    $form['authorize_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Authorize URL'),
      '#default_value' => $this->config['authorize_url'],
      '#required' => TRUE,
    );
    $form['method'] = array(
      '#type' => 'select',
      '#title' => t('Method'),
      '#default_value' => $this->config['method'],
      '#options' => array('get' => 'GET', 'post' => 'POST'),
    );

    return $form;
  }

  /**
   * Validate config.
   */
  public function configFormValidate(&$values) {
    $values['site_id'] = trim($values['site_id']);
    if (!preg_match('/^[\w-]*$/', $values['site_id'])) {
      form_set_error('site_id', t('Site identifier must contain alphanumerics and hyphens only.'));
    }
  }

  /**
   * Get authenticated user.
   */
  protected function getAuthenticatedUser($source) {
    global $user;
    $source_node = node_load(@$source->feed_nid);
    return !empty($this->config['uid']) ? $this->config['uid'] : (
      $source_node ? $source_node->uid : $user->uid
    );
  }

  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
    $form = parent::sourceForm($source_config);
    if (empty($source_config)) return $form;

    // FIXME Ugly hack to get caller FeedsSource object. We know a FeedsSource object will call this method.
    $trace = debug_backtrace();
    $source = $trace[1]['object'];

    // Check existence of access tokens.
    $uid = $this->getAuthenticatedUser($source);
    $access_tokens = call_user_func($this->config['authenticator'], $uid, $this->config['site_id'], $this->id);
    if (empty($access_tokens) && !empty($this->config['site_id'])) {
      global $user;
      if ($uid === $user->uid) { // user can request new access token
        drupal_set_message(t('Could not find OAuth access tokens for site %site. 
          You should <a href="@url">authenticate first</a> to access protected information.',
          array('%site' => $this->config['site_id'], '@url' => url('feeds/oauth' . ($this->two ? '2' : '') . '/authenticate/' . $this->id))
        ), 'warning');
      }
      else { // another user was selected, issue warning
        $authenticating_user = user_load($uid);
        drupal_set_message(t('Could not find OAuth access tokens for site %site.
          In addition, the authenticating user for this feed is set to be %username, 
          so you cannot import this feed until %username authenticates to %site.',
          array('%site' => $this->config['site_id'], '%username' => $authenticating_user->name)
        ), 'warning');
      }
    }
    return $form;
  }
}
