<?php

/**
 * Implements hook_token_info().
 */
function invite_fboauth_token_info() {
  $tokens = array();

  $tokens['invite']['invite-fboauth-accept-link'] = array(
    'name' => t('Invite FBOauth action link: accept'),
    'description' => t('Displays link for invite accepting.'),
  );

  return array(
    'tokens' => $tokens,
  );
}

/**
 * Implements hook_tokens().
 */
function invite_fboauth_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $return = array();

  if ($type == 'invite' && $data['invite']) {
    $invite = $data['invite'];

    foreach ($tokens as $name => $original) {
      if ($name == 'invite-fboauth-accept-link') {
        $link = invite_fboauth_link_properties($invite);
        $return[$original] = url($link['href'], array('query' => $link['query']));
      }
    }
  }

  return $return;
}
