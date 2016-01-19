<?php

class KalturaNotificationClient {
  public $id;
  public $type;
  public $puser;
  public $partner;
  public $data;
  public $multi = FALSE;
  public $valid_signature = NULL;

  function KalturaNotificationClient($notification_params = array(), $admin_secret = NULL, $validate_sig = TRUE) {
    if (!count($notification_params)) {
      return $this;
    }
    if ($validate_sig) {
      $this->validate_signature($notification_params, $admin_secret);
      if (!$this->valid_signature) {
        return $this;
      }
    }

    $this->id = $notification_params['notification_id'];
    $this->type = $notification_params['notification_type'];
    $this->puser = $notification_params['puser_id'];
    $this->partner = $notification_params['partner_id'];

    $data = array();
    foreach ($notification_params as $k => $v) {
      switch ($k) {
        case 'partner_id':
          break;

        default:
          $data[$k] = $v;
      }
    }
    if (isset($data['multi_notification']) && $data['multi_notification'] === 'true') {
      $this->multi = TRUE;
      $res = $this->splitMultiNotifications($data);
    }
    else {
      $res[0] = $data;
    }
    $this->data = $res;
    return $this;
  }

  function splitMultiNotifications($data) {
    $not_data = array();
    foreach ($data as $name => $value) {
      $match = preg_match("/^(not[^_]*)_(.*)$/", $name, $parts);
      if (!$match) {
        continue;
      }

      $not_name_parts = $parts[1];
      $not_property = @$parts[2];
      $num = (int) str_replace('not', '', $not_name_parts);
      $not_data[$num][$not_property] = $value;
    }
    return $not_data;
  }

  function validate_signature($notification_params, $admin_secret) {
    ksort($notification_params);
    $str = '';
    $valid_params = array();

    if (key_exists('signed_fields', $notification_params)) {
      $valid_params = explode(',', $notification_params['signed_fields']);
    }

    foreach ($notification_params as $k => $v) {
      if ($k == 'sig') {
        continue;
      }

      if (!in_array($k, $valid_params) && count($valid_params) > 1 && !$notification_params['multi_notification']) {
        if ($k != 'multi_notification' && $k != 'number_of_notifications') {
          continue;
        }
      }

      $str .= $k . $v;
    }

    if (md5($admin_secret . $str) == $notification_params['sig']) {
      $this->valid_signature = TRUE;
    }
    else {
      $this->valid_signature = FALSE;
    }
  }
}
