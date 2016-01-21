<?php

/**
 * @file
 * Admin page callbacks for the path level blocks module.
 *
 * @name Node-level Blocks Admin
 * @author skilip
 */

/**
 * Extra form elements for the node type form.
 */
function node_level_blocks_node_type_form(&$form, &$form_state) {
  $settings = node_level_blocks_node_type_settings($form['#node_type']->type);

  $module_options = array();
  foreach (module_implements('block_info') as $module) {
    $module_options[$module] = $module;
  }

  $form['node_level_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node-level Blocks'),
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
    '#collapsed' => !($settings->enabled),
    '#access' => user_access('administer node_level_blocks'),
    '#tree' => TRUE,
  );
  $form['node_level_blocks']['node_level_blocks_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Node-level Blocks functionality'),
    '#description' => '',
    '#default_value' => $settings->enabled,
  );
  $form['node_level_blocks']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'input[name=node_level_blocks_enabled]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['node_level_blocks']['settings']['node_level_blocks_display_node_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display blocks on node-edit pages'),
    '#default_value' => $settings->display_node_edit,
  );

  if (module_exists('nodeblock') && nodeblock_type_enabled($form['#node_type']->type)) {
    $form['node_level_blocks']['settings']['node_level_blocks_nodeblock'] = array(
      '#type' => 'checkbox',
      '#title' => t('Node Block integration'),
      '#description' => t('When a Node Block is used in the Node Level Blocks configuration, check the Node Blocks for Node Level Blocks as well.'),
      '#default_value' => isset($settings->nodeblock) ? $settings->nodeblock : FALSE,
    );
  }
  $form['node_level_blocks']['settings']['node_level_blocks_multi'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow duplication of blocks on node pages'),
    '#default_value' => $settings->multi,
  );
  $form['node_level_blocks']['settings']['node_level_blocks_regions'] = array(
    '#type' => 'select',
    '#title' => t('Enabled regions'),
    '#description' => t('Choose which regions are available for the current content type'),
    '#options' => system_region_list(variable_get('theme_default', 'garland')),
    '#multiple' => TRUE,
    '#default_value' => $settings->regions,
    '#size' => 10,
  );
  $form['node_level_blocks']['settings']['node_level_blocks_modules'] = array(
    '#type' => 'select',
    '#title' => t('Modules'),
    '#description' => t('Here you can choose which modules to use for providing block contents.'),
    '#options' => $module_options,
    '#multiple' => TRUE,
    '#default_value' => ($settings->modules ? $settings->modules : $module_options),
    '#size' => 10,
  );

  array_unshift($form['#submit'], 'node_level_blocks_node_type_form_submit');
  $form['#attached']['js'][] = drupal_get_path('module', 'node_level_blocks') . '/js/node_level_blocks.js';
}

/**
 * Custom submit callback for the node type form.
 */
function node_level_blocks_node_type_form_submit($form, &$form_state) {
  $settings = (object) array(
    'enabled' => $form_state['values']['node_level_blocks']['node_level_blocks_enabled'],
    'display_node_edit' => $form_state['values']['node_level_blocks']['settings']['node_level_blocks_display_node_edit'],
    'multi' => $form_state['values']['node_level_blocks']['settings']['node_level_blocks_multi'],
    'regions' => $form_state['values']['node_level_blocks']['settings']['node_level_blocks_regions'],
    'modules' => $form_state['values']['node_level_blocks']['settings']['node_level_blocks_modules'],
  );

  if (isset($form_state['values']['node_level_blocks']['settings']['node_level_blocks_nodeblock'])) {
    $settings->nodeblock = $form_state['values']['node_level_blocks']['settings']['node_level_blocks_nodeblock'];
  }

  variable_set('node_level_blocks_enabled_' . $form_state['values']['type'], $settings);
  // Unset to make sure no variables are made.
  unset($form_state['values']['node_level_blocks']);
}

/**
 * Extra form elements for the node type form.
 */
function node_level_blocks_fieldset_node_edit_form(&$form, &$form_state) {
  $node = $form['#node'];

  // Set default values.
  $settings = isset($node->node_level_blocks['settings']) ? $node->node_level_blocks['settings'] : node_level_blocks_node_type_settings($node->type);

  $submitted = $form_state['submitted'] && isset($form_state['values']['node_level_blocks']);
  $node_blocks = !$submitted && isset($node->node_level_blocks['blocks']) ? $node->node_level_blocks['blocks'] : array();

  // If a node is submitted with the 'preview' button,
  // the data can be collected using the form state.
  // Othewise the stored nodes, or default values can be used.
  if ($submitted) {
    if (!isset($form_state['values']['node_level_blocks']['settings'])) {
      $form_state['values']['node_level_blocks']['settings'] = $settings;
    }

    foreach ($form_state['values']['node_level_blocks']['blocks'] as $node_block) {
      $node_blocks[$node_block['bid']] = (object) $node_block;
    }
  }

  $context = array('node' => $node);

  $form['node_level_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node-level Blocks'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#group' => 'additional_settings',
    '#collapsed' => !($settings->enabled),
    '#access' => ($settings->enabled && user_access('edit node_level_blocks')),
  );

  node_level_blocks_block_overview($form['node_level_blocks'], $node_blocks, $settings, $context);
}

/**
 * Process variables for block-admin-display.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see block-admin-display.tpl.php
 * @see theme_block_admin_display()
 */
function template_preprocess_node_level_blocks_fieldset(&$variables) {
  // set default theme for retrieving regions and blocks
  $theme_key = variable_get('theme_default', 'garland');

  $settings = $variables['form']['#settings'];

  $block_regions = system_region_list($theme_key);

  $variables['block_regions'] = $block_regions + array(BLOCK_REGION_NONE => t('Disabled'));

  foreach (array_keys($block_regions) as $key) {
    // Initialize an empty array for the region.
    if (in_array($key, $settings->regions)) {
      $variables['block_listing'][$key] = array();
    }
    else {
      unset($variables['block_regions'][$key]);
    }
  }

  // Initialize disabled blocks array.
  $variables['block_listing'][BLOCK_REGION_NONE] = array();

  foreach (element_children($variables['form']) as $i) {
    $block = &$variables['form'][$i];

    // Only take form elements that are blocks.
    if (isset($block['info'])) {
      // Fetch region for current block.
      $region = $block['region']['#default_value'];

      // Set special classes needed for table drag and drop.
      $variables['form'][$i]['region']['#attributes']['class'] = array('block-region-select block-region-' . $region);
      $variables['form'][$i]['weight']['#attributes']['class'] = array('block-weight block-weight-' . $region);

      $variables['block_listing'][$region][$i] = new stdClass();
      $variables['block_listing'][$region][$i]->row_class = !empty($block['#attributes']['class']) ? implode(' ', $block['#attributes']['class']) : '';
      $variables['block_listing'][$region][$i]->block_modified = !empty($block['#attributes']['class']) && in_array('block-modified', $block['#attributes']['class']);
      $variables['block_listing'][$region][$i]->block_title = drupal_render($block['info']);
      $variables['block_listing'][$region][$i]->region_select = drupal_render($block['region']) . drupal_render($block['theme']);
      $variables['block_listing'][$region][$i]->weight_select = drupal_render($block['weight']);
      $variables['block_listing'][$region][$i]->printed = FALSE;
    }
  }
}
