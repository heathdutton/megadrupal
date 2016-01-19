<?php

/**
 * @file
 * Rules integration for the RDFViz module.
 */

/**
 * Publishes that rdfviz processing can trigger rules events.
 *
 * Implements hook_rules_data_info().
 * @ingroup rules
 */
function rdfviz_rules_data_info() {
  return array(
    'rdfviz' => array(
      'label' => t('RDFViz'),
    ),
  );
}

/**
 * Publish the rdf processing events.
 *
 * Makes new events available on /admin/config/workflow/rules/reaction/add .
 *
 * Describe what variables we expect these events to add to us.
 *
 * @see rdfviz_dot()
 *
 * Implements hook_rules_event_info().
 */
function rdfviz_rules_event_info() {
  $items = array();
  // We have two events.
  // rdfviz_subject_render
  // rdfviz_statement_render
  // called with
  // $variables = array('subject' => $subject, 'arc' => $arc);
  return array(
    'rdfviz_subject_render' => array(
      'group' => t('RDFViz'),
      'label' => t('An RDF subject is being rendered'),
      'variables' => array(
        'subject' => array('type' => 'array', 'label' => t('RDF subject')),
        'arc' => array('type' => 'stdClass', 'label' => t('ARC destination object')),
      ),
    ),
    'rdfviz_statement_render' => array(
      'group' => t('RDFViz'),
      'label' => t('An RDF statement is being rendered'),
      'variables' => array(
        'subject' => array('type' => 'array', 'label' => t('RDF subject')),
        'predicate' => array('type' => 'string', 'label' => t('RDF predicate')),
        'object' => array('type' => 'string', 'label' => t('RDF object (array)')),
        'arc' => array('type' => 'stdClass', 'label' => t('ARC destination object')),
      ),
    ),
  );
}

