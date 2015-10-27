<?php
/**
 * @file
 * Builds chart tokens for Webform nodes.
 */

/**
 * Implements hook_token_info().
 */
function webform_charts_token_info() {
  // Webform submission tokens.
  $info['tokens']['node']['webform-charts'] = array(
    'name' => t('Webform charts'),
    'description' => t('Tokens for printing charts for individual components. Replace the "?" with the "field key", including any parent field keys separated by colons.'),
    'dynamic' => TRUE,
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function webform_charts_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  // Webform tokens (caching globally).
  if ($type !== 'node' || empty($data['node']) || empty($data['node']->webform) || empty($data['node']->webform['components'])) {
    return;
  }

  // Prepare all the submission data that we will likely need more than once.
  $node = $data['node'];
  $format = $sanitize ? 'html' : 'text';

  // Replace individual tokens that have an exact replacement.
  foreach ($tokens as $name => $original) {
    foreach ($node->webform['components'] as $cid => $component) {
      // Build the list of parents for this component.
      $parents = ($component['pid'] == 0) ? array($component['form_key']) : webform_component_parent_keys($node, $component);
      $parent_token = 'webform-charts:' . implode(':', $parents);
      if (strpos($name, $parent_token) === 0) {
        if ($chart_renderable = webform_charts_component_chart($node, $component)) {
          $replacements[$original] = drupal_render($chart_renderable);
        }
      }
    }

  }

  return $replacements;
}
