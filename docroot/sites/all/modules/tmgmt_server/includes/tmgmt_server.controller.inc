<?php

/**
 * Entity controller for the remote source entity.
 */
class TMGMTRemoteSourceController extends EntityAPIController {

  /**
   * Overrides EntityAPIController::save().
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    // Update the 'changed' timestamp.
    $entity->changed = REQUEST_TIME;

    parent::save($entity, $transaction);
  }

}

/**
 * DS entity controller class.
 */
class TMGMTDirectoryServerController extends EntityAPIController {

  /**
   * Gets DS by url.
   *
   * @param string $url
   * @return TMGMTDirectoryServer
   */
  function loadByUrl($url) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'tmgmt_directory_server');
    $query->propertyCondition('url', $url);
    $result = $query->execute();

    if (isset($result['tmgmt_directory_server'])) {
      $result = array_keys($result['tmgmt_directory_server']);
      return entity_load_single('tmgmt_directory_server', array_shift($result));
    }

    return NULL;
  }
}

/**
 * Entity controller for the client entity.
 */
class TMGMTClientController extends EntityAPIController {

  public function create(array $values = array()) {
    $values['create'] = time();
    return parent::create($values);
  }

  /**
   * Overrides EntityAPIController::save().
   *
   * On insert action will issue a key for the client.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $result = parent::save($entity, $transaction);

    if ($result == SAVED_NEW) {
      tmgmt_auth_issuer_controller()->issue('tmgmt_server_client', $entity->cid);
    }

    return $result;
  }

  /**
   * Overrides EntityAPIController::delete()
   *
   * Will delete associated keys.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {

    foreach ($ids as $id) {
      $keys = tmgmt_auth_issuer_controller()->loadAllByEntity('tmgmt_server_client', $id);
      tmgmt_auth_issuer_controller()->delete(array_keys($keys), $transaction);
    }

    parent::delete($ids, $transaction);
  }

  /**
   * Will create a new user account and a remote client associated with it.
   *
   * In case there is an existing account for supplied email address the client
   * will be associated with this account.
   *
   * @param int $ds_uid
   *   Unique user account identifier.
   * @param string $uuid
   *   Client UUID provided by DS.
   * @param string $mail
   *   User email to create the user account.
   * @param array $user_info
   *   Additional user info defined by xNal standard.
   *
   * @return TMGMTClient
   *   Client entity.
   * @throws TMGMTException
   *   In case tmgmt_server_create_user() did not succeeded.
   */
  function createOnTheFly($ds_uid, $uuid, $mail, $user_info = array()) {

    $account = $this->loadUserByDSUid($ds_uid);
    $label = t('Remote Client (UUID: @uuid)', array('@uuid' => $uuid));

    if (empty($account)) {
      $tc_rid = variable_get('tmgmt_server_tc_rid');
      $name = tmgmt_server_get_user_login_from_email($mail);
      $account = tmgmt_server_create_user($name, $mail, array($tc_rid => $tc_rid), array(
        'uuid' => $uuid,
        'ds_uid' => $ds_uid,
      ));
    }

    /**
     * @var TMGMTClientController $tc_controller
     */
    $tc_controller = entity_get_controller('tmgmt_server_client');
    $tc = $tc_controller->loadByUUID($uuid);

    // If TC was not created during user_save process, create it.
    if (empty($tc)) {
      $tc = $tc_controller->create(array(
        'label' => $label,
        'uuid' => $uuid,
        'uid' => $account->uid,
        'ds_uid' => $ds_uid,
      ));
      $tc_controller->save($tc);
    }

    rules_invoke_all('tmgmt_server_client_created_via_service', $account, $tc, $user_info);

    return $tc;
  }

  /**
   * Gets client entity by its UUID.
   *
   * @param string $uuid
   *   Client UUID.
   *
   * @return TMGMTClient
   */
  function loadByUUID($uuid) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'tmgmt_server_client');
    $query->propertyCondition('uuid', $uuid);
    $result = $query->execute();

    if (isset($result['tmgmt_server_client'])) {
      $result = array_keys($result['tmgmt_server_client']);
      return entity_load_single('tmgmt_server_client', array_shift($result));
    }

    return NULL;
  }

  /**
   * Gets client entity based on ds uid.
   *
   * @param int $ds_uid
   *   DS unique identifier.
   *
   * @return object
   *   User account
   */
  function loadUserByDSUid($ds_uid) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'tmgmt_server_client');
    $query->propertyCondition('ds_uid', $ds_uid);
    $result = $query->execute();

    if (isset($result['tmgmt_server_client'])) {
      $result = array_keys($result['tmgmt_server_client']);
      $client = tmgmt_server_client_load(array_shift($result));
      return user_load($client->uid);
    }

    return NULL;
  }
}
