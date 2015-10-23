<?php
/**
 * The Excel exporter currently is just a tab-delimited export.
 */
class webform_google_exporter extends webform_exporter_excel_xlsx {
  public $options;

  function download($node) {
    // For headers on this file, we actually redirect the user to Google's
    // authentication page. Then the menu handler at webform/google-exporter
    // then will actually do the work of transfering the current file to Google.
    $query = array(
      'response_type' => 'code',
      'redirect_uri' => url('webform/google-exporter', array('absolute' => TRUE)),
      'client_id' => variable_get('webform_google_exporter_client_id', NULL),
      'scope' => 'https://www.googleapis.com/auth/drive.file',
    );

    // Save information about the export to the session while we authenticate.
    $_SESSION['webform_google_exporter'] = array(
      'nid' => $node->nid,
      'file' => $this->options['file_name'],
    );

    drupal_goto('https://www.google.com/accounts/o8/oauth2/authorization', array('query' => $query));
  }
}
