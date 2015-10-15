<?php
/**
 * @file
 * Social Content Instagram class.
 */

class SocialContentInstagram extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {

    if (isset($this->settings['instance']) && !empty($this->settings['instance']['type'])) {

      return t('Instagram (@type)', array(
        '@type' => ucfirst($this->settings['instance']['type']),
      ));
    }

    return t('Instagram');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'instagram';
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
   * Fields to save from the row.
   *
   * Get fields to save.
   *
   * @return array
   *   List of fields to save.
   */
  public function fields() {
    return array(
      'id' => 'field_instagram_external_id',
      'created' => 'created',
      'user_id' => '',
      'user_name' => '',
      'user_link' => '',
      'caption' => 'body',
      'link' => 'field_instagram_link',
      'picture' => 'field_instagram_picture',
      'video' => 'field_instagram_video',
      'video_url' => 'field_instagram_video_url',
      'hashtag' => 'field_instagram_hashtag',
    ) + parent::fields();
  }

  /**
   * The shared global settings form for all Instagram instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {
    $settings = $this->settings['global'];
    $token_generator_link = isset($settings['client_id']) ? l(t('Get Access Token'), $this->getAccessTokenUrl()) : '';
    if (isset($_GET['code']) && empty($_POST)) {
      $settings['access_token'] = $this->getAccessToken($settings, $_GET['code']);
      if ($settings['access_token']) {
        drupal_set_message(t('Access token has been generated, please save the form.'));
      }
    }

    $form = parent::globalSettingsForm();

    $form['description'] = array(
      '#markup' => '<p>' . t('See !link', array(
          '!link' => l('instagram.com/developer', 'https://instagram.com/developer/clients/manage/'),
        )) . '</p>',
    );

    $form['access_token'] = array(
      '#type' => 'textfield',
      '#title' => t('Access Token'),
      '#default_value' => isset($settings['access_token']) ? $settings['access_token'] : NULL,
      '#description' => $token_generator_link ? $token_generator_link : t('Save this form to get the Access Token.'),
      '#maxlength' => 300,
    );
    $redirect_uri = isset($settings['redirect_uri']) ? $settings['redirect_uri'] : current_path();
    $form['redirect_uri'] = array(
      '#type' => 'textfield',
      '#title' => t('Redirect path'),
      '#default_value' => $redirect_uri,
      '#description' => t('Used when generating access token. Leave alone if not sure.') . '<br/>' .
        t('You will need to specify this URL (!url) in your <a href="!instagram_app">Instagram app</a>.', array(
          '!url' => url($redirect_uri, array('absolute' => TRUE)),
          '!instagram_app' => 'https://instagram.com/developer/clients/manage/',
        )
      ),
    );
    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => t('API URL'),
      '#description' => t('Do not include a trailing slash. If not sure, use %url', array(
        '%url' => 'https://api.instagram.com',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : 'https://api.instagram.com',
      '#required' => TRUE,
    );
    $form['api_version'] = array(
      '#type' => 'textfield',
      '#title' => t('API version'),
      '#description' => t("i.e. 'v1'."),
      '#default_value' => isset($settings['api_version']) ? $settings['api_version'] : 'v1',
      '#required' => TRUE,
    );
    $form['client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Client ID'),
      '#default_value' => isset($settings['client_id']) ? $settings['client_id'] : NULL,
      '#required' => TRUE,
    );
    $form['client_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Client secret'),
      '#default_value' => isset($settings['client_secret']) ? $settings['client_secret'] : NULL,
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
      '#required' => TRUE,
      '#attributes' => array('class' => array('social-content-instagram-type')),
    );
    $form['user_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram User ID'),
      '#description' => t("The Instagram user id to pull the statuses from. E.g. '235448524'. Use !link to get it.", array(
        '!link' => l('jelled.com/instagram/lookup-user-id', 'http://jelled.com/instagram/lookup-user-id'),
      )),
      '#default_value' => isset($settings['user_id']) ? $settings['user_id'] : NULL,
      '#states' => array(
        'visible' => array(
          '.social-content-instagram-type' => array('value' => 'account'),
        ),
      ),
      '#element_validate' => array('element_validate_integer_positive'),
    );
    $form['hashtag'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram hashtag'),
      '#description' => t("The Instagram #hashtag to pull the statuses from. Leading '#' is not required."),
      '#default_value' => isset($settings['hashtag']) ? $settings['hashtag'] : NULL,
      '#states' => array(
        'visible' => array(
          '.social-content-instagram-type' => array('value' => 'hashtag'),
        ),
      ),
    );
    $form['min_resolution'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum image resolution'),
      '#description' => t('Only posts that have images that meet the minimum image resolution (in {width}x{height} format) will be imported.'),
      '#default_value' => isset($settings['min_resolution']) ? $settings['min_resolution'] : NULL,
    );

    $form['include'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Only include'),
      '#options' => array('image' => t('Image'), 'video' => t('Video')),
      '#description' => t('Restrict what type of Instagram posts should be imported. Leave empty to import all post types.'),
      '#default_value' => isset($settings['include']) ? $settings['include'] : array(),
    );
    return $form;
  }

  /**
   * Save instance settings.
   *
   * @param array $settings
   *   The settings to save.
   */
  public function saveInstanceSettings($settings) {

    if ($settings['type'] == 'account') {
      $global_settings = $this->settings['global'];
      // Lets lookup the users friendly name.
      $params = array('access_token' => $global_settings['access_token']);
      $url = $global_settings['api_url'] . '/' . $global_settings['api_version'] . '/users/' . $settings['user_id'];
      $result = $this->httpRequest(url($url, array('query' => $params, 'external' => TRUE)));
      if ($result->code == 200) {

        $settings['account'] = json_decode($result->data)->data->username;
      }
    }
    return parent::saveInstanceSettings($settings);
  }

  /**
   * Different types of Instagram instances.
   */
  protected function getImportTypes() {
    return array(
      'account' => t('Pull from account'),
      'hashtag' => t('Pull from hashtag'),
    );
  }

  /**
   * Get the rows to import.
   *
   * @param mixed $last_id
   *   The id of the last import.
   *
   * @return array
   *   Array with the rows.
   */
  public function getRows($last_id = NULL) {

    $settings = $this->settings['instance'];
    $global_settings = $this->settings['global'];

    $rows = array();

    if (empty($global_settings['access_token'])) {
      drupal_set_message(t('Error: Access Token is empty.'), 'error');
    }
    else if ($settings['type'] == 'hashtag') {
      $rows = $this->getRowsHashtag($settings, $global_settings, $last_id);
    }
    elseif ($settings['type'] == 'account') {
      $rows = $this->getRowsAccount($settings, $global_settings, $last_id);
    }

    return $rows;
  }

  /**
   * Get rows from a Instagram user_id.
   *
   * @param array $settings
   *   The settings to use to get the Instagram content.
   * @param array $global_settings
   *   The global settings for Instagram.
   * @param mixed $last_id
   *   The id of the last imported Instagram post.
   *
   * @return mixed
   *   Array of Instagram items or FALSE on error.
   */
  protected function getRowsAccount($settings, $global_settings, $last_id) {

    if (!empty($settings['user_id'])) {

      $params['access_token'] = $global_settings['access_token'];
      $params['user_id'] = $settings['user_id'];

      if ($last_id) {
        $params['min_id'] = $last_id;
      }
      if (!empty($settings['limit'])) {
        $params['count'] = $settings['limit'];
      }
      $url = $global_settings['api_url'] . '/' . $global_settings['api_version'] . '/users/' . $settings['user_id'] . '/media/recent/';
      $result = $this->httpRequest(url($url, array('query' => $params, 'external' => TRUE)));
      if ($result->code == 200) {
        $posts = json_decode($result->data);
        return $posts->data;
      }
      else {
        watchdog('social_content_instagram', 'Error fetching feed, data: %data', array('%data' => $result->data), WATCHDOG_WARNING);
        return FALSE;
      }
    }
    return FALSE;
  }

  /**
   * Get rows from a list of hashtags.
   *
   * @param array $settings
   *   The settings to use to get the Instagram content.
   * @param array $global_settings
   *   The global settings for Instagram.
   * @param mixed $last_id
   *   The id of the last imported Instagram posts.
   *
   * @return mixed
   *   Array of Instagram posts or FALSE on error.
   */
  protected function getRowsHashtag($settings, $global_settings, $last_id = NULL) {

    if (!empty($settings['hashtag'])) {

      $params = array(
        'access_token' => $global_settings['access_token'],
      );

      if ($last_id) {
        $params['min_tag_id'] = $last_id;
      }

      if (!empty($settings['limit'])) {
        $params['count'] = $settings['limit'];
      }

      $posts = array();

      $hashtags = explode(' ', $settings['hashtag']);

      // Explode hashtags by space and request images for each hashtag.
      foreach ($hashtags as $hashtag) {
        $url = $global_settings['api_url'] . '/' . $global_settings['api_version'] . '/tags/' . trim($hashtag, '#') . '/media/recent/?' . drupal_http_build_query($params);
        $result = $this->httpRequest($url);
        if ($result->code == 200) {
          $result_data = json_decode($result->data);
          foreach ($result_data->data as &$row) {
            $row->hashtag = $hashtag;
          }
          $posts = array_merge($posts, $result_data->data);
        }
        else {
          watchdog('social_content_instagram', 'Error fetching feed, data: %data', array('%data' => $result->data), WATCHDOG_WARNING);
          return FALSE;
        }
      }
      return $posts;
    }
    return FALSE;
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {

    $settings = $this->settings['instance'];

    if (!empty($settings['include']['image']) && !isset($row->images->standard_resolution->url)) {
      watchdog('social_content_instagram', 'Missing image URL on Instagram Import', array(), WATCHDOG_WARNING);
    }
    if (!empty($settings['include']['video']) && empty($settings['include']['image']) && !isset($row->videos)) {
      // No point in continuing if we are only meant to import videos but the
      // current record does not have any videos.
      watchdog('social_content_instagram', 'Missing video URL on Instagram Import', array(), WATCHDOG_WARNING);
      return FALSE;
    }

    $mappings = $this->getFieldMappings();
    $row->created = $row->created_time;
    $row->user_id = $row->user->id;
    $row->user_name = $row->user->username;
    $row->user_link = 'http://instagram.com/' . $row->user_name;

    $row->caption = isset($row->caption) ? $row->caption->text : '';
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Save the picture if we need to.
    if ($mappings['picture']) {
      $validators = array();
      if (!empty($settings['min_resolution'])) {
        $validators = array(
          'file_validate_image_resolution' => array(0, $settings['min_resolution']),
        );
      }
      $picture = $this->saveExternalFile($row->images->standard_resolution->url, $mappings['picture'], $validators);
      $row->picture = !empty($picture) ? $picture : NULL;
    }

    // Save the video if we need to.
    if (isset($row->videos) && $mappings['video']) {
      $video_url = isset($row->videos->standard_resolution) ? $row->videos->standard_resolution->url : $row->videos->low_resolution->url;
      $video = $this->saveExternalFile($video_url, $mappings['video']);
      $row->video = !empty($video) ? $video : NULL;
    }
    // Save the video if we need to.
    if (isset($row->videos) && $mappings['video_url']) {
      $row->video_url = isset($row->videos->standard_resolution) ? $row->videos->standard_resolution->url : $row->videos->low_resolution->url;
    }
  }

  /**
   * Get url to request an access token.
   *
   * @return string
   *   The url to get an access token.
   */
  public function getAccessTokenUrl() {

    $global_settings = $this->settings['global'];

    $query = array(
      'client_id' => $global_settings['client_id'],
      'redirect_uri' => url($global_settings['redirect_uri'], array('query' => array(drupal_get_destination()), 'absolute' => TRUE)),
      'response_type' => 'code',
    );
    return url($global_settings['api_url'] . '/oauth/authorize', array('query' => $query, 'external' => TRUE));
  }

  /**
   * Get access token from Instagram.
   *
   * @param array $settings
   *   Settings used to get access token.
   * @param string $code
   *   Code from the authorize request.
   *
   * @return string|NULL
   *   The token code, or NULL on error.
   */
  public function getAccessToken($settings, $code) {

    $fields = array(
      'client_id' => $settings['client_id'],
      'client_secret' => $settings['client_secret'],
      'grant_type' => 'authorization_code',
      'redirect_uri' => url($settings['redirect_uri'], array('query' => array(drupal_get_destination()), 'absolute' => TRUE)),
      'code' => $code,
    );
    $result = drupal_http_request($settings['api_url'] . '/oauth/access_token', array('method' => 'POST', 'data' => drupal_http_build_query($fields)));
    $data = json_decode($result->data);
    if ($result->code != 200) {
      drupal_set_message(t('Error: @error', array('@error' => $result->data)), 'error');
      return NULL;
    }
    else {
      return $data->access_token;
    }
  }

}
