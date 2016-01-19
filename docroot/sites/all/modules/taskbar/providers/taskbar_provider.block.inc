<?php
/**
 * @file
 *   Block provider for Taskbar.
 */

/**
 */
function taskbar_provider_block_definition() {
  return array(
    'name' => 'block',
    'title' => 'Blocks',
    'ajax' => TRUE,
    'realtime' => FALSE,
    'callback' => 'taskbar_provider_block',
    'settings form' => 'taskbar_provider_block_settings',
    'settings form submit' => 'taskbar_provider_block_settings_submit',
    'file' => 'taskbar_provider.block.inc',
    'path' => drupal_get_path('module', 'taskbar') . '/providers',
  );
}

/**
 */
function taskbar_provider_block($item, $action = 'status') {
  $output = array();
  if ($action == 'status') {
    $output['content'] = $item->settings['text'];
  }
  if ($action == 'display' || !$item->settings['ajax']) {
    $block = block_load($item->settings['module'], $item->settings['delta']);
    if (isset($block->bid)) {
      $block_content = _block_render_blocks(array($block));
      $build = _block_get_renderable_array($block_content);
      $output['extra'] = drupal_render($build);
    }
    if (!isset($block->bid) || empty($output['extra'])) {
      // Hide this item if the block is not found or block content is empty
      $output['content'] = FALSE;
    }
  }

  return $output;
}

/**
 * Settings form for the block provider.
 */
function taskbar_provider_block_settings(&$form, &$form_state, $item) {
  $form['provider_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block settings'),
    '#collapsible' => TRUE,
  );

  if (empty($item->settings['module'])) {
    $item->settings['module'] = '';
  }
  if (empty($item->settings['delta'])) {
    $item->settings['delta'] = 0;
  }

  $blocks = array();
  foreach (_block_rehash() as $block) {
    $blocks[$block['module'] . ' ' . $block['delta']] = $block['info'];
  }
  $form['provider_settings']['block'] = array(
    '#type' => 'select',
    '#title' => t('Block'),
    '#description' => t('Select the block you would like to use for this item.'),
    '#required' => TRUE,
    '#default_value' => $item->settings['module'] . ' ' . $item->settings['delta'],
    '#options' => $blocks,
  );
}

/**
 */
function taskbar_provider_block_settings_submit($form, &$form_state, &$item) {
  list($module, $delta) = explode(' ', $form_state['values']['block']);
  $item->settings['module'] = $module;
  $item->settings['delta'] = $delta;
}

