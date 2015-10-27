<?php
/**
 * @file
 * Social Content Facebook class.
 */

class SocialContentFacebook extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {
    return t('Facebook');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'facebook';
  }

  /**
   * Get the source being used to get the rows i.e. account / hashtag.
   *
   * @return string
   *   The account being used to fetch the rows.
   */
  public function getSource() {
    return $this->settings['instance']['account'];
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
      'id' => 'field_facebook_external_id',
      'created' => 'created',
      'account' => '',
      'account_link' => '',
      'message' => 'body',
      'link' => 'field_facebook_link',
      'picture' => 'field_facebook_picture',
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
    $form = parent::globalSettingsForm($settings);

    $form['description'] = array(
      '#markup' => '<p>' . t('See !link', array(
        '!link' => l('developers.facebook.com', 'https://developers.facebook.com/apps'),
      )) . '</p>',
    );

    $form['graph_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook Graph URL'),
      '#description' => t('Do not include a trailing slash. If not sure, use %url', array(
        '%url' => 'https://graph.facebook.com',
      )),
      '#default_value' => isset($settings['graph_url']) ? $settings['graph_url'] : 'https://graph.facebook.com',
      '#required' => TRUE,
    );
    $form['access_token'] = array(
      '#type' => 'textfield',
      '#title' => t('Access Token'),
      '#description' => t('This is required to interact with Facebook. Use %url to generate one.', array(
        '%url' => 'https://graph.facebook.com/oauth/access_token?client_id=APP_ID&client_secret=APP_SECRET&&grant_type=client_credentials',
      )),
      '#default_value' => isset($settings['access_token']) ? $settings['access_token'] : NULL,
      '#maxlength' => 300,
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

    $form['account'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook Page User ID'),
      '#description' => t('You can use !link to get this. Or leave empty and it will be generated using the Page Name.', array(
        '!link' => l('findmyfacebookid.com', 'http://findmyfacebookid.com/'),
      )),
      '#default_value' => isset($settings['account']) ? $settings['account'] : NULL,
    );
    $form['page_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook Page Name'),
      '#description' => t("E.g. 'cocacola' for http://facebook.com/cocacola page."),
      '#default_value' => isset($settings['page_name']) ? $settings['page_name'] : NULL,
      '#required' => TRUE,
    );
    $form['min_resolution'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum image resolution'),
      '#description' => t('Only posts that have images that meet the minimum image resolution (in {width}x{height} format) will be imported.'),
      '#default_value' => isset($settings['min_resolution']) ? $settings['min_resolution'] : NULL,
      '#required' => FALSE,
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

    $global_settings = $this->settings['global'];

    // Look for the Facebook Page id.
    if (!empty($settings['page_name'])) {
      $url = $global_settings['graph_url'] . '/' . $settings['page_name'];
      $result = $this->httpRequest(url($url, array('query' => array('access_token' => $global_settings['access_token']), 'external' => TRUE)));
      if ($result->code == 200) {
        $data = json_decode($result->data);
        if (!empty($data->id)) {
          $settings['account'] = $data->id;
        }
      }
    }
    return parent::saveInstanceSettings($settings);
  }

  /**
   * Get the rows to import.
   *
   * @param mixed $last_id
   *   The id of the last import.
   *
   * @return bool|object
   *   The data object, or FALSE on error.
   */
  public function getRows($last_id = NULL) {

    $settings = $this->settings['instance'];
    $global_settings = $this->settings['global'];

    $params = array(
      'access_token' => $global_settings['access_token'],
      'fields' => 'message,id,created_time,from,full_picture',
    );
    if (!empty($settings['limit'])) {
      $params['limit'] = $settings['limit'];
    }
    $url = url($global_settings['graph_url'] . '/' . $settings['account'] . '/posts', array('query' => $params, 'external' => TRUE));
    $result = $this->httpRequest($url);
    if ($result->code == 200) {
      $posts = json_decode($result->data);
      return $posts->data;
    }
    else {
      watchdog('social_content_facebook', 'Error fetching feed, data: %data', array('%data' => $result->data), WATCHDOG_WARNING);
      return FALSE;
    }
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {
    // Only add direct wall posts.
    if (isset($row->to)) {
      return FALSE;
    }

    // Dicard updates without content.
    if (!isset($row->message)) {
      return FALSE;
    }

    $global_settings = $this->settings['global'];

    $id_parts = explode('_', $row->id);
    $row->id = $id_parts[1];
    $row->link = 'http://www.facebook.com/' . $row->from->id . '/posts/' . $row->id;
    $row->created = strtotime($row->created_time);
    $row->account = $row->from->name;
    $row->account_link = 'http://www.facebook.com/' . $row->from->id;
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    if (!empty($row->full_picture)) {
      $image_url = $this->getImageUrl($row, $global_settings);
      if ($image_url) {
        $mappings = $this->getFieldMappings();
        $picture = $this->saveExternalFile($image_url, $mappings['picture']);
      }
    }

    // For legacy reasons we store this in $row->picture.
    // Facebook changed where they stored this field.
    // @See https://www.drupal.org/node/2559873
    $row->picture = !empty($picture) ? $picture : NULL;

    return TRUE;
  }

  /**
   * Get the image url from a given row.
   *
   * @param object $row
   *   The url of the image.
   * @param string $settings
   *   The settings for this instance.
   *
   * @return bool|string
   *   The URL, or FALSE on error.
   */
  protected function getImageUrl($row, $settings) {
    if (isset($row->full_picture)) {
      $picture_url = $row->full_picture;
    }
    if (isset($row->object_id)) {
      $picture_url = $settings['graph_url'] . '/' . $row->object_id . '/picture';
    }
    else {
      $picture_url_parts = drupal_parse_url($row->full_picture);
      if (isset($picture_url_parts['query']['url'])) {
        $picture_url = $picture_url_parts['query']['url'];
      }
      if (isset($picture_url_parts['query']['src'])) {
        $picture_url = $picture_url_parts['query']['src'];
      }
    }

    if (!isset($picture_url)) {
      return FALSE;
    }
    return $picture_url;
  }
}
