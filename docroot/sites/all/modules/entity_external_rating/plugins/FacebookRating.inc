<?php

/**
 * @file
 *  Facebook plugin for the Entity External Rating module.
 */
class FacebookRating extends EntityExtRatingSource {

  public function __construct() {
    parent::__construct();
    // If the API url is not yet defined, rely on a default value.
    if (!isset ($this->config['facebook_api_url'])) {
      $this->config['facebook_api_url'] = 'https://api.facebook.com/method/fql.query';
    }
  }

  /**
   * Overwrites the configForm() function and adds some custom settings.
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    // Add the Facebook API URL setting.
    $form['facebook_api_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook API URL'),
      '#description' => t('Please enter the URL of the facebook API.'),
      '#default_value' => isset($this->config['facebook_api_url'])?$this->config['facebook_api_url']:'https://api.facebook.com/method/fql.query',
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
    return 'Facebook';
  }

  /**
   * Makes the call to Facebook API.
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
    $urls = array();
    foreach ($params as $key => $value) {
      if (!isset ($params[$key]['params']['url'])) {
        continue;
      }
      $urls[$params[$key]['rating_id']] = $params[$key]['params']['url'];
    }
    if (count($urls)) {
      $url = $this->config['facebook_api_url'] . '?query=select total_count, url from link_stat where url IN ("' . implode('", "', $urls) . '")';
      $result = simplexml_load_file($url);
      if (is_object($result)) {
        foreach ($result->link_stat as $key => $value) {
          $rating_id = array_search($value->url, $urls);
          if ($rating_id !== FALSE) {
            $params[$rating_id]['rating_value'] = (int)$value->total_count;
          }
        }
      }
      else {
        return FALSE;
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
    return theme('entity_ext_rating_facebook_widget', array('params' => $params));
  }
}