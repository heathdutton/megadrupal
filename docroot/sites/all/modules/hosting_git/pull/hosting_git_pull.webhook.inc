<?php

/**
 * @file
 * Git webhook code
 */

/**
 * URL callback that is invoked by git to create a code pull task.
 */
function hosting_git_pull_callback($name, $hash) {

  // Load the node & list of allowed IPs.
  $node = hosting_context_load($name);
  $name_clean = check_plain($name);
  $allowed_ips = explode("\n", trim(variable_get('hosting_git_pull_webhook_ip_acl', HOSTING_GIT_WEBHOOK_DEFAULT_ALLOWED_IPS)));
  array_filter(array_map('trim', $allowed_ips));

  // Make sure we got the node.
  if (!$node) {
    $message = "Project $name_clean not found.";
  }
  // Make sure the security code is valid.
  elseif (_hosting_git_pull_webhook_hash_create($node) != $hash) {
    $message = "Security code $hash is not valid!";
    $status = HOSTING_GIT_WEBHOOK_STATUS_INVALID_CODE;
  }
  // Make sure the project has pull callback enabled.
  elseif ($node->git['pull_method'] != HOSTING_GIT_PULL_CALLBACK) {
    $message = "Project $name_clean is NOT configured to use Pull Code URL callback!";
  }
  // Make sure the client's IP address is on the list.
  elseif (!hosting_git_pull_ip_match(ip_address(), $allowed_ips)) {
    $message = ip_address() . " is not authorized to invoke a Pull Code request.";
    $status = HOSTING_GIT_WEBHOOK_STATUS_ACCESS_DENIED;
  }
  // All checks pass! Server is allowed to trigger tasks!
  else {
    $message = "Commit Received! Invoked by " . ip_address();
    $status = HOSTING_GIT_WEBHOOK_STATUS_OK;

    // @TODO: Check Payload Here... If we see a GitHub payload with branches,
    // filter out the $environments_to_pull based on those branches.

    $args = array('reset_hard' => TRUE);
    hosting_add_task($node->nid, 'git-pull', $args);
  }

  // Log It, only if there is a status.
  if (isset($status)) {
    db_update('hosting_git_pull')
      ->fields(array(
        'last_pull' => REQUEST_TIME,
        'last_pull_status' => $status,
        'last_pull_ip' => ip_address(),
      ))
      ->condition('nid', $node->nid)
      ->execute();
  }

  // Output a message, no matter what.
  watchdog('hosting_git_pull', $message, array(), WATCHDOG_INFO);
  print $message;

  // Save a variable to help when using the settings page.
  variable_set('hosting_git_pull_webhook_last_ip', ip_address());
}

/**
 * Check whether a given ip address matches a list of allowed ip addresses.
 *
 * A CIDR is allowed.
 *
 * @param string $ip
 *   The ip addy to test.
 * @param array $list
 *   The list to test against.
 */
function hosting_git_pull_ip_match($ip, $list) {
  foreach ($list as $cidr) {
    if (trim($ip) === trim($cidr)) {
      return TRUE;
    }
    @list($net, $mask) = explode('/', trim($cidr));
    if (isset($mask)) {
      if ($mask > 32) {
        // IPv6 is not supported... yet.
        return FALSE;
      }
      $bitmask = ~((1 << (32 - $mask)) - 1);
      if ((ip2long($net) & $bitmask) == (ip2long($ip) & $bitmask)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Create the full URL that is displayed in the node view.
 *
 * This URL can be given to the GitHub WebHook to invoke a pull after a commit.
 */
function _hosting_git_pull_callback_url($node) {
  // TODO The second parameter to this function call should be an array.
  return url(HOSTING_GIT_WEBHOOK_CALLBACK_URL
    . '/' . $node->hosting_name
    . '/' . _hosting_git_pull_webhook_hash_create($node),
    array('absolute' => TRUE));
}

/**
 * Create a security hash code based on the node.
 */
function _hosting_git_pull_webhook_hash_create($node) {
  return md5($node->hosting_name . $node->nid);
}
