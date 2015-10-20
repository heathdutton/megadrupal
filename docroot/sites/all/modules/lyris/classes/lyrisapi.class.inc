<?php
/**
 * @file
 * This is our version agnostic API.  These are the only methods that should be
 * called from within modules.  We map each call here to the appropriate Lyris
 * API calls so the system will run across various Lyris API versions.
 *
 * Method Name Convention:
 *   - Methods in the form of 'getOBJECT()' will return an object defined by
 *     one of our API classes.
 *
 *   - Methods in the form of 'loadOBJECT()' will return an object struct
 *     directly from Lyris.
 *
 *   - Get is local.  Load is remote.
 */
class LyrisApi extends LyrisConn {
  /**
   * Constructor
   */
  public function LyrisApi($account, $mode) {
    parent::__construct($account, $mode);

    if ($this->isConnected()) {
      try {
        // Load information about the current Lyris user.
        $this->loadUserData();
      } catch (Exception $e) {$this->handleException($e);}
    }
  }

  /*****************************************************************************
  * LyrisAPI Helpers
  * All connections to Lyris should invoke one of these methods.
  *****************************************************************************/
  /**
   * Invoke an API call while handling faults and errors.
   */
  public function call() {
    $args = func_get_args();
    $method = array_shift($args);

    // Ensure the LyrisAPI object has established a connection.
    if (!$this->isConnected()) {
      throw new LyrisConnectionException($this);
    }

    // Log the lyris call
    if (lyris_show_devel('lyris_devel_log_api', TRUE)) {
      lyris_devel_log(array("{$this->username}: $method", $this->user), 'api call');
    }

    try {
      $this->checkMode(__FUNCTION__);

      if (method_exists($this->mlapi, $method)) {
        $res = call_user_func_array(array($this->mlapi, $method), $args);

        // Check for Lyris faults and errors
        if ($this->mlapi->fault) {
          throw new LyrisApiException($this, $this->mlapi->faultstring . '.', WATCHDOG_CRITICAL);
        }
        elseif ($err = $this->mlapi->getError()) {
          if (lyris_show_devel('lyris_devel_debug', TRUE)) {
            $err .= '<hr /><pre>' . $this->mlapi->debug_str . '</pre>';
          }
          throw new LyrisApiException($this, $err);
        }
        else {
          return $res;
        }
      }
      else {
        throw new LyrisMissingApiException($this, $method, TRUE);
      }
    } catch (Exception $e) {$this->handleException($e);}

    return FALSE;
  }

  /**
   * Run a Lyris SQL Select Query
   *
   * Only server admins have access to the sql interface so if we cannot
   * determine that the current user is a server admin we will need to create a
   * new, administrative connection to Lyris in order to run the command.
   *
   * @param $sql
   *   A sql query with formatted placeholders.
   * @param $args
   *   An array of arguments for the sql placeholders.
   * @param $labels
   *   Substitution labels for Lyris db fields since it won't let us use 'as'
   *   aliasing.
   * @return array
   *   An array of DB rows or empty.
   */
  public function lyrisSelect($sql, $args = array(), $labels = array()) {
    // Ensure the LyrisAPI object has established a connection.
    if (!$this->isConnected()) {
      throw new LyrisConnectionException($this);
    }

    $this->checkMode(__FUNCTION__);

    $_args = array_merge(array($sql), $args);
    $query = call_user_func_array('sprintf', $_args);

    // Display the query in debug mode.
    if (lyris_show_devel() && variable_get('lyris_devel_show_queries')) {
      $this->setMessage($query, 'debug');
    }

    // Ensure we can have SQL access.
    if ($this->accountIsAdmin() || $this->isServerAdmin()) {
      $res = $this->call('sqlSelect', $query);
    }
    else {
      $lyris_admin = lyris('admin', $this->mode);
      $res = $lyris_admin->call('sqlSelect', $query);
    }

    // Parse the results into a usable array.
    $cols = array_shift($res);
    $ret = array();
    foreach ($res as $row => $rows) {
      foreach ($rows as $key => $field) {
        $ret[$row][(isset($labels[$cols[$key]]) ? $labels[$cols[$key]] : $cols[$key])] = $field;
      }
    }

    return $ret;
  }

  /**
   * Run a Lyris SQL Update Query
   *
   * Only server admins have access to the sql interface so if we cannot
   * determine that the current user is a server admin we will need to create a
   * new, administrative connection to Lyris in order to run the command.
   *
   * @param $sql
   *   A sql query with formatted placeholders.
   * @param $args
   *   An array of arguments for the sql placeholders.
   * @return array
   *   Boolean result.
   */
  public function lyrisUpdate($table, $args, $where) {
    // Ensure the LyrisAPI object has established a connection.
    if (!$this->isConnected()) {
      throw new LyrisConnectionException($this);
    }

    $this->checkMode(__FUNCTION__);

    // Ensure we can have SQL access.
    if ($this->accountIsAdmin() || $this->isServerAdmin()) {
      return $this->call('sqlUpdate', $table, $args, $where);
    }
    else {
      $lyris_admin = lyris('admin', $this->mode);
      return $lyris_admin->call('sqlUpdate', $table, $args, $where);
    }
  }

  /**
   * Check that the mode we are operating in allows us to connect to Lyris and
   * throw an exception if we can't.
   */
  protected function checkMode($func = NULL) {
    if ($this->mode == LYRIS_MODE_OFFLINE) {
      throw new LyrisModeException($this, $func);
    }

    if ($this->mode != variable_get('lyris_mode')) {
      // This is killing forced modes.  Do we really need it?
      //throw new LyrisModeException($this, $func, 'The request at %_func was for the %_mode server but the current mode is set to %_lyris_mode.');
    }
  }

  /*****************************************************************************
  * API Mapping Functions
  *****************************************************************************/
  /**
   * Create list content.
   */
  public function createContent($lc) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          return $this->call('CreateContent', $lc->struct);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Create a new Lyris list.
   */
  public function createList($list) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
        case '10':
        case '11':
        case '12':
          $result = $this->call('CreateList', $list->type, $list->struct['ListName'], $list->struct['ShortDescription'], $this->username, $this->user_mail, password_field_decrypt($this->userpass), $list->struct['Topic']);

          // If the creation was a success, store the ID and update the list
          // with the full struct settings.
          if (is_numeric($result)) {
            $list->struct['ListID'] = $result;
            $list->setLocalVal('ListID', $result);
            $this->updateList($list);
          }
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
      if ($list->structVal('ListID')) {
        $this->watchdog(t('New Lyris list %l created.', array('%l' => $list->struct['ListName'])));
        return TRUE;
      }
    } catch (Exception $e) {$this->handleException($e);}

    return FALSE;
  }

  /**
   * Create many members
   */
  public function createManyMembers($TinyMemberStruct, $ListName) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
        case '10':
        case '11':
        case '12':
          return $this->call('CreateManyMembers', $TinyMemberStruct, $ListName, FALSE);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Create a single member.
   */
  public function createMember($member) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
        case '10':
        case '11':
        case '12':
          $MemberID = $this->call('CreateSingleMember', $member->structVal('EmailAddress'), $member->structVal('FullName'), $member->structVal('ListName'));

          if (is_numeric($MemberID)) {
            $member->setLocalVal('MemberID', $MemberID);
            $member->save();
            $member->changedFieldClear();
            return TRUE;
          }
          else {
            return FALSE;
          }

          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Delete content.
   */
  public function deleteContent($lyris_content) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          if ($this->call('DeleteContent', $lyris_content->struct)) {
            $this->watchdog(t('Content %title deleted from Lyris.', array('%title' => $lyris_content->localVal('Title'))));
            return TRUE;
          }
          else {
            return FALSE;
          }
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Delete a list from Lyris.
   */
  public function deleteList($ListName) {
    try {
      $this->checkMode(__FUNCTION__);

      if ($this->call('DeleteList', $ListName)) {
        $this->watchdog(t('List %name deleted from Lyris.', array('%name' => $ListName)));
        return TRUE;
      }
      else {
        return FALSE;
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Get a fully loaded list object.
   *
   * @param $params
   *   An array containing parameters to load a list from Lyris.  Valid options
   *   are ListID or ListName.
   *
   * @return $list
   *   A list with a loaded Lyris struct.
   */
  public function getList($params = array()) {
    $list = new LyrisList();

    try {
      $this->checkMode(__FUNCTION__);

      if (isset($params['ListID'])) {
        $result = $this->lyrisSelect("SELECT name_ FROM lists_ WHERE listid_ = %d", array($params['ListID']), array('name_' => 'ListName'));
        return $this->loadList(array('ListName' => $result[0]));
      }
      if (isset($params['ListName'])) {
        $list->setStruct($this->loadList(array('ListName' => $params['ListName'])));
        return $list;
      }
    } catch (Exception $e) {$this->handleException($e);}

    return $list;
  }

  /**
   * Retrieve a list of all lists a given email belongs to.
   */
  public function getMemberLists($mail) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
        case '10':
        case '11':
        case '12':
          return $this->call('EmailOnWhatLists', $mail);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Import a SimpleMailingStruct from a lyris content.
   */
  public function importContent($ContentID) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          return $this->call('ImportContent', $ContentID);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Is the current user a server admin?
   */
  public function isServerAdmin() {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
          return (isset($this->user['serveradm_']) && $this->user['serveradm_'] == 'T');
          break;

        case '10':
        case '11':
        case '12':
          return (isset($this->user['SERVERADM_']) && $this->user['SERVERADM_'] == 'T');
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Is the current user a site admin?  If so, return the sites they have
   * permission to.
   */
  public function isSiteAdmin() {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
          return ((isset($this->user['siteadmin_']) && $this->user['siteadmin_'] == 'T') ? $this->user['whatsites_'] : FALSE);
          break;

        case '10':
        case '11':
        case '12':
          return ((isset($this->user['SITEADMIN_']) && $this->user['SITEADMIN_'] == 'T') ? $this->user['WHATSITES_'] : FALSE);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Determine whether a list name already exists.
   *
   * @param $name
   *   The Lyris listname to search for.
   */
  public function listExists($name) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
        case '10':
        case '11':
        case '12':
          $sql = "SELECT listid_ FROM lists_ WHERE name_ = '%s'";
          $list = $this->lyrisSelect($sql, array($name));
          return !empty($list);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Load a full Lyris List.
   */
  public function loadList($params = array()) {
    try {
      $this->checkMode(__FUNCTION__);

      if (isset($params['ListID'])) {
        $result = $this->lyrisSelect("SELECT name_ FROM lists_ WHERE listid_ = %d", array($params['ListID']), array('name_' => 'ListName'));

        if (empty($result[0])) {
          throw new Exception(t('Unable to load list by ListID %id.', array('%id' => $params['ListID'])), WATCHDOG_ERROR);
        }
        else {
          return $this->loadList(array('ListName' => $result[0]['ListName']));
        }
      }
      elseif (isset($params['ListName'])) {
        $result = $this->call('SelectLists', $params['ListName'], '');

        if (empty($result[0])) {
          throw new Exception(t('Unable to load list by ListName %name.', array('%name' => $params['ListName'])), WATCHDOG_ERROR);
        }
        else {
          return $result[0];
        }
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Get a list of Lyris List names for a given list admin.
   *
   * @param $account
   *   The user account to search for.
   * @return $lists
   *   An array of list names.
   */
  public function loadListsByAdmin($account = NULL, $filter = NULL, $limit = 0) {
    $lists = array();
    $cond = array();
    $mail  = (is_object($account) && isset($account->mail) ? $account->mail : $this->user_mail);

    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
          $where = ($filter ? "AND m.list_ LIKE '%%%s%%'" : '');
          $sql = "SELECT m.list_, l.listid_ FROM members_ m LEFT JOIN lists_ l ON m.list_ = l.name_ WHERE LOWER(m.emailaddr_) = '%s' AND m.islistadm_ = 'T' $where LIMIT $limit";
          $lists = $this->lyrisSelect($sql, array(strtolower($mail), $filter));

          $ret = array();
          foreach ($lists as $list) {
            $ret[$list['listid_']] = $list['list_'];
          }
          return $ret;
          break;

        case '10':
        case '11':
        case '12':
          // Prepare conditions
          if ($filter) {
            $cond[] = "m.LIST_ LIKE '%%%s%%'";
          }
          if ((int) $limit > 0) {
            $cond[] = "ROWNUM <= " . (int) $limit;
          }
          $_cond = (!empty($cond) ? 'AND ' . implode(' AND ', $cond) : '');

          $sql = "SELECT m.LIST_, l.LISTID_ FROM MEMBERS_ m LEFT JOIN lists_ l ON m.LIST_ = l.NAME_ WHERE LOWER(m.EMAILADDR_) = '%s' AND m.ISLISTADM_ = 'T' {$_cond}";
          $lists = $this->lyrisSelect($sql, array(strtolower($mail), $filter));

          $ret = array();
          foreach ($lists as $list) {
            $ret[$list['LISTID_']] = $list['LIST_'];
          }
          return $ret;
          break;


        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}

    return $lists;
  }

  /**
   * Get a list of members for a given list.
   *
   * @return array || FALSE
   *   Will return an array of all list members or FALSE if the list cannot be
   *   loaded.
   */
  public function loadListMembers($ListName, $fields = array(), $filters = array(), $key = NULL) {
    $members = array();

    // Add the ListName to the filters.
    $filters['ListName'] = $ListName;

    // Build a static filter for Lyris.
    foreach ($filters as $field  => $value) {
      $filter[] = "{$field} = {$value}";
    }

    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
          if ($res = $this->call('SelectMembers', $filter)) {
            foreach ($res as $member) {
              $members[strtolower($member['EmailAddress'])] = $member;
            }
          }
          else {
            return FALSE;
          }
          break;

        case '10':
        case '11':
        case '12':
          // If we only want specific fields, use the more efficient
          // SelectMembersEx function.
          if (!empty($fields)) {
            if ($res = $this->call('SelectMembersEx', $fields, $filter)) {
              $members = $this->processSelectResults($res, $key);
            }
            else {
              return FALSE;
            }
          }
          else {
            if ($res = $this->call('SelectMembers', $filter)) {
              $members = $res;
            }
            else {
              return FALSE;
            }
          }
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}

    return (!empty($members) ? (array) $members : array());
  }

  /**
   * Wrapper for a direct query.
   */
  public function loadListMembersRange($fields = array(), $filters = array(), $range = array(0, 50)) {
    $_fields = '';

    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          // Translate struct format fields to db format.
          $labels = array();
          foreach ($fields as &$field) {
            $labels[$this->structNameToDB($field)] = $field;
          }
          $fields = array_keys($labels);

          // Build the fields string
          $_fields = (!empty($fields) ? implode(', ', $fields) : '*');

          // Build the filters
          $_filters = array();
          foreach ($filters as $field => $value) {
            $_filters[] = $this->structNameToDB($field) . " = " . (is_numeric($value) ? '%d' : "'%s'");
          }
          $_filter = (!empty($filters) ? implode(' AND ', $_filters) : '1');

          // Set the range
          $_range = ($range[1] > 0 ? "AND rownum <= {$range[1]}" : '');

          $sql = "SELECT $_fields FROM members_ WHERE $_filter $_range";

          return $this->lyrisSelect($sql, $filters, $labels);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Get a list of all lists a user has permission to access.  Let's keep this
   * as simple as possible so we can use it in autocomplete forms.
   */
  public function loadUserAllowedLists($filter = '', $limit = 10) {
    $lists = array();
    $mylists = array();
    $sql = '';

    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
          $lim = ((int) $limit > 0 ? "LIMIT " . (int) $limit : '');

          // Get the appropriate lists for the user's permission level.
          if ($this->isServerAdmin()) {
            $sql = "SELECT listid_, name_ FROM lists_";
          }
          elseif ($sites = $this->isSiteAdmin()) {
            $sql = "SELECT listid_, name_ FROM lists_ WHERE topic_ IN ('" . str_replace('|', "','", $sites) . "')";
            $mylists = $this->loadListsByAdmin(NULL, $filter, $limit);
          }
          else {
            $mylists = $this->loadListsByAdmin(NULL, $filter, $limit);
          }

          // Get the custom results from server and site admins.
          if ($sql) {
            $res = $this->lyrisSelect($sql, array(check_plain($filter)));
            foreach ($res as $list) {
              $lists[$list['LISTID_']] = $list['NAME_'];
            }
          }
          break;


        case '10':
        case '11':
        case '12':
          $lim = ((int) $limit > 0 ? "AND ROWNUM <= " . (int) $limit : '');

          if ($this->isServerAdmin()) {
            $sql = "SELECT LISTID_, NAME_ FROM lists_";
          }
          elseif ($sites = $this->isSiteAdmin()) {
            $sql = "SELECT LISTID_, NAME_ FROM lists_ WHERE TOPIC_ IN ('" . str_replace('|', "','", $sites) . "')";
            $mylists = $this->loadListsByAdmin(NULL, $filter, $limit);
          }
          else {
            $mylists = $this->loadListsByAdmin(NULL, $filter, $limit);
          }

          // Get the custom results from server and site admins.
          if ($sql) {
            $res = $this->lyrisSelect($sql, array(check_plain($filter)));
            foreach ($res as $list) {
              $lists[$list['LISTID_']] = $list['NAME_'];
            }
          }
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

      // Merge and sort the lists.
      $lists += $mylists;
      asort($lists);

      // Store the allowed lists for this user
      $this->allowed_lists = $lists;

      // Filter a result.
      $filtered = array();
      $count = 0;
      foreach ($lists as $ID => $list) {
        if (!$filter || stristr($list, $filter)) {
          $filtered[$ID] = $list;
          $count++;
        }

        if ($limit > 0 && $count >= $limit) {
          break;
        }
      }
    } catch (Exception $e) {$this->handleException($e);}

    return $filtered;
  }

  /**
   * Get the current user's Lyris permissions from the people_ table.
   */
  private function loadUserData() {
    $this->checkMode(__FUNCTION__);

    switch ($this->api_major_version) {
      case '1.5.2a':
        $sql = "SELECT * FROM people_ WHERE name_ = '%s'";
        $res = $this->lyrisSelect($sql, array($this->username));
        if (!empty($res)) {
          $this->user = $res[0];
        }
        break;

      case '10':
      case '11':
      case '12':
        $sql = "SELECT * FROM people_ WHERE NAME_ = '%s'";
        $res = $this->lyrisSelect($sql, array($this->username));
        if (!empty($res)) {
          $this->user = $res[0];
        }
        break;

      default:
        throw new LyrisMissingApiException($this, __FUNCTION__);
    }
  }

  /**
   * Send a Lyris Mailing.
   */
  public function sendMailing($mailing) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          return $this->call('SendMailing', NULL, $mailing->struct);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Send a Test Lyris Mailing.
   */
  public function sendMailingTest($mailing, $emails = array()) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          return $this->call('SendMailingDirect', $emails, NULL, $mailing->struct);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Update Lyris Content.
   */
  public function updateContent($lc) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          if ($this->call('UpdateContent', $lc->struct)) {
            $lc->changedFieldClear();
            return TRUE;
          }
          else {
            return FALSE;
          }
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Update a list.
   *
   * @param $list
   *   A list object containing a loaded struct.
   *
   * @return
   *   A boolean success flag.
   */
  public function updateList($list) {
    try {
      $this->checkMode(__FUNCTION__);
      if ($this->call('UpdateList', $list->struct)) {
        $list->changedFieldClear();
        return TRUE;
      }
      else {
        return FALSE;
      }
    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Update a member record.  This function requires a database record for the
   * member and will sync the changed fields.
   *
   * There is no comprehensive UpdateMember API function. The available options
   * are:
   *
   * - UpdateMemberDemographics
   * - UpdateMemberPassword
   * - UpdateMemberKind
   * - UpdateMemberEmail
   * - UpdateMemberStatus
   */
  public function updateMember($member) {
    try {
      $this->checkMode(__FUNCTION__);

      // Correlate the field being updated with the API function called to
      // perform the update.
      $api_map = array(
        'Demographics'   => 'UpdateMemberDemographics',
        'EmailAddress'   => 'UpdateMemberEmail',
        'MembershipKind' => 'UpdateMemberKind',
        'Password'       => 'UpdateMemberPassword',
        'MemberStatus'   => 'UpdateMemberStatus',
      );

      $old_values = unserialize($member->old_values);

      // Prepare the data for Lyris.
      $member->populateStruct();

      switch ($this->api_major_version) {
        case '1.5.2a':
        case '10':
        case '11':
        case '12':
          if (!$old_values) {
            $this->watchdog('Lyris Member entity @eid was marked as changed but had no fields listed as changed.', WATCHDOG_WARNING, array('@eid' => $member->eid));
            return TRUE;
          }

          // Get the SimpleMemberStruct to hand off to Lyris.
          $struct = $member->simpleMemberStruct();

          $struct_args = array();
          foreach ($struct as $field => $val) {
            $struct_args[] = "$field: $val";
          }
          $struct_arg = implode(", ", $struct_args);

          // Update each field specified in the old_values field.
          foreach ((array) $old_values as $field => $old_value) {
            $args = array(
              '@field'  => $field,
              '@newval' => $member->structVal($field),
              '@member' => $struct_arg,
            );

            switch ($field) {
              case 'Demographics':
              case 'EmailAddress':
              case 'MembershipKind':
              case 'Password':
              case 'MemberStatus':
                if ($this->call($api_map[$field], $struct, $member->structVal($field))) {
                  $member->changedFieldClear($field);
                  unset($old_values[$field]);
                }
                else {
                  $this->watchdog('Member update failed: Unable to change @field to \'@newval\' for member:<br />@member', WATCHDOG_ERROR, $args);
                }
                break;

              case 'DateUnsubscribed':
                if ($this->updateMemberField($field, format_date($member->localVal($field), 'custom', 'Y-m-d\ H:i:s', 'GMT'), $struct['MemberID'])) {
                  $member->changedFieldClear($field);
                  unset($old_values[$field]);
                }
                else {
                  $this->watchdog('Member update failed: Unable to change @field to \'@newval\' for member:<br />@member', WATCHDOG_ERROR, $args);
                }
                break;

              default:
                if ($this->updateMemberField($field, $member->structVal($field), $struct['MemberID'])) {
                  $member->changedFieldClear($field);
                  unset($old_values[$field]);
                }
                else {
                  $this->watchdog('Member update failed: Unable to change @field to \'@newval\' for member:<br />@member', WATCHDOG_ERROR, $args);
                }
                break;
            }
          }

          $member->old_values = serialize($old_values);

          return (count($old_values) > 0 ? FALSE : TRUE);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Update a member's name.
   */
  private function updateMemberField($field, $value, $MemberID) {
    try {
      $this->checkMode(__FUNCTION__);

      // Kick out if we don't have a valid MemberID to identify the record.
      if (!$MemberID) {
        $e = new LyrisGeneralException($this);
        $e->setArg('@field', $field);
        $e->setMessage('Unable to update field @field. MemberID not provided.');
        throw $e;
      }

      // Map struct field names to their equivalent DB field names.
      $db_map = array(
        'FullName' => 'FULLNAME_',
        'DateUnsubscribed' => 'DATEUNSUB_',
      );

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          return $this->lyrisUpdate('members_', array(array('Name' => $db_map[$field], 'Value' => $value)), "MEMBERID_ = $MemberID");
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Directly update a Lyris member's status.  Local member not required.
   */
  public function updateMemberStatus($sms, $status) {
    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '1.5.2a':
        case '10':
        case '11':
        case '12':
          return $this->call('UpdateMemberStatus', $sms, $status);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Check user list access.
   */
  public function userListAccess($ListName) {
    if (!$this->isConnected()) {
      return FALSE;
    }

    try {
      $this->checkMode(__FUNCTION__);

      switch ($this->api_major_version) {
        case '10':
        case '11':
        case '12':
          if (empty($this->allowed_lists)) {
            $this->loadUserAllowedLists('', 0);
          }

          return in_array($ListName, $this->allowed_lists);
          break;

        default:
          throw new LyrisMissingApiException($this, __FUNCTION__);
      }

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Select results are returned as an array where the first element lists the
   * columns and the remaining elements are the DB rows.  This function will
   * build a usable array of results with proper array keys matching the column
   * names.
   */
  private function processSelectResults($res, $key = NULL) {
    if (empty($res)) {
      return array();
    }

    $results = array();
    $cols = array_shift($res);

    // Lowercase all keys so we can be sure of an accurate comparison.
    $key = strtolower($key);
    foreach ($cols as &$col) {
      $col = strtolower($col);
    }

    $col_names = array_flip($cols);

    foreach ($res as $row) {
      foreach ($row as $_key => $value) {
        $_row[$cols[$_key]] = $value;
      }

      if ($key) {
        $results[$row[$col_names[$key]]] = $_row;
      }
      else {
        $results[] = $_row;
      }
    }

    return $results;
  }

  /**
   * Translate a struct name to a db field name.
   */
  private function structNameToDB($field) {
    // Handle special cases
    switch ($field) {
      case 'EmailAddress':  $field = 'emailaddr'; break;
      case 'ListName':      $field = 'list'; break;
      case 'MemberStatus':  $field = 'membertype'; break;
    }

    // Translate the syntax
    switch ($this->api_major_version) {
      case '10':
      case '11':
      case '12':
        return strtoupper($field) . "_";
        break;
    }
  }
}
