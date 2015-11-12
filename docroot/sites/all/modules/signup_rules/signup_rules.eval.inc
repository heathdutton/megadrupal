<?php

/**
 * @file
 * Rules evaluation for Signup Rules module.
 */

/**
 * Condition: Content signup total has value.
 */
function signup_rules_condition_signup_total($node, $operator, $input) {
  $query = db_select('signup_log');
  $query->condition('nid', $node->nid);
  $count = $query->countQuery()->execute()->fetchField();
  switch ($operator) {
    case 'greater':
      if ($count > $input) {
        return TRUE;
      }
      return FALSE;
    case 'equal':
      if ($count == $input) {
        return TRUE;
      }
      return FALSE;
    case 'less':
      if ($count < $input) {
        return TRUE;
      }
      return FALSE;
  }
}

/**
 * Condition: User is signed up for content.
 */
function signup_rules_condition_user_is_signed_up($node, $user) {
  $result = db_query("SELECT uid FROM {signup_log} WHERE uid = :uid AND nid = :nid", array(':uid' => $user->uid, ':nid' => $node->nid))->fetchObject();
  return !empty($result);
}

/**
 * Condition: Signups are open for content.
 */
function signup_rules_condition_signups_open($node) {
  return isset($node->signup_status) ? $node->signup_status == 1 : FALSE;
}

/**
 * Condition: Signup is marked attended.
 */
function signup_rules_condition_signup_attended($signup) {
  return isset($signup->attended) ? $signup->attended == 1 : FALSE;
}

/**
 * Condition: Signup is marked not attended.
 */
function signup_rules_condition_signup_not_attended($signup) {
  return isset($signup->attended) ? $signup->attended == 0 : FALSE;
}

/**
 * Action: Load signup data.
 */
function signup_rules_action_load_signup($node, $user) {
  $signup = db_query('SELECT sl.*, n.title, u.name, u.mail FROM {signup_log} sl INNER JOIN {node} n ON sl.nid = n.nid INNER JOIN {users} u ON sl.uid = u.uid WHERE sl.uid = :uid AND sl.nid = :nid', array(':uid' => $user->uid, ':nid' => $node->nid))->fetchObject();
  if (!empty($signup)) {
    return array('signup_loaded' => $signup);
  }
  return;
}

/**
 * Action: Sign up a user to content.
 */
function signup_rules_action_signup_user($user, $node, $force) {
  // Data must be populated in an array for signup's sign up function.
  $data['nid'] = $node->nid;
  $data['uid'] = $user->uid;
  if (variable_get('signup_node_default_state_'. $node->type, 'disabled') != 'disabled') {
    if (empty($force)) {
      // First, check if signups are enabled for the node.
      $query = db_select('signup');
      $query->fields(array('nid', 'status'));
      $query->condition('nid', $node->nid);
      $result = $query->execute()->fetchObject();
      // Check the node signup status before signing up the user.
      if ($result->status == 1) {
        signup_sign_up_user($data);
      }
    }
    else {
      // Sign up the user regardless of node signup status.
      signup_sign_up_user($data);
    }
  }
}

/**
 * Action: Cancel all signups for a node.
 */
function signup_rules_action_cancel_node_signups($node, $settings) {
  $signups = signup_get_signups($node->nid);
  foreach ($signups as $signup) {
    signup_cancel_signup($signup, FALSE);
  }
  return;
}

/**
 * Action: Cancel all signups for a user.
 */
function signup_rules_action_cancel_user_signups($user) {
  $signups = signup_rules_get_user_signups($user->uid);
  foreach ($signups as $signup) {
    signup_cancel_signup($signup, FALSE);
  }
}

/**
 * Action: Close signups for a node.
 *
 * The conditional checks to see if signups are even enabled for
 * the node type. If they aren't then there is no attempt to close
 * them as they should be closed by Signup already.
 */
function signup_rules_action_close_signups($node) {
  if (variable_get('signup_node_default_state_'. $node->type, 'disabled') != 'disabled') {
    signup_close_signup($node->nid);
  }
}

/**
 * Action: Open signups for a node.
 *
 * The conditional checks to ensure that signups are enabled for
 * the node before programatically opening signups.
 */
function signup_rules_action_open_signups($node) {
  if (variable_get('signup_node_default_state_'. $node->type, 'disabled') != 'disabled') {
    signup_open_signup($node->nid);
  }
}

/**
 * Action: Send mail to all users signed up to content.
 */
function signup_rules_action_send_mail($node, $limit, $subject, $message, $from = NULL, $settings, RulesState $state, RulesPlugin $element) {
  $signups = array();

  // Build the proper query for selecting limited signups.
  if ($limit == 'attended') {
    $result = db_query('SELECT u.uid, u.name, u.mail, u.language, s_l.* FROM {signup_log} s_l INNER JOIN {users} u ON u.uid = s_l.uid WHERE s_l.nid = :nid AND attended = 1', array(':nid' => $node->nid));
  }
  elseif ($limit == 'not attended') {
    $result = db_query('SELECT u.uid, u.name, u.mail, u.language, s_l.* FROM {signup_log} s_l INNER JOIN {users} u ON u.uid = s_l.uid WHERE s_l.nid = :nid AND attended = 0', array(':nid' => $node->nid));
  }

  // If there is no limit then we leave the empty array to pass to Signup.
  if ($limit != 'all') {
    foreach ($result as $signup) {
      $signups[] = $signup;
    }
  }

  // The 'from' address will default to the site-wide address if none is given.
  $from = !empty($from) ? str_replace(array('\r', '\n'), '', $from) : variable_get('site_mail', ini_get('sendmail_from'));
  $subject = str_replace(array("\r", "\n"), '', $subject);

  // If limit was set to all then an empty array will be passed in $signups
  // and signup_set_broadcaster() will populate the array with all signups.
  module_load_include('inc', 'signup', 'includes/broadcast');
  signup_send_broadcast($node->nid, $subject, $message, $from, FALSE, $signups);
  watchdog('signup', 'Sent email to %limit users signed up to %node', array('%limit' => $limit, '%node' => $node->title));
}

/**
 * Action: Prevent display of signup form.
 */
function signup_rules_action_suppress($suppress) {
  $suppress->value = TRUE;
}
