<?php

/**
 * @file
 * services_locale.resource.inc
 */

/**
 * Return list of countries.
 */
function services_locale_countries() {
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  return country_get_list();
}

/**
 * Return list of languages.
 */
function services_locale_languages() {
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  return locale_language_list();
}
