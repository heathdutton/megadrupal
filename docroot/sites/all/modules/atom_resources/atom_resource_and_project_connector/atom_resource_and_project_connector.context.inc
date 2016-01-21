<?php
/**
 * @file
 * atom_resource_and_project_connector.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function atom_resource_and_project_connector_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'atom_resource_project_sidebar';
  $context->description = 'Adds a resources block to the project page';
  $context->tag = 'type';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'project' => 'project',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'atom_project-atom_project_resources' => array(
          'module' => 'atom_project',
          'delta' => 'atom_project_resources',
          'region' => 'sidebar',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds a resources block to the project page');
  t('type');
  $export['atom_resource_project_sidebar'] = $context;

  return $export;
}
