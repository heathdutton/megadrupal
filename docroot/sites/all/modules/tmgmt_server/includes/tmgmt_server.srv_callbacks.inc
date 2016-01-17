<?php
/**
 * @file
 *   TS service callbacks.
 */

/**
 * @return array
 */
function _tmgmt_server_srv_status_index() {
  $response = new TMGMTResponse();
  $response->setData('Yes, I am alive!');
  return $response->getResponse();
}

/**
 * Provides index of available languages.
 *
 * @param string $source_language
 *   Lang code in ISO format.
 *
 * @return array
 *   Response array.
 */
function _tmgmt_server_srv_language_pairs_index($source_language = NULL) {
  $translator = tmgmt_translator_load(variable_get('tmgmt_server_translator', 'local'));
  $response = new TMGMTResponse();
  $languages = array();

  if (empty($source_language)) {
    // We need to collect target languages for each of our local language.
    foreach (language_list() as $key => $info) {
      foreach ($translator->getSupportedTargetLanguages($key) as $target_language) {
        $languages[] = array(
          'source_language' => $key,
          'target_language' => $target_language,
        );
      }
    }
  }
  else {
    foreach ($translator->getSupportedTargetLanguages($source_language) as $target_language) {
      $languages[] = array(
        'source_language' => $source_language,
        'target_language' => $target_language,
      );
    }
  }

  $response->setData($languages);
  return $response->getResponse();
}

/**
 * Retrieves translation of a single job item/remote source.
 *
 * This service should be called by the client upon being notified by the server
 * to pick up translation for a specific job item.
 *
 * @param int $rsid
 *   Translation job item id.
 *
 * @return array
 *   Response array.
 */
function _tmgmt_server_srv_job_item_translation_retrieve($rsid) {
  $remote_source = tmgmt_server_remote_source_load($rsid);
  $job_item = tmgmt_server_job_item_load_by_reference('tmgmt_server_remote_source', $rsid);

  $response = new TMGMTResponse();
  // In case the job has not went through the internal review process, provide
  // non translated error message.
  if (!$job_item->isAccepted()) {
    $response->setErrorCode('#5001');
    return $response->getResponse();
  }

  $authenticated_entity = tmgmt_auth_issuer_controller()->getAuthenticatedEntity();

  $job_item->addMessage('Job item translation picked up by client @uuid with authenticated user @user.', array(
    '@uuid' => $authenticated_entity->uuid,
    '@user' => l($GLOBALS['user']->name, $GLOBALS['user']->uid),
  ));
  $response->setData($remote_source->data);
  return $response->getResponse();
}

/**
 * Provides all job items belonging to a specific job that has been translated.
 *
 * This service should be available for clients to manually pull translated job
 * items from the server.
 *
 * @param $tjid
 *   Translation job id.
 *
 * @return array
 *   Response array.
 */
function _tmgmt_server_srv_job_retrieve($tjid) {
  /**
   * @var TMGMTJob $job
   */
  $job = tmgmt_job_item_load($tjid);
  $response = new TMGMTResponse();
  $translations = array();

  foreach ($job->getItems() as $job_item) {
    if ($job_item->getState != TMGMT_JOB_ITEM_STATE_ACCEPTED) {
      continue;
    }
    // Make sure we do not query remote source table with other entity type id.
    if ($job_item->item_type != 'tmgmt_server_remote_source') {
      continue;
    }

    $remote_source = tmgmt_server_remote_source_load($job_item->item_id);
    $translations[$remote_source->reference] = $remote_source->data;
  }

  if (empty($translations)) {
    $response->setErrorCode('#5002');
  }
  else {
    $response->setData($translations);
  }

  $authenticated_entity = tmgmt_auth_issuer_controller()->getAuthenticatedEntity();

  $job->addMessage('Job items translations picked up by client @uuid with authenticated user !user. Number of available translations: @count', array(
    '@uuid' => $authenticated_entity->uuid,
    '!user' => l($GLOBALS['user']->name, $GLOBALS['user']->uid),
    '@count' => count($translations),
  ));

  return $response->getResponse();
}

/**
 * Creates a translation job.
 *
 * @param array $job_data
 *   Data to create the translation job.
 *
 * @return array
 *   Response array.
 */
function _tmgmt_server_srv_job_create($job_data) {
  $response = new TMGMTResponse();
  $translator = tmgmt_translator_load(variable_get('tmgmt_server_translator', 'local'));
  $languages = tmgmt_server_languages_predefined_list();

  if (empty($job_data['source_language']) || empty($job_data['target_language'])) {
    $response->setErrorCode('#5003');
    return $response->getResponse();
  }

  if (!in_array($job_data['target_language'], $translator->getSupportedTargetLanguages($job_data['source_language']))) {
    $response->setErrorCode('#5004', t('Translation server cannot translate from @source_language to @target_language', array(
      '@source_language' => $languages[$job_data['source_language']],
      '@target_language' => $languages[$job_data['target_language']],
    )));
    return $response->getResponse();
  }

  /**
   * @var TMGMTClient $client
   */
  $client = tmgmt_auth_issuer_controller()->getAuthenticatedEntity();

  try {
    $headers = getallheaders();
    $processed_translation_request = tmgmt_server_receive_translation_request(
      $client, $job_data['items'], $job_data['source_language'], $job_data['target_language'], $translator, $headers['User-Agent'], $job_data['job_comment']);
  }
  catch (TMGMTServerTranslationOrderException $e) {
    $response->setErrorCode('#5005');
    return $response->getResponse();
  }

  $response->setData($processed_translation_request);
  return $response->getResponse();
}

function _tmgmt_server_srv_job_cancel($reference, $comment) {
  /**
   * @var TMGMTJob $job
   */
  $job = tmgmt_job_load($reference);
  $response = new TMGMTResponse();

  if (empty($job)) {
    $response->setErrorCode('#5006');
    return $response->getResponse();
  }

  if ($job->cancelled() == TMGMT_JOB_STATE_CANCELLED) {
    $response->setEventCode('#4001');
  }
  else {
    $response->setErrorCode('#5007');
  }

  return $response->getResponse();
}

function _tmgmt_server_srv_job_reject($reference, $comment) {

}

/**
 * Service callback to request a key by remote client.
 *
 * @param array $tc_data
 *   Remote client data.
 *
 * @return array
 *   Response result.
 */
function _tmgmt_server_remote_client_create($tc_data) {

  $ds_connector = new TMGMTServerDSConnector(variable_get('tmgmt_ds_url'));
  $ds_response = NULL;
  $verification_token = $tc_data['verification_token'];
  $response = new TMGMTResponse();

  // Verify the token.
  try {
    $ds_response = $ds_connector->getProofOfIdentity($verification_token);
  }
  catch (TMGMTException $e) {
    watchdog_exception('tmgmt_server', $e);
    $response->setErrorCode('#5008');
    return $response->getResponse();
  }

  // Make sure DS has provided sufficient data to create a TC entity.
  if (empty($ds_response['ds_uid']) || empty($ds_response['uuid']) || !valid_email_address($ds_response['mail'])) {
    $response->setErrorCode('#5011');
    return $response->getResponse();
  }

  watchdog('tmgmt_server', 'Client verification token verified by DS party. Received client data: %data',
    array('%data' => var_export($ds_response, TRUE)), WATCHDOG_INFO);

  /**
   * @var TMGMTClientController $client_controller
   */
  $client_controller = entity_get_controller('tmgmt_server_client');
  $client = $client_controller->loadByUUID($ds_response['uuid']);

  // No existing client for provided uuid, create new one.
  if (empty($client)) {
    try {
      $client = $client_controller->createOnTheFly(
        $ds_response['ds_uid'], $ds_response['uuid'], $ds_response['mail'], $ds_response['user_info']);
      watchdog('tmgmt_server', 'Client created with id %cid and associated with account id %uid.',
        array('%cid' => $client->cid, '%uid' => $client->uid),
        WATCHDOG_INFO, l($client->uid, 'user/' . $client->uid));
    }
    catch (TMGMTException $e) {
      watchdog_exception('tmgmt_server', $e);
      $response->setErrorCode('#5009');
      return $response->getResponse();
    }
  }

  // Try to search an existing key prior to creating a new one - we might be
  // contacted by a client that lost its key.
  $tc_key_to_transfer = tmgmt_auth_issuer_controller()->loadByEntity('tmgmt_server_client', $client->cid);
  if (empty($tc_key_to_transfer)) {
    watchdog('tmgmt_server', 'Key issued with pub %pub for client id %cid.',
      array('%pub' => $tc_key_to_transfer->pub, '%cid' => $client->cid), WATCHDOG_INFO);

    $tc_key_to_transfer = tmgmt_auth_issuer_controller()->issue('tmgmt_server_client', $client->cid);
  }

  // Transfer key.
  try {
    $transfer_key = array(
      'pub' => $tc_key_to_transfer->pub,
      'private' => $tc_key_to_transfer->private,
      'created' => $tc_key_to_transfer->created,
      'expires' => $tc_key_to_transfer->expires,
    );
    $ds_connector->createTransferKey($transfer_key, $verification_token);

    watchdog('tmgmt_server', 'Key with pub %pub for client id %cid pushed to DS.',
      array('%pub' => $tc_key_to_transfer->pub, '%cid' => $client->cid), WATCHDOG_INFO);

    // If all went fine, send back to the client an okay message in response.
    $response->setEventCode('#4002');
    return $response->getResponse();
  }
  catch (TMGMTException $e) {
    watchdog_exception('tmgmt_server', $e);
    $response->setErrorCode('#5010');
    return $response->getResponse();
  }
}

function _tmgmt_server_srv_job_access($op, $args) {

  // For all actions we need to be authenticated.
  $authenticated_entity = tmgmt_auth_issuer_controller()->getAuthenticatedEntity();
  if (empty($authenticated_entity)) {
    return FALSE;
  }

  switch ($op) {
    case 'retrieve':
      $job = tmgmt_job_load($args[0]);
      if (empty($job)) {
        return FALSE;
      }
      return $job->uid == $GLOBALS['user']->uid;
      break;
    case 'create':

      if (user_is_anonymous()) {
        return FALSE;
      }

      $roles = array_keys($GLOBALS['user']->roles);
      return in_array(variable_get('tmgmt_server_tc_rid'), $roles);
      break;
    case 'act_cancel':
      $job = tmgmt_job_load($args[0]);
      if (empty($job)) {
        return FALSE;
      }

      if (!$job->isActive()) {
        return FALSE;
      }

      return $job->uid == $GLOBALS['user']->uid;

      break;
    case 'act_reject':

      break;
    case 'rel_get_item':
      $remote_source = tmgmt_server_remote_source_load($args[0]);
      $job_item = tmgmt_server_job_item_load_by_reference('tmgmt_server_remote_source', $remote_source->rsid);

      if (empty($job_item)) {
        return FALSE;
      }

      return $job_item->getJob()->uid == $GLOBALS['user']->uid;
      break;
  }

  return FALSE;
}

/**
 * Access callback for available languages service.
 */
function _tmgmt_server_srv_language_pairs_access() {
  $authenticated_entity = tmgmt_auth_issuer_controller()->getAuthenticatedEntity();

  if (empty($authenticated_entity)) {
    return FALSE;
  }

  return $authenticated_entity->entityType() == 'tmgmt_server_client';
}

/**
 * Access callback for remote clients resource.
 */
function _tmgmt_server_remote_client_access($op, $args) {
  switch ($op) {
    case 'create':

      // Check if we have valid data to go for the process of creating client.
      if (empty($args[0]['verification_token'])) {
        return FALSE;
      }
      try {
        tmgmt_auth_string_parse($args[0]['verification_token']);
      }
      catch (TMGMTRemoteException $e) {
        watchdog('tmgmt_server', 'Attempt to create client entity without verification token.', array(), WATCHDOG_WARNING);
        return FALSE;
      }

      return TRUE;
      break;
  }

  return FALSE;
}

/**
 * Access callback for the status resource.
 *
 * @return bool
 */
function _tmgmt_server_srv_status_access() {
  // We allow all.
  return TRUE;
}
