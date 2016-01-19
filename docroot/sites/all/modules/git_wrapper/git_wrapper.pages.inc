<?php

/**
 * @file
 * Page callbacks Git Wrapper module.
 */

/**
 * Page callback; Returns the version of the Git binary.
 */
function git_wrapper_version_page() {
  try {
    $data = array(
      'status' => TRUE,
      'version' => git_wrapper()->version(),
    );
  }
  catch (Exception $e) {
    $data = array(
      'status' => FALSE,
      'error' => t('Could not get version from the Git executable.'),
    );
    watchdog_exception('git_wrapper', $e);
  }

  return array(
    '#type' => 'ajax',
    '#commands' => array(
      array(
        'command' => 'gitWrapperVersion',
        'data' => $data,
      ),
    ),
  );
}
