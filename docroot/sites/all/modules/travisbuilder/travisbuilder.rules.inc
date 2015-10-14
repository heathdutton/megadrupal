<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function travisbuilder_rules_action_info() {
  return array(
    'travisbuilder_action_trigger_build' => array(
      'label' => t('Trigger a Travis build'),
      'group' => t('System'),
      'parameter' => array(
        'ignore_lifetime' => array(
          'type' => 'boolean',
          'label' => t('Ignore lifetime'),
          'description' => t('Ignore the recent build minimum lifetime when triggering Travis from this rule action.'),
          'restriction' => 'input',
        ),
        'branch' => array(
          'type' => 'text',
          'label' => t('Branch'),
          'description' => t('Travis trigger this branch on the configured repository (just one).'),
          'restriction' => 'input',
          'sanitize' => TRUE,
          'optional' => TRUE,
        ),
        // @todo Branch param.
      ),
      'base' => 'travisbuilder_action_trigger_build',
    ),
  );
}


/**
 * Trigger a Travis build via Rules.
 *
 * @param boolean $ignore_lifetime
 *   Whether to ignore the minimum lifetime.
 * @param string $branch
 *   Name of the repository branch to act upon.
 */
function travisbuilder_action_trigger_build($ignore_lifetime, $branch) {
  $configs = _travisbuilder_configs();
  $branch = ($branch) ? $branch : $configs['branch'];

  _travisbuilder_trigger_build($branch, $ignore_lifetime);
}
