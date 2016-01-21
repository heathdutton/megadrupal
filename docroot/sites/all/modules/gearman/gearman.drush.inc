<?php
/**
 * Implements of hook_drush_help().
 */
function gearman_drush_help($section) {
  switch ($section) {
    case 'drush:gearman-work':
      return dt("Start a gearman worker.");
    case 'drush:gearman-job':
      return dt("Queue a gearman task.");
  }
}

/**
 * Implements of hook_drush_command().
 */
function gearman_drush_command() {
  $items['gearman-worker'] = array(
    'callback' => 'gearman_drush_worker',
    'description' => 'Launch a drush instance as a gearman worker.',
    'examples' => array(
      'drush gearman-worker' => 
        'Launch a drush instance as a gearman worker.',
    ),
    'arguments' => array(),
    'options' => array(
      'quiet' => 'Run in quiet (non verbose) mode',
    ),
    'drupal dependencies' => array('gearman'),
    'core' => array(),
  );
  $items['gearman-client'] = array(
    'callback' => 'gearman_drush_client',
    'description' => 'Create a client and add a job to the queue.',
    'examples' => array(
      'drush gearman-task reverse "This is a string to reverse" --background' => 
        'Launch a drush instance as a gearman worker.',
    ),
    'options' => array(
      'background' => 'Queue this task in the background',
    ),
    'arguments' => array(
      'command' => 'The command to use',
      'data' => 'data to be passed in to your command'
    ),
    'drupal dependencies' => array('gearman'),
    'core' => array(),
  );

  return $items;
}

/**
 * General Drush Worker Callback
 *
 * This contains some safeguards against memory leakage.
 */
function gearman_drush_worker() {  
  static $memory_overhead = FALSE;
  if (!$memory_overhead) {
    $memory_overhead = memory_get_usage();
  }
  drush_log(dt("Starting drush worker thread"));
  
  // Create our worker object.
  $gmworker= new GearmanWorker();
  
  // Connect to our server(s)
  $servers = variable_get('gearman_server', 'localhost:4730');
  $gmworker->addServers($servers);
  
  // Pick up gearman functions implemented in other modules.
  $functions = module_invoke_all('gearman_drush_function');
  
  foreach ($functions as $fn) {
    drush_log(dt('Registering !function_name to !function', array('!function_name' => $fn['function_name'], '!function' => $fn['function'])));
    $fn += array(
      'timeout' => 0,
      'context' => NULL,
    );
    $gmworker->addFunction($fn['function_name'], $fn['function'], $fn['context'], $fn['timeout']);
  }

  drush_log(dt("Waiting for job..."), 'ok');
  
  while($gmworker->work())
  {
    $m = memory_get_usage();
    drush_log(dt("Memory used by worker: !delta\nTotal memory usage: !memory\n", array('!memory' => $m, '!delta' => $m - $memory_overhead)));
    if ($gmworker->returnCode() != GEARMAN_SUCCESS)
    {
      drush_log(dt("return_code: " . $gmworker->returnCode()), 'error', $gmworker->error());
      break;
    }
  }
}

/**
 * Generic Client Callback
 *
 * Send the function and data to the queue to be picked up by a worker.
 *
 * Adapt this general pattern to suit your own specific user-case. If you write
 * sufficiently economical code, you can have workers with very long lives.
 */
function gearman_drush_client($function_name, $data = NULL) {  
  // Create our client object.
  $gmclient= new GearmanClient();
  
  // Connect to our server(s)
  $servers = variable_get('gearman_server', 'localhost:4730');
  $gmclient->addServers($servers);

  drush_log(dt("Sending job: $function_name"));
  
  if (drush_get_option('background')) {    
    $job_handle = $gmclient->doBackground($function_name, $data);
  
    if ($gmclient->returnCode() != GEARMAN_SUCCESS) {
      drush_log(dt("Bad return code: !code\n", array('!code' => $gmclient->returnCode())), 'error');
    }
    else {
      drush_log(dt("Job queued as: !handle\n", array('!handle' => $job_handle)), 'success');
    }
  }
  else {
    // Send the job right now
    do {
      $result = $gmclient->do($function_name, $data);
    
      # Check for various return packets and errors.
      switch($gmclient->returnCode()) {
        case GEARMAN_WORK_DATA:
          drush_log("Data: $result", 'success');
          break;
        case GEARMAN_WORK_STATUS:
          list($numerator, $denominator)= $gmclient->doStatus();
          drush_log("Status: $numerator/$denominator complete.", 'success');
          break;
        case GEARMAN_WORK_FAIL:
          drush_log("Failed.", 'error', $gmclient->error());
          exit;
        case GEARMAN_SUCCESS:
          drush_log($result, 'success');
          break;
        default:
          drush_log("RET: " . $gmclient->returnCode(), 'warning');
          exit;
      }
    }
    while($gmclient->returnCode() != GEARMAN_SUCCESS);
  }
}