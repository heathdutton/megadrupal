<?php
/**
 * @file
 * A quick interface to the Formstack REST APIv2. Documentation to the API can be
 * found here: https://www.formstack.com/developers/api
 *
 * @author michael & artis@drupal.org(updates to APIv2)
 */

class Formstack {

  public static $apiUrl = 'https://www.formstack.com/api/v2';

  /**
   * Makes a Formstack API request.
   *
   * This function uses v2 of the Formstack API
   * and returns a JSON decoded response.
   *
   * @param string $oauth_token
   *   The Formstack OAuth Access Token
   * @param string $method
   *   The API web method
   * @param array $args
   *   The parameters for the API request
   *
   * @return stdobject
   *   The JSON decodes response
   */
  public static function request($oauth_token, $method, $args = array()) {

    $args['oauth_token'] = $oauth_token;

    $url = self::$apiUrl . "/" . $method . '.json';

    $query = drupal_http_build_query($args);

    $url = $url . '?' . $query; //due to issues with drupal_http_request
    $res = drupal_http_request($url);

    if ($res->code == 200 || $res->code == 201) {
      return json_decode($res->data);
    }

  return $res->code;
  }

  /**
   * Returns a list of forms or details of a single form.
   *
   * @param string $oauth_token
   *   The Formstack OAuth Access Token
   * @param string $form_id
   *   The Formstack ID of a specific Form
   *
   * @return array
   *   An array of forms.
   */
  public static function form($oauth_token, $form_id = NULL) {

    if (!empty($oauth_token)) {

      $method = 'form';

      if ($form_id !== NULL) {
        $method .= '/' . $form_id;  //method for specific form details
      }

      $res = Formstack::request($oauth_token, $method);

      return $res;
    }
    else {
      return array();
    }
  }
}
