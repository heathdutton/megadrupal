<?php
/**
 * @file
 * Page callbacks for the Webform Google Exporter module.
 */

/**
 * Menu callback; Provides handling return responses from Google.
 */
function webform_google_exporter_oauth_callback() {
  $errors = array();
  $token = webform_google_exporter_get_token($_GET['code'], $errors);

  if ($token) {
    $node = node_load($_SESSION['webform_google_exporter']['nid']);
    $metadata = array(
      'mimeType' => 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet',
      'title' => $node->title,
    );
    $filepath = $_SESSION['webform_google_exporter']['file'];
    $file_data = webform_google_exporter_upload_file($token, $metadata, $filepath, $errors);

    if ($file_data) {
      drupal_goto($file_data->alternateLink);
    }
  }

  // In the event of errors.
  if ($errors) {
    $error = key($errors);
    $message = reset($errors);
    return t('The export could not be sent to Google. Google returned the error "@error" with the message "@message".', array('@error' => $error, '@message' => $message));
  }
  else {
    return t('The export could not be sent to Google for an unknown reason.');
  }
}

/**
 * Given an authorization code from the user's browser, request an access token.
 *
 * This access token is then used to perform actual API requests against the
 * Google Drive API.
 */
function webform_google_exporter_get_token($authcode, &$errors) {
  $parameters = array(
    'code' => $authcode,
    'grant_type' => 'authorization_code',
    'redirect_uri' => url('webform/google-exporter', array('absolute' => TRUE)),
    'client_id' => variable_get('webform_google_exporter_client_id'),
    'client_secret' => variable_get('webform_google_exporter_client_secret'),
  );

  $options = array(
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded; charset=UTF-8'),
    'method' => 'POST',
    'data' => http_build_query($parameters, '', '&'),
  );
  $response = drupal_http_request('https://www.google.com/accounts/o8/oauth2/token', $options);
  if ($response->code == '200') {
    $data = json_decode($response->data);
    $token = $data->access_token;
    return $token;
  }
  else {
    $response_data = json_decode($response->data);
    $errors[$response->error] = $response_data->error;
    return FALSE;
  }
}

/**
 * Make an API request to upload a file to Google Drive.
 *
 * @param string $token
 *   A valid OAuth token with which this request will be authenticated.
 * @param array $metadata
 *   The metadata attached to the file. i.e. Title, Description, etc.
 * @param string $filepath
 *   The path to the file to be uploaded.
 * @param array $errors
 *   An array passed by reference to hold any errors that occur while uploading.
 * @return object
 *   The decoded JSON response from Google Drive with information about the
 *   newly uploaded file. See the Google Drive documentation for a complete
 *   reference: https://developers.google.com/drive/v2/reference/files
 */
function webform_google_exporter_upload_file($token, $metadata, $filepath, &$errors) {
  $boundary = 'aaaaaaaaaaa1Boundary1aaaaaaaaaaa';
  $options = array(
    'method' => 'POST',
    'headers' => array(
      'Content-Type' => 'multipart/related; boundary="' . $boundary . '"',
      'Authorization' => 'OAuth ' . $token,
    ),
    'data' => webform_google_exporter_multipart_encode($boundary, $metadata, $filepath),
  );

  if (function_exists('gzencode')) {
    $options['headers']['Content-Encoding'] = 'gzip';
    $options['data'] = gzencode($options['data']);
  }

  // The query string "convert" parameter converts the TSV file into a
  // spreadsheet automatically. For a list of other parameters accepted by
  // Google, see https://developers.google.com/drive/v2/reference/files/insert.
  $response = drupal_http_request('https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true', $options);
  if ($response->code == '200') {
    return json_decode($response->data);
  }
  else {
    $response_data = json_decode($response->data);
    $errors[$response_data->error->code] = $response_data->error->message;

    // Provide helpful information for first-time setups.
    if ($response_data->error->code == 403) {
      drupal_set_message(t('Make sure that you have configured your Google API project with the "Drive API" service enabled.'), 'warning');
    }

    return FALSE;
  }
}

/**
 * Encode a file for sending to Google Drive via the API.
 *
 * When uploading a file with metadata, Google accepts a POST request of the
 * type "multipart/related". This means there are exactly two parts to the
 * request: data and related content.
 *
 * @see https://developers.google.com/drive/manage-uploads
 */
function webform_google_exporter_multipart_encode($boundary, $metadata, $filepath) {
  $mime_type = isset($metadata['mimeType']) ? $metadata['mimeType'] : 'application/octet-stream';
  $output = "";
  $output .= "--$boundary\r\n";
  $output .= webform_google_exporter_multipart_encode_meta($metadata);
  $output .= "--$boundary\r\n";
  $output .= webform_google_exporter_multipart_encode_file($filepath, $mime_type);
  $output .= "--$boundary--";
  return $output;
}

/**
 * Helper for webform_google_exporter_multipart_encode().
 */
function webform_google_exporter_multipart_encode_meta($values) {
  $values = json_encode($values);
  return "Content-Type: application/json\r\n\r\n$values\r\n";
}

/**
 * Helper for webform_google_exporter_multipart_encode().
 */
function webform_google_exporter_multipart_encode_file($filepath, $mime_type) {
  $data = "";
  $data .= "Content-Type: $mime_type\r\n";
  $data .= "Content-Transfer-Encoding: base64\r\n\r\n";
  $data .= base64_encode(file_get_contents($filepath)) . "\r\n";
  return $data;
}
