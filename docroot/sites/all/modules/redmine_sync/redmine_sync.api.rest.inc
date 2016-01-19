<?php


/**
 * redmine_sync_init_rest().
 */
function redmine_sync_init_rest() {
  $err_code = null;
  $auth_mode = variable_get('redmine_sync_rest_api_auth_mode', REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY);
  // Admin key.
  if ($auth_mode == REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY) {
    if (variable_get('redmine_sync_rest_api_src_host') &&
        variable_get('redmine_sync_rest_api_src_port') &&
        variable_get('redmine_sync_rest_api_admin_key')) {
      $users = redmine_sync_rest_get_users("limit=1");
      $err_code = count($users) ? REDMINE_SYNC_REST_API_INIT_OK :
                                  REDMINE_SYNC_REST_API_INIT_INVALID_PARAMETERS_OR_SERVER_NOT_AVAILABLE;
    } else {
      $err_code = REDMINE_SYNC_REST_API_INIT_EMPTY_PARAMETERS;
    }
  }
  // User key.
  if ($auth_mode == REDMINE_SYNC_REST_API_AUTH_MODE_USER_KEYS) {
    if (redmine_sync_rest_get_user_key($GLOBALS['user']->uid) == '') {
      $err_code = REDMINE_SYNC_REST_API_INIT_NO_USER_KEY;
    } else {
      if (variable_get('redmine_sync_rest_api_src_host') &&
          variable_get('redmine_sync_rest_api_src_port')) {
        $usr_info = redmine_sync_rest_get_my_info();
        $err_code = isset($usr_info['api_key']) ? REDMINE_SYNC_REST_API_INIT_OK :
                                                  REDMINE_SYNC_REST_API_INIT_INVALID_PARAMETERS_OR_SERVER_NOT_AVAILABLE;
      } else {
        $err_code = REDMINE_SYNC_REST_API_INIT_EMPTY_PARAMETERS;
      }
    }
  }
  return $err_code;
}


/**
 * redmine_sync_rest_get_user_key().
 */
function redmine_sync_rest_get_user_key($uid) {
  $user = user_load($uid);
  if (isset($user->rdm_access_key[LANGUAGE_NONE][0]['value']) && $user->rdm_access_key[LANGUAGE_NONE][0]['value']) {
    return $user->rdm_access_key[LANGUAGE_NONE][0]['value'];
  } else {
    return '';
  }
}


/**
 * redmine_sync_rest_request().
 * redmine_sync_rest_get_user().
 * redmine_sync_rest_get_users().
 * redmine_sync_rest_get_user_by_mail().
 * redmine_sync_rest_get_my_info().
 * redmine_sync_rest_get_user_groups().
 * redmine_sync_rest_get_user_roles().
 * redmine_sync_rest_get_user_role().
 * redmine_sync_rest_get_projects().
 * redmine_sync_rest_get_project().
 * redmine_sync_rest_get_project_memberships().
 * redmine_sync_rest_get_project_versions().
 * redmine_sync_rest_get_project_wiki().
 * redmine_sync_rest_get_time_entry().
 * redmine_sync_rest_get_time_entries().
 * redmine_sync_rest_get_time_entry_activities().
 * redmine_sync_rest_get_issue().
 * redmine_sync_rest_get_issues().
 * redmine_sync_rest_get_issue_categories().
 * redmine_sync_rest_get_issue_statuses().
 * redmine_sync_rest_get_issue_relations().
 * redmine_sync_rest_get_issue_priorities().
 * redmine_sync_rest_get_trackers().
 * redmine_sync_rest_get_queries().
 */
function redmine_sync_rest_request($service, $method, $post_params = null, $rdm_login = null, $conn_params = null) {
  $host      = isset($conn_params['host'])      ? $conn_params['host']      : variable_get('redmine_sync_rest_api_src_host');
  $port      = isset($conn_params['port'])      ? $conn_params['port']      : variable_get('redmine_sync_rest_api_src_port');
  $auth_mode = isset($conn_params['auth_mode']) ? $conn_params['auth_mode'] : variable_get('redmine_sync_rest_api_auth_mode', REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY);
  $admin_key = isset($conn_params['admin_key']) ? $conn_params['admin_key'] : variable_get('redmine_sync_rest_api_admin_key');
  $user_key  = isset($conn_params['user_key'])  ? $conn_params['user_key']  : redmine_sync_rest_get_user_key($GLOBALS['user']->uid);
  if ($host && (($auth_mode == REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY && $admin_key) ||
                 $auth_mode == REDMINE_SYNC_REST_API_AUTH_MODE_USER_KEYS && $user_key)) {
    // Prepare url.
    $query = strrchr($service, '?');
    $url_pattern = "http://[host][port]/[service].xml[query]";
    $url = str_replace('[host]',    $host, $url_pattern);
    $url = str_replace('[port]',    $port ? ':'.$port : '', $url);
    $url = str_replace('[service]', strlen($query) ? substr($service, 0, -strlen($query)) : $service, $url);
    $url = str_replace('[query]',   $query, $url);

    // Request.
    $headers = _redmine_sync_prepare_headers('xml');

    if ($post_params) {
      $request_data = _redmine_sync_rest_prepare_xml($post_params);
    }

    switch ($auth_mode) {
      case REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY :
        $headers['X-Redmine-API-Key'] = $admin_key;
        if ($rdm_login) {
          $headers['X-Redmine-Switch-User'] = $rdm_login;
        }
        break;
      case REDMINE_SYNC_REST_API_AUTH_MODE_USER_KEYS :
        $headers['X-Redmine-API-Key'] = $user_key;
        break;
    }

    $response = drupal_http_request($url, array(
      'method'  => $method,
      'headers' => $headers,
      'data'    => $post_params ? $request_data : null,
    ));

    // Prepare data.
    if (isset($response->data) && trim($response->data)) {
      $response->data_prepared = simplexml_load_string($response->data);
      $response->total_count   = (int)$response->data_prepared->attributes()->total_count;
      $response->offset        = (int)$response->data_prepared->attributes()->offset;
      $response->limit         = (int)$response->data_prepared->attributes()->limit;
    }
    return $response;
  }
}


function _redmine_sync_prepare_headers($type = 'xml') {
  // @todo: Implement JSON headers.
  $headers = array('Content-Type' => 'application/xml');
  return $headers;
}


function _redmine_sync_rest_prepare_xml($data) {
  return format_xml_elements($data);
}


function _redmine_sync_rest_prepare_json($data) {
  // Stub for JSON formatted API calls.
}


function redmine_sync_rest_get_user($user_id, $query = null, $rdm_login = null) {
  $response = redmine_sync_rest_request($query ? "users/$user_id?$query" : "users/$user_id", 'GET', null, $rdm_login);
  if ($response->code == 200) {
    $user_info = (array)$response->data_prepared;
    // Prepare roles by projects.
    $roles_by_projects = array();
    if (isset($response->data_prepared->memberships)) {
      foreach ($response->data_prepared->memberships->membership as $c_membership) {
        $c_project_id = (int)$c_membership->project->attributes()->id;
        if ($c_project_id) {
          foreach ($c_membership->roles->role as $c_role) {
            $c_role_id = (int)$c_role->attributes()->id;
            if ($c_role_id) {
              $roles_by_projects[$c_project_id][]= $c_role_id;
            }
          }
        }
      }
    }
    $user_info['roles_by_projects'] = $roles_by_projects;
    // Return value.
    return $user_info;
  }
}


function redmine_sync_rest_get_users($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "users?limit=100&offset=$offset&$query" : "users?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->user) && $offset < $limit) {
      foreach ($response->data_prepared->user as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity;  break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->login ? (string)$c_entity->login : 'id:'.(int)$c_entity->id; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_user_by_mail($mail, $query = null, $rdm_login = null) {
  $rdm_users = redmine_sync_rest_get_users('name='.$mail.($query ? '&'.$query: ''), REDMINE_SYNC_RET_VAL_TYPE_DEFAULT, $rdm_login);
  foreach ($rdm_users as $c_info) {
    if ($c_info['mail'] == $mail) {
      return $c_info;
    }
  }
}


function redmine_sync_rest_get_my_info($rdm_login = null) {
  $response = redmine_sync_rest_request("users/current", 'GET', null, $rdm_login);
  if ($response->code == 200) {
    return (array)$response->data_prepared;
  }
}


function redmine_sync_rest_get_user_groups($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "groups?limit=100&offset=$offset&$query" : "groups?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->group) && $offset < $limit) {
      foreach ($response->data_prepared->group as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_user_roles($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "roles?limit=100&offset=$offset&$query" : "roles?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->role) && $offset < $limit) {
      foreach ($response->data_prepared->role as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_user_role($role_id, $query = null, $rdm_login = null) {
  $response = redmine_sync_rest_request($query ? "roles/$role_id?$query" : "roles/$role_id", 'GET', null, $rdm_login);
  if ($response->code == 200) {
    return (array)$response->data_prepared;
  }
}


function redmine_sync_rest_get_projects($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "projects?limit=100&offset=$offset&$query" : "projects?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->project) && $offset < $limit) {
      foreach ($response->data_prepared->project as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_project($project_id_or_name, $query = null, $rdm_login = null) {
  $response = redmine_sync_rest_request($query ? "projects/$project_id_or_name?$query" : "projects/$project_id_or_name", 'GET', null, $rdm_login);
  if ($response->code == 200) {
    return (array)$response->data_prepared;
  }
}


function redmine_sync_rest_get_project_memberships($project_id_or_name, $query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "projects/$project_id_or_name/memberships?limit=100&offset=$offset&$query" : "projects/$project_id_or_name/memberships?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->membership) && $offset < $limit) {
      foreach ($response->data_prepared->membership as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (int)$c_entity->id; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_project_versions($project_id_or_name, $query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "projects/$project_id_or_name/versions?limit=100&offset=$offset&$query" : "projects/$project_id_or_name/versions?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->version) && $offset < $limit) {
      foreach ($response->data_prepared->version as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_project_wiki($project_id_or_name, $query = null, $rdm_login = null) {
  $response = redmine_sync_rest_request($query ? "projects/$project_id_or_name/wiki?$query" : "projects/$project_id_or_name/wiki", 'GET', null, $rdm_login);
  if ($response->code == 200) {
    return (array)$response->data_prepared;
  }
}


function redmine_sync_rest_get_time_entry($time_entry_id, $query = null, $rdm_login = null) {
  $response = redmine_sync_rest_request($query ? "time_entries/$time_entry_id?$query" : "time_entries/$time_entry_id", 'GET', null, $rdm_login);
  if ($response->code == 200) {
    return (array)$response->data_prepared;
  }
}


function redmine_sync_rest_get_time_entries($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "time_entries?limit=100&offset=$offset&$query" : "time_entries?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->time_entry) && $offset < $limit) {
      foreach ($response->data_prepared->time_entry as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (int)$c_entity->id; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_time_entry_activities($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "enumerations/time_entry_activities?limit=100&offset=$offset&$query" : "enumerations/time_entry_activities?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->time_entry_activity) && $offset < $limit) {
      foreach ($response->data_prepared->time_entry_activity as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_issue($issue_id, $query = null, $rdm_login = null) {
  $response = redmine_sync_rest_request($query ? "issues/$issue_id?$query" : "issues/$issue_id", 'GET', null, $rdm_login);
  if ($response->code == 200) {
    return (array)$response->data_prepared;
  }
}


function redmine_sync_rest_get_issues($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  $data_groups = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "issues?limit=100&offset=$offset&$query" : "issues?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->issue) && $offset < $limit) {
      foreach ($response->data_prepared->issue as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT             : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST         : $data[(int)$c_entity->id] = t('Issue #!num', array('!num' => (int)$c_entity->id)).': '.(string)$c_entity->subject; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST_GROUPED : $data[(string)$c_entity->status->attributes()->name][(int)$c_entity->id] = t('Issue #!num', array('!num' => (int)$c_entity->id)).': '.(string)$c_entity->subject; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_issue_categories($project_id_or_name, $query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "projects/$project_id_or_name/issue_categories?limit=100&offset=$offset&$query" : "projects/$project_id_or_name/issue_categories?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->issue_category) && $offset < $limit) {
      foreach ($response->data_prepared->issue_category as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_issue_statuses($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "issue_statuses?limit=100&offset=$offset&$query" : "issue_statuses?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->issue_status) && $offset < $limit) {
      foreach ($response->data_prepared->issue_status as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_issue_relations($issue_id, $query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "issues/$issue_id/relations?limit=100&offset=$offset&$query" : "issues/$issue_id/relations?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->relation) && $offset < $limit) {
      foreach ($response->data_prepared->relation as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (int)$c_entity->id; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_issue_priorities($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "enumerations/issue_priorities?limit=100&offset=$offset&$query" : "enumerations/issue_priorities?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->issue_priority) && $offset < $limit) {
      foreach ($response->data_prepared->issue_priority as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_trackers($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "trackers?limit=100&offset=$offset&$query" : "trackers?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->tracker) && $offset < $limit) {
      foreach ($response->data_prepared->tracker as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}


function redmine_sync_rest_get_queries($query = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request($query ? "queries?limit=100&offset=$offset&$query" : "queries?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 200 && count($response->data_prepared->query) && $offset < $limit) {
      foreach ($response->data_prepared->query as $c_entity) {
        switch ($ret_val_type) {
          case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity; break;
          case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
        }
      }
    } else {
      break;
    }
  }
  return $data;
}

/**
 * Get info about Redmine custom fields 
 * @param  string $customized_type Redmine entity name to filter by, like issue, project, time_entry, etc.
 * If provided, function returns only custom fields which are attached to this entity in Redmine.
 */
function redmine_sync_rest_get_custom_fields($customized_type = null, $ret_val_type = REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $rdm_login = null, $limit = 10000) {
  $data = array();
  while (1) {
    $offset = isset($offset) ? $offset + 100 : 0;
    $response = redmine_sync_rest_request("custom_fields?limit=100&offset=$offset", 'GET', null, $rdm_login);
    if ($response->code == 403) {
      return null;
    } else if ($response->code == 200 && count($response->data_prepared->custom_field) && $offset < $limit) {
      foreach ($response->data_prepared->custom_field as $c_entity) {
        if ($customized_type == null || $customized_type == $c_entity->customized_type) {      
          switch ($ret_val_type) {
            case REDMINE_SYNC_RET_VAL_TYPE_DEFAULT     : $data[(int)$c_entity->id] = (array)$c_entity;  break;
            case REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST : $data[(int)$c_entity->id] = (string)$c_entity->name; break;
          }
        }
      }
    } else {
      break;
    }
  }
  return $data;
}
