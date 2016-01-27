<?php

/**
 * @file
 * Media integration for the media_theplatform_mpx module.
 */

/**
 * Implements hook_media_internet_providers().
 */
function media_theplatform_mpx_media_internet_providers() {
  return array(
    'MediaInternetThePlatformMpxHandler' => array(
      'title' => 'MPX Media',
      'hidden' => TRUE,
    ),
  );
}

/**
 * Implements hook_media_parse().
 *
 * @todo This hook should be deprecated. Refactor Media module to not call it
 *   any more, since media_internet should be able to automatically route to the
 *   appropriate handler.
 */
function media_theplatform_mpx_media_parse($embed_code) {
  module_load_include('media_theplatform_mpx', 'inc', 'includes/MediaInternetThePlatformMpxHandler');
  $handler = new MediaInternetThePlatformMpxHandler($embed_code);
  return $handler->parse($embed_code);
}
