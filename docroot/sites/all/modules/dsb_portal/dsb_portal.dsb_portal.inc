<?php

/**
 * @file
 * dsb Portal hook implementations.
 */

/**
 * Implements hook_dsb_portal_rest_api_filters_alter().
 */
function dsb_portal_dsb_portal_rest_api_filters_alter(&$filters) {
  // Prepare the language filter, if necessary.
  if (variable_get('dsb_portal_search_only_current_lang', 0)) {
    global $language;
    $filters['language'][0] = $language->language;
  }
}

/**
 * Implements hook_dsb_portal_facet_name_alter().
 */
function dsb_portal_dsb_portal_facet_name_alter(&$facet_name, $facet) {
  // We implement our own hook to cleanly modify the language facets, if
  // enabled.
  if ($facet_name == 'language') {
    $facet_name = t("Language", array(), array('context' => 'dsb_portal:facet'));
  }
  else {
    $languages = dsb_portal_get_languages();
    if (in_array($facet_name, array_keys($languages))) {
      $facet_name = $languages[$facet_name];

      // We try to localize it, if possible.
      if (function_exists('locale')) {
        $facet_name = locale($facet_name);
      }
    }
  }
}
