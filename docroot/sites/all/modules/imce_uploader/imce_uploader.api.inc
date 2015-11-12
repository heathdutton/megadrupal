<?php

/**
 * @file
 * API documentation for IMCE Uploader.
 */

/**
 * Register list of uploaders to IMCE.
 *
 * Each item may have the following properties
 * - title: The name of uploader.
 * - description: The description of uploader.
 * - callback: Process function of uploader when imce's called.
 * - weight: The weight of this uploader in the list.
 *
 * @return array
 *   List of uploaders.
 */
function hook_imce_uploader() {
  $uploaders = array();

  $uploaders['plupload'] = array(
    'title'       => t('Plupload'),
    'description' => t('Allows you to upload multiple files using HTML5, Gears, Silverlight, Flash, BrowserPlus or normal forms.'),
    'callback'    => 'imce_uploader_plupload',
  );

  return $uploaders;
}

/**
 * Change the list of registered uploaders.
 *
 * @param array $uploaders
 *   List of uploaders.
 */
function hook_imce_uploader_alter(&$uploaders) {
  if (isset($uploaders['plupload'])) {
    $uploaders['plupload']['weight'] = -100;
  }
}
