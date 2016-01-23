<?php

/**
 * @file
 *   TMGMT Directory connector class.
 */

class TMGMTServerDSConnector extends TMGMTRemoteConnector {

  function getUserAgent() {
    $path = drupal_get_path('module', 'tmgmt_client') . '/tmgmt_client.info';
    $info = drupal_parse_info_file($path);
    $version = isset($info['version']) ? $info['version'] : 'dev';

    return parent::getUserAgent() . ' tmgmt_server/' . $version;
  }

  function authenticate($resource, array $options = array()) {
    $key = tmgmt_server_get_ds_key();
    $auth_token = NULL;
    if (!empty($key)) {
      $auth_token = tmgmt_auth_receiver_controller()->createAuthToken($key);
    }
    $this->addHeader('Authenticate', $auth_token);
  }

  function getEndPoint() {
    return 'api/v1';
  }

  /**
   * Sends request to register a user account at DS of a type 'server'.
   *
   * @param string $mail
   *   User email address.
   * @param string $name
   *   User name.
   *
   * @return array
   *   Status response.
   */
  function registerDSUser($mail, $name = NULL) {
    $this->addPostParam('mail', $mail);
    $this->addPostParam('name', $name);
    $this->addPostParam('type', 'server');
    return $this->getResponse('ds-users');
  }

  /**
   * Sends request to create a server entity at DS.
   *
   * @param string $name
   *   Existing user name.
   * @param $pass
   *   User account password.
   * @param $url
   *   TS system url.
   *
   * @return array
   *   Response containing DS key.
   */
  function createTS($name, $pass, $url) {
    $this->addPostParam('name', $name);
    $this->addPostParam('pass', $pass);
    $this->addPostParam('url', $url);
    $this->addPostParam('server_name', variable_get('site_name'));
    return $this->getResponse('ds-users/translation-server-create');
  }

  /**
   * Gets supported languages.
   *
   * @return array
   */
  function getSupportedLanguages() {
    return $this->getResponse('supported-languages');
  }

  /**
   * Gets proof of identity of a client.
   *
   * @param string $verification_token
   *   Client token to be verified.
   *
   * @return array
   *   Client data to create user account and client entity.
   */
  function getProofOfIdentity($verification_token) {
    $this->addPostParam('verification_token', $verification_token);
    return $this->getResponse('verification-tokens/proof-of-identity');
  }

  /**
   * Pushes key for a client to the DS.
   *
   * @param string $transfer_key
   *   Key for the client in auth_string format.
   * @param string $verification_token
   *   Client token for identification.
   *
   * @return array
   *   Response data.
   */
  function createTransferKey($transfer_key, $verification_token) {
    $this->addPostParam('transfer_key', $transfer_key);
    $this->addPostParam('verification_token', $verification_token);
    return $this->getResponse('transfer-keys');
  }

  /**
   * Does request to test connection with DS.
   *
   * @return NULL
   *   Empty response will arrive.
   */
  function testConnection() {
    $this->addPostParam('test_data', 'test_value');
    return $this->getResponse('ds-users/test-connection');
  }

  function pushLangPairs($language_capabilities) {
    $this->addPostParam('capabilities', $language_capabilities);
    return $this->getResponse('language-pairs/sync');
  }

  function pushProposedSupportedLanguages($supported_languages) {
    $this->addPostParam('supported_languages', $supported_languages);
    return $this->getResponse('supported-languages');
  }

  function getLangPairs() {
    /**
     * @var TMGMTDirectoryServerController $ds_controller
     */
    $ds_controller = entity_get_controller('tmgmt_directory_server');
    $ds = $ds_controller->loadByUrl(variable_get('tmgmt_ds_url'));

    if (empty($ds)) {
      return NULL;
    }

    return $this->getResponse('translation-servers/' . $ds->remote_identifier . '/language-pairs');
  }

  /**
   * Adds language capabilities.
   *
   * @param array $capabilities
   *   Array of source => target pairs.
   *
   * @return array
   *   Response array.
   */
  function addLangCapabilities($capabilities) {
    $this->addPostParam('capabilities', $capabilities);
    return $this->getResponse('language-pairs');
  }

  /**
   * Remove language capability.
   *
   * @param string $source_language
   *   Source language ISO code.
   * @param string $target_language
   *   Target language ISO code.
   *
   * @return array
   *   Response array.
   */
  function removeLangCapability($source_language, $target_language) {
    return $this->getResponse('language-pairs/' . $source_language . '__' . $target_language,
      array('method' => 'DELETE'));
  }

  /**
   * Sends info of a job that has been submitted to TS to the TMGMT Directory.
   *
   * @param TMGMTJobItem $job_item
   *   Job for which we want to send info.
   * @param int $action
   *   Action that has been performed over the job.
   * @param string $client_uuid
   *   UUID of the client.
   * @param string $note
   *   Custom data to be attached.
   */
  static function sendJobInfo(TMGMTJobItem $job_item, $action, $client_uuid, $note = NULL) {

    $instance = new TMGMTServerDSConnector(variable_get('tmgmt_ds_url'));

    $job_data = array(
      'tjid' => $job_item->tjid,
      'source_language' => $job_item->getJob()->source_language,
      'target_language' => $job_item->getJob()->target_language,
      'item_word_count' => $job_item->getWordCount(),
      'job_word_count' => $job_item->getJob()->getWordCount(),
      'item_state' => $job_item->getState(),
      // TODO - we must send data only if it has been allowed on client.
      'data' => $job_item->getData(),
    );

    $instance->addPostParam('job_data', $job_data);
    $instance->addPostParam('client_uuid', $client_uuid);
    $instance->addPostParam('action', $action);
    $instance->addPostParam('origin', TMGMT_DS_JOB_ACTION_ORIGIN_SERVER);
    $instance->addPostParam('note', $note);

    try {
      $instance->getResponse('job-messages');
    }
    catch (Exception $e) {
      watchdog_exception('tmgmt_ds', $e);
    }
  }

}
