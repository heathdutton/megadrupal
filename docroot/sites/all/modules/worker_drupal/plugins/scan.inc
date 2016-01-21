<?php
/**
 * @file
 * Provide Drupal scan worker plugin.
 *
 * - php syntax
 * - list tests
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('Drupal scan'),
  'description' => t('Provide Drupal scan worker plugin (ie. php syntax and obtain list of tests).'),
  'perform' => 'scan_perform',
);

/**
 * Perform the job.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   An array containing a boolean for pass/fail and the result. The result is
 *   an string error message; an array of syntax errors, see worker_syntax();
 *   or a list of test classes found.
 *
 * @see worker_perform()
 */
function scan_perform(array $properties) {
  if ($errors = worker_syntax($properties)) {
    // Encapsulate the errors so the type can be easily determined.
    return array(FALSE, $errors);
  }
  elseif ($errors === FALSE) {
    // An error occurred and should be treated as a job failure.
    return array(FALSE, worker_log_last());
  }

  // Get the list of test cases.
  return array(TRUE, scan_list($properties));
}

/**
 * Scan test files to get a list of test cases.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   List of test cases, or FALSE.
 */
function scan_list(array $properties) {
  worker_log('Assemble list of relevant test files.');

  // Since the syntax check has passed at this point we can assume the list of
  // files will return properly.
  $files = worker_file_list($properties);

  // Remove all non-test files.
  foreach ($files as $key => $file) {
    if (pathinfo($file, PATHINFO_EXTENSION) != 'test') {
      unset($files[$key]);
    }
  }

  if ($properties['version'] == 7) {
    // The files[] listed in the .info file must be respected as Drupal 7 test
    // harness uses the list to obtain the .test files to scan.
    // @TODO Implement.
  }

  worker_log('> Found ' . number_format(count($files)) . '.');

  // Search the list of test files for classes containing a getInfo() method.
  // This method assumes that a class only has a getInfo() method if it is a
  // proper Drupal test case and that no function named getInfo() exists in the
  // test file.
  $classes = array();
  foreach ($files as $file) {
    // Tokenize the file and cycle through the tokens.
    $tokens = token_get_all(file_get_contents(JOB_ROOT . '/' . $file));
    while ($token = next($tokens)) {
      if ($token[0] == T_CLASS) {
        // Keep track of class names.
        $class = scan_list_string($tokens);
      }
      else if ($token[0] == T_FUNCTION) {
        // If a getInfo method is found then add the class to the list.
        if (scan_list_string($tokens) == 'getInfo') {
          $classes[] = $class;
        }
      }
    }
  }

  worker_log('Found ' . number_format(count($classes)) . ' test case(s).');

  // An empty array will not encode properly so use FALSE.
  return $classes ? $classes : FALSE;
}

/**
 * Get the next string.
 *
 * Note that the internal pointer of the passed array will be incremented.
 *
 * @param $tokens
 *   Array of PHP tokens to search for string in.
 * @return
 *   The next string found in tokens, otherwise FALSE.
 */
function scan_list_string(&$tokens) {
  while ($token = next($tokens)) {
    if ($token[0] == T_STRING) {
      return $token[1];
    }
  }
  return FALSE;
}
