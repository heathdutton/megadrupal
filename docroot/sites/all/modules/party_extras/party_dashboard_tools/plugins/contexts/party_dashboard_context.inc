<?php

/**
 * @file
 * Plugin to provide a ctools context for Party Dashboard .
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Party Dashboard Context"),
  'description' => t('Context for Party Dashboard.'),
  'context' => 'ctools_context_create_party_dashboard_context',
  'keyword' => 'party_dashboard_context',
  'context name' => 'party_dashboard_context',
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function ctools_context_create_party_dashboard_context($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('party_dashboard_context');
  $context->plugin = 'party_dashboard_context';

  if ($empty) {
    return $context;
  }

  // Statically cache this so we don't have to do it multiple times.
  $context_value = &drupal_static(__FUNCTION__, NULL);

  if (!isset($context_value)) {
    if ($conf) {
      // @TODO: Allow a custom set context through the UI.
    }
    else {
      // This is coming programmattically.
      $context_value = $data;
      if (!isset($context_value)) {
        // Retrieve from the session.
        $context_value = isset($_SESSION['party_dashboard']['context']) ? $_SESSION['party_dashboard']['context']: 'default';
      }
    }

    // Validate that it's correct.
    if ($context_value && !in_array($context_value, array_keys(ctools_export_crud_load_all('party_dashboard_context')))) {
      $context_value = NULL;
    }
  }

  // Update our session.
  if (!$context_value) {
    $context_value = 'general';
  }

  $_SESSION['party_dashboard']['context'] = $context_value;
  $context->data = $context_value;

  return $context;
}
