<?php
/*
 * @file
 * Part of Code Server module for Drupal by netgenius.co.uk
 * https://www.drupal.org/project/codeserver
 *
 * This .module is intended to be compatible with D6, D7 and D8.
 * See codeserver_compat.inc for version-specific code.
 */

/**
 * Implement hook_drush_command().
 */
function codeserver_client_drush_command() {

  $items['codeserver-call'] = array(
    'description' => 'Call a PHP function on a remote site via Code Server.',
    'aliases' => array('codeserver-call', 'cscall', 'csc'),
    // Define arguments.
    'arguments' => array(
      'service_id' => 'The id of a Code Server service configuration.',
      'function' => 'The name of a PHP function to call.',
      'arguments' => 'Any arguments to be passed to the called function.'
    ),
    'examples' => array(
      'drush csc live time' => 'Displays value of time() from the live site.',
      'drush csc live microtime 1' => 'Displays the value of microtime(TRUE) from the live site.',
      'drush csc dev variable_get site_name' => 'Displays the site-name of the dev site.',
      'drush csc dev variable_set site_name "Dev Site"' => 'Sets the dev site name to "Dev Site".',
    ),
  );

  $items['codeserver-run'] = array(
    'description' => 'Run PHP code on a remote site via Code Server.',
    'aliases' => array('codeserver-run', 'csrun', 'csr'),
    // Define arguments.
    'arguments' => array(
      'service_id' => 'The id of a Code Server service configuration.',
      'code' => 'The PHP code to run or the special value "file".',
      'file' => 'The path to the PHP file if "file" was specified above.',
    ),
    'examples' => array(
      'drush csr live "return time()"' => 'Displays value of time() from the live site.',
      'drush csr live "return microtime(TRUE)"' => 'Displays the value of microtime(TRUE) from the live site.',
      "drush csr dev \"return variable_get('site_name')\"" => 'Displays the site-name of the dev site.',
      "drush csr dev \"variable_set('site_name', 'Dev Site')\"" => 'Sets the dev site name to "Dev Site".',
      'drush csr dev file test.php' => 'Runs PHP on the dev site from the local test.php file.',
    ),
  );

  return $items;
}

/*
 * Function for the codeserver-call (csc) command.
 */
function drush_codeserver_client_codeserver_call($config_id, $function) {
  $config = codeserver_get_config($config_id);
  if (!$config) {
    drush_codeserver_client_codeserver_call_validate();
    return FALSE;
  }
  $remote = new CodeServer($config_id);
  $args = func_get_args();
  unset($args[0], $args[1]);

  // Call the requested function, passing any parameters given.
  $result = call_user_func_array(array($remote, '__call'), array($function, array_values($args)));
  drupal_set_message( t('Result: !result', array('!result' => print_r($result, TRUE))) );
}

function drush_codeserver_client_codeserver_call_validate($config_id = FALSE, $function = FALSE){
  if (!$config_id || !$function) {
    $tvars = array('!usage' => "drush codeserver-call <config_id> <function> [args ...]");
    drush_set_error(t('Usage: !usage', $tvars));
  }
}

/*
 * Function for the codeserver-run (csr) command.
 */
function drush_codeserver_client_codeserver_run($config_id, $code, $filename = '') {
  $config = codeserver_get_config($config_id);
  if (!$config) {
    drush_codeserver_client_codeserver_run_validate();
    return FALSE;
  }
  $remote = new CodeServer($config_id);
  // Run the given $code. Note if $code is 'file', then $filename is required.
  $result = $remote->run($code, $filename);
  drupal_set_message( t('Result: !result', array('!result' => print_r($result, TRUE))) );
}

function drush_codeserver_client_codeserver_run_validate($config_id = FALSE, $code = FALSE){
  if (!$config_id || !$code) {
    $tvars = array('!usage' => "drush codeserver-run <config_id> <code>");
    drush_set_error(t('Usage: !usage', $tvars));
  }
}
