<?php
/**
 * @file
 * crm_core_donation.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function crm_core_donation_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'cmcd_lybunt_sybunt';
  $context->description = 'Adds simple widgets to forms for selecting date options for reports.';
  $context->tag = 'CRM Core Donations';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'crm-core/reports/cmcd-donors/lybunt' => 'crm-core/reports/cmcd-donors/lybunt',
        'crm-core/reports/cmcd-donors/sybunt' => 'crm-core/reports/cmcd-donors/sybunt',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'crm_core_donation-cmcd_date_year_chooser' => array(
          'module' => 'crm_core_donation',
          'delta' => 'cmcd_date_year_chooser',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds simple widgets to forms for selecting date options for reports.');
  t('CRM Core Donations');
  $export['cmcd_lybunt_sybunt'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'cmcd_source_summary';
  $context->description = 'Displays charts around the CRM Core Donations Source Summary report';
  $context->tag = 'CRM Core Donations';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'crm-core/reports/cmcd/donation-sources' => 'crm-core/reports/cmcd/donation-sources',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'crm_core_donation-cmcd_source_bar_chart' => array(
          'module' => 'crm_core_donation',
          'delta' => 'cmcd_source_bar_chart',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('CRM Core Donations');
  t('Displays charts around the CRM Core Donations Source Summary report');
  $export['cmcd_source_summary'] = $context;

  return $export;
}
