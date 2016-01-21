<?php

/**
 * @package    d4os_io rest
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
class D4OS_IO_rest_Users implements D4OS_IO_Users_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->response = new stdClass();
    $this->values = array();
  }

  function ping() {
    $this->response->success = FALSE;
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_users/ping/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = TRUE;
      }
    }
    return;
  }

  function load_user($values) {
    $this->response->success = FALSE;
    $params = '';

    // parse values
    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    // request the server
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_users/load_user/' . $params . 'format/json');

    // parse the answer
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if (is_array($data->data)) {
        $this->response->success = TRUE;
        return $data->data;
      }
    }
    return NULL;
  }

  function save_user($grid_user) {
    $data = array("data" => json_encode($grid_user));
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($data, '', '&')
    );
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_users/save_user/format/json', $options);
    $data = json_decode($result->data);
    return $data->data;
  }

  function delete_user($uuid) {
    $this->response->success = FALSE;
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_users/delete_user/uuid/' . rawurlencode($uuid) . '/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = TRUE;
        if (is_array($data->messages)) {
          foreach ($data->messages as $message) {
            drupal_set_message(check_plain($message));
          }
        }
      }
    }
    return;
  }

  function get_online_users_list($values) {
    $this->response->success = FALSE;
    $params = '';
    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_users/get_online_users_list/' . $params . '/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_object($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function set_user_level($params) {
    $data = array("data" => json_encode($params));
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($data, '', '&')
    );
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_users/set_user_level/format/json', $options);
    $data = json_decode($result->data);
    return $data;
  }

  /**
   * Assign a grid uuid to a Drupal user id
   * @param Integer Drupal user id
   * @param Integer Grid user uuid
   */
  function set_uuid($uid, $uuid) {
    // check the link between drupal and the grid
    $uid_exists = db_query("SELECT uid FROM {d4os_ui_users} WHERE UUID=:uuid OR uid=:uid", array(':uuid' => $uuid, ':uid' => $uid))->fetchField();
    if ($uid_exists) {
      db_query("DELETE FROM {d4os_ui_users} WHERE UUID = :uuid OR uid = :uid", array(':uuid' => $uuid, ':uid' => $uid));
    }
    db_query("INSERT INTO {d4os_ui_users} (UUID, uid) VALUES (:uuid, :uid)", array(':uuid' => $uuid, ':uid' => $uid));
  }

}