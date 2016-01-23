<?php
/**
 * @file
 * Node Tab rules integration.
 */

/**
 * Implements hook_rules_condition_info().
 */
function node_tab_rules_condition_info() {
  $conditions['node_tab_rules_condition'] = array(
    'label' => t('Content page has tabs'),
    'group' => t('Node tab'),
    'callbacks' => array(
      'execute' => 'node_tab_rules_condition',
    ),
  );
  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function node_tab_rules_action_info() {
  $actions['node_tab_redirect'] = array(
    'label' => t('Redirect to default node tab'),
    'parameter' => array(),
    'group' => t('Node tab'),
    'callbacks' => array(
      'execute' => 'node_tab_rules_redirect',
    ),
  );
  return $actions;
}

/**
 * Rules condition: checks whether a node page is being displayed,
 * and whether it has tabs.
 */
function node_tab_rules_condition() {
  $item = menu_get_item();
  // Only return TRUE if there is no argument after node/%.
  // This prevents a redirect loop in case there's no node/%/% page yet.
  if ($item['path'] == 'node/%' && !arg(2)) {
    // Get the nid from the url and load the node manually in order to support
    // both Panels pages and regular node paths.
    $nid = arg(1);
    $node = node_load($nid);
    if ($node && node_tab_type_has_tabs($node->type)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Rules action: Redirects to the default tab of a node.
 */
function node_tab_rules_redirect() {
  // Get the nid from the url and load the node manually in order to support
  // both Panels pages and regular node paths.
  $nid = arg(1);
  $node = node_load($nid);
  $default_tab = node_tab_load_default($node->type);
  if ($default_tab) {
    $path = node_tab_get_node_path($node);
    $options = array(
      'query' => drupal_get_query_parameters(),
    );
    drupal_goto($path . '/' . $default_tab->name, $options, 301);
  }
}
