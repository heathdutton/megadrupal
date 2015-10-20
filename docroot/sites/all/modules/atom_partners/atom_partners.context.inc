<?php
/**
 * @file
 * atom_partners.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function atom_partners_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'partner_front';
  $context->description = 'Add partner block to front page';
  $context->tag = 'page';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-partners-block' => array(
          'module' => 'views',
          'delta' => 'partners-block',
          'region' => 'sidebar_first',
          'weight' => '-8',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Add partner block to front page');
  t('page');
  $export['partner_front'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'partners_block';
  $context->description = 'Adds a custom text block to the partner\'s page';
  $context->tag = 'page';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'partner' => 'partner',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'partners' => 'partners',
        'partners/*' => 'partners/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'atom_partners_extra-partner_info' => array(
          'module' => 'atom_partners_extra',
          'delta' => 'partner_info',
          'region' => 'sidebar_first',
          'weight' => '10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds a custom text block to the partner\'s page');
  t('page');
  $export['partners_block'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'partners_menu';
  $context->description = '';
  $context->tag = 'section';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'partner' => 'partner',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'partners' => 'partners',
        'partners/*' => 'partners/*',
      ),
    ),
  );
  $context->reactions = array(
    'breadcrumb' => 'partners',
    'menu' => 'partners',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('section');
  $export['partners_menu'] = $context;

  return $export;
}
