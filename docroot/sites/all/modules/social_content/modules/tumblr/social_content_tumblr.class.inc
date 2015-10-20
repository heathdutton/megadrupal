<?php
/**
 * @file
 * Social Content Tumblr class.
 */

class SocialContentTumblr extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {

    return t('Tumblr');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {

    return 'tumblr';
  }

  /**
   * Get the source being used to get the rows.
   *
   * @return string
   *   The source used to fetch the rows.
   */
  public function getSource() {

    return $this->settings['instance']['host_name'];
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
      'id' => NULL,
      'timestamp' => 'created',
      'body' => 'body',
      'short_url' => 'field_tumblr_link',
      'title' => NULL,
      'picture' => 'field_tumblr_picture',
      'blog_name' => NULL,
      'blog_title' => 'field_tumblr_blog_title',
      'blog_link' => 'field_tumblr_blog_link',
      'video_url' => 'field_tumblr_video_link',
      'video_embed' => 'field_tumblr_video_embed',
    ) + parent::fields();
  }

  /**
   * The shared global settings form for all Tumblr instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {

    $settings = $this->settings['global'];
    $form = parent::globalSettingsForm();

    $form['description'] = array(
      '#markup' => '<p>' . t('See !link', array(
          '!link' => l('tumblr.com/oauth/apps', 'https://www.tumblr.com/oauth/apps'),
        )) . '</p>',
    );

    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => 'API URL',
      '#description' => t('Do not include trailing slash. Example: !url', array(
        '!url' => 'https://api.tumblr.com/v2',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : 'https://api.tumblr.com/v2',
      '#required' => TRUE,
    );
    $form['oauth_consumer_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Consumer key'),
      '#default_value' => isset($settings['oauth_consumer_key']) ? $settings['oauth_consumer_key'] : NULL,
      '#required' => TRUE,
    );
    $form['oauth_consumer_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Secret Key'),
      '#default_value' => isset($settings['oauth_consumer_secret']) ? $settings['oauth_consumer_secret'] : NULL,
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

    $form['host_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Blog Hostname'),
      '#description' => t("Each blog has a unique hostname. i.e. 'natgeofound.tumblr.com'"),
      '#default_value' => isset($settings['host_name']) ? $settings['host_name'] : NULL,
      '#required' => TRUE,
    );

    $form['type'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Type'),
      '#options' => array(
        'text' => t('Text'),
        'photo' => t('Photo'),
        //'quote' => t('Quote'),
        //'link' => t('Link'),
        //'chat' => t('Chat'),
        //'audio' => t('Audio'),
        'video' => t('Video'),
        //'answer' => t('Answer'),
      ),
      '#required' => TRUE,
      '#description' => t("Which content types should be imported. For now, only 'Text', 'Photo' and 'Video' are supported ."),
      '#default_value' => isset($settings['type']) ? $settings['type'] : array(),
    );
    return $form;
  }

  /**
   * Get the rows to import.
   *
   * @param mixed $last_id
   *   The id of the last import.
   *
   * @return mixed
   *   The array of the rows, or FALSE on error.
   */
  public function getRows($last_id = NULL) {

    $settings = $this->settings['instance'];
    $global_settings = $this->settings['global'];

    $url = $global_settings['api_url'] . '/blog/' . $settings['host_name'];
    $params['api_key'] = $global_settings['oauth_consumer_key'];
    if (!empty($settings['limit'])) {
      $params['limit'] = $settings['limit'];
    }
    if (!empty($last_id)) {
      $params['offset'] = $last_id;
    }
    $params = drupal_http_build_query($params);

    $rows = array();
    foreach ($settings['type'] as $key => $value) {
      if (!$value) {
        continue;
      }
      $response = drupal_http_request($url . "/posts/$key?$params");
      if ($response->code == 200) {
        $data = json_decode($response->data);
        if (isset($data->response->posts)) {

          // Add blog title to each row.
          foreach ($data->response->posts as &$post) {
            $post->blog_title = $data->response->blog->title;
          }
          $rows += $data->response->posts;
        }
      }
    }
    return $rows;
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $row->created = $row->timestamp;
    $row->blog_link = 'http://' . $this->settings['instance']['host_name'];

    if ($row->type == 'text') {

      // Try to get an image from the body.
      $mappings = $this->getFieldMappings();
      $doc = new DOMDocument();
      @$doc->loadHTML($row->body);
      $tags = $doc->getElementsByTagName('img');
      foreach ($tags as $tag) {
        $picture = $this->saveExternalFile($tag->getAttribute('src'), $mappings['picture']);
        continue;
      }
      $row->picture = !empty($picture) ? $picture : NULL;

      $filter = filter_format_exists('tumblr') ? 'tumblr' : 'filtered_html';
      $row->body = array('value' => $row->body, 'format' => $filter);

    }
    else if ($row->type == 'photo') {

      // Save first photo only for now.
      foreach ($row->photos as $photo) {
        $mappings = $this->getFieldMappings();
        $picture = $this->saveExternalFile($photo->original_size->url, $mappings['picture']);
        continue;
      }
      $row->picture = !empty($picture) ? $picture : NULL;

      // Put the caption field to the body.
      $filter = filter_format_exists('tumblr') ? 'tumblr' : 'filtered_html';
      $row->body = array('value' => $row->caption, 'format' => $filter);

    }
    else if ($row->type == 'video') {

      // Save the video thumbnail.
      $mappings = $this->getFieldMappings();
      $picture = $this->saveExternalFile($row->thumbnail_url, $mappings['picture']);
      $row->picture = !empty($picture) ? $picture : NULL;

      // Put the caption field to the body.
      $filter = filter_format_exists('tumblr') ? 'tumblr' : 'filtered_html';
      $row->body = array('value' => $row->caption, 'format' => $filter);

      // Get video embed URL.
      $row->video_embed = NULL;
      $player = end($row->player);
      if ($player && isset($player->embed_code)) {
        $doc = new DOMDocument();
        @$doc->loadHTML($player->embed_code);
        $tags = $doc->getElementsByTagName('iframe');
        foreach ($tags as $tag) {
          $row->video_embed = $tag->getAttribute('src');
          continue;
        }
      }
    }

    return TRUE;
  }
}
