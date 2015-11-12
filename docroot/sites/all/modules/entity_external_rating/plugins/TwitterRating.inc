<?php

/**
 * @file
 *  Twitter plugin for the Entity External Rating module.
 */

class TwitterRating extends EntityExtRatingSource {

  public function __construct() {
    parent::__construct();
    // If the API url is not yet defined, rely on a default value.
    if (!isset ($this->config['twitter_api_url'])) {
      $this->config['twitter_api_url'] = 'http://urls.api.twitter.com/1/urls/count.json';
    }
  }

  /**
   * Overwrites the configForm() function and adds some custom settings.
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    // Add the Twitter API URL setting.
    $form['twitter_api_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Twitter API URL'),
        '#description' => t('Please enter the URL of the twitter API.'),
        '#default_value' => isset($this->config['twitter_api_url'])?$this->config['twitter_api_url']:'http://urls.api.twitter.com/1/urls/count.json',
        '#required' => TRUE,
    );
    return $form;
  }

  /**
   * The source name of this plugin.
   *
   * @return string
   *   The source name of this plugin.
   */
  public function sourceName() {
    return 'Twitter';
  }

  /**
   * Makes the call to the Twitter API.
   *
   * @param array $params
   *   It has the same meaning as in the parent class.
   *
   * @see EntityExtRatingSource::saveExternalRating()
   *
   * @return bool
   *   TRUE if the request was succesful, FALSE otherwise.
   */
  public function getExternalRating(&$params) {
    // @todo: log the eventual errors.
    // Construct the url using the $params array.
    foreach ($params as $key => $value) {
      if (!isset ($params[$key]['params']['url'])) {
        continue;
      }
      $url = $this->config['twitter_api_url'] . '?url=' . $params[$key]['params']['url'];
      $result = drupal_http_request($url);
      if ($result->code == 200) {
        $response = json_decode($result->data);
        // If we got a valid response, then just add the rating value to the
        // object in the array.
        if (is_object($response)) {
          $params[$key]['rating_value'] = $response->count;
        }
      }
    }
    return TRUE;
  }

   /**
   * Loads the html output of this widget.
   *
   * @param array $params
   *   An array with parameters that will be used to create the widget. The
   *   'url' parameter is required, and it is the only parameter needed so far.
   *
   * @return string
   *   The html output of the widget.
   */
  public function getHtml($params) {
    static $printed_js;
    $output = theme('entity_ext_rating_twitter_widget', array('params' => $params));
    // The external js doesn't need to be added more than once.
    if (!$printed_js) {
      $printed_js = 1;
      $output .= '<script type="text/javascript" src="//platform.twitter.com/widgets.js"></script>';
    }
    return $output;
  }
}