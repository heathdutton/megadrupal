<?php
/**
 * @file
 * deploy_addon.deploy_plans.inc
 */

/**
 * Implements hook_deploy_plans_default().
 */
function deploy_addon_next_deploy_plans_default() {
  $export = array();

  $plan = new DeployPlan();
  $plan->disabled = FALSE; /* Edit this to true to make a default plan disabled initially */
  $plan->api_version = 1;
  $plan->name = 'push_to_next';
  $plan->title = 'Push to Next';
  $plan->description = 'This is a default plan called "Push to Next".';
  $plan->debug = variable_get('deploy_addon_debug');
  $plan->aggregator_plugin = 'DeployAggregatorManaged';
  $plan->aggregator_config = array(
    'delete_post_deploy' => 0,
  );
  $plan->fetch_only = 0;
  $plan->processor_plugin = 'DeployProcessorMemory';
  $plan->processor_config = array();
  $plan->endpoints = array(
    'next' => 'next',
  );
  $export['push_to_next'] = $plan;

  return $export;
}
