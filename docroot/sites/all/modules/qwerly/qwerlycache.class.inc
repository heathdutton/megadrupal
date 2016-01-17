<?php
/**
 * @file
 * A cached Qwerly object
 *
 * This is the ORM wrapper to Qwerly's API wrapper.
 *
 */
class QwerlyCache extends Qwerly {

  /**
   * Creates a cached Qwerly object, optional key pass.
   * If key is not passed it's gotten from settings.
   */
  function QwerlyCache($key = FALSE) {
    if (!$key) {
      $this->settings = qwerly_get_settings();
      $key = $this->settings->api_key;
    }
    parent::Qwerly($key);
  }

  /**
   * Retrieves entry by ID
   */
  function by_id($id) {
    $result = db_query('SELECT * FROM {qwerly_data} WHERE `id` = :id', array(':id' => $id, ))->fetchObject();
    return $this->handle_row($result);
  }

  /**
   * Retrives entry by a User ID
   */
  function by_uid($uid, $dont_create = FALSE, $extended = FALSE) {
    $result = db_query('SELECT * FROM {qwerly_data} WHERE `uid` = :uid', array(':uid' => $uid, ))->fetchObject();

    // result was FALSE, so is there a user with that ID?
    if (!$result) {
      $user = user_load($uid);
      if ($user && $dont_create === FALSE) {
        $this->create_user_cache($user->uid, $user->mail);
        return $this->by_uid($uid, TRUE, $extended);
      }
      return FALSE;
    }

    return $this->handle_row($result, TRUE, $extended);
  }

  /**
   * Retrives the oldest (expires) record, with optional offset (41 for 42nd oldest, etc.)
   */
  function by_oldest($offset = 0) {
    if (!$this->settings) {
      $this->settings = qwerly_get_settings();
    }

    $c = (!$this->settings->update_indefinitely) ? ' WHERE `stage` < 4 ' : '';

    $result = db_query('SELECT * FROM {qwerly_data} ' . $c . ' ORDER BY `expires` ASC LIMIT :offset, 1', array(':offset' => $offset, ))->fetchObject();
    return $this->handle_row($result);
  }

  /**
   * Retrieves all expired rows.
   */
  function get_expired() {
    if (!$this->settings) {
      $this->settings = qwerly_get_settings();
    }

    $c = (!$this->settings->update_indefinitely) ? ' AND `stage` < 4 ' : '';

    $rows = db_query('SELECT * FROM {qwerly_data} WHERE `expires` < ' . time() . $c . ' ORDER BY `expires` ASC')->fetchAll();

    foreach ($rows as $k => $v) {
      $v = $this->handle_row($v, FALSE, FALSE);
      $o = new QwerlyCache($this->settings->api_key);
      $o->settings = $this->settings;
      $o->data = $v->data;

      $rows[$k] = $o;
    }

    return $rows;
  }

  /**
   * Takes an object and adds it to either the instance itself
   * or to an stdClass which it returns.
   * Optional third flag (extended) causes the related user data
   * to be loaded when set to TRUE
   */
  private function handle_row($object, $local = TRUE, $extended = TRUE) {
    if (!$object) {
      return FALSE;
    }

    $object->data = @json_decode($object->data);

    if (!isset($object->data->profile)) {
      $object->data->profile = new stdClass;
    }

    if (!isset($object->data->profile->services)) {
      $object->data->profile->services = array();
    }


    // data is the raw data, edata is the extended data (with inferred stuff)
    if ($local)
      $ret =& $this;
    else
      $ret = new stdClass;

    $ret->data = $object;

    if ($extended) {
      $ret->edata = $object;
      $ret->edata->ttl = $object->expires - time();
      $ret->edata->status =& $this->data->status;
      $ret->edata->services =& $this->data->profile->services;
    }

    return $extended ? $ret->edata : $ret;
  }

  /**
   * Adds a cache entry for the passed $uid/email to the database
   * Does a Qwerly request.
   */
  function create_user_cache($uid, $email = FALSE) {
    if (!$email) {
      $user = user_load($uid);
      $email = $user->mail;
    }

    // get data from Qwerly.
    $this->email($email);

    if ($this->by_uid($uid, TRUE)) {
      $this->mergesave();
    }
    else {
      // get stage from request status
      $stage = $this->get_stage($this->last_request->status);


      db_insert('qwerly_data')
        ->fields(array(
            'uid' => $uid,
            'expires' => $this->get_expiry($stage),
            'stage' => $stage,
            'data' => json_encode($this->last_request->result),
          ))
        ->execute();
    }
  }

  function get_stage($status, $current = FALSE) {
    // we have done this request some times before
    if ($current && $current >= 2) {
      return $current + 1;
    }

    switch ($status) {
      case 202:
        return 1;
      case 200:
      case 404:
        return max(2, $current + 1);
      default:
        return 0;
    }
  }

  function get_expiry($stage) {
    if (!$this->settings) {
      $this->settings = qwerly_get_settings();
    }

    if ($stage >= 3) {
      return time() + abs($this->settings->cache_time);
    }
    if ($stage == 2) {
      return time() + 120;
    }
    if ($stage == 1) {
      return time() + 30;
    }
    if ($stage == 0) {
      return 0;
    }
  }

  /**
   * merge save - merges data from the result with existing data
   * and writes it to the database.
   *
   * Attempts to remove duplicate services from list
   */
  function mergesave() {

    if (isset($this->last_request->result)) {
      foreach ($this->last_request->result->profile->services as $key => $value) {
        $found = FALSE;

        foreach ($this->data->data->profile->services as $k => $v) {
          if ($v->type == $value->type && ($v->url == $value->url || $v->username == $value->username)) {
            $found = TRUE;
          }
        }

        if (!$found) {
          $this->data->data->profile->services[] = $value;
        }
      }
      $this->data->data->status = $this->last_request->result->status;

      foreach ($this->last_request->result->profile as $key => $value) {
        if ($key != 'services' && $value != $this->data->data->profile->$key) {
          $this->data->data->profile->$key = $value;
        }
      }
    }
    $this->save();
  }

  function save() {
    $this->data->stage = $this->get_stage($this->data->data->status, $this->data->stage);
    $this->data->expires = $this->get_expiry($this->data->stage);

    db_update('qwerly_data')
      ->fields(array(
          'stage' => $this->data->stage,
          'expires' => $this->data->expires,
          'data' => json_encode($this->data->data),
        ))
      ->condition('id', $this->data->id)
      ->execute();

  }

}
