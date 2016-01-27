<?php

// Tell IMCE we want to have a custom process function.
$GLOBALS['conf']['imce_custom_process']['feature_wysiwyg_editor_imce_process'] = TRUE;


/**
 * Custom process callback for imce.
 * @param $imce
 */
function feature_wysiwyg_editor_imce_process(&$imce) {

  if ($imce['dir'] === '.' && !empty($imce['subdirectories'])) {

    // Retrieve the directories we wish to exclude
    $excluded_directories  = $imce['excluded'];

    // Loop through the subdirectories and remove if necessary
    foreach($imce['subdirectories'] as $key => $value) {
      if (drupal_match_path($value,$excluded_directories)) {
        unset($imce['subdirectories'][$key]);
      }
    }
  }
}