<?php
/**
 * @file
 * Social Content Twitter class.
 */

class SocialContentTwitter extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {

    if (isset($this->settings['instance']) && !empty($this->settings['instance']['type'])) {

      return t('Twitter (@type)', array(
        '@type' => ucfirst($this->settings['instance']['type']),
      ));
    }

    return t('Twitter');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'twitter';
  }

  /**
   * Get the source being used to get the rows i.e. account / hashtag.
   *
   * @return string
   *   The hashtag / account being used to fetch the rows.
   */
  public function getSource() {
    return $this->settings['instance']['type'] == 'account' ? $this->settings['instance']['account'] : $this->settings['instance']['hashtag'];
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {
    $settings = $this->settings['instance'];
    // Check if we are dealing with a retweet and whether we should import it.
    if (isset($row->retweeted_status) && !empty($settings['exclude_retweets'])) {
      return FALSE;
    }
    elseif (isset($row->retweeted_status)) {
      // If we do need import retweets then get the full tweet.
      $start = strpos($row->text, ':');
      if ($start) {
        $row->text = substr($row->text, 0, $start + 1) . ' ' . $row->retweeted_status->text;
      }
    }

    $mappings = $this->getFieldMappings();
    $row->id = $row->id_str;
    $row->created = strtotime($row->created_at);
    $row->account = $row->user->screen_name;
    $row->name = $row->user->name;
    $row->link = 'http://twitter.com/' . $row->user->screen_name . '/status/' . $row->id;
    $row->account_link = 'http://twitter.com/' . $row->user->screen_name;
    $filter = filter_format_exists('tweet') ? 'tweet' : 'filtered_html';
    $row->text = array('value' => $row->text, 'format' => $filter);
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if (isset($mappings['user_picture'])) {
      $picture = $this->saveExternalFile($row->user->profile_image_url, $mappings['user_picture']);
      $row->user_picture = !empty($picture) ? $picture : NULL;
    }

    if (isset($mappings['media'])) {
      if (isset($row->entities->media)) {
        foreach ($row->entities->media as $item) {
          if (isset($item->media_url)) {
            $picture = $this->saveExternalFile($item->media_url, $mappings['media']);
            $row->media = !empty($picture) ? $picture : NULL;
            if ($picture) {
              break;
            }
          }
        }
      }
    }
  }

  /**
   * Fields to save from the row.
   *
   * Get fields to save.
   *
   * @return array
   *   List of fields to save.
   */
  public function fields() {
    return array(
      'id' => 'field_tweet_external_id',
      'created' => 'created',
      'text' => 'body',
      'hashtag' => 'field_tweet_hashtag',
      'account' => 'field_tweet_user',
      'account_link' => 'field_tweet_user_link',
      'link' => 'field_tweet_link',
      'media' => 'field_tweet_media',
      'user_picture' => 'field_tweet_user_picture',
      'name' => 'field_tweet_user_name',
    ) + parent::fields();
  }

  /**
   * The shared global settings form fro all twitter instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {

    $settings = $this->settings['global'];
    $form = parent::globalSettingsForm($settings);

    $form['description'] = array(
      '#markup' => '<p>' . t('See !link', array(
          '!link' => l('apps.twitter.com', 'https://apps.twitter.com/'),
        )) . '</p>',
    );

    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => t('API URL'),
      '#description' => t('Do not include trailing slash. Example: !url', array(
        '!url' => 'https://api.twitter.com/1.1',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : 'https://api.twitter.com/1.1',
      '#required' => TRUE,
    );

    $form['oauth_consumer_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Consumer Key (API Key)'),
      '#default_value' => isset($settings['oauth_consumer_key']) ? $settings['oauth_consumer_key'] : NULL,
      '#required' => TRUE,
    );
    $form['oauth_consumer_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Consumer Secret (API Secret)'),
      '#default_value' => isset($settings['oauth_consumer_secret']) ? $settings['oauth_consumer_secret'] : NULL,
      '#required' => TRUE,
    );
    $form['oauth_token'] = array(
      '#type'  => 'textfield',
      '#title' => t('Access Token'),
      '#default_value' => isset($settings['oauth_token']) ? $settings['oauth_token'] : NULL,
      '#required' => TRUE,
    );
    $form['oauth_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Access Token Secret'),
      '#default_value' => isset($settings['oauth_secret']) ? $settings['oauth_secret'] : NULL,
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Instance settings form.
   *
   * @return array
   *   Any instance settings that will be included on all
   *    instance forms for the current global.
   */
  public function instanceSettingsForm() {

    $settings = $this->settings['instance'];
    $form = parent::instanceSettingsForm($settings);

    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Import'),
      '#options' => $this->getImportTypes(),
      '#description' => t('What should be imported.'),
      '#default_value' => isset($settings['type']) ? $settings['type'] : NULL,
      '#attributes' => array('class' => array('social-content-twitter-type')),
    );
    $form['account'] = array(
      '#type' => 'textfield',
      '#title' => t('Twitter account'),
      '#description' => t("The Twitter @username to pull the statuses from. Do not include leading '@'."),
      '#default_value' => isset($settings['account']) ? $settings['account'] : NULL,
      '#states' => array(
        'visible' => array(
          '.social-content-twitter-type' => array('value' => 'account'),
        ),
      ),
    );
    $form['hashtag'] = array(
      '#type' => 'textfield',
      '#title' => t('Twitter hashtag'),
      '#description' => t("The Twitter #hashtag to pull the statuses from. Include leading '#'."),
      '#default_value' => isset($settings['hashtag']) ? $settings['hashtag'] : NULL,
      '#states' => array(
        'visible' => array(
          '.social-content-twitter-type' => array('value' => 'hashtag'),
        ),
      ),
    );
    $form['exclude_retweets'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude retweets'),
      '#description' => t('If checked, retweets will not be imported.'),
      '#default_value' => isset($settings['exclude_retweets']) ? $settings['exclude_retweets'] : NULL,
    );

    return $form;
  }

  /**
   * Different types of twitter instances.
   */
  protected function getImportTypes() {
    return array(
      'account' => t('Tweets from account'),
      'hashtag' => t('Tweets from hashtag'),
    );
  }
  /**
   * Get the rows to import.
   *
   * @param mixed $last_id
   *   The id of the last import.
   *
   * @return array
   *   Array of rows.
   */
  public function getRows($last_id = NULL) {

    $settings = $this->settings['instance'];
    $global_settings = $this->settings['global'];

    $rows = array();
    if ($settings['type'] == 'hashtag') {
      $rows = $this->getRowsHashtag($settings, $global_settings, $last_id);
    }
    elseif ($settings['type'] == 'account') {
      $rows = $this->getRowsAccount($settings, $global_settings, $last_id);
    }
    return $rows;
  }

  /**
   * Get rows from a Twitter account.
   *
   * @param array $settings
   *   The settings to use to get the tweets.
   * @param array $global_settings
   *   The global settings for Twitter.
   * @param mixed $last_id
   *   The id of the last imported tweet.
   *
   * @return mixed
   *   Array of tweets or FALSE on error.
   */
  protected function getRowsAccount($settings, $global_settings, $last_id) {
    $params = array(
      'screen_name' => $settings['account'],
      'exclude_replies' => TRUE,
      'count' => $settings['limit'],
    );
    if ($last_id) {
      $params['since_id'] = $last_id;
    }
    $data = $this->request($settings, $global_settings, 'statuses/user_timeline.json', $params);
    if ($data) {
      return $data;
    }
    return FALSE;
  }

  /**
   * Get rows from a list of hashtags.
   *
   * @param array $settings
   *   The settings to use to get the tweets
   * @param array $global_settings
   *   The global settings for Twitter.
   * @param mixed $last_id
   *   The id of the last imported tweet.
   *
   * @return mixed
   *   Array of tweets or FALSE on error.
   */
  protected function getRowsHashtag($settings, $global_settings, $last_id = NULL) {

    $hashtags = explode(' ', $settings['hashtag']);

    $rows = array();
    foreach ($hashtags as $hashtag) {
      $params = array(
        'q' => $hashtag,
        'count' => $settings['limit'],
      );
      if ($last_id) {
        $params['since_id'] = $last_id;
      }
      $data = $this->request($settings, $global_settings, 'search/tweets.json', $params);
      if ($data && isset($data->statuses) && is_array($data->statuses)) {
        foreach ($data->statuses as &$row) {
          $row->hashtag = $hashtag;
        }
        $rows = array_merge($rows, $data->statuses);
      }
      else {
        return FALSE;
      }
    }
    return $rows;
  }

  /**
   * Make a request out to Twitter.
   *
   * @param array $global_settings
   *   The settings to use to use.
   * @param string $endpoint
   *   The twitter api endpoint.
   * @param array $params
   *   parameters to use in the request.
   *
   * @return mixed
   *   The feed returned by Twitter.
   */
  public function request($settings, $global_settings, $endpoint, $params) {

    $endpoint = $global_settings['api_url'] . '/' . $endpoint;

    $oauth = array(
      'consumerKey' => $global_settings['oauth_consumer_key'],
      'consumerSecret' => $global_settings['oauth_consumer_secret'],
      'oauthToken' => $global_settings['oauth_token'],
      'oauthSecret' => $global_settings['oauth_secret'],
    );
    $consumer = new OAuthConsumer($oauth['consumerKey'], $oauth['consumerSecret'], NULL);
    $token = new OAuthConsumer($oauth['oauthToken'], $oauth['oauthSecret']);

    $oauth_request = OAuthRequest::from_consumer_and_token($consumer, $token, "GET", $endpoint, $params);
    $oauth_request->sign_request(new OAuthSignatureMethod_HMAC_SHA1(), $consumer, $token);
    $result = $this->httpRequest($oauth_request->to_url());
    if ($result->code == 200) {
      return json_decode($result->data);
    }
    else {
      watchdog('social_content_twitter', 'Error fetching feed, data: %data', array('%data' => $result->data), WATCHDOG_WARNING);
      return FALSE;
    }
  }
}
