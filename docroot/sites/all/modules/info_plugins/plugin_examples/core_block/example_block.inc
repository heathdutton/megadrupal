<?php

$plugin = array(
  'title' => t('Example block'),
  // Whether or not the block should show the title.
  'show title' => FALSE,
  // Your own callback for building the display of the block (called by `hook_block_view()`)
  // Leaving this empty, the function name would default to `my_module_example_block_view()`
  'view' => 'my_module_render_example_block',
  // Your own callback for building the config form for the block (called by `hook_block_configure()`)
  // Leaving this empty, the function name would default to `my_module_example_block_configure()`
  'configure' => 'my_module_configure_example_block',
  // Your own callback for saving the config for the block (called by `hook_block_saved()`)
  // Leaving this empty, the function name would default to `my_module_example_block_save()`
  'save' => 'my_module_save_example_block',
  // ... all other options you may want to pass to `hook_block_info()`
);

/**
 * Returns a renderable array that represents the block content, i.e. the same
 * as you would return from `hook_block_view()`.
 *
 * @param $delta
 *   The block delta
 */
function my_module_render_example_block($delta) {
  return array(
    '#type' => 'markup',
    '#markup' => 'My custom example block'
  );
}

/**
 * Returns a form to be used as the block configuration form, i.e. the same
 * as you would return from `hook_block_configure()`
 *
 * @param $delta
 *   The block delta
 */
function my_module_configure_example_block($delta) {
  $form = array();
  $form['my_custom_field'] = array(
    '#type' => 'textfield',
    '#title' => t('My custom block field'),
    '#default_value' => variable_get($delta, '')
  );

  return $form;
}

/**
 * Saves the block configuration, i.e. the same as you would do inside
 * `hook_block_save()`
 *
 * @param $block
 *   An array of values representing the block with it's configuration, i.e. the
 *   `$edit` array passed to `hook_block_save()`
 */
function my_module_save_example_block($block) {
  variable_set($block['delta'], $block['my_custom_field']);
}