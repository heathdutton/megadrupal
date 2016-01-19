<?php
/**
 * @file
 * cash_flow_expenses.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function cash_flow_expenses_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'payments_for_expense';
  $context->description = '';
  $context->tag = 'expense';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'expense' => 'expense',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-expense_node_shortcuts' => array(
          'module' => 'boxes',
          'delta' => 'expense_node_shortcuts',
          'region' => 'content',
          'weight' => '-10',
        ),
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-9',
        ),
        'views-payments-block_1' => array(
          'module' => 'views',
          'delta' => 'payments-block_1',
          'region' => 'content',
          'weight' => '-8',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('expense');
  $export['payments_for_expense'] = $context;

  return $export;
}
