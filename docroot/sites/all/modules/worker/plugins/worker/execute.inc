<?php
/**
 * @file
 * Provide command execution job type worker plugin.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('Execute'),
  'description' => t('Provides basic command execution.'),
  'perform' => 'worker_execute_perform',
);

/**
 * Perform the job.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   An array containing a boolean for pass/fail and the result. The result is
 *   the result of the command that was executed.
 */
function worker_execute_perform(array $properties) {
  worker_chdir();
  worker_log('Execute: `' . $properties['command'] . '`.');
  $return = worker_execute_output(escapeshellcmd($properties['command']) . ' < /dev/null');
  chdir(DRUPAL_ROOT);
  return array(TRUE, implode("\n", $return));
}
