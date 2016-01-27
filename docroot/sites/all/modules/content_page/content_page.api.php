<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Flag a request as one that should be displayed in content only mode.
 *
 * Modules implementing this hook should return only return a value if they
 * want to control the 'content only' setting for the request. Returning TRUE
 * will set the request in 'content only' mode and will not ask modules with
 * lower weight. Similarly, returning FALSE will set the request as a normal
 * request and will not ask modules with a lower weight. Not returning anything
 * (or the implicit NULL) will allow modules with a lower weight to set the
 * 'content only' mode for the request.
 */
function hook_content_page_is_active_page() {
  if (isset($_GET['ajax_request'])) {
    return TRUE;
  }
}
