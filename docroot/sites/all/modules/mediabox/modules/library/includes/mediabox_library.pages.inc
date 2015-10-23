<?php

/**
 * @file
 * Mediabox library pages.
 */

/**
 * Uploads new media objects.
 *
 * @return array|mixed
 */
function mediabox_library_upload() {
  module_load_include('inc', 'mediabox_library', 'includes/mediabox_library.forms');
  return drupal_get_form('mediabox_library_form_upload');
}
