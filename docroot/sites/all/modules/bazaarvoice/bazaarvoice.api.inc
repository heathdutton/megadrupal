<?php
/**
 * @file
 * bazaarvoice.api.inc
 *
 * Defines bazaarvoice hooks that can be used by other modules.
 */

/**
 * Alter the array of Bazaarvoice locales.
 *
 * This hook is invoked from bazaarvoice_get_locales() after the locales have
 * been retrieved from the database.
 *
 * @param array $locales
 *   Associative array of locales keyed by language code.
 *   Each array item contains the following attributes:
 *    - language : The two character language code.
 *    - locale : The local code to be used by Bazaarvoice.
 *    - is_default : Boolean flag is this locale is the default one.
 *
 * @ingroup bazaarvoice_alter_hooks
 */
function hook_bazaarvoice_locales_alter(array &$locales) {
  // Add klingon as a language locale.
  $locales['kl'] = array(
    'language' => 'kl',
    'locale' => 'kl_US',
    'is_default' => 1,
  );
}

/**
 * Alter the locale code used as the default.
 *
 * This hook is invoked from bazaarvoice_get_default_locale() after the default
 * locale has been retrieve from the database or default language.
 *
 * @param string|NULL $default_locale
 *   The string value for the default locale or NULL if not retrieved.
 *
 * @ingroup bazaarvoice_alter_hooks
 */
function hook_bazaarvoice_default_locale_alter(&$default_locale) {
  // Force klingon to be the default locale.
  $default_locale = 'kl_US';
}

/**
 * Perform action before saving locales.
 *
 * This hook is invoked from bazzarvoice_set_locales() before saving locales
 * to the database.
 *
 * @param array $locales
 *   Associative array of locales to be stored to the database.
 *   The associative array has the following attributes:
 *    - language : The two character language code.
 *    - locale : The local code to be used by Bazaarvoice.
 *    - is_default : Boolean flag is this locale is the default one.
 *
 * @return array
 *   Return modified locales array.
 *
 * @ingroup bazaarvoice_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_presave_locales(array $locales) {
  // Unset all locales as default.
  foreach ($locales as &$locale) {
    $locale['is_default'] = 0;
  }
  // Force klingon to be default locale.
  if (isset($locales['kl'])) {
    $locales['kl']['is_default'] = 1;
  }

  return $locales;
}

/**
 * Perform an action after the locales have been deleted.
 *
 * This hook is invoked from bazaarvoice_delete_locales() after deleting locales
 * from the database.
 *
 * @param array $languages
 *   Array of language codes that were deleted.
 *
 * @ingroup bazaarvoice_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_postdelete_locales(array $languages) {
  if (isset($languages['kl'])) {
    watchdog('bazaarvoice', "Hu'tegh! Klingon was deleted!");
  }
}
