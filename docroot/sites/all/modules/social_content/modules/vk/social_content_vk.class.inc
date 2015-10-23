<?php
/**
 * @file
 * Social Content VK class.
 */

class SocialContentVK extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {

    if (isset($this->settings['instance']) && !empty($this->settings['instance']['type'])) {

      return t('VK (@type)', array(
        '@type' => ucfirst($this->settings['instance']['type']),
      ));
    }

    return t('VK');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'vk';
  }

  /**
   * Get the source being used to get the rows i.e. account / hashtag.
   *
   * @return string
   *   The hashtag / account being used to fetch the rows.
   */
  public function getSource() {
    return 'account';
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
      'id' => 'id',
      'text' => 'body',
    ) + parent::fields();
  }

  /**
   * The shared global settings form for all VK instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {
    $settings = $this->settings['global'];
    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => t('API URL'),
      '#description' => t('Do not include trailing slash. Example: !url', array(
        '!url' => 'https://api.vk.com',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : NULL,
      '#required' => TRUE,
    );
    $form['api_version'] = array(
      '#type' => 'textfield',
      '#title' => t('API version'),
      '#description' => t("i.e. 'v1'."),
      '#default_value' => isset($settings['api_version']) ? $settings['api_version'] : '5.21',
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
    $form['owner_id'] = array(
      '#type' => 'textfield',
      '#title' => t('VK Owner ID'),
      '#description' => t('The VK user/group id to pull the statuses from. Use negative numbers for groups.'),
      '#default_value' => isset($settings['owner_id']) ? $settings['owner_id'] : NULL,
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
   */
  public function getRows($last_id = NULL) {

    $settings = array_merge($this->settings['global'], $this->settings['instance']);

    if (!empty($settings['owner_id'])) {
      $params = array('owner_id' => $settings['owner_id'], 'photo_sizes' => 1);
      if (!empty($settings['limit'])) {
        $params['count'] = $settings['limit'];
      }
      if (!empty($settings['api_version'])) {
        $params['v'] = $settings['api_version'];
      }      
      $url = $settings['api_url'] . '/method/wall.get';
      $result = $this->httpRequest(url($url, array('query' => $params, 'external' => TRUE)));
      if ($result->code == 200) {
        $response = json_decode($result->data);
        if (!isset($response->error)) {
          return $response->response->items;
        }
        else {
          watchdog('social_content_vk', 'Error fetching feed, data: %data', array('%data' => $response->error->error_msg), WATCHDOG_WARNING);
        }
      }
    }
    return FALSE;
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {
    $row->created = $row->date;
    return parent::prepareRow($row);
  }
}
