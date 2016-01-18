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
class D4OS_IO_rest_Profiles implements D4OS_IO_Profiles_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->response = new stdClass();
    $this->values = array();
  }

  function ping() {
    $this->response->success = FALSE;
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/ping/format/json');
    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      if ($data->status === 'success') {
        $this->response->success = TRUE;
      }
    }
    return;
  }

  function load_profile($values) {
    $this->response->success = FALSE;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/load_profile/' . $params . 'format/json');

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data[0];
      }
    }
    return NULL;
  }

  function save_profile($values) {
    $data = array("data" => json_encode($values));
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($data, '', '&')
    );
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/save_profile/format/json', $options);
    if (isset($result) && in_array($result->code, array('200'))) {
      drupal_set_message(t('User profile added'));
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function delete_profile($useruuid) {
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/delete_profile/useruuid/' . $useruuid . '/format/json');
    if (isset($result) && in_array($result->code, array('200'))) {
      drupal_set_message(t('User removed from profiles.'));
      return TRUE;
    }
    return FALSE;
  }

  /*
   * Profile services
   */

  function avatarclassifiedsrequest($values) {
    $this->response->success = FALSE;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatarclassifiedsrequest/' . $params . 'format/json');

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function classified_update($values) {
    $data = array("data" => json_encode($values));
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($data, '', '&'),
      'timeout' => 5
    );
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/classified_update/format/json', $options);
    if (isset($result) && in_array($result->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function classified_delete($values) {
    $id = $values['classifiedID'];
    $this->response->success = FALSE;
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/classified_delete/classifiedID/' . rawurlencode($id) . '/format/json');
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

  function avatarpicksrequest($values) {
    $this->response->success = FALSE;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatarpicksrequest/' . $params . 'format/json');

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function pickinforequest($values) {
    $this->response->success = FALSE;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/pickinforequest/' . $params . 'format/json');

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function picks_update($values) {
    $data = array("data" => json_encode($values));
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($data, '', '&')
    );
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/picks_update/format/json', $options);
    if (isset($result) && in_array($result->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function picks_delete($values) {
    $id = $values['pick_id'];
    $this->response->success = FALSE;
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/picks_delete/pick_id/' . rawurlencode($id) . '/format/json');
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

  function avatarnotesrequest($values) {
    $this->response->success = FALSE;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatarnotesrequest/' . $params . 'format/json');

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function avatar_notes_update($values) {
    $data = array("data" => json_encode($values));
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($data, '', '&')
    );
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_notes_update/format/json', $options);
    if (isset($result) && in_array($result->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function avatar_properties_request($values) {
    $this->response->success = FALSE;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_properties_request/' . $params . 'format/json');

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function avatar_properties_update($values) {
    $data = array("data" => json_encode($values));
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($data, '', '&')
    );
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_properties_update/format/json', $options);

    if (isset($result) && in_array($result->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => $result,
    );
  }

  function avatar_interests_request($values) {
    $this->response->success = FALSE;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_interests_request/' . $params . 'format/json');

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function avatar_interests_update($values) {
    $data = array("data" => json_encode($values));
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($data, '', '&')
    );
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/avatar_interests_update/format/json', $options);
    if (isset($result) && in_array($result->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

  function user_preferences_request($values) {
    $this->response->success = FALSE;
    $params = '';

    if (is_array($values)) {
      foreach ($values as $key => $value) {
        $params .= $key . '/' . rawurlencode($value) . '/';
      }
    }

    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/user_preferences_request/' . $params . 'format/json');

    if (isset($result) && in_array($result->code, array('200')) && $result->data != '') {
      $data = json_decode($result->data);
      $this->response->success = TRUE;
      if (is_array($data)) {
        return $data;
      }
    }
    return NULL;
  }

  function user_preferences_update($values) {
    $options = array("data" => json_encode($values));
    $result = d4os_io_rest_http_request(D4OS_IO_REST_URL . '/index.php/d4os_io_rest_profiles/user_preferences_update/format/json', $options);
    if (isset($result) && in_array($result->code, array('200'))) {
      return array(
        'success' => TRUE,
        'errorMessage' => "",
      );
    }
    return array(
      'success' => FALSE,
      'errorMessage' => "",
    );
  }

}