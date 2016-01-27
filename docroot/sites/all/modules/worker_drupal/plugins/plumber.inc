<?php
/**
 * @file
 * Provide Drupal plumber test runner plugin.
 *
 * - install
 * - run tests
 * - code coverage (later)
 *
 * The following Drupal variables are expected to be set as shown.
 * - simpletest_clear_results: 0
 * - simpletest_verbose: 0
 *
 * As noted in the code comment found in plumber_results() the 'context'
 * property must be greater than PLUMBER_INCREMENT.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * The maximum number of assertions to process at one time.
 */
define('PLUMBER_INCREMENT', 1000);

/**
 * The maximum number of relevant assertions.
 */
define('PLUMBER_MAX', 1000);

$plugin = array(
  'title' => t('Drupal plumber'),
  'description' => t('Provide Drupal plumber test runner plugin.'),
  'perform' => 'plumber_perform',
);

/**
 * Perform the job.
 *
 * @param $properties
 *   Associative array of properties applicable to the current job.
 *   Supported keys and values are:
 *   - database: The type of database to connect to. A valid plugin with the
 *       same name should exist in the worker/plugins/database directory.
 *   - version: The Drupal primary version number.
 *   - plugin: The name of the plugin to load. A valid plugin with the
 *       same name should exist in the worker-drupal/plugins directory.
 *
 *     Plumber parameters.
 *   - variables: Associative array of Drupal variables to set.
 *   - classes: Array of test class names to run tests on.
 *   - statuses: Array of relevant assertion statuses to store.
 *       Example: array('fail', 'exception', 'debug')
 *       Default: ?
 *   - context: Integer indicating the number of assertions surrounding a
 *      "relevant" assertion to return. Relevant assertions are defined by the
 *      "statuses" key.
 *
 * @return
 *   An array containing a boolean for pass/fail and the result. The result
 *   is a string message or an associative array with the following keys:
 *   log, see worker_execute_concurrent(); results, see plumber_results(); meta,
 *   see plumber_meta(); and coverage, see plumber_coverage().
 *
 * @see worker_perform()
 */
function plumber_perform(array $properties) {
  // Load common Drupal conduit functions.
  require_once drupal_get_path('module', 'worker_drupal') . '/worker_drupal.inc';

  // Set the SimpleTest flags to ensure the results are available and verbose
  // files are not needlessly generated.
  $properties['variables']['simpletest_clear_results'] = FALSE;
  $properties['variables']['simpletest_verbose'] = FALSE;

  // If coverage is enabled then filter the files based on the file list.
  if ($properties['coverage']) {
    $properties['variables']['code_coverage_filter_files'] = worker_file_list($properties);
  }

  if (!worker_drupal_install($properties)) {
    return array(FALSE, worker_log_last());
  }

  if (!worker_drupal_module_enable('simpletest', $properties['version'])) {
    return array(FALSE, worker_log_last());
  }

  // Coverage requires the coverage module to be enabled.
  if ($properties['coverage'] && !worker_drupal_module_enable('code_coverage', $properties['version'])) {
    return array(FALSE, worker_log_last());
  }

  if (!($log = plumber_plumb($properties))) {
    // Unrecoverable error related to configuration occurred.
    return array(FALSE, worker_log_last());
  }

  // Process the results of the test and parse the test meta data.
  $results = plumber_results($properties);
  $meta = plumber_meta($properties);

  // Collect code coverage data if requested.
  if ($properties['coverage']) {
    $coverage = plumber_coverage($properties);
  }

  // If there are no fails or exceptions then consider a pass.
  return array($results['#total'][1] + $results['#total'][2] == 0, array(
    'log' => $log,
    'result' => $results,
    'meta' => $meta,
    'coverage' => (isset($coverage) ? $coverage : FALSE),
  ));
}

/**
 * Plumb the plumbing (ie. run tests).
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   Associative array keyed by test class name and containing the length of
 *   time, in seconds, that the task took to complete, otherwise FALSE.
 * @see worker_execute_concurrent()
 */
function plumber_plumb(array $properties) {
  // Generate list of commands to run all test cases.
  $commands = array();
  $command = plumber_script() . ' --class ';
  foreach ($properties['classes'] as $class) {
    $commands[$class] = $command . $class;
  }

  // Run tests using manual concurrency.
  worker_log('Run ' . number_format(count($properties['classes'])) . ' test case(s) in ' . number_format(count($commands)) . ' batch(es).');
  worker_chdir();
  $log = worker_execute_concurrent($commands);
  chdir(DRUPAL_ROOT);

  return $log;
}

/**
 * Process the results of running the tests.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   Multi-dimentional, associative array of plumber results keyed by the
 *   test class and containing an array of assertions with the keys: line,
 *   type, message, function, group, and file; and a sum of the number of
 *   assertions of each type. The line key will FALSE if there is no line
 *   associated with the message.
 *
 *   @code
 *     $result = array(
 *       '#total' => array(1, 0, 0),
 *       'TestClass' => array(
 *         '#total' => array(1, 0, 0),
 *         array(
 *           'line' => 17, // number or FALSE
 *           'type' => 'pass', // pass|fail|exception|debug
 *           'message' => 'Massive fail'
 *           'function' => 'TestClass->foo()',
 *           'group' => 'Some Group',
 *           'file' => 'file.php',
 *         ),
 *       ),
 *     );
 *   @endcode
 */
function plumber_results(array $properties) {
  worker_log('Process results.');

  // Get the worker database so the results can be queried.
  $connection = worker_database('drupal', $properties['database']);

  // Select a summary of the results of each test class.
  $query = $connection->select('simpletest', 's');
  $query->groupBy($query->addField('s', 'test_class'));
  $query->groupBy($query->addField('s', 'status'));
  $query->addExpression('COUNT(message_id)', 'count');
  $result = $query->execute();

  // Map of message type (status) to #total key.
  $map = array_flip(array('pass', 'fail', 'exception', 'debug'));

  // Build an associative array of summaries keyed by the test class and
  // containing the count of each status type found in the results for the test
  // class. Also compile a list of classes that have status types that are
  // found in the 'statues' property.
  $results = array('#total' => array(0, 0, 0, 0));
  $classes = array();
  while ($summary = $result->fetchAssoc()) {
    if (empty($results[$summary['test_class']])) {
      $results[$summary['test_class']]['#total'] = array(0, 0, 0, 0);
    }
    $type = $map[$summary['status']];
    $results['#total'][$type] += $summary['count'];
    $results[$summary['test_class']]['#total'][$type] = $summary['count'];

    // If there are relevant assertions related to the test class then add the
    // class to the list of classes for  later processing when the actual
    // assertions are loaded and check for relevancy.
    if (in_array($summary['status'], $properties['statuses'])) {
      $classes[$summary['test_class']] = $summary['test_class'];
    }
  }
  worker_log('> ' . number_format(count($classes)) . ' relevant class(es) found.');

  // Calculate the context length ahead of time since it is static. The length
  // includes the amount of context on either side of the relevant assertion.
  $context_length = $properties['context'] * 2 + 1;

  // Cycle through the classes that are known to have assertions with statuses
  // that are to be included in the results.
  $total = 0;
  foreach ($classes as $class) {
    // Clear out the message_id which is used to loop over assertions for a
    // test class to ensure that memory usage stays minimal. Also reset the
    // assertions array so the loop is not confused into thinking it is context
    // from the previous cycle within a test class.
    $message_id = 0;
    $assertions = array();
    do {
      // If there are assertions then this is not the first iteration of the
      // do-while loop so we need to keep enough assertions so there is context
      // in case the first few assertions need to be reported.
      if ($assertions) {
        // Use the message_id from the last assertion processed when making
        // another loop interation.
        $message_id = $assertion['message_id'];

        // In case of test cases or loop interations with very few assertions
        // the count may be lower then the context amount in which case the
        // result would be incorrect.
        $context = min($count, $properties['context']);
        $assertions = array_slice($assertions, -1 * $context, $context, TRUE);
      }

      // Select the next set of assertions.
      $assertions += $connection->select('simpletest')
        ->fields('simpletest')
        ->condition('message_id', $message_id, '>')
        ->condition('test_class', $class)
        ->orderBy('message_id')
        ->range(0, PLUMBER_INCREMENT)
        ->execute()
        ->fetchAllAssoc('message_id', PDO::FETCH_ASSOC);

      // Cycle through each assertion looking for assertions that should be
      // reported and grab the relevant context around the assertion.
      $i = 0;
      // @todo Isn't this code reiterating over assertions previously processed?
      foreach ($assertions as $assertion) {
        if (in_array($assertion['status'], $properties['statuses'])) {
          // Since the $assertions array is keyed by message_id adding an
          // assertion multiple times will not result in duplicate entries. In
          // case the assertion is at the beginning of the list we need to
          // ensure the offset parameter to array_splice() is non-negative.
          $length = $properties['context'] > $i ? $context_length - ($properties['context'] - $i) : $context_length;
          $slice = array_slice($assertions, max($i - $properties['context'], 0), $length, TRUE);
          $results[$class] = array_merge($results[$class], $slice);

          // If the number of relevant assertions has gone over the maximum
          // number allowed then break out of the processing loops.
          if ($total + count($results[$class]) - 1 >= PLUMBER_MAX) {
            worker_log('Reached maximum number (' . number_format(PLUMBER_MAX) . ') of assertions.');
            break 3;
          }
        }
        $i++;
      }

      // Store the number of assertions processed in the loop since the value
      // may be used multiple times.
      $count = count($assertions);
    }
    // If the number of assertions returned is the same as or greater than the
    // increment amount, due to context from previous loop interation, then
    // there is a possibility for more assertions to exist. This assumes the
    // increment amount is greater then the context.
    while ($count >= PLUMBER_INCREMENT);

    // Clean results for the class.
    plumber_results_clean($results[$class]);

    // Add the number of results for the current class to the total minus one
    // for the #total entry.
    $total += count($results[$class]) - 1;
  }
  worker_log('> ' . number_format($total) . ' relevant assertions found.');

  // Return the summaries as the results are not guaranteed to contain, and most
  // likely will not contain, all the assertions for all the tests.
  return $results;
}

/**
 * Clean results for a single class.
 *
 * @param $results
 *   Array of results.
 */
function plumber_results_clean(&$results) {
  foreach ($results as $key => &$result) {
    // Ignore #total key.
    if (is_numeric($key)) {
      // Rename keys and make file path relative to job root.
      $result['type'] = $result['status'];
      $result['group'] = $result['message_group'];
      $result['file'] = basename($result['file']);

      // Remove unused and renamed keys.
      unset($result['message_id']);
      unset($result['test_id']);
      unset($result['test_class']);
      unset($result['status']);
      unset($result['message_group']);
    }
  }
}

/**
 * Obtain list of test meta data.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   An associative array keyed by test class and containing keys 'name' and
 *   'group' as specified in each tests getInfo().
 */
function plumber_meta(array $properties) {
  worker_log('Obtain test meta data.');

  // Cycle through all the lines in --list output and parse the test meta data
  // in the following format.
  // Group
  //  - Some test name (TestCaseClassName)
  //  - Another test (AnotherClass)
  worker_chdir();
  $lines = worker_execute_output(plumber_script() . ' --list');
  chdir(DRUPAL_ROOT);

  $group = 'Unknown';
  $info = array();
  foreach ($lines as $line) {
    // Check to see if line contains test information.
    if (!empty($line[1]) && $line[1] == '-') {
      // Attempt to parse line of test information and only bother to record it
      // if the meta data is for a relevant test class.
      if (preg_match('/^ - (?P<name>.*?) \((?P<class>\w+)\)$/m', $line, $match) && in_array($match['class'], $properties['classes'])) {
        $info[$match['class']] = array(
          'name' => $match['name'],
          'group' => $group,
        );
      }
    }
    else {
      // If line does not have test information then it can be assumed to have
      // group information even though first few lines of output will hit this
      // case since all test lines come after a valid group.
      $group = $line;
    }
  }
  worker_log('> Found meta data for ' . number_format(count($info)) . ' test class(es).');
  return $info;
}

/**
 * Generate the common part to the run-tests.sh command.
 *
 * @return
 *   Common part, or prefix, to the run-tests.sh command.
 */
function plumber_script() {
  if (!($php = worker_php_path())) {
    return FALSE;
  }

  // Escape arguments.
  $url = escapeshellarg(JOB_URL);
  $php = escapeshellarg($php);

  // Generate required (and general) portion of run-tests.sh command.
  return "php scripts/run-tests.sh --php $php --url $url";
}

/**
 * Export code coverage data.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   An associative array containing arrays of file information each with the
 *   following set of keys:
 *     - path: the path to the file relative to the JOB_ROOT.
 *     - executed: the number of line within the file that were executed.
 *     - executable: the number of line within the file that are executable.
 *     - data: a map of line number to TRUE for executed otherwise FALSE for
 *         executable.
 *     - file: (if chunk 0) contents of the file encoded in base64.
 */
function plumber_coverage(array $properties) {
  worker_chdir();

  worker_log('Export code coverage data.');

  // Process coverage set 1 since the database will always start fresh, export
  // the processed report, and load the JSON encoded coverage data.
  worker_execute('drush code-coverage-process 1');
  $directory = reset(worker_execute('drush code-coverage-export 1', TRUE));
  $coverage = json_decode(file_get_contents($directory . '/coverage.json'), TRUE);

  worker_log('> Code coverage data found for ' . number_format(count($coverage)) . ' files(s).');

  // Since only one copy of the file contents is needed only include if chunk 0.
  worker_log('Process relevant files not included in coverage data.');

  // Keep a list of the paths found in the original coverage report.
  $paths = array();
  foreach ($coverage as $key => $file) {
    $paths[] = $file['path'];
  }

  // @TODO Limit the maximum number of files.
  // Determine the relevant list of files and the number of files to be
  // processed per chunk. Use ceiling to ensure that all the files are included.
  $list = worker_file_list($properties);
  $total = count($list);
  $count = ceil($total / $properties['item']->data['count']);

  // Slice the list of files to provide the set relevant to this chunk.
  $list = array_slice($list, $count * $properties['item']->data['delta'], $count);
  worker_log('> ' . number_format(count($list)) . ' of ' . number_format($total) . ' files to process during this chunk.');

  // Generate commands to determine the number of executable lines for files not
  // already covered in $coverage.
  $log_file = JOB_ROOT . '/plumber_coverage.' . sha1(drupal_random_bytes(64));
  $commands = array();
  foreach ($list as $path) {
    if (!in_array($path, $paths)) {
      $commands[$path] = 'drush code-coverage-executable --full ' . escapeshellarg($path) . ' >> ' . $log_file;
    }
  }
  worker_log('> ' . number_format(count($commands)) . ' additional relevant files found.');

  // Determine code coverage for additional files with a 1/10 of second sleep.
  worker_execute_concurrent($commands, 1000000 / 10);

  // Load the log file, decode the JSON coverage data, and add the additional
  // data to the coverage report.
  if (file_exists($log_file) && ($lines = file($log_file))) {
    foreach ($lines as $line) {
      if ($result = json_decode($line, TRUE)) {
        $coverage[] = $result;
      }
    }
  }

  // Encode file data for files from the $list slice since each chunk will
  // include the file data for its slice.
  worker_log('Encode file data for all relevant files.');
  foreach ($coverage as &$file) {
    if (in_array($file['path'], $list)) {
      $file['file'] = base64_encode(file_get_contents($file['path']));
    }
  }

  chdir(DRUPAL_ROOT);
  return $coverage;
}
