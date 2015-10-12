<?php

/**
 * @file
 *
 * Plugin to provide an argument handler for party dashboard context.
 */
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Party Dashboard Context'),
  'description' => t('Context for Party Dashboard.'),
  'context' => 'ctools_context_party_dashboard_context',
  'keyword' => 'party_dashboard_context',
);

/**
 * Discover if this argument gives us the term we crave.
 */
function ctools_context_party_dashboard_context($arg = NULL, $conf = NULL, $empty = FALSE) {

  $context = ctools_context_create('party_dashboard_context', $arg);
  $context->original_argument = $arg;

  return $context;
}
