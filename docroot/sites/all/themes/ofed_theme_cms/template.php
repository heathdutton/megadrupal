<?php
/**
 * @file
 * CMS theme template file
 */

/**
 * Force language to be English in this theme.
 */
// Get the list of languages.
$languages = language_list();
// Set up the new language code.
$lang_code = 'en';
// Make sure the required language object is actually set.
if (isset($languages[$lang_code])) {
  // Overwrite the global language object.
  global $language;
  $language = $languages[$lang_code];
}


/**
 * Register a theme implementations.
 */
function cms_theme_theme() {
  return array();
}

/**
 * Common Implementation.
 */
require_once 'template_generic.php';
require_once 'template_menu.php';
require_once 'template_view.php';
require_once 'template_module.php';
