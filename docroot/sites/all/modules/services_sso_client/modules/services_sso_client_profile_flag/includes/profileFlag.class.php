<?php

/**
 * @file
 * Define profile flag classes and related functions.
 */

 /**
  * ProfileFlag class
  */
class ProfileFlag {
  public $string = ''; // Lower-case hyphenated string
  public $title = ''; // Full English name of flag
  public $tid = 0; // Term id on the SSO
  public $actions = array(); // Array containing actions attachable to flags
  public $weight = 0; // Weight
  public $fid = 0; // Auto-incrementing primary key
  public $last_processed = 0;

  /**
   * Set the user flags locally in a table given a remote user account that's already created locally.
   *
   * @param $user
   *  Remote users object.
   */
  public static function saveUserFlags($remote_user) {
    // Find the corresponding local user account.
    $uid = db_query_range("SELECT uid FROM {authmap} WHERE authname=:remote_uid", 0, 1, array(':remote_uid' => $remote_user->uid))->fetchField();

    // If there are flags in the remote account, do stuff.
    if (!empty($uid) && !empty($remote_user->field_taxonomy_flags)) {
      // Delete existing flags first.
      $num_deleted = db_delete('services_sso_client_profile_flag_user')
        ->condition('uid', $uid)
        ->execute();

      // Iterate through each flag in remote user object.
      foreach ($remote_user->field_taxonomy_flags->und as $term) {
        // Find the corresponding flag object's id given the remote term id.
        $fid = db_query_range("SELECT fid FROM {services_sso_client_profile_flag} WHERE tid=:tid", 0, 1, array(':tid' => $term->tid))->fetchField();

        if (!empty($uid) && !empty($term->tid) && !empty($fid)) {
          // Check to see if it exists first before attempting to insert it.
          $match = db_select('services_sso_client_profile_flag_user')
            ->condition('uid', $uid)
            ->condition('fid', $fid)
            ->condition('tid', $term->tid)
            ->countQuery()
            ->execute()
            ->fetchField();
          // Insert into database table the association to the flag.
          if (empty($match)) {
            db_insert('services_sso_client_profile_flag_user')
              ->fields(array(
                'uid' => $uid,
                'fid' => $fid,
                'tid' => $term->tid,
              ))
              ->execute();
          }
        }
      }
    }
  }

  /**
   * Find all users with this flag and apply all the actions of this flag to those users.
   */
  public function process() {
    $result = db_query("SELECT uid FROM {services_sso_client_profile_flag_user} WHERE fid=:fid ORDER BY uid", array(':fid' => $this->fid));

    // Assemble a list of users with this ProfileFlag
    $uids = array();
    foreach ($result as $obj) {
      $uids[] = $obj->uid;
    }

    // Iterate through each action for these users
    foreach ($this->actions as $action) {
      // Invoke post action application hook
      module_invoke_all('services_sso_client_profile_flag_action_pre_apply', $this, $action, $uids);

      $action->apply($uids);

      // Invoke post action application hook
      module_invoke_all('services_sso_client_profile_flag_action_post_apply', $this, $action, $uids);
    }

    // Record the time the processing occured
    $this->last_processed = REQUEST_TIME;
    $this->save();
  }

  /**
   * Find a specific user with this flag and apply all the actions of this flag to that user.
   */
  public function processUser($uid = 0) {

    // Just making sure the user actually has this flag
    $result = db_query_range("SELECT uid FROM {services_sso_client_profile_flag_user} WHERE fid=:fid AND uid=:uid", 0, 1, array(':fid' => $this->fid, ':uid' => $uid))->fetchField();
    if (!empty($result)) {
      // Iterate through each action for these users
      foreach ($this->actions as $action) {
        // Invoke post action application hook
        module_invoke_all('services_sso_client_profile_flag_action_pre_apply', $this, $action, array($uid));

        $action->apply($uid);

        // Invoke post action application hook
        module_invoke_all('services_sso_client_profile_flag_action_post_apply', $this, $action, array($uid));
      }
    }
  }

  /**
   * Find all the flags of the provided user's uid and process each one.
   */
  public static function processUserFlags($uid = 0) {
    $result = db_query("SELECT fid FROM {services_sso_client_profile_flag_user} WHERE uid=:uid", array(':uid' => $uid));
    foreach ($result as $row) {
      $flags = ProfileFlag::loadByID($row->fid);
      foreach ($flags as $flag) {
        $flag->processUser($uid);
      }

      // Clear the static loading cache.
      entity_get_controller('user')->resetCache(array($uid));
    }
  }

  /**
   * Make REST request to get list of all possible flags.
   *
   * @param $save
   *
   */
  public static function retrieve($save = FALSE) {
    $flags = array();

    // Construct proper endpoint URL for the services resource
    $endpoint_url = variable_get('services_sso_client_server_address', '') . '/' . variable_get('services_sso_client_profile_flag_endpoint', '');

    // Make request to REST services user.retrieve
    $response = drupal_http_request($endpoint_url . '/taxonomy_vocabulary');

    // Output some kinda error if there are errors.
    if (!empty($response->error)) {
      drupal_set_message($response->code . ' : ' . $response->error . ' (' . $endpoint_url . ')', 'error');
    }

    if (services_sso_client_verify_response($response)) {
      $vocabularies = json_decode($response->data);

      foreach ($vocabularies as $vocab) {
        if ($vocab->machine_name == 'account_flags') {
          $vid = $vocab->vid;
          break;
        }
      }
    }

    // Once we have the vid, retrieve all the terms of that vocabulary.
    if (!empty($vid)) {
      $headers = array(
        'Content-Type' => 'application/x-www-form-urlencoded',
        'X-CSRF-Token' => _services_sso_client_get_csrf_token(),
      );
      $response = drupal_http_request($endpoint_url . '/taxonomy_vocabulary/getTree', array('headers' => $headers, 'method' => 'POST', 'data' => 'vid=' . $vid));

      if (services_sso_client_verify_response($response)) {
        $data = json_decode($response->data);

        foreach ($data as $i => $obj) {
          $flags[(string)$obj->tid] = new ProfileFlag($obj);

          // Only save if specifically asked to save.
          if ($save) {
            // Check to see if a flag based off the given tid already exists
            // need to update existing record if this is the case.
            if ($existing_flag = $flags[(string)$obj->tid]->exists()) {
              $flags[(string)$obj->tid]->fid = $existing_flag->fid;
              // Got load the actions in order to preserve them
              $flags[(string)$obj->tid]->actions = $flags[(string)$obj->tid]->loadActions();
            }
            $flags[(string)$obj->tid]->save();
          }
        }

        // Save the time of the retrieval.
        if ($save) {
          variable_set('services_sso_client_profile_flag_last_retrieval', REQUEST_TIME);
        }

        return $flags;
      }
    }
  }

  /**
   * Return array of profile flag objects that are not currently in the local database.
   */
  public static function retrieveNew($save = FALSE) {
    $flags = self::retrieve();
    $flags_diff = array();

    // Get all the TID's.
    $remote_tids = array_keys($flags);

    // Get all the TID's from local flags db.
    $result = db_query("SELECT tid FROM {services_sso_client_profile_flag}");
    $local_tids = array();
    foreach ($result as $obj) {
      $local_tids[] = $obj->tid;
    }

    // Get TID's of flags that are not present locally.
    $diff = array_diff($remote_tids, $local_tids);

    foreach ($flags as $tid => $flag) {
      if (in_array($tid, $diff)) {
        $flags_diff[(string)$tid] = $flag;

        // Only save if specifically asked to save.
        if ($save) {
          $flags_diff[(string)$tid]->save();
        }
      }
    }

    // Save the time of the retrieval.
    if ($save) {
      variable_set('services_sso_client_profile_flag_last_retrieval', REQUEST_TIME);
    }

    return $flags_diff;
  }

  /**
   * Get all the profile flag records and return them in an array.
   *
   * @return
   *  Array of all profile flag objects.
   */
  public static function load($type = 'default') {
    $profile_flags = array();
    $args = func_get_args();
    // Get the proper query based on the preset 'type'.
    $result = call_user_func_array('self::loadQuery', $args);

    foreach ($result as $obj) {
      $profile_flags[(string)$obj->tid] = new ProfileFlag($obj);

      // Attempt to load all the actions into there.
      $profile_flags[(string)$obj->tid]->actions = $profile_flags[(string)$obj->tid]->loadActions();
    }

    return $profile_flags;
  }

  /**
   * Shortcut for load('string').
   *
   * @return Array of profile flag objects with string partially matching the provided $string.
   */
  public static function loadByString($string) {
    return self::load('string', $string);
  }

  /**
   * Shortcut for load('fid').
   *
   * @return Array of profile flag objects with primary id matching the provided $fid.
   */
  public static function loadByID($fid) {
    return self::load('id', $fid);
  }

  /**
   * Short cut for load('range').
   *
   * @return Array of profile flags limited by a combination of offset and number of records to get.
   */
  public static function loadByRange($offset = 0, $num = 18446744073709551615) {
    return self::load('range', $offset, $num);
  }

  /**
   * Several preset query types for development convenience.
   *
   * @return
   *  Appropriate Drupal db result object or FALSE if the 'type' parameter not a valid choice.
   */
  public static function loadQuery($type = 'default') {
    $args = func_get_args();

    switch ($type) {
      // Partial string matching.
      case 'string':
        $string = $args[1];
        if (drupal_strlen($string)) {
          return db_select('services_sso_client_profile_flag', 'sf')
            ->fields('sf')
            ->condition('string', "%$string%", 'LIKE')
            ->orderBy('weight', 'DESC')
            ->orderBy('fid')
            ->execute();
        }
        break;
      case 'id':
        $fid = $args[1];
        if (is_numeric($fid) && $fid > 0) {
          return db_select('services_sso_client_profile_flag', 'sf')
            ->fields('sf')
            ->condition('fid', $fid)
            ->orderBy('weight', 'DESC')
            ->range(0, 1)
            ->execute();
        }
        break;
      case 'range':
        return db_select('services_sso_client_profile_flag', 'sf')
          ->fields('sf')
          ->orderBy('weight', 'DESC')
          ->orderBy('fid')
          ->range($args[1], $args[2])
          ->execute();
        break;
      case 'weight_asc':
        return db_select('services_sso_client_profile_flag', 'sf')
          ->fields('sf')
          ->orderBy('weight')
          ->orderBy('fid')
          ->execute();
        break;
      // All results.
      default:
        return db_select('services_sso_client_profile_flag', 'sf')
          ->fields('sf')
          ->orderBy('weight', 'DESC')
          ->orderBy('fid')
          ->execute();
        break;
    }
    return FALSE;
  }

/**
 * Constructor.
 *
 * @param $obj
 *  A mixed type variable. The constructor will take care of figuring out how to create a ProfileFlag object from this.
 *
 * @return
 *  Will return TRUE if $this->tid becomes a non-zero integer.
 */
  function __construct($obj = array()) {
    // If given obj argument
    if (is_object($obj)) {
      // Iterate through the returned record and populate as much as possible.
      foreach ($obj as $key => $val) {
        if (isset($this->$key)) {
          $this->$key = $val;
        }
      }

      // Check to see if there are any fields that are not set because we populated from a taxonomy term
      if (isset($obj->vid)) {
        $this->title = $obj->name;
        $this->string = self::html_class($obj->name);
      }
    }

    return (!empty($this->tid)) ? TRUE : FALSE;
  }

  /**
   * Add a ProfileFlagAction object to the end of $this->actions.
   *
   * @param $obj
   *  A mixed type variable. The constructor will take care of figuring out how to create a ProfileFlagAction object from this.
   *
   * @return
   *  Will return the newly created ProfileFlagAction object if successful.
   */
  function pushAction($obj) {
    if ($action = new ProfileFlagAction($obj)) {
      $this->actions[] = $action->map();
      return $action;
    }

    return FALSE;
  }

  /**
   * Shortcut for deleteAction(0).
   */
  function popAction() {
    return $this->deleteAction(0);
  }

  /**
   * Delete
   */
  function deleteAction($position = 0) {
    if (isset($this->actions[$position])) {
      unset($this->actions[$position]);
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Delete the action with the corresponding ID.
   *
   * @param $faid
   *  Matching faid to delete.
   */
  function deleteActionByID($faid = 0) {
    if (!empty($faid) && is_numeric($faid)) {
      foreach ($this->action as $i => $action) {
        if ($action->faid == $faid) {
          // Call member delete method.
          $this->deleteAction($i);
        }
      }
    }
  }

  /**
   * Load all actions belonging to this flag from the DB.
   */
  function loadActions() {
    $actions = array();

    $result = db_query("SELECT * FROM {services_sso_client_profile_flag_action} WHERE fid=:fid ORDER BY weight DESC", array(':fid' => $this->fid));

    foreach ($result as $obj) {
      if ($action = new ProfileFlagAction($obj)) {
        $actions[] = $action->map();
      }
    }
//    $this->actions = $actions;

    return $actions;
  }

  /**
   * Destructor.
   */
  function __destruct() {
    foreach ($this->actions as $key => $actions) {
      unset($this->actions[$key]);
    }
  }

  /**
   * Delete the corresponding record from the database.
   *
   * @return
   *  Return TRUE if deletion worked all the way through.
   */
  function delete() {
    if (!empty($this->fid)) {
      // First delete all the actions associated with this.
      foreach ($this->actions as $key => $action) {
        if ($action->delete()) {
          unset($this->actions[$key]);
        }
      }

      // Somehow actions didn't all get deleted first, fail
      if (count($this->actions) > 0) {
        return FALSE;
      }

      // Now delete the record
      if ($num_deleted = db_delete('services_sso_client_profile_flag')->condition('fid', $this->fid)) {
        // Reset the fid.
        $this->fid = 0;
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Create or update a profile flag record.
   *
   * This function will create/update any actions that are part of this profile flag
   * first.
   */
  function save() {
    // Setup the data to save.
    $record = array(
      'string' => $this->string,
      'title' => $this->title,
      'tid' => $this->tid,
      'weight' => $this->weight,
      'last_processed' => $this->last_processed,
    );
    // Check to see if a flag based off the given tid already exists
    // need to update existing record if this is the case.
    if ($existing_flag = $this->exists()) {
      $this->fid = $existing_flag->fid;

      $record['last_processed'] = ($this->last_processed > $existing_flag->last_processed) ? $this->last_processed : $existing_flag->last_processed;
    }

    // Write records to DB.
    if (!empty($this->fid)) {
      $record['fid'] = $this->fid;
      $result = drupal_write_record('services_sso_client_profile_flag', $record, 'fid');
    }
    else {
      $result = drupal_write_record('services_sso_client_profile_flag', $record);
    }

    $record = (object) $record;

    // If we were creating a new record, once it's created, save the fid back to the object.
    switch ($result) {
      case SAVED_NEW:
        $this->fid = $record->fid;
      case SAVED_UPDATED:
        foreach ($this->actions as $i => $action)  {
          $this->actions[$i]->fid = $this->fid;
        }
        $this->saveActions();
        break;
      case FALSE:

        break;
    }

    return $result;
  }

  /**
   * Create/update records for actions.
   *
   * @return
   *  The number of records successfully written is returned.
   */
  function saveActions() {
    $count = 0;

    $old_actions = $this->loadActions();
    $faids_to_save = self::get_faids($this->actions);
    // Delete from the database all the flag actions that will no longer be used.
    foreach ($old_actions as $old_action) {
      if (!in_array($old_action->faid, $faids_to_save)) {
        $old_action->delete();
      }
    }
    // Proceed to save all new and updated actions.
    foreach ($this->actions as $action) {
      if ($action->save($count)) {
        $count++;
      }
    }
    return $count;
  }

  /**
   * Cherry-pick the faid's from an array of flag actions.
   */
  static function get_faids($actions = array()) {
    $faids = array();
    foreach ($actions as $action) {
      if (!empty($action->faid)) {
        $faids[] = $action->faid;
      }
    }
    return $faids;
  }

  /**
   * Check to see if the current flag object exists in the database already.
   */
  function exists() {
    $record = db_select('services_sso_client_profile_flag', 'sf')
      ->fields('sf')
      ->condition('tid', $this->tid)
      ->range(0, 1)
      ->execute()
      ->fetch();
    return new self($record);
  }

  static function clean_css_identifier($identifier, $filter = array(' ' => '-', '_' => '-', '/' => '-', '[' => '-', ']' => '')) {
    // By default, we filter using Drupal's coding standards.
    $identifier = strtr($identifier, $filter);

    // Valid characters in a CSS identifier are:
    // - the hyphen (U+002D)
    // - a-z (U+0030 - U+0039)
    // - A-Z (U+0041 - U+005A)
    // - the underscore (U+005F)
    // - 0-9 (U+0061 - U+007A)
    // - ISO 10646 characters U+00A1 and higher
    // We strip out any character not in the above list.
    $identifier = preg_replace('/[^\x{002D}\x{0030}-\x{0039}\x{0041}-\x{005A}\x{005F}\x{0061}-\x{007A}\x{00A1}-\x{FFFF}]/u', '', $identifier);

    return $identifier;
  }

  /**
   * Helper function for Drupal 7's drupal_html_class() function in D6.
   */
  static function html_class($class) {
    return self::clean_css_identifier(drupal_strtolower($class));
  }
}
