<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on cm_airing being loaded from the database.
 *
 * This hook is invoked during $cm_airing loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $cm_airing entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_cm_airing_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $cm_airing is inserted.
 *
 * This hook is invoked after the $cm_airing is inserted into the database.
 *
 * @param ExampleAiring $cm_airing
 *   The $cm_airing that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_cm_airing_insert(ExampleAiring $cm_airing) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('cm_airing', $cm_airing),
      'extra' => print_r($cm_airing, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $cm_airing being inserted or updated.
 *
 * This hook is invoked before the $cm_airing is saved to the database.
 *
 * @param ExampleAiring $cm_airing
 *   The $cm_airing that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_cm_airing_presave(ExampleAiring $cm_airing) {
  $cm_airing->name = 'foo';
}

/**
 * Responds to a $cm_airing being updated.
 *
 * This hook is invoked after the $cm_airing has been updated in the database.
 *
 * @param ExampleAiring $cm_airing
 *   The $cm_airing that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_cm_airing_update(ExampleAiring $cm_airing) {
  db_update('mytable')
    ->fields(array('extra' => print_r($cm_airing, TRUE)))
    ->condition('id', entity_id('cm_airing', $cm_airing))
    ->execute();
}

/**
 * Responds to $cm_airing deletion.
 *
 * This hook is invoked after the $cm_airing has been removed from the database.
 *
 * @param ExampleAiring $cm_airing
 *   The $cm_airing that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_cm_airing_delete(ExampleAiring $cm_airing) {
  db_delete('mytable')
    ->condition('pid', entity_id('cm_airing', $cm_airing))
    ->execute();
}

/**
 * Act on a cm_airing that is being assembled before rendering.
 *
 * @param $cm_airing
 *   The cm_airing entity.
 * @param $view_mode
 *   The view mode the cm_airing is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $cm_airing->content prior to rendering. The
 * structure of $cm_airing->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_cm_airing_view($cm_airing, $view_mode, $langcode) {
  $cm_airing->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for cm_airings.
 *
 * @param $build
 *   A renderable array representing the cm_airing content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * cm_airing content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the cm_airing rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_cm_airing().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_cm_airing_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on cm_airing_type being loaded from the database.
 *
 * This hook is invoked during cm_airing_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of cm_airing_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_cm_airing_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a cm_airing_type is inserted.
 *
 * This hook is invoked after the cm_airing_type is inserted into the database.
 *
 * @param ExampleAiringType $cm_airing_type
 *   The cm_airing_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_cm_airing_type_insert(ExampleAiringType $cm_airing_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('cm_airing_type', $cm_airing_type),
      'extra' => print_r($cm_airing_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a cm_airing_type being inserted or updated.
 *
 * This hook is invoked before the cm_airing_type is saved to the database.
 *
 * @param ExampleAiringType $cm_airing_type
 *   The cm_airing_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_cm_airing_type_presave(ExampleAiringType $cm_airing_type) {
  $cm_airing_type->name = 'foo';
}

/**
 * Responds to a cm_airing_type being updated.
 *
 * This hook is invoked after the cm_airing_type has been updated in the database.
 *
 * @param ExampleAiringType $cm_airing_type
 *   The cm_airing_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_cm_airing_type_update(ExampleAiringType $cm_airing_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($cm_airing_type, TRUE)))
    ->condition('id', entity_id('cm_airing_type', $cm_airing_type))
    ->execute();
}

/**
 * Responds to cm_airing_type deletion.
 *
 * This hook is invoked after the cm_airing_type has been removed from the database.
 *
 * @param ExampleAiringType $cm_airing_type
 *   The cm_airing_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_cm_airing_type_delete(ExampleAiringType $cm_airing_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('cm_airing_type', $cm_airing_type))
    ->execute();
}

/**
 * Define default cm_airing_type configurations.
 *
 * @return
 *   An array of default cm_airing_type, keyed by machine names.
 *
 * @see hook_default_cm_airing_type_alter()
 */
function hook_default_cm_airing_type() {
  $defaults['main'] = entity_create('cm_airing_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default cm_airing_type configurations.
 *
 * @param array $defaults
 *   An array of default cm_airing_type, keyed by machine names.
 *
 * @see hook_default_cm_airing_type()
 */
function hook_default_cm_airing_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
