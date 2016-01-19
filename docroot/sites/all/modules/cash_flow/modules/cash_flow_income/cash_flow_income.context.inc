<?php
/**
 * @file
 * cash_flow_income.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function cash_flow_income_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'payments_from_income';
  $context->description = '';
  $context->tag = 'income';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'income' => 'income',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-income_node_shortcuts' => array(
          'module' => 'boxes',
          'delta' => 'income_node_shortcuts',
          'region' => 'content',
          'weight' => '-10',
        ),
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-9',
        ),
        'views-payments-block' => array(
          'module' => 'views',
          'delta' => 'payments-block',
          'region' => 'content',
          'weight' => '-8',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('income');
  $export['payments_from_income'] = $context;

  return $export;
}
