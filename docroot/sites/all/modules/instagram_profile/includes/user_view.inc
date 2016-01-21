<?php
/**
 * @file
 * User interface builder functions for viewing a user profile.
 */


/**
 * Builds the Instagram part of the user profile page.
 *
 * @param $account stdClass
 *    The Drupal user of the account/profile being viewed.
 *
 * @param Array
 *    Drupal renderable array.
 */
function _instagram_profile_user_view($account) {
  $element = array(
    '#type'       => 'container',
    '#attributes' => array('class' => 'instagram-profile'),
    '#attached'   => array('css' => array(_instagram_profile_asset('view.css'))),
  );

  // Add a title like the "History" and other profile sections.
  $element['title'] = array(
    '#markup' => '<h3>' . t('Instagram') . '</h3>',
  );

  if (_instagram_profile_self($account->uid)) {
    // Add elements to configure or reconfigure Instagram integration.
    $element['authenticate'] = instagram_profile_authenticate_link();
  }

  // Add the elements to view the feed.
  $element['feed'] = instagram_profile_feed($account);

  return $element;
}

/**
 * Builds an element to view a shared Instagram feed.
 *
 * @param $account stdClass
 *    The Drupal user of the account/profile being viewed.
 *
 * @param Array
 *    Drupal renderable array.
 */
function instagram_profile_feed($account) {
  $js = array(
    // Attach the script to fetch and build the view of the shared feed.
    _instagram_profile_asset('view.js'),
    // Settings shared with all occurences of this renderable on this page.
    _instagram_profile_js_setting('clientId', instagram_profile_client_id()),
  );

  // Include the username when viewing other people's profiles.
  if (!_instagram_profile_self($account->uid)) {
    $js[] = _instagram_profile_js_setting('userName', format_username($account));
  }

  return array(
    '#theme'        => 'instagram_profile_placeholder',
    // Get the Drupal' user's shared Instagram user ID, if any.
    '#instagram_id' => instagram_profile_instagram_user_id($account->uid),
    '#attached'     => array('js' => $js),
  );
}

/**
 * Builds an element to share an (or change a shared) Instagram feed.
 *
 * @param Array
 *    Drupal renderable array.
 */
function instagram_profile_authenticate_link() {
  // A CSRF token is required to keep the AJAX API/endpoint secure.  It is
  // unique per session.
  if (empty($_SESSION['instagram_csrf_token'])) {
    $_SESSION['instagram_csrf_token'] = drupal_random_key();
  }

  // Javascript attachments.
  $path = drupal_get_path('module', 'instagram_profile');
  $attach = array(
    // Attach script & settings to handle the authenticate link and "Disable"
    // button.  Make it heavier than view.js.
    _instagram_profile_asset('configure.js', 10),
    _instagram_profile_js_setting('csrfToken', $_SESSION['instagram_csrf_token']),
    _instagram_profile_js_setting('assetsDirectory', url("$path/assets")),
  );

  // Add the demo user ID if it is enabled.
  $demo = variable_get('instagram_profile_demo');
  if ($demo) {
    $attach[] = _instagram_profile_js_setting('demo', $demo);
  }

  return array(
    '#theme'     => 'link__instagram_profile_authenticate',
    // JS sets the text depending on state.  (If/when a feed has been shared.)
    '#text'    => '',
    '#path'     => 'https://instagram.com/oauth/authorize',
    '#options'  => array(
      'attributes' => array(
        'class' => array(
          'authenticate'
        ),
      ),
      'query' => array(
        'client_id'     => instagram_profile_client_id(),

        // Client-Side (Implicit) Authentication
        // @see http://instagram.com/developer/authentication/
        'response_type' => 'token',

        // The redirect uri configured on Instagram.com/developer must match.
        'redirect_uri'  => url('user', array('absolute' => TRUE)),
      ),
    ),
    '#attached' => array('js' => $attach),
  );
}
