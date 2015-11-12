<?php

/**
 * @file
 * Functions that handle UI-based use-cases...
 */


/**
 * @see drupal_deliver_html_page()
 */
function entity_xliff_deliver_xlf($page_callback_result) {
  // If we didn't receive a string, revert to the standard page delivery call.
  if (is_int($page_callback_result)) {
    drupal_deliver_html_page($page_callback_result);
  }
  elseif (isset($page_callback_result)) {
    // Set an appropriate content-type header.
    drupal_add_http_header('Content-Type', 'application/x-xliff+xml');
    drupal_add_http_header('Content-Disposition', 'attachment; filename="' . entity_xliff_file_name() . '"');

    // Print the contents.
    print $page_callback_result;

    // Perform end-of-request tasks.
    drupal_page_footer();
  }
}

/**
 * Primary page callback to return a given entity as an XLIFF file.
 *
 * @param string $type
 *   The entity type (e.g. node or user).
 *
 * @param mixed $entity
 *   The entity object itself.
 *
 * @return string|int
 *   Returns the given entity as an XLIFF string. If the entity couldn't be
 *   serialized to XLIFF, the MENU_NOT_FOUND constant will be returned.
 */
function entity_xliff_to_xlf($type, $entity) {
  $wrapper = entity_metadata_wrapper($type, $entity);
  $targetlang = entity_xliff_get_target_lang();

  if ($xliff = entity_xliff_get_xliff($wrapper, $targetlang)) {
    entity_xliff_file_name(implode('-', array($type, $wrapper->getIdentifier(), $targetlang)) . '.xlf');
    return $xliff;
  }
  else {
    return MENU_NOT_FOUND;
  }
}

/**
 * Get or set the entity XLIFF file name for this request.
 *
 * @param string|null $name
 *   (Optional) If provided, sets the XLIFF file name for this request to this
 *   $name.
 *
 * @return string
 *   Returns the file name for this request.
 */
function entity_xliff_file_name($name = NULL) {
  $filename = &drupal_static(__FUNCTION__, 'entity.xlf');

  if ($name) {
    $filename = $name;
  }

  return $filename;
}

/**
 * Returns the target language on this request.
 *
 * @return string
 *   The language code representing the target language for this request.
 */
function entity_xliff_get_target_lang() {
  $params = drupal_get_query_parameters();

  // Check if a target language was provided via query param.
  if (isset($params['targetLang'])) {
    $languages = language_list();

    // Validate the given language.
    if (isset($languages[$params['targetLang']])) {
      return $params['targetLang'];
    }
  }

  return $GLOBALS['language']->language;
}
