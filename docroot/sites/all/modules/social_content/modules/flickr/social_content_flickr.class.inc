<?php
/**
 * @file
 * Social Content Flickr class.
 */

class SocialContentFlickr extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {
    return t('Flickr');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'flickr';
  }

  /**
   * Get the source being used to get the rows i.e. account / hashtag.
   *
   * @return string
   *   The hashtag / account being used to fetch the rows.
   */
  public function getSource() {
    return $this->settings['instance']['type'] == 'user' ? $this->settings['instance']['user_id'] : $this->settings['instance']['group_id'];
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
      'id' => 'field_flickr_external_id',
      'created' => 'created',
      'user_id' => '',
      'user_name' => '',
      'user_link' => '',
      'caption' => 'body',
      'link' => 'field_flickr_link',
      'picture' => 'field_flickr_picture',
    ) + parent::fields();
  }

  /**
   * The shared global settings form for all Flickr instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {

    $settings = $this->settings['global'];
    $form = parent::globalSettingsForm();

    $form['description'] = array(
      '#markup' => '<p>' . t('See !link', array(
          '!link' => l('flickr.com/services/apps', 'https://www.flickr.com/services/apps/create/apply/'),
        )) . '</p>',
    );

    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => t('API URL'),
      '#description' => t('Do not include trailing slash. Example: !url', array(
        '!url' => 'https://api.flickr.com/services/rest',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : 'https://api.flickr.com/services/rest',
      '#required' => TRUE,
    );

    $form['api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('API Key'),
      '#default_value' => isset($settings['api_key']) ? $settings['api_key'] : NULL,
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
      '#attributes' => array('class' => array('social-content-flickr-type')),
    );

    $form['user_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Flickr User ID'),
      '#description' => t("The Flickr User ID to pull photos from his photostream. E.g. '21195291@N06'. Use !link to get it.", array(
        '!link' => "<a target='_blank' href='http://idgettr.com/'>idGettr</a>",
      )),
      '#default_value' => isset($settings['user_id']) ? $settings['user_id'] : NULL,
      '#states' => array(
        'visible' => array(
          '.social-content-flickr-type' => array('value' => 'user'),
        ),
      ),
    );

    $form['group_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Flickr Group ID'),
      '#description' => t("The Flickr Group ID to pull photos from. E.g. '20029208@N00'. Use !link to get it.", array(
        '!link' => "<a target='_blank' href='http://idgettr.com/'>idGettr</a>",
      )),
      '#default_value' => isset($settings['group_id']) ? $settings['group_id'] : NULL,
      '#states' => array(
        'visible' => array(
          '.social-content-flickr-type' => array('value' => 'group'),
        ),
      ),
    );

    return $form;
  }

  /**
   * Different types of Flickr instances.
   */
  protected function getImportTypes() {
    return array(
      'user' => t('Pull from a user account'),
      'group' => t('Pull from a group'),
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

    if (!empty($settings['user_id']) || !empty($settings['group_id'])) {

      $params = array(
        'extras' => 'url_o,url_sq,url_t,url_s,url_q,url_m,url_n,url_z,url_c,url_l,date_upload,owner_name,description',
        'format' => 'json',
        'nojsoncallback' => '1',
        'api_key' => $global_settings['api_key'],
      );
      if (!empty($settings['limit'])) {
        $params['per_page'] = $settings['limit'];
      }

      if ($settings['type'] == 'user') {
        $params['method'] = 'flickr.people.getPublicPhotos';
        $params['user_id'] = $settings['user_id'];
      }
      else if ($settings['type'] == 'group') {
        $params['method'] = 'flickr.groups.pools.getPhotos';
        $params['group_id'] = $settings['group_id'];
      }

      $url = $global_settings['api_url'] . '/';
      $result = $this->httpRequest(url($url, array(
        'query' => $params,
        'external' => TRUE,
      )));

      if ($result->code == 200) {
        $posts = json_decode($result->data);
        if (isset($posts->photos) && isset($posts->photos->photo)) {
          return $posts->photos->photo;
        }
      }
      else {
        watchdog('social_content_flickr', 'Error fetching feed, data: %data', array('%data' => $result->data), WATCHDOG_WARNING);
      }
    }

    return array();
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {

    $mappings = $this->getFieldMappings();
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $row->caption = !empty($row->description->_content) ? $row->description->_content : '';

    $row->created = $row->dateupload;
    $row->user_name = $row->ownername;
    $row->user_id = $row->owner;
    $row->user_link = 'https://www.flickr.com/photos/' . $row->user_id;
    $row->link = $row->user_link . '/' .  $row->id;

    $image_url =  $this->getImageUrl($row);
    if ($image_url) {
      $picture = $this->saveExternalFile($image_url, $mappings['picture']);
      $row->picture = $picture ? $picture : NULL;
    }


    return TRUE;
  }

  /**
   * Get the bigger image url from a given row.
   *
   * @param object $row
   *   The row containing the feed.
   *
   * @return bool|string
   *   The URL of the biggest image, or FALSE if there aren't any.
   */
  protected function getImageUrl($row) {
    $photo = $row;

    if (!empty($photo->url_o)) {
      return $photo->url_o;
    }
    if (!empty($photo->url_l)) {
      return $photo->url_l;
    }
    if (!empty($photo->url_c)) {
      return $photo->url_c;
    }
    if (!empty($photo->url_z)) {
      return $photo->url_z;
    }
    if (!empty($photo->url_n)) {
      return $photo->url_n;
    }
    if (!empty($photo->url_m)) {
      return $photo->url_m;
    }
    if (!empty($photo->url_s)) {
      return $photo->url_s;
    }
    if (!empty($photo->url_t)) {
      return $photo->url_t;
    }
    if (!empty($photo->url_q)) {
      return $photo->url_q;
    }
    if (!empty($photo->url_sq)) {
      return $photo->url_sq;
    }

    return FALSE;
  }
}
