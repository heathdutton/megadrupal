<?php
/**
 * @file
 * Social Content Picasa class.
 */

class SocialContentPicasa extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {
    return t('Picasa');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'picasa';
  }

  /**
   * Get the source being used to get the rows.
   *
   * @return string
   *   The album / user id being used to fetch the rows.
   */
  public function getSource() {
    return $this->settings['instance']['type'] == 'album_id' ? $this->settings['instance']['album_id'] : $this->settings['instance']['user_id'];
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
      'id' => 'field_picasa_external_id',
      'created' => 'created',
      'caption' => 'body',
      'link' => 'field_picasa_link',
      'picture' => 'field_picasa_picture',
    ) + parent::fields();
  }

  /**
   * The shared global settings form for all Picasa instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {

    $settings = $this->settings['global'];
    $form = parent::globalSettingsForm();

    $form['description'] = array(
      '#markup' => '<p>' . t('See !link', array(
          '!link' => l('developers.google.com/picasa-web', 'https://developers.google.com/picasa-web/'),
        )) . '</p>',
    );

    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => t('API URL'),
      '#description' => t('Do not include trailing slash. Example: !url', array(
        '!url' => 'https://picasaweb.google.com/data/feed/api',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : 'https://picasaweb.google.com/data/feed/api',
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
      '#attributes' => array('class' => array('social-content-picasa-type')),
    );

    $form['user_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Picasa User ID'),
      '#description' => t("The Picasa User ID to pull photos from his photostream. E.g. '112664698474339089009'."),
      '#required' => TRUE,
      '#default_value' => isset($settings['user_id']) ? $settings['user_id'] : NULL,
    );

    $form['album_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Picasa Album ID'),
      '#description' => t("The Picasa Album ID to pull photos from. E.g. '5632158210784470017'."),
      '#default_value' => isset($settings['album_id']) ? $settings['album_id'] : NULL,
      '#states' => array(
        'visible' => array(
          '.social-content-picasa-type' => array('value' => 'album'),
        ),
      ),
    );

    return $form;
  }

  /**
   * Different types of Picasa instances.
   */
  protected function getImportTypes() {
    return array(
      'user' => t('Pull from a user account'),
      'album' => t('Pull from a specific album'),
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

    if (!empty($settings['user_id'])) {

      $params = array(
        'alt' => 'json',
        'kind' => 'photo',
      );

      $url = $global_settings['api_url'] . '/user/' . $settings['user_id'];

      if ($settings['type'] == 'album' && !empty($settings['album_id'])) {
        $url .= '/albumid/' . $settings['album_id'];
      }

      $result = $this->httpRequest(url($url, array(
        'query' => $params,
        'external' => TRUE,
      )));

      if ($result->code == 200) {
        $data = json_decode($result->data);
        if (isset($data->feed->entry)) {
          return $data->feed->entry;
        }
      }
      else {
        watchdog('social_content_picasa', 'Error fetching feed, data: %data', array('%data' => $result->data), WATCHDOG_WARNING);
      }
    }

    return array();
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   *
   * https://picasaweb.google.com/data/feed/api/user/106517422559421444487/albumid/5287516703641643329?alt=json&kind=photo
   */
  public function prepareRow($row) {

    $mappings = $this->getFieldMappings();

    $row->id = $row->{'gphoto$id'}->{'$t'};
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $row->caption = !empty($row->summary) && isset($row->summary->{'$t'}) ? $row->summary->{'$t'} : '';
    $row->title = $row->title->{'$t'};

    $row->created = strtotime($row->published->{'$t'});
    $row->picture = $this->saveExternalFile($row->content->src, $mappings['picture']);

    foreach ($row->link as $link) {
      if (strpos($link->rel, 'canonical') !== FALSE) {
        $row->link = $link->href;
        break;
      }
    }

    return TRUE;
  }
}
