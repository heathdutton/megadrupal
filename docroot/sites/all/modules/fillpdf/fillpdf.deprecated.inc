<?php
/**
 * @deprecated fillpdf_merge_execute_pdf_action()
 * Figure out what to do with the PDF and do it.
 *
 * @return void
 * @param $pdf_info An object containing the loaded record from {fillpdf_forms}.
 * @param $pdf_data A string containing the content of the merged PDF.
 * @param $token_objects An array of objects to be used in replacing tokens.
 * Here, specifically, it's for generating the filename of the handled PDF.
 * @param $action One of the following keywords: default, download, save,
 * redirect. These correspond to performing the configured action (from
 * admin/structure/fillpdf/%), sending the PDF to the user's browser, saving it
 * to a file, and saving it to a file and then redirecting the user's browser to
 * the saved file.
 * @param $force_download If set, this function will always end the request by
 * sending the filled PDF to the user's browser.
 */
function fillpdf_merge_handle_pdf($pdf_info, $pdf_data, $token_objects, $action = 'download', $force_download = FALSE) {
  if ($pdf_info->scheme === 'private') {
    throw new Exception('fillpdf_merge_handle_pdf() does not work properly with
    private files. Please use fillpdf_merge_execute_pdf_action() instead.');
  }

  if (in_array($action, array('default', 'download', 'save', 'redirect')) === FALSE) {
    // Do nothing if the function is called with an invalid action.
    return;
  }
  // Generate the filename of downloaded PDF from title of the PDF set in
  // admin/structure/fillpdf/%fid
  $output_name = _fillpdf_process_filename($pdf_info->title, $token_objects);

  if ($action == 'default') {
    // Determine the default action, then re-set $action to that.
    if (empty($pdf_info->destination_path) === FALSE) {
      $action = 'save';
    }
    else {
      $action = 'download';
    }
  }

  // Initialize variable containing whether or not we send the user's browser to
  // the saved PDF after saving it (if we are)
  $redirect_to_file = FALSE;

  // Get a load of this switch...they all just fall through!
  switch ($action) {
    case 'redirect':
      $redirect_to_file = $pdf_info->destination_redirect;
    case 'save':
      fillpdf_save_to_file($pdf_info, $pdf_data, $token_objects, $output_name, !$force_download, $redirect_to_file);
    // FillPDF classic!
    case 'download':
      drupal_add_http_header("Pragma", "public");
      drupal_add_http_header('Expires', 0);
      drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
      drupal_add_http_header('Content-type', 'application-download');
      // This must be strlen(), not drupal_strlen() because the length in bytes,
      // not in characters, is what is needed here.
      drupal_add_http_header('Content-Length', strlen($pdf_data));
      drupal_add_http_header('Content-disposition', 'attachment; filename="' . $output_name . '"');
      drupal_add_http_header('Content-Transfer-Encoding', 'binary');
      echo $pdf_data;
      drupal_exit();
      break;
  }
}

/**
 * @deprecated Use fillpdf_action_save_to_file() instead.
 *
 * @param stdClass $pdf_info
 * @param string $pdf_data
 * @param array $token_objects
 * @param $output_name
 * @param bool $redirect
 * @param bool $redirect_to_file
 * @param null $destination_path_override
 * @return bool|string|void Nothing (if redirected), the path to the file
 *   (if saving the file succeeded), or FALSE (if it didn't).
 * @throws \Exception
 */
function fillpdf_save_to_file($pdf_info, $pdf_data, $token_objects, $output_name, $redirect = TRUE, $redirect_to_file = FALSE, $destination_path_override = NULL) {
  if ($pdf_info->scheme === 'private') {
    throw new Exception('fillpdf_save_to_file() does not work properly with
    private files. Please use fillpdf_action_save_to_file() instead.');
  }

  if (isset($destination_path_override) && empty($destination_path_override) !== FALSE) {
    $destination_path = $destination_path_override;
  }
  if (empty($pdf_info->destination_path) && empty($destination_path_override)) {
    // If this function is called and the PDF isn't set up with a destination
    // path, give it one.
    $destination_path = 'fillpdf';
  }
  else {
    $destination_path = $pdf_info->destination_path;
  }
  $destination_path = _fillpdf_process_destination_path($pdf_info->destination_path, $token_objects);
  $path_exists = file_prepare_directory($destination_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  if ($path_exists === FALSE) {
    watchdog('fillpdf', "The path %destination_path does not exist and could not be
      automatically created. Therefore, the previous submission was not saved. If
      the URL contained download=1, then the PDF was still sent to the user's browser.
      If you were redirecting them to the PDF, they were sent to the homepage instead.
      If the destination path looks wrong and you have used tokens, check that you have
      used the correct token and that it is available to FillPDF at the time of PDF
      generation.",
      array('%destination_path' => $destination_path));
  }
  else {
    // Full steam ahead!
    $saved_file_path = file_unmanaged_save_data($pdf_data, $destination_path . "/$output_name", FILE_EXISTS_RENAME);
    if ($redirect === TRUE) {
      if (isset($_GET['destination']) === FALSE) {
        // Should we send the user directly to the saved PDF? If so, do that.
        if ($redirect_to_file) {
          drupal_goto(file_create_url($saved_file_path));
        }
      }
    }
  }

  if ($redirect === TRUE) {
    // Allow the "destination" query string parameter to be used
    // e.g. fillpdf?nid=1&fid=1&destination=node/1
    // If no destination is provided, drupal_goto() will send the
    // user to the front page.
    drupal_goto();
  }

  return $saved_file_path;
}

