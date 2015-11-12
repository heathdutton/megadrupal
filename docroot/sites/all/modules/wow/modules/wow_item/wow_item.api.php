<?php

/**
 * @file
 * Hooks provided by the WoW Item module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * An item is about to be created or updated.
 *
 * This hook is primarily intended for modules that want to modify properties in
 * the item entity before saving.
 *
 * @param object $item
 *   The item object on which the operation is performed.
 *
 * @see hook_wow_item_insert()
 * @see hook_wow_item_update()
 */
function hook_wow_item_presave($item) {

}

/**
 * An item was created.
 *
 * The module should save its custom additions to the item object into the
 * database.
 *
 * @param object $item
 *   The item object on which the operation is performed.
 *
 * @see hook_wow_item_presave()
 * @see hook_wow_item_update()
 */
function hook_wow_item_insert($item) {
  db_insert('mytable')
    ->fields(array(
      'myfield' => $custom,
      'iid' => $item->id,
    ))
    ->execute();
}

/**
 * An item was updated.
 *
 * Modules may use this hook to update their item data in a custom storage
 * after a item object has been updated.
 *
 * @param object $item
 *   The item object on which the operation is performed.
 *
 * @see hook_wow_item_presave()
 * @see hook_wow_item_insert()
 */
function hook_wow_item_update($item) {
  db_insert('wow_guild_changes')
    ->fields(array(
      'iid' => $item->id,
      'changed' => REQUEST_TIME,
    ))
    ->execute();
}

/**
 * Respond to item deletion.
 *
 * This hook is invoked from wow_item_delete_multiple() before
 * field_attach_delete() is called and before items are actually removed
 * from the database.
 *
 * @param object $item
 *   The item object on which the operation is performed.
 *
 * @see wow_item_delete_multiple()
 */
function hook_wow_item_delete($item) {
  db_delete('mytable')
    ->condition('iid', $item->id)
    ->execute();
}

/**
 * The item's object information is being displayed.
 *
 * The module should format its custom additions for display and add them to
 * the $item->content array.
 *
 * @param object $item
 *   The item object on which the operation is being performed.
 * @param string $view_mode
 *   View mode, e.g. 'full'.
 * @param string $langcode
 *   The language code used for rendering.
 *
 * @see hook_wow_item_view_alter()
 * @see hook_entity_view()
 */
function hook_wow_item_view($item, $view_mode, $langcode) {

}

/**
 * The item was built; the module may modify the structured content.
 *
 * This hook is called after the content has been assembled in a structured array
 * and may be used for doing processing which requires that the complete item
 * content structure has been build.
 *
 * If the module wishes to act on the rendered HTML of the item rather than the
 * structured content array, it may use this hook to add a #post_render callback.
 *
 * @param array $build
 *   A renderable array representing the item.
 *
 * @see wow_item_view()
 * @see hook_entity_view_alter()
 */
function hook_wow_item_view_alter(&$build) {
  // Check for the existence of a field added by another module.
  if (isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;
  }

  // Add a #post_render callback to act on the rendered HTML of the item.
  $build['#post_render'][] = 'my_module_wow_item_post_render';
}

/**
 * @} End of "addtogroup hooks".
 */
