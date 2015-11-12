<?php
/**
 * @file
 * Provide better intergration with the rules module.
 */

/**
 * Implements hook_rules_event_info().
 */
function facebook_rules_rules_event_info() {
  return array(
    'facebook_rules_registered_through_fbconnect' => array(
      'label' => t("User registered using Facebook connect."),
      'group' => t('Facebook'),
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
      ),
    ),
    'facebook_rules_linked_to_facebook' => array(
      'label' => t("User connected existing own account to Facebook."),
      'group' => t('Facebook'),
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
      ),
    ),
    'facebook_rules_registered_through_fbconnect_requires_approval' => array(
      'label' => t("User registered using Facebook connect, requires admin approval."),
      'group' => t('Facebook'),
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
      ),
    ),
    'facebook_rules_login_through_facebook' => array(
      'label' => t("User logged in using Facebook connect."),
      'group' => t('Facebook'),
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function facebook_rules_rules_action_info() {
  return array(
    'facebook_rules_post_to_my_wall' => array(
      'label' => t("Post an update on user's facebook wall."),
      'group' => t('Facebook'),
      'parameter' => array(
            'message' => array(
              'type' => 'text',
		      'optional' => TRUE,
              'label' => t('Message'),
              'description' => t("The message to post on Facebook wall."),
            ),
		    'link' => array(
              'type' => 'text',
		      'optional' => TRUE,
              'label' => t('Link to post'),
              'description' => t("The link to post."),
            ),
		    'name' => array(
              'type' => 'text',
		      'optional' => TRUE,
              'label' => t('Name of the link'),
              'description' => t("The name of the link. If not specified, default would be used."),
            ),
		    'caption' => array(
              'type' => 'text',
		      'optional' => TRUE,
              'label' => t('Caption of the link.'),
              'description' => t("The caption of the link (appears beneath the link name). If not specified, default would be used."),
            ),
		    'description' => array(
              'type' => 'text',
		      'optional' => TRUE,
              'label' => t('Description of the link.'),
              'description' => t("A description of the link (appears beneath the link caption). If not specified, default would be used."),
            ),
        ),
    ),
  );
}

/**
 * Callback for hook_rules_action_info().
 */
function facebook_rules_post_to_my_wall($message, $link, $name, $caption, $description) {
  global $user;

  // Check if user is connected to facebook.
  // Check if user has granted permission to post on facebook on his behalf.
  $check_grant = facebook_rules_profile_user_grant($user->uid);
  if (!$check_grant || ($check_grant && $check_grant == '0')) {
    return;
  }

  $url = "https://graph.facebook.com/me/feed";
  $access_token = facebook_rules_get_token($user->uid);
  $data = array(
    'access_token' => $access_token,
    'message' => $message,
	'link' => $link,
	'name' => $name,
	'caption' => $caption,
	'description' => $description,
    );
  facebook_rules($url, $data);
}

/**
 * Implements hook_rules_condition_info().
 */
function facebook_rules_rules_condition_info() {
  return array(
    'facebook_rules_user_connected_to_facebook' => array(
      'label' => t("User account is connected to his Facebook account."),
      'group' => t('Facebook'),
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t("User being checked."),
        ),
      ),
    ),
  );
}

/**
 * Callback for hook_rules_condition_info().
 */
function facebook_rules_user_connected_to_facebook($user) {
  return fboauth_fbid_load($user->uid) ? TRUE : FALSE;
}

