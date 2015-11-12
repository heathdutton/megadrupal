<?php
/**
 * @file
 * Social Content Youtube class.
 */

class SocialContentYoutube extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {
    return t('Youtube');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'youtube';
  }

  /**
   * Get the source being used to get the rows i.e. account / hashtag.
   *
   * @return string
   *   The Youtube Channel ID used to fetch the rows.
   */
  public function getSource() {
    return $this->settings['instance']['youtube_id'];
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
      'id' => 'field_youtube_external_id',
      'created' => 'created',
      'account_id' => '',
      'account_name' => '',
      'account_link' => '',
      'caption' => 'body',
      'link' => 'field_youtube_link',
      'embed_link' => 'field_youtube_embed',
      'picture' => 'field_youtube_picture',
    ) + parent::fields();
  }

  /**
   * The shared global settings form for all Youtube instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {

    $settings = $this->settings['global'];
    $form = parent::globalSettingsForm();

    $form['description'] = array(
      '#markup' => '<p>' . t('See !link', array(
          '!link' => l('developers.google.com/youtube', 'https://developers.google.com/youtube/registering_an_application'),
        )) . '</p>',
    );

    $form['api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Google API Key'),
      '#default_value' => isset($settings['api_key']) ? $settings['api_key'] : NULL,
      '#required' => TRUE,
    );

    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => t('API URL'),
      '#description' => t('Do not include a trailing slash. If not sure, use %url', array(
        '%url' => 'https://www.googleapis.com/youtube/v3',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : 'https://www.googleapis.com/youtube/v3',
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

    $form['youtube_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Youtube User Name'),
      '#description' => t("E.g. 'NationalGeographic' for http://youtube.com/user/NationalGeographic"),
      '#default_value' => isset($settings['youtube_username']) ? $settings['youtube_username'] : NULL,
      '#required' => TRUE,
    );
    $form['youtube_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Youtube Channel ID'),
      '#description' => t('This field is generated automatically.'),
      '#default_value' => isset($settings['youtube_id']) ? $settings['youtube_id'] : NULL,
      '#disabled' => TRUE,
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

    // Look for the Youtube Channel ID.
    if (!empty($settings['youtube_username'])) {

      $url = $global_settings['api_url'] . '/channels';
      $params = array(
        'key' => $global_settings['api_key'],
        'forUsername' => $settings['youtube_username'],
        'part' => 'id',
      );

      $result = $this->httpRequest(url($url, array('query' => $params, 'external' => TRUE)));
      if ($result->code == 200) {
        $data = json_decode($result->data);
        if (isset($data->items) && is_array($data->items)) {
          foreach ($data->items as $item) {
            $settings['youtube_id'] = $item->id;
          }
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
   * @return array
   *   Array with the rows.
   */
  public function getRows($last_id = NULL) {

    $settings = $this->settings['instance'];
    $global_settings = $this->settings['global'];


    if (!empty($settings['youtube_id'])) {

      $params = array(
        'order'     => 'date',
        'part'      => 'snippet,id',
        'key'       => $global_settings['api_key'],
        'channelId' => $settings['youtube_id'],
        'type'      => 'video',
      );

      if (!empty($settings['limit'])) {
        $params['maxResults'] = $settings['limit'];
      }

      $url = $global_settings['api_url'] . '/search' ;
      $result = $this->httpRequest(url($url, array(
        'query' => $params,
        'external' => TRUE,
      )));
      if ($result->code == 200) {
        $data= json_decode($result->data);

        // $items->id is an object causes conflicts.
        foreach ($data->items as &$item) {
          $id_number = $item->id->videoId;
          unset($item->id);
          $item->id = $id_number;
        }
        return $data->items;
      }
    }

    return array();
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {

    $settings = $this->settings['instance'];
    $mappings = $this->getFieldMappings();

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $row->title = $row->snippet->title;
    $row->caption = $row->snippet->description;

    $row->account_id = $settings['youtube_id'];
    $row->account_name = $settings['youtube_username'];
    $row->account_link = 'https://www.youtube.com/user/' . $settings['youtube_username'];

    $date = new DateTime($row->snippet->publishedAt);
    $row->created = $date->getTimestamp();

    $row->link = 'https://www.youtube.com/watch?v=' . $row->id;
    $row->embed_link = 'https://www.youtube.com/embed/' . $row->id;

    // @todo: Make sure saving file to disk is optional.
    $image_url = 'http://i.ytimg.com/vi/' . $row->id . '/hqdefault.jpg';
    $picture = $this->saveExternalFile($image_url, $mappings['picture']);
    $row->picture = $picture ? $picture : NULL;

    return TRUE;
  }
}
