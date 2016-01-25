<?php

/**
 * Class ZuoraAccount.
 */
class ZuoraAccount {

  /**
   * Gets an account from the API.
   *
   * @param $account_key
   *
   * @return ZuoraAccountObject
   */
  public static function get($account_key) {
    $data = ZuoraRest::instance()->get('/accounts/' . $account_key)->data();
    if ($data['success']) {
      return new ZuoraAccountObject($data);
    }

    return NULL;
  }

  /**
   * Gets an account summary from the API.
   *
   * @param $account_key
   *
   * @return ZuoraAccountObject
   */
  public static function summary($account_key) {
    $data = ZuoraRest::instance()->get('/accounts/' . $account_key . '/summary')->data();
    if ($data['success']) {
      return new ZuoraAccountObject($data);
    }

    return NULL;
  }

  /**
   * Creates an account in the API.
   *
   * @param ZuoraAccountObject $data
   *
   * @return mixed
   */
  public static function create(ZuoraAccountObject $data) {
    return ZuoraRest::instance()->post('/accounts', $data->toArray())->data();
  }

  /**
   * Updates an account in the API.
   *
   * @param $account_key
   * @param ZuoraAccountObject $data
   *
   * @return mixed
   */
  public static function update($account_key, ZuoraAccountObject $data) {
    return ZuoraRest::instance()->put('/accounts/' . $account_key, $data->toArray())->data();
  }
}
