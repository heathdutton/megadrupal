<?php
/**
 * Branch 7.x-6.x introduced huge API changes.
 * This file contains wrapper functions for backward compatibility to
 * methods of the 7.x-5.x branch.
 */


/**
 * Searches JIRA issues filtered by the given JQL string.
 *
 * Usage example returning issues not resolved/closed from PROJECTNAME:
 *
 * $foundissues = jira_rest_searchissue("project=PROJECTNAME&status!=RESOLVED&status!=CLOSED");
 *
 * @param string $jql_string
 *   the JQLString (filtering the searched issues)
 * @param array $options
 *   overwrites default values of specified options (see official JIRA REST API documentation for further information):
 *     maxResults int maximum number of issues returned
 *     startAt int index of the first issue to return (0-based)
 *     fields string comma-separated list of fields that should be returned
 *     expand string comma-separated list of parameters to extend
 *     validateQuery boolean whether to validate the JQL query
 * @see jira_rest_default_curl_options
 *
 * @return object|array
 *   the issues found, can be accessed via object->issues if used with 7.x-6.x compatibility
 *   or by accessing the returned array if used with 7.x-5.x compatibility
 *   some examples for getting information from a single issue:
 *   $issuekey = $issue->key;
 *   $parentkey = $issue->fields->parent->key;
 *   $customfielddata = $issue->fields->customfield_10404;
 *
 * @throws JiraRestException
 *
 * @deprecated
 */
function jira_rest_searchissue() {

  // compatibility to deprecated 7.x-6.x function
  // jira_rest_searchissue($jql_string, $options = array())
  if (func_num_args() >= 1 && (func_num_args() < 2 || is_array(func_get_arg(1)))) {
    $jql_string = func_get_arg(0);
    $options = array();

    if (func_num_args() > 1) {
      $options = func_get_arg(1);
    }

    $options['allowMultipleCalls'] = FALSE; // this option was never allowed for this method

    $issues = jira_rest_issue_search($jql_string, $options);
    $issues_object = new stdClass;
    $issues_object->issues = $issues;

    return $issues_object;
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_searchissue($username, $password, $jqlstring, $maxresults = '999')
  elseif (func_num_args() >= 3) {
    $username = func_get_arg(0);
    $password = func_get_arg(1);
    $jql_string = func_get_arg(2);
    $max_results = 999;

    if (func_num_args() > 3) {
      $max_results = func_get_arg(3);
    }

    $options = array(
      'username' => $username,
      'password' => $password,
      'allowMultipleCalls' => FALSE,
      'maxResults' => $max_results,
    );

    return jira_rest_issue_search($jql_string, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}


/**
 * @see jira_rest_issue_search
 * @deprecated
 */
function jira_rest_searchissue_all_issues($jql_string, $options = array()) {
  return jira_rest_issue_search($jql_string, $options);
}


/**
 * @see jira_rest_issue_get
 * @deprecated
 */
function jira_rest_getfullissue() {
  // compatibility to deprecated 7.x-6.x function
  // jira_rest_getfullissue($issue_key, $options = array())
  if (func_num_args() >= 1 && (func_num_args() < 2 || is_array(func_get_arg(1)))) {
    $issue_key = func_get_arg(0);
    $options = array();

    if (func_num_args() > 1) {
      $options = func_get_arg(1);
    }

    return jira_rest_issue_get($issue_key, $options);
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_getfullissue($username, $password, $issueid_or_key)
  elseif (func_num_args() == 3) {
    $username = func_get_arg(0);
    $password = func_get_arg(1);
    $issue_key = func_get_arg(2);

    $options = array(
      'username' => $username,
      'password' => $password,
    );

    return jira_rest_issue_get($issue_key, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}

/**
 * @see jira_rest_issue_create
 * @deprecated
 */
function jira_rest_createissue() {
  // compatibility to deprecated 7.x-6.x function
  // jira_jira_rest_createissue($issue_data, $options = array())
  if (func_num_args() >= 1 && (func_num_args() < 2 || is_array(func_get_arg(1)))) {
    $issue_data = func_get_arg(0);
    $options = array();

    if (func_num_args() > 1) {
      $options = func_get_arg(1);
    }

    return jira_rest_issue_create($issue_data, $options);
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_createissue($username, $password, $issuedata)
  elseif (func_num_args() == 3) {
    $username = func_get_arg(0);
    $password = func_get_arg(1);
    $issue_data = func_get_arg(2);

    $options = array(
      'username' => $username,
      'password' => $password,
    );

    return jira_rest_issue_create($issue_data, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}

/**
 * @see jira_rest_issue_update
 * @deprecated
 */
function jira_rest_updateissue($issue_data, $issue_key, $options = array()) {
  return jira_rest_issue_update($issue_key, $issue_data, $options);
}

/**
 * @see jira_rest_issue_attach_files_absolutepath
 * @deprecated
 */
function jira_rest_attachmultiplefilestoissue_absolutepath() {
  // compatibility to deprecated 7.x-6.x function
  // jira_rest_attachmultiplefilestoissue_absolutepath($files, $issue_key, $options = array())
  if (func_num_args() >= 2 && (func_num_args() < 3 || is_array(func_get_arg(2)))) {
    $files = func_get_arg(0);
    $issue_key = func_get_arg(1);
    $options = array();

    if (func_num_args() > 2) {
      $options = func_get_arg(2);
    }

    return jira_rest_issue_attach_files_absolutepath($issue_key, $files, $options);
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_attachmultiplefilestoissue_absolutepath($mixed, $username, $password, $issuekey_or_id, $delete_files = FALSE, $force_delete = FALSE)
  elseif (func_num_args() >= 4) {
    $files = func_get_arg(0);
    $username = func_get_arg(1);
    $password = func_get_arg(2);
    $issue_key = func_get_arg(3);
    $delete_files = FALSE;
    $force_delete = FALSE;

    if (func_num_args() > 4) {
      $delete_files = func_get_arg(4);
    }
    if (func_num_args() > 5) {
      $force_delete = func_get_arg(5);
    }

    // branch 7.x-5.x supported also strings for this parameter
    if (gettype($files) == 'string') {
      $files = array($files);
    }
    elseif (gettype($files) != 'array') {
      throw new JiraRestException('Not supported parameter type: ' . gettype($files));
    }

    $options = array(
      'username' => $username,
      'password' => $password,
      'delete_files' => $delete_files,
      'force_delete' => $force_delete,
    );

    return jira_rest_issue_attach_files_absolutepath($issue_key, $files, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}

/**
 * @see jira_rest_issue_attach_file_absolutepath
 * @deprecated
 */
function jira_rest_attachfiletoissue_absolutepath() {
  // compatibility to deprecated 7.x-6.x function
  // jira_rest_attachfiletoissue_absolutepath($filepath, $issue_key, $options = array())
  if (func_num_args() >= 2 && (func_num_args() < 3 || is_array(func_get_arg(2)))) {
    $filepath = func_get_arg(0);
    $issue_key = func_get_arg(1);
    $options = array();

    if (func_num_args() > 2) {
      $options = func_get_arg(2);
    }

    return jira_rest_issue_attach_file_absolutepath($issue_key, $filepath, $options);
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_attachfiletoissue_absolutepath($filepath, $username, $password, $issuekey_or_id, $delete_files = FALSE, $force_delete = FALSE)
  elseif (func_num_args() >= 4) {
    $filepath = func_get_arg(0);
    $username = func_get_arg(1);
    $password = func_get_arg(2);
    $issue_key = func_get_arg(3);
    $delete_files = FALSE;
    $force_delete = FALSE;

    if (func_num_args() > 4) {
      $delete_files = func_get_arg(4);
    }
    if (func_num_args() > 5) {
      $force_delete = func_get_arg(5);
    }

    $options = array(
      'username' => $username,
      'password' => $password,
      'delete_files' => $delete_files,
      'force_delete' => $force_delete,
    );

    return jira_rest_issue_attach_file_absolutepath($issue_key, $filepath, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}

/**
 * @see jira_rest_issue_attach_file
 * @deprecated
 */
function jira_rest_attachfiletoissue() {
  // compatibility to deprecated 7.x-6.x function
  // jira_rest_attachfiletoissue($filename, $issue_key, $options = array())
  if (func_num_args() >= 2 && (func_num_args() < 3 || is_array(func_get_arg(2)))) {
    $filename = func_get_arg(0);
    $issue_key = func_get_arg(1);
    $options = array();

    if (func_num_args() > 2) {
      $options = func_get_arg(2);
    }

    return jira_rest_issue_attach_file($issue_key, $filename, $options);
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_attachfiletoissue($username, $password, $filename, $issuekey)
  elseif (func_num_args() == 4) {
    $username = func_get_arg(0);
    $password = func_get_arg(1);
    $filename = func_get_arg(2);
    $issue_key = func_get_arg(3);

    $options = array(
      'username' => $username,
      'password' => $password,
    );

    return jira_rest_issue_attach_file($issue_key, $filename, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}

/**
 * @see jira_rest_issue_close_fixed
 * @deprecated
 */
function jira_rest_closeissuefixed() {
  // compatibility to deprecated 7.x-6.x function
  // jira_rest_closeissuefixed($issue_key, $transition_data = array(), $options = array())
  if (func_num_args() >= 1 && (func_num_args() < 3 || is_array(func_get_arg(2)))) {
    $issue_key = func_get_arg(0);
    $transition_data = array();
    $options = array();

    if (func_num_args() > 1) {
      $transition_data = func_get_arg(1);
    }
    if (func_num_args() > 2) {
      $options = func_get_arg(2);
    }

    return jira_rest_issue_close_fixed($issue_key, $transition_data, $options);
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_closeissuefixed($username, $password, $issuekey, $comment = 'closed by drupal service')
  elseif (func_num_args() >= 3) {
    $username = func_get_arg(0);
    $password = func_get_arg(1);
    $issue_key = func_get_arg(2);
    $comment = 'closed by drupal service';

    if (func_num_args() > 3) {
      $comment = func_get_arg(3);
    }

    $transition_data = array(
      "update" => array('comment' => array(array('add' => array('body' => $comment)))),
    );

    $options = array(
      'username' => $username,
      'password' => $password,
    );

    return jira_rest_issue_close_fixed($issue_key, $transition_data, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}

/**
 * @see jira_rest_issue_resolve_fixed
 * @deprecated
 */
function jira_rest_resolveissuefixed() {
  // compatibility to deprecated 7.x-6.x function
  // jira_rest_resolveissuefixed($issue_key, $transition_data = array(), $options = array())
  if (func_num_args() >= 1 && (func_num_args() < 3 || is_array(func_get_arg(2)))) {
    $issue_key = func_get_arg(0);
    $transition_data = array();
    $options = array();

    if (func_num_args() > 1) {
      $transition_data = func_get_arg(1);
    }
    if (func_num_args() > 2) {
      $options = func_get_arg(2);
    }

    return jira_rest_issue_resolve_fixed($issue_key, $transition_data, $options);
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_resolveissuefixed($username, $password, $issuekey, $comment = 'resolved by drupal service')
  elseif (func_num_args() >= 3) {
    $username = func_get_arg(0);
    $password = func_get_arg(1);
    $issue_key = func_get_arg(2);
    $comment = 'resolved by drupal service';

    if (func_num_args() > 3) {
      $comment = func_get_arg(3);
    }

    $transition_data = array(
      "update" => array('comment' => array(array('add' => array('body' => $comment)))),
    );

    $options = array(
      'username' => $username,
      'password' => $password,
    );

    return jira_rest_issue_resolve_fixed($issue_key, $transition_data, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}

/**
 * @see jira_rest_issue_worklog
 * @deprecated
 */
function jira_rest_get_worklog($issue_id, $options = array()) {
  return jira_rest_issue_worklog($issue_id, $options);
}

/**
 * @see jira_rest_project_get
 * @deprecated
 */
function jira_rest_get_project($options = array()) {
  return jira_rest_project_get($options);
}

/**
 * @see jira_rest_status_get
 * @deprecated
 */
function jira_rest_get_status($options = array()) {
  return jira_rest_status_get($options);
}

/**
 * @see jira_rest_create_comment
 * @deprecated
 */
function jira_rest_create_comment($comment_data, $issue_key, $options = array()) {
  return jira_rest_issue_create_comment($issue_key, $comment_data, $options);
}

/**
 * @see jira_rest_custom_field_option_get
 * @deprecated
 */
function jira_rest_get_customFieldOption($id, $options = array()) {
  return jira_rest_custom_field_option_get($id, $options);
}

/**
 * @see jira_rest_issue_update
 * @deprecated
 */
function jira_rest_updateticket() {
  // compatibility to deprecated 7.x-6.x function
  // jira_rest_updateticket($issue_key, $issue_data, $options = array())
  if (func_num_args() >= 2 && (func_num_args() < 3 || is_array(func_get_arg(2)))) {
    $issue_data = func_get_arg(0);
    $issue_key = func_get_arg(1);
    $options = array();

    if (func_num_args() > 2) {
      $options = func_get_arg(2);
    }

    return jira_rest_issue_update($issue_key, $issue_data, $options);
  }
  // compatibility to deprecated 7.x-5.x function
  // jira_rest_updateticket($username, $password, $issuedata, $issuekey)
  elseif (func_num_args() == 4) {
    $username = func_get_arg(0);
    $password = func_get_arg(1);
    $issue_data = func_get_arg(2);
    $issue_key = func_get_arg(3);

    $options = array(
      'username' => $username,
      'password' => $password,
    );

    return jira_rest_issue_update($issue_key, $issue_data, $options);
  }
  // else fail
  else {
    throw new JiraRestException(t('Jira REST: Incompatible usage of a deprecated function!'));
  }
}
