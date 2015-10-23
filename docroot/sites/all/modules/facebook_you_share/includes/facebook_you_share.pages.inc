<?php

/**
 * @filesource
 *   Defines functions for sharing content with facebook
 */

/**
 * Call back for share on facebook, this function will share the review on facebook
 */
function facebook_share($node) {
  global $user, $base_url;
  // If URL has node in it then verify it.
  if (is_numeric($node->nid)) {
    // Define variable to use ahead.
    $data = array(
      'node' => $node,
      'user' => $user,
    );
    // Title of the post
    $post_title = token_replace(variable_get($node->type . '_post_title', ''), $data);
    // Link of the post
    $post_link = token_replace(variable_get($node->type . '_post_link', ''), $data);
    // Get description for fb share.
    $post_body = token_replace(variable_get($node->type . '_post_description', ''), $data);
    // Replace all tags from body because this might cause problem in showing desc on fb
    $post_body = preg_replace("/<.*?>/", "", $post_body);
    // truncate description with 200
    $post_body = truncate_utf8($post_body, 200, TRUE, TRUE);
    // Caption
    $caption = token_replace(variable_get($node->type . '_post_caption', ''), $data);
    // Get image token and try replacing its value using available values.
    $image = token_replace(variable_get($node->type . '_post_image', theme_get_setting('logo')), $data);

    // If image is empty then set it to null
    if (empty($image)) {
      $image = NULL;
    }

    // Create a link to share
    $link = 'http://www.facebook.com/dialog/feed?app_id=' . variable_get('fb_app_id', '') .
    '&link=' . $post_link .
    '&picture=' . $image .
    '&name=' . urlencode($post_title) .
    '&caption=' . $caption .
    '&description=' . $post_body .
    '&display=' . variable_get($node->type . '_post_display', '') .
    '&redirect_uri=' . variable_get('fb_redirect_url', '');

    if (module_exists('userpoints')) {
      // Just make a call to give points to user.
      _facebook_you_share_give_userpoints($node->uid);
    }

    // Redirect user to share link
    drupal_goto($link);
  }

  else {
    return t('Oops ! something went wrong, no such node to share, please try again');
  }
}

/**
 * Helper function to give points to user.
 *
 * @param Integer $uid
 *   Node author id.
 */
function _facebook_you_share_give_userpoints($uid) {
  $params = array();

  // Create an array to provide to userpoints api.
  // @TODO: Provide user the ability to alter the message shown to user..
  $params = array(
    'points' => variable_get('fb_share_userpoints_points', 10),
    'uid' => $uid,
    // 'operation' => 'userpoints_nc_node_gain',
  );

  // Call userpoints api to give points to user.
  userpoints_userpointsapi($params);

  return TRUE;
}
