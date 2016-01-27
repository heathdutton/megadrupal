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
 * Acts on media_event being loaded from the database.
 *
 * This hook is invoked during $media_event loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $media_event entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_media_event_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $media_event is inserted.
 *
 * This hook is invoked after the $media_event is inserted into the database.
 *
 * @param ExampleMediaEvent $media_event
 *   The $media_event that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_media_event_insert(ExampleMediaEvent $media_event) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('media_event', $media_event),
      'extra' => print_r($media_event, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $media_event being inserted or updated.
 *
 * This hook is invoked before the $media_event is saved to the database.
 *
 * @param ExampleMediaEvent $media_event
 *   The $media_event that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_media_event_presave(ExampleMediaEvent $media_event) {
  $media_event->name = 'foo';
}

/**
 * Responds to a $media_event being updated.
 *
 * This hook is invoked after the $media_event has been updated in the database.
 *
 * @param ExampleMediaEvent $media_event
 *   The $media_event that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_media_event_update(ExampleMediaEvent $media_event) {
  db_update('mytable')
    ->fields(array('extra' => print_r($media_event, TRUE)))
    ->condition('id', entity_id('media_event', $media_event))
    ->execute();
}

/**
 * Responds to $media_event deletion.
 *
 * This hook is invoked after the $media_event has been removed from the database.
 *
 * @param ExampleMediaEvent $media_event
 *   The $media_event that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_media_event_delete(ExampleMediaEvent $media_event) {
  db_delete('mytable')
    ->condition('pid', entity_id('media_event', $media_event))
    ->execute();
}

/**
 * Act on a media_event that is being assembled before rendering.
 *
 * @param $media_event
 *   The media_event entity.
 * @param $view_mode
 *   The view mode the media_event is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $media_event->content prior to rendering. The
 * structure of $media_event->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_media_event_view($media_event, $view_mode, $langcode) {
  $media_event->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for media_events.
 *
 * @param $build
 *   A renderable array representing the media_event content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * media_event content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the media_event rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_media_event().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_media_event_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on media_event_type being loaded from the database.
 *
 * This hook is invoked during media_event_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of media_event_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_media_event_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a media_event_type is inserted.
 *
 * This hook is invoked after the media_event_type is inserted into the database.
 *
 * @param ExampleMediaEventType $media_event_type
 *   The media_event_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_media_event_type_insert(ExampleMediaEventType $media_event_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('media_event_type', $media_event_type),
      'extra' => print_r($media_event_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a media_event_type being inserted or updated.
 *
 * This hook is invoked before the media_event_type is saved to the database.
 *
 * @param ExampleMediaEventType $media_event_type
 *   The media_event_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_media_event_type_presave(ExampleMediaEventType $media_event_type) {
  $media_event_type->name = 'foo';
}

/**
 * Responds to a media_event_type being updated.
 *
 * This hook is invoked after the media_event_type has been updated in the database.
 *
 * @param ExampleMediaEventType $media_event_type
 *   The media_event_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_media_event_type_update(ExampleMediaEventType $media_event_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($media_event_type, TRUE)))
    ->condition('id', entity_id('media_event_type', $media_event_type))
    ->execute();
}

/**
 * Responds to media_event_type deletion.
 *
 * This hook is invoked after the media_event_type has been removed from the database.
 *
 * @param ExampleMediaEventType $media_event_type
 *   The media_event_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_media_event_type_delete(ExampleMediaEventType $media_event_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('media_event_type', $media_event_type))
    ->execute();
}

/**
 * Define default media_event_type configurations.
 *
 * @return
 *   An array of default media_event_type, keyed by machine names.
 *
 * @see hook_default_media_event_type_alter()
 */
function hook_default_media_event_type() {
  $defaults['main'] = entity_create('media_event_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default media_event_type configurations.
 *
 * @param array $defaults
 *   An array of default media_event_type, keyed by machine names.
 *
 * @see hook_default_media_event_type()
 */
function hook_default_media_event_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
