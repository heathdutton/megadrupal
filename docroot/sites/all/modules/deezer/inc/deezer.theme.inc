<?php
/**
 * @file
 * Stores all theme implementations.
 */

/**
 * Generates a login link.
 *
 * @param array $vars
 *   An associative array containing:.
 *     -link: Complete link to open login popup.
 *     -anonym_text: The translated string for anonymous user.
 *     -logged_text: The translated string for logged in user.
 */
function theme_deezer_login_popup($vars) {
  $link = $vars['link'];

  return $link;
}

/**
 * Generates JS script to close the login popup.
 */
function theme_deezer_login_popup_js_close($vars) {
  $script = "<script>
              window.close();
              window.opener.location.reload();
            </script>";

  return $script;
}

/**
 * Preprocess the deezer_login_popup theme hook.
 */
function template_preprocess_deezer_login_popup(&$vars) {
  // Prevent link to show if application id isn't set.
  $app_id      = DEEZER_APP_ID;
  $anonym_text = $vars['anonym_text'];
  $logged_text = $vars['logged_text'];
  $link        = '';

  if (!empty($app_id)) {
    // Attaching JS logic for the popup.
    $path = drupal_get_path('module', 'deezer') . '/js/deezer.js';
    drupal_add_js($path);

    $text = user_is_anonymous() ? $anonym_text : $logged_text;
    $link = l($text, '', array(
        'fragment' => ' ',
        'external' => TRUE,
        'attributes' => array('id' => 'deezer-login'))
    );
  }

  $vars['link'] = $link;
}
