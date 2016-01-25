<?php
/**
 * @file
 * Menu callbacks for the Ooyala Upload module.
 */

/**
 * Menu callback; Create a new asset.
 *
 * @return
 *   Prints a JSON response string to the browser.
 */
function ooyala_upload_asset_create() {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');

  // POST requests from Ooyala's JS use JSON-formatted POST data, which won't
  // populate the $_POST variable. Read from the input stream wrapper instead.
  $input = file_get_contents('php://input');
  $asset = $input ? json_decode($input) : NULL;

  // Only allow the following properties to be passed to Ooyala.
  $properties = array(
    'name',
    'description',
    'file_name',
    'file_size',
    'asset_type',
  );
  foreach ($properties as $property) {
    if (!isset($asset->{$property})) {
      return ooyala_upload_error("Missing asset attribute $property");
    }
  }

  $asset = ooyala_api_asset_add($asset);

  // Build our response to the client. We return any properties set by the
  // client that might have been modified.
  $response = new stdClass();
  foreach ($properties as $property) {
    if (isset($asset->{$property})) {
      $response->{$property} = $asset->{$property};
    }
  }
  $response->embed_code = $asset->embed_code;

  ooyala_upload_json_output($response);
}

/**
 * Load an Ooyala asset from the Ooyala backlot.
 *
 * Due to the way the Ingestion library works, we have to include the
 * $uploading_urls parameter here so we can return something completely
 * different if it's set.
 *
 * @param string $embed_code
 *   The embed code of the asset to load.
 * @param string $uploading_urls
 *   Optional parameter indicating to load uploading URLs instead. Set to
 *   'uploading_urls' to return them instead of the asset object.
 */
function ooyala_upload_asset_load($embed_code, $uploading_urls = FALSE) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');
  $asset = ooyala_api_video_load($embed_code);
  $asset = (object) $asset;

  if ($uploading_urls) {
    $urls = ooyala_api_asset_uploading_urls($asset);
    ooyala_upload_json_output($urls);
    return;
  }

  ooyala_upload_json_output($asset);
}

/**
 * Menu callback; Mark an asset as uploaded.
 *
 * @param $embed_code
 *   The embed code of the asset to mark as uploaded.
 *
 * @return
 *   Prints a JSON response string to the browser.
 */
function ooyala_upload_asset_upload_complete($embed_code) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');

  $asset = ooyala_api_video_load($embed_code);

  if ($asset) {
    $asset = (object) $asset;
    if ($asset->status === 'uploading') {
      $asset->status = 'uploaded';
      $asset = ooyala_api_asset_uploaded($asset);
      ooyala_upload_json_output($asset);
    }
    else {
      ooyala_upload_error(t('The specified embed code has already been marked as uploaded, it has the current status "@status."', array('@status' => $asset->status)));
    }
  }
  else {
    ooyala_upload_error(t("Video not found."));
  }
}

/**
 * Utility to return errors to the browser.
 *
 * @param $error_message
 *   The message to be returned to the browser.
 * @param int $response_code
 *   Optional response code to be set as the "status" header. Defaults to 406.
 */
function ooyala_upload_error($error_message, $response_code = 406) {
  drupal_add_http_header('status', $response_code);
  ooyala_upload_json_output(array('error' => $error_message));
}

/**
 * Wrapper around the current Drupal version's JSON output mechanism.
 */
function ooyala_upload_json_output($variable) {
  return drupal_json_output($variable);
}