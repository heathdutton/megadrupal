<?php
/**
 * @file
 * Obiba Agate Mdule.
 *
 * Copyright (c) 2015 OBiBa. All rights reserved.
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Class UserProfile.
 */
class ObibaAgateUserProfile extends ObibaAgateClient {


  /**
   * UserProfile constructor.
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Get user profile details.
   */
  public function getUserProfile() {
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/ticket/' . $_SESSION[self::OBIBA_COOKIE] . '/profile';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => array(
        'Accept' => array('application/json'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
    ));
    $client = $this->client();
    try {
      $body = $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      return json_decode($body);
    }
    catch (HttpClientException $e) {
      drupal_set_message('Error get profile, server message : ' . $e->getMessage());
      watchdog('Agate Client', 'Request failed, @code: @message',
        array('@code' => $e->getCode(), '@message' => $e->getMessage()), WATCHDOG_WARNING);
      return array();
    }

  }

  /**
   * Update the user profile.
   *
   * @param string $user_data
   *   The user detail to save on Agate server.
   *
   * @return array|bool
   *   An array errors or true if user updated.
   */
  public function updateUserProfile($user_data) {
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/ticket/' . $_SESSION[self::OBIBA_COOKIE] . '/profile';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_PUT,
      'headers' => array(
        'Accept' => array(parent::HEADER_JSON),
        'Content-Type' => array(parent::HEADER_JSON),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
      'data' => $user_data,

    ));
    $client = $this->client();
    try {
      $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      return TRUE;
    }
    catch (HttpClientException $e) {
      watchdog('Agate Client', 'Request failed, @code: @message',
        array('@code' => $e->getCode(), '@message' => $e->getMessage()), WATCHDOG_WARNING);
      return array('errorServer' => t('Server error :') . $e->getMessage());
    }
  }

  /**
   * Update the user password.
   *
   * @param string $current_password
   *   Current password.
   * @param string $new_password
   *   The new password.
   *
   * @return array|bool
   *   An array of error message or TRUE if password updated.
   */
  public function updateUserPassword($current_password, $new_password) {
    global $user;
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/user/_current/password';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_PUT,
      'headers' => array(
        'Content-Type' => array('application/x-www-form-urlencoded'),
        'Accept' => array('application/json'),
        ObibaAgateClient::AUTHORIZATION_HEADER => array('Basic ' . base64_encode($user->name . ':' . $current_password)),
      ),
      'data' => 'password=' . $new_password,
    ));

    $client = $this->client();
    try {
      $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      return TRUE;
    }
    catch (HttpClientException $e) {
      $error_message = NULL;
      $error_response = $e->getResponse();
      if ($e->getCode() != 403) {
        $error_message = drupal_json_decode($error_response->body)['message'];
      }
      else {
        $error_message = t('Forbidden');
      }

      watchdog('Agate Client update password', 'Request failed, @code: @message',
        array('@code' => $e->getCode(), '@message' => $error_message), WATCHDOG_WARNING);
      return array('errorServer' => $error_message);
    }

  }

}
