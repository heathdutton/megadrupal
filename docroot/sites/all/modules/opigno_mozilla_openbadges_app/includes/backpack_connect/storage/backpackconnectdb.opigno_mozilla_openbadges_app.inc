<?php

/**
 * @File
 * This file contains the BackpackConnectDb class.
 */


/**
 * Class DbBackpackConnect
 * This class allow the user to use the "opigno_openbadges_backpack_connect" table in order to make some requests on.
 */
  class BackpackConnectDb
{

  /**
   * The DB table name
   */
  const TABLE_NAME = 'opigno_openbadges_backpack_connect';

  /**
   * The DB field name for the user ID
   */
  const FIELD_UID = 'uid';

  /**
   * The DB field name for the access token
   */
  const FIELD_ACCESS_TOKEN = 'access_token';

  /**
   * The DB field name for the refresh token
   */
  const FIELD_REFRESH_TOKEN = 'refresh_token';

  /**
   * The DB field name for the expires timestamp
   */
  const FIELD_EXPIRES_TIMESTAMP = 'expires_timestamp';

  /**
   * The DB field name for the recipient
   */
  const FIELD_RECIPIENT = 'recipient';

  /**
   * The DB field name for the api root
   */
  const FIELD_API_ROOT = 'api_root';


  /**
   * Save a new entry or update an existing one from the DB
   *
   * @param $uid Integer The user ID
   * @param $accessToken String The access token given from the backpack
   * @param $refreshToken String The refresh token given from the backpack
   * @param $expiresTime Integer The number of seconds before needing to take a new access token
   * @param $apiRoot String The url to the api
   * @param $recipient String json The Json with the recipient given from the backpack
   *
   * @throws Exception
   * @throws InvalidMergeQueryException
   */
  public static function save($uid, $accessToken, $refreshToken, $expiresTime, $apiRoot, $recipient)
  {
    // Params verifications
    //
    if (empty($uid)) {
      watchdog('Opigno Mozilla Openbadges App', 'Error while saving in BD. The UID given is empty.');
      return;
    }
    if (empty($accessToken)) {
      watchdog('Opigno Mozilla Openbadges App', 'Error while saving in BD. The Access Token given is empty.');
      return;
    }
    if (empty($refreshToken)) {
      watchdog('Opigno Mozilla Openbadges App', 'Error while saving in BD. The Refresh Token given is empty.');
      return;
    }
    if (empty($expiresTime)) {
      watchdog('Opigno Mozilla Openbadges App', 'Error while saving in BD. The expires time given is empty.');
      return;
    }
    if (!valid_url($apiRoot, true)) {
      watchdog('Opigno Mozilla Openbadges App', 'Error while saving in BD. The Api root given is false or empty. ' .
        'Api root given: '. $apiRoot);
      return;
    }
    if (empty($recipient)) {
      watchdog('Opigno Mozilla Openbadges App', 'Error while saving in BD. The Recipient given is empty.');
      return;
    }

    $expiresDatetime = new DateTime('now');
    $expiresDatetime->add(new DateInterval('PT'. $expiresTime .'S'));

    db_merge(self::TABLE_NAME)
      ->fields(array(
        self::FIELD_UID => $uid,
        self::FIELD_ACCESS_TOKEN => $accessToken,
        self::FIELD_REFRESH_TOKEN => $refreshToken,
        self::FIELD_EXPIRES_TIMESTAMP => $expiresDatetime->getTimestamp(),
        self::FIELD_API_ROOT => $apiRoot,
        self::FIELD_RECIPIENT => $recipient
      ))
      ->key(array(
        self::FIELD_UID => $uid,
        self::FIELD_API_ROOT => $apiRoot
      ))
      ->execute();
  }

  /**
   * Get all the entries for a user given in parameter
   *
   * @param $uid Integer The user ID
   *
   * @return bool|array
   *   False if there was a problem or if there is no entries for this user.
   *   Else, an associative array for all the entries
   */
  public static function getEntries($uid)
  {
    $access = db_query('SELECT * FROM {'. self::TABLE_NAME .'} WHERE '. self::FIELD_UID .' = '. $uid)
      ->fetchAllAssoc(self::FIELD_API_ROOT, PDO::FETCH_ASSOC);

    if (empty($access))
    {
      return false;
    }

    return $access;
  }

    /**
     * Delete all the entries for the user ID given and the API root given in params.
     *
     * @param $uid integer The user ID (internal)
     * @param $apiRoot String The API URL to use as a condition of the request
     */
    public static function clear($uid, $apiRoot)
  {
    db_delete(self::TABLE_NAME)
      ->condition(self::FIELD_UID, $uid, '=')
      ->condition(self::FIELD_API_ROOT, $apiRoot, '=')
      ->execute();
  }

  /**
   * Simple array with the primary keys of this table
   *
   * @return array The fields name of the primary keys
   */
  public static function getPrimaryKey()
  {
    return array(self::FIELD_UID, self::FIELD_API_ROOT);
  }

}