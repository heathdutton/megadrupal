<?php
/**
 * @file
 * Provide Drupal coder integration.
 *
 * Requires access to drush and the following patches:
 *  - http://drupal.org/node/946978
 *  - http://drupal.org/node/948124
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Number of files to be reviewed at the same time.
 */
define('CODER_CHUNK', 30);

/**
 * The maximum number of relevant messages.
 */
define('CODER_MAX', 1000);

$plugin = array(
  'title' => t('Drupal coder'),
  'description' => t('Provide Drupal coder integration.'),
  'perform' => 'coder_perform',
);

/**
 * Perform the job.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   An array containing a boolean for pass/fail and the result. The result is
 *   a multi-dimentional, associative array of coder_review results keyed by
 *   the file path and containing an array of messages with the keys: line,
 *   type, and message; and a sum of the number of messages of each type.
 *   The line key will FALSE if there is no line associated with the message.
 *
 *   @code
 *     $result = array(
 *       '#total' => array(1, 0, 0),
 *       'file1.php' => array(
 *         '#total' => array(1, 0, 0),
 *         array(
 *           'line' => 17, // number or FALSE
 *           'type' => 'major', // major|normal|minor
 *           'message' => 'This file is terrible.'
 *         ),
 *       ),
 *     );
 *   @endcode
 *
 * @see worker_perform()
 */
function coder_perform(array $properties) {
  // Assemble list of relevant files and append the path reletive to
  // DRUPAL_ROOT to the JOB_ROOT since coder will be executed from the host.
  worker_log('Assemble list of relevant files.');
  $files = worker_file_list($properties);
  $path = str_replace(DRUPAL_ROOT . '/', '', JOB_ROOT);
  array_walk($files, 'coder_file_path', $path);

  // If the severity to be recorded is 'normal' then pass nothing to drush.
  $severity = $properties['severity'] == 'normal' ? '' : $properties['severity'] . ' ';

  // Build the common part of the drush coder_review command and determine the
  // file in which the results will be logged.
  $command = 'drush coder-review ' . $severity . implode(' ', $properties['reviews']) . ' ';
  $log_file = JOB_ROOT . '/coder_review.' . mt_rand(100, 10000);

  // Split the files into chunks that will be processed separately and build
  // the complete drush command.
  $i = 1;
  $chunks = array_chunk($files, CODER_CHUNK);
  $commands = array();
  foreach ($chunks as $chunk) {
    $commands['batch ' . $i++] = $command . implode(' ', $chunk) . ' >> ' . $log_file;
  }

  // Execute the set of drush coder_review commands.
  $log = worker_execute_concurrent($commands, 100);

  // Parse the result log file and collect the code review messages.
  $map = array_flip(array('critical', 'normal', 'minor'));
  $results = array('#total' => array(0, 0, 0));
  if (file_exists($log_file) && ($lines = file($log_file))) {
    $length = strlen($path);
    $file = FALSE;
    foreach ($lines as $line) {
      // Relevant lines either match the format
      // '+line_number: [severity] message' where the line_number is optional or
      // provide a file reference that starts with the $path determined above.
      if (preg_match('/(\+(?P<line>-?\d+): )?\[(?P<severity>\w+)\] (?P<message>.*?)$/m', $line, $match)) {
        // Add message to overall total and file total.
        $type = $map[$match['severity']];
        $results['#total'][$type]++;
        $results[$file]['#total'][$type]++;

        $results[$file][] = array(
          'line' => empty($match['line']) ? FALSE : $match['line'],
          'type' => $match['severity'],
          'message' => $match['message'],
        );

        // If the number of messages has reached the max then stop.
        if (array_sum($results['#total']) >= CODER_MAX) {
          worker_log('Reached maximum number (' . number_format(CODER_MAX) . ') of messages.');
          break;
        }
      }
      elseif (substr($line, 0, $length) == $path) {
        // Extract the file name and make the path relative to the
        // $properties['path'] by replacing that portion.
        $file = coder_file_path_result(substr($line, $length + 1, -2), $properties['path']);
        $results[$file] = array('#total' => array(0, 0, 0));
      }
    }
  }

  if ($properties['annotate']) {
    // @TODO Implement annotate functionality (use concurrency).
  }

  // If there are no critical or normal messages then codered a pass.
  return array($results['#total'][0] + $results['#total'][1] == 0, $results);
}

/**
 * Append the path to the file.
 *
 * @see array_walk()
 */
function coder_file_path(&$file, $key, $path) {
  $file = $path . '/' . $file;
}

/**
 * Make a file path relative to a set of paths.
 *
 * @param $file
 *   A file path to make relative.
 * @param $paths
 *   An array of paths to remove from the path.
 * @return
 *   The relative version of the file path.
 */
function coder_file_path_result($file, array $paths) {
  // Loop over each of the paths and attempt to remove them from the front of
  // the file path. If the path was removed the stop processing and return the
  // altered path.
  $count = 0;
  foreach ($paths as $path) {
    $file = ltrim(preg_replace("|^$path|m", '', $file, 1, $count), '/');
    if ($count) break;
  }
  return $file;
}
