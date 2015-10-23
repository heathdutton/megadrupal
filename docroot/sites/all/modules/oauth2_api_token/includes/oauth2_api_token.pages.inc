<?php
/**
 * @file
 * Page callbacks for the OAuth2 API Token module.
 */

/**
 * Page callback for the user's tokens list.
 */
function oauth2_api_token_user_list_page($account) {
  $view = views_get_view('oauth2_api_token');
  if (!$view) {
    watchdog('oauth2_api_token', 'View not found: oauth2_api_token', array(), WATCHDOG_ERROR);

    return MENU_NOT_FOUND;
  }
  $view->override_path = current_path();

  return array(
    'list' => array(
      '#markup' => $view->preview('embed_1', array($account->uid)),
    ),
  );
}

/**
 * Page callback for creating a token.
 */
function oauth2_api_token_create_page() {
  drupal_set_title(t('Create an API token'), PASS_THROUGH);
  module_load_include('inc', 'oauth2_api_token', 'includes/oauth2_api_token.forms');

  return array(
    'form' => drupal_get_form('oauth2_api_token_form', NULL),
  );
}

/**
 * Page callback for editing a token.
 */
function oauth2_api_token_edit_page(OAuth2ServerToken $token) {
  $account = oauth2_api_token_get_account($token);
  if (!$account) {
    return MENU_ACCESS_DENIED;
  }
  module_load_include('inc', 'oauth2_api_token', 'includes/oauth2_api_token.forms');

  return array(
    'form' => drupal_get_form('oauth2_api_token_form', $token, $account),
  );
}
