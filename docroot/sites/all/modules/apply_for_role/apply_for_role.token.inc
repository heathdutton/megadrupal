<?php

/**
 * @file
 * Token callbacks for the token module.
 */

/**
 * Implements hook_token_info().
 */
function apply_for_role_token_info() {
  $types['apply-for-role'] = array(
    'name' => t("Apply for role"),
    'description' => t("Tokens for role applications."),
  );

  // Role application tokens.
  $apply['uid'] = array(
    'name' => t("User ID"),
    'description' => t("The applicant's user ID."),
  );
  $apply['name'] = array(
    'name' => t("Name"),
    'description' => t("The applicant's username."),
  );
  $apply['mail'] = array(
    'name' => t("E-mail"),
    'description' => t("The applicant's e-mail address."),
  );
  $apply['rid'] = array(
    'name' => t("Role ID"),
    'description' => t("The ID of the role being applied for."),
  );
  $apply['role'] = array(
    'name' => t("Role"),
    'description' => t("The name of the role being applied for."),
  );
  $apply['status'] = array(
    'name' => t("Status"),
    'description' => t("The status of the role being applied for."),
  );

  $apply['message'] = array(
    'name' => t("Message"),
    'description' => t("The message written by the role applicant."),
  );

  // Chained tokens for nodes.
  $apply['applied'] = array(
    'name' => t("Date applied"),
    'description' => t("The date the application was posted."),
    'type' => 'date',
  );
  $apply['processed'] = array(
    'name' => t("Date processed"),
    'description' => t("The date the application was processed."),
    'type' => 'date',
  );

  return array(
    'types' => $types,
    'tokens' => array(
      'apply-for-role' => $apply,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function apply_for_role_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'apply-for-role' && !empty($data['apply_for_role'])) {
    $apply = $data['apply_for_role'];

    $roles = user_roles();
    $user = isset($apply->user) ? $apply->user : user_load($apply->uid);

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'uid':
          $replacements[$original] = $sanitize ? check_plain($apply->uid) : $apply->uid;
          break;

        case 'name':
          $replacements[$original] = $sanitize ? check_plain($user->name) : $user->name;
          break;

        case 'mail':
          $replacements[$original] = $sanitize ? check_plain($user->mail) : $user->mail;
          break;

        case 'rid':
          $replacements[$original] = $sanitize ? check_plain($apply->rid) : $apply->rid;
          break;

        case 'role':
          $replacements[$original] = $sanitize ? check_plain($roles[$apply->rid]) : $roles[$apply->rid];
          break;

        case 'status':
          $replacements[$original] = $sanitize ? check_plain(theme('apply_for_role_status', array('status' => $apply->approved))) : theme('apply_for_role_status', array('status' => $apply->approved));
          break;

        case 'message':
          $replacements[$original] = $sanitize ? check_plain($apply->message) : $apply->message;
          break;

        // Default values for the chained tokens handled below.
        case 'applied':
          $replacements[$original] = format_date($apply->apply_date, 'medium', '', NULL, $language_code);
          break;

        case 'processed':
            $replacements[$original] = format_date($apply->approve_date, 'medium', '', NULL, $language_code);
          break;
      }
    }
    if ($applied_tokens = token_find_with_prefix($tokens, 'applied')) {
      $replacements += token_generate('date', $applied_tokens, array('date' => $apply->apply_date), $options);
    }

    if ($processed_tokens = token_find_with_prefix($tokens, 'processed')) {
      $replacements += token_generate('date', $processed_tokens, array('date' => $apply->approve_date), $options);
    }
  }

  return $replacements;
}
