<?php
/**
 * @file
 * Contains menu callback functions for pages defined in this module
 */

/**
 * Menu callback to display the selection page.
 */
function language_selection_page_selection_page() {
  module_load_include('inc', 'language_selection_page', 'includes/language_selection_page.helpers');

  if (variable_get('language_selection_page_js_cookie_redirect', FALSE)) {
    // This adds a Javascript-based automatic redirect based on language in the
    // user's language cookie. This may be useful for sites using page caching
    // such as Varnish or Boost.
    $param = variable_get('language_cookie_param', 'language');
    drupal_add_js(array('language_selection_page' => array('cookieName' => $param)), 'setting');
    drupal_add_js(drupal_get_path('module', 'language_selection_page') . '/themes/js/cookie_redirect.js');
  }
  else {
    // Go to the destination directly if
    // a valid language negotiation is detected
    if (language_selection_page_is_negotiation_detected()) {
      // Get the query parameters and set the destination to '<front>' if
      // no destination is set.
      $query = drupal_get_query_parameters() + array('destination' => '<front>');
      drupal_goto($query['destination']);
    }
  }

  // Get the array of data.
  $language_selection_page = language_selection_page_selection_page_data();
  // Convert the array of data in HTML.
  $content = language_selection_page_selection_page_content($language_selection_page);

  switch (variable_get('language_selection_page_redirect_type', LANGUAGE_SELECTION_PAGE_TEMPLATE_ONLY)) {
    case LANGUAGE_SELECTION_PAGE_TEMPLATE_IN_THEME:
      return $content;
      break;
    case LANGUAGE_SELECTION_PAGE_TEMPLATE_ONLY:
      drupal_add_css(drupal_get_path('module', 'language_selection_page') . '/themes/css/language_selection_page.css');
      $html['title'] = t("Language selection");
      $html['page']['#children'] = $content;
      $html['language_selection_page'] = $language_selection_page;
      print theme('html', $html);
      exit;
  }
}


function language_selection_page_is_valid_request() {
  $query = drupal_get_query_parameters();

  //$language = $GLOBALS['language'];
  $language_selection_page_path = variable_get('language_selection_page_path', 'language_selection');
  $request = array_pop(explode('/', request_path()));

  if (
    $request == $language_selection_page_path &&
    isset($query['destination']) &&
    !empty($query['destination']) &&
    $query['destination'] != $language_selection_page_path
  ) {
    return TRUE;
  }

  return FALSE;
}
