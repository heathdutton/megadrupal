<?php
/**
 * @file
 * Social Content Pinterest class.
 */

class SocialContentPinterest extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {

    return t('Pinterest');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {

    return 'pinterest';
  }

  /**
   * Get the source being used to get the rows.
   *
   * @return string
   *   The source used to fetch the rows.
   */
  public function getSource() {

    return $this->settings['instance']['user'];
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
      'id' => 'field_pinterest_guid',
      'created' => 'created',
      'description' => 'body',
      'link' => 'field_pinterest_link',
      'title' => 'title',      
      'picture' => 'field_pinterest_picture',
    ) + parent::fields();
  }

  /**
   * The shared global settings form for all Pinterest instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {

    $settings = $this->settings['global'];
    $form = parent::globalSettingsForm();
    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => 'API URL',
      '#description' => t('Use @user and @board tokens. Example: !url', array(
        '!url' => 'https://pinterest.com/@user/@board.rss',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : 'https://pinterest.com/@user/@board.rss',
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

    $form['user'] = array(
      '#type' => 'textfield',
      '#title' => t('Pinterest User'),
      '#description' => t('Enter the Pinterest user name.'),
      '#default_value' => isset($settings['user']) ? $settings['user'] : NULL,
      '#required' => TRUE,
    );

    $form['board'] = array(
        '#type' => 'textfield',
        '#title' => t('User\'s Board'),
        '#description' => t('Enter the specific board of the user.'),
        '#default_value' => isset($settings['board']) ? $settings['board'] : NULL,
        '#required' => TRUE,
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
   *   The array of the raws, or FALSE on error.
   */
  public function getRows($last_id = NULL) {

    $settings = $this->settings['instance'];
    $global_settings = $this->settings['global'];

    $url = $global_settings['api_url'];
    $url = str_replace('@user', $settings['user'], $url);
    $url = str_replace('@board', $settings['board'], $url);


    $response = drupal_http_request($url);

    $xml = simplexml_load_string($response->data);
    $json = json_encode($xml);
    $array = json_decode($json, TRUE);
    $rows = $array['channel']['item'];    
    foreach ($rows as $key => $row) {
      $rows[$key] = (object)$row;
    }
    return $rows;
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {

    $row->created = strtotime($row->pubDate);
    $filter = filter_format_exists('pinterest') ? 'pinterest' : 'filtered_html';
    $row->body = array('value' => $row->description, 'format' => $filter);
    $id = str_replace('https://www.pinterest.com/pin/', '', $row->guid);
    $id = str_replace('/', '', $id);
    $row->id = $id;

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $doc = new DOMDocument();
    @$doc->loadHTML($row->description);

    $tags = $doc->getElementsByTagName('img');
    $mappings = $this->getFieldMappings();
    if ($img = $tags->item(0)) {
      $src = $img->getAttribute('src');
      $picture = $this->saveExternalFile($src, $mappings['picture']);
      $row->picture = $picture ? $picture : NULL;
    }

    return TRUE;
  }
}
