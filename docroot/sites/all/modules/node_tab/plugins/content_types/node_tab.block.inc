<?php

/**
 * @file
 * Plugin to provide a content_type that displays a list of all available tabs.
 */

$plugin = array(
  'title' => t('Available tabs'),
  'description' => t('Shows all tabs that the user has access to.'),
  // 'single' means not to be subtyped.
  'single' => TRUE,
  'icon' => 'icon_page.png',
  'render callback' => 'node_tab_panels_manage_render',
  'category' => t('Node Tab'),
  'render last' => TRUE,
  'required context' => array(
    new ctools_context_required(t('Node Tab'), 'entity:node_tab'),
    new ctools_context_required(t('Node'), 'entity:node'),
  ),
);

/**
 * Outputs the tab's list.
 */
function node_tab_panels_manage_render($subtype, $conf, $panel_args, $context) {
  $context_tab = $context[0]->data;
  $context_node = $context[1]->data;
  $tabs = node_tab_load_multiple($context_node->type);
  if (!$tabs) {
    return FALSE;
  }
  $default_tab = reset($tabs);
  $path = node_tab_get_node_path($context_node);
  $path = url($path, array('absolute' => TRUE));

  $theme_tabs = array();
  foreach ($tabs as $tab) {
    if (node_tab_access('view', $tab, NULL, NULL, $context_node)) {
      $theme_tabs[] = array(
        'path' => $path . '/' . $tab->name,
        'label' => $tab->getTranslation('label'),
        'active' => $tab->name == $context_tab->name,
      );
    }
  }
  $theme_args = array(
    'tabs' => $theme_tabs,
  );
  $block = new stdClass();
  $block->content = theme('node_tabs', $theme_args);
  return $block;
}
