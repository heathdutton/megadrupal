<?php

/**
 * @File
 * This file contains the BackpackConnectApi class.
 */

/**
 * Class BackpackApi
 * The purpose of this class is to make all the api call for the Mozilla Backpack Connect API possibles.
 */
class BackpackConnectApi
{

  /**
   * @var String The api URL
   */
  protected $apiRoot;

  /**
   * @var String The access token
   */
  protected $accessToken;

  /**
   * @var String The refresh token
   */
  protected $refreshToken;

  /**
   * @param $apiRoot
   * @param $accessToken
   * @param $refreshToken
   */
  function __construct($apiRoot, $accessToken, $refreshToken)
  {
    if (!valid_url($apiRoot))
    {
      watchdog('Opigno Mozilla Openbadges App', 'The Backpack Connect Api root given is not good. Api root given: '. $apiRoot);
      return;
    }

    if (empty($accessToken))
    {
      watchdog('Opigno Mozilla Openbadges App', 'The Backpack Connect Api Access Token given is empty. Api root given: '. $apiRoot);
      return;
    }

    if (empty($refreshToken))
    {
      watchdog('Opigno Mozilla Openbadges App', 'The Backpack Connect Api Refresh Token given is empty. Api root given: '. $apiRoot);
      return;
    }

    $this->apiRoot = $apiRoot;
    $this->accessToken = $accessToken;
    $this->refreshToken = $refreshToken;
  }

  // TODO: Maybe use a single $ch for all the object (reusability and better performance ?)

  /**
   * Get the recipient from the api
   * The recipient contains the hashed email to use when issuing the badge.
   *
   * @return bool|mixed False if there was a problem. The recipient object otherwise.
   */
  public function getRecipient()
  {
    $url = $this->apiRoot .'/identity';

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json',
      'Authorization: Bearer ' . base64_encode($this->accessToken)
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    $response = curl_exec($ch);
    curl_close($ch);

    $recipientObj = json_decode($response);
    if (json_last_error() != JSON_ERROR_NONE
      || empty($recipientObj->recipient)
      || empty($recipientObj->salt)
      || empty($recipientObj->type)
    ) {
      watchdog('Opigno Mozilla Openbadges App', 'The recipient from the Backpack is empty or not good. '.
        'Api root: '. $this->apiRoot .' ; Object received: '. print_r($recipientObj, true));
      return false;
    }

    return $recipientObj;
  }

  /**
   * Get a new token using the refresh token.
   *
   * @return bool|mixed
   *   False if there was an error. If success, an object who contain the new access token, the new refresh token and
   *   the time in seconds before expiration of the access token
   */
  public function getNewToken()
  {
    $data = array(
      'grant_type'    => 'refresh_token',
      'refresh_token' => $this->refreshToken
    );
    $url = $this->apiRoot . '/token';

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POST, count($data));
    curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($data));
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json'
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    $response = curl_exec($ch);
    curl_close($ch);

    $newToken = json_decode($response);
    if (empty($newToken))
    {
      watchdog('Opigno Mozilla Openbadges App', 'The new token from the Backpack is empty. Api root: '. $this->apiRoot);
      return false;
    }

    return $newToken;
  }

  /**
   * Issue a badge to the backpack
   *
   * @param $badgeUri String The uri of the badge to issue
   *
   * @return bool True if success or if the badge was already in the backpack. False otherwise.
   */
  public function issueBadge($badgeUri)
  {
    $data = array(
      'badge' => file_create_url($badgeUri)
    );
    $url = $this->apiRoot . '/issue';

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POST, count($data));
    curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($data));
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json',
      'Authorization: Bearer ' . base64_encode($this->accessToken)
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    $response = curl_exec($ch);
    curl_close($ch);

    // If empty response, the badge was already in backpack...
    $response = json_decode($response);
    if (empty($response) || (!empty($response->exists) && !empty($response->badge)))
    {
      return true;
    }
    else
    {
      watchdog('Opigno Mozilla Openbadges App', 'The badge was not issued to the Backpack. '.
        'Api root: '. $this->apiRoot .' ; Object received: '. print_r($response, true));
      return false;
    }
  }

}