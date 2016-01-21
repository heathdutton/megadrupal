<?php
/**
 * @file
 *
 * Contains the language negotiation callback.
 */

/**
 * Callback function.
 *
 * @param $languages
 * @return string
 */
function features_language_redirect_to_default() {
  global $language;
  $path = 'admin/structure/features';

  $default_language = language_default('language');

  // Admin language module check.
  $admin_language_default = variable_get('admin_language_default', '');
  if (!empty($admin_language_default)) {
    $default_language = $admin_language_default;
  }
  // End admin language check.

  // Check for correct path and not in english.
  if (!drupal_is_cli() && strpos(request_uri(), $path) !== FALSE
    && _features_language_get_language_from_url() != $default_language) {
    // Goto Default Language version of features to not have an language issue.
    // Small message so that users know why they have to wait longer.
    drupal_set_message(t('Feature language: I have redirected you to the
     default language ie: @language, version of the features page.',
      array('@language' => $default_language)), 'warning');

    // Set language to default language for our object.
    $languages = language_list();
    // Overwrite the global language object.
    $language = $languages[$default_language];
    // Redirect.

    // PHP-based redirect is needed because drupal_goto() is not
    // necessarily available in this phase of the bootstrap.
    $domain = $_SERVER['HTTP_HOST'];
    $protocol = _features_language_url_is_secure() ? 'https' : 'http';
    $url = $protocol . '://' . $domain . '/' . $default_language . '/'. $path;
    header('Location: ' . $url);
    drupal_exit($url);
  }

  // Just return the language that it was set, not the version from admin
  // language.
  return $language->language;
}

/**
 * TRUE if site is being served over https.
 */
function _features_language_url_is_secure() {
  return
    (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off')
    || $_SERVER['SERVER_PORT'] == 443;
}

/**
 * Get the language from the url.
 *
 * This is because the language, from the global variable does not represent
 * the true correct language in the case of admin language module.
 *
 * @return mixed
 */
function _features_language_get_language_from_url() {
  $uri = explode('/', request_uri());

  if (!empty($uri[1])) {
    return $uri[1];
  }
}
