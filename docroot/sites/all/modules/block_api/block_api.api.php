<?php

/**
 * @file
 * API documentation for Block API module.
 */

/**
 * Register block types.
 *
 * This hook is invoked from block_api_get_info() and allows modules to register
 * custom block types. Site administrators are able to create block instances of
 * a certain block type.
 *
 * @return
 *   An associative array whose keys are internal block type names and whose
 *   values are again associative arrays describing the block type:
 *   - title: (required) An administrative, localized title of the block type.
 *   - description: An administrative, localized description of the block type,
 *     if a block type cannot be sufficiently described with the title only.
 *   - storage: One of the following constants:
 *     - BLOCK_API_STORAGE_DATABASE: (default) @todo 
 *     - BLOCK_API_STORAGE_OWN: @todo 
 *   - settings: An associative array of configuration settings to apply to new
 *     block instances of the block type by default. These default settings are
 *     only applied upon (re-)saving the block instance configuration through
 *     the UI.
 *   - ...: Any other properties to pass through to hook_block_info().
 */
function hook_block_api_info() {
  $types['html'] = array(
    'title' => t('HTML content'),
    'description' => t('Resembles a "custom" block of the core Block module.'),
    'storage' => BLOCK_API_STORAGE_DATABASE,
    'settings' => array(
      'body[value]' => 'Example default value.',
    ),
  );
  return $types;
}

/**
 * Return a configuration form for a block instance.
 *
 * This hook is invoked from block_api_block_configure() and allows the block
 * type module to return additional configuration form elements for a block
 * instance.
 *
 * @param $instance
 *   The block type instance being configured.
 */
function hook_block_api_TYPE_form($instance) {
  $form['body'] = array(
    '#type' => 'text_format',
    '#title' => t('Block body'),
    '#description' => t('The content of the block as shown to the user.'),
    '#rows' => 15,
    '#required' => TRUE,
    '#default_value' => isset($instance->settings['body']['value']) ? $instance->settings['body']['value'] : '',
    '#format' => isset($instance->settings['body']['format']) ? $instance->settings['body']['format'] : NULL,
  );
  return $form;
}

/**
 * Prepare block instance configuration settings for saving.
 *
 * This hook is invoked from block_api_block_save() and allows the block
 * type module to prepare the block instance settings for saving. The block type
 * module should perform changes to the block instance settings contained in
 * $instance->settings, if any.
 *
 * @param $instance
 *   The block type instance being saved. $instance->settings contains the
 *   (new) submitted block instance settings.
 * @param $edit
 *   All submitted form values of the administrative block configuration form,
 *   as originally passed to hook_block_save(), resp. block_api_block_save().
 */
function hook_block_api_TYPE_save($instance) {
}

/**
 * Return a rendered or renderable view of a block instance.
 *
 * This hook is invoked from block_api_block_view().
 *
 * @param $instance
 *   The block type instance to render.
 *
 * @see hook_block_view()
 */
function hook_block_api_TYPE_view($instance) {
  $block['content'] = check_markup($instance->settings['body']['value'], $instance->settings['body']['format'], '', TRUE);
  return $block;
}

/**
 * Act on block instances being loaded from the database.
 *
 * This hook is invoked during block loading, which is handled by entity_load(),
 * via DrupalDefaultEntityController.
 *
 * @param $instances
 *   An array of block type instances being loaded, keyed by delta.
 */
function hook_block_api_load($instances) {
}

/**
 * Respond to creation of a new block instance.
 *
 * This hook is invoked from block_api_save() after the block instance has been
 * inserted into the database.
 *
 * @param $instance
 *   The block type instance that is being created.
 */
function hook_block_api_insert($instance) {
}

/**
 * Respond to updates to a block instance.
 *
 * This hook is invoked from block_api_save() after the block instance has been
 * updated in the database.
 *
 * @param $instance
 *   The block type instance that is being updated.
 */
function hook_block_api_update($instance) {
}

/**
 * Respond to a block instance deletion.
 *
 * This hook is invoked from block_api_delete_multiple() after the block
 * instance was deleted from the {block_api} database table, and before any
 * corresponding records in {block}, {block_role}, and {block_node_type} are
 * deleted.
 *
 * @param $instance
 *   The block type instance that is being deleted.
 */
function hook_block_api_delete($instance) {
}
