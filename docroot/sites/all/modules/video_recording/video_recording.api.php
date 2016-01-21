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
 * Acts on video_recording being loaded from the database.
 *
 * This hook is invoked during $video_recording loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $video_recording entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_video_recording_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $video_recording is inserted.
 *
 * This hook is invoked after the $video_recording is inserted into the database.
 *
 * @param ExampleAiring $video_recording
 *   The $video_recording that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_video_recording_insert(ExampleAiring $video_recording) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('video_recording', $video_recording),
      'extra' => print_r($video_recording, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $video_recording being inserted or updated.
 *
 * This hook is invoked before the $video_recording is saved to the database.
 *
 * @param ExampleAiring $video_recording
 *   The $video_recording that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_video_recording_presave(ExampleAiring $video_recording) {
  $video_recording->name = 'foo';
}

/**
 * Responds to a $video_recording being updated.
 *
 * This hook is invoked after the $video_recording has been updated in the database.
 *
 * @param ExampleAiring $video_recording
 *   The $video_recording that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_video_recording_update(ExampleAiring $video_recording) {
  db_update('mytable')
    ->fields(array('extra' => print_r($video_recording, TRUE)))
    ->condition('id', entity_id('video_recording', $video_recording))
    ->execute();
}

/**
 * Responds to $video_recording deletion.
 *
 * This hook is invoked after the $video_recording has been removed from the database.
 *
 * @param ExampleAiring $video_recording
 *   The $video_recording that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_video_recording_delete(ExampleAiring $video_recording) {
  db_delete('mytable')
    ->condition('pid', entity_id('video_recording', $video_recording))
    ->execute();
}

/**
 * Act on a video_recording that is being assembled before rendering.
 *
 * @param $video_recording
 *   The video_recording entity.
 * @param $view_mode
 *   The view mode the video_recording is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $video_recording->content prior to rendering. The
 * structure of $video_recording->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_video_recording_view($video_recording, $view_mode, $langcode) {
  $video_recording->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for video_recordings.
 *
 * @param $build
 *   A renderable array representing the video_recording content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * video_recording content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the video_recording rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_video_recording().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_video_recording_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on video_recording_type being loaded from the database.
 *
 * This hook is invoked during video_recording_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of video_recording_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_video_recording_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a video_recording_type is inserted.
 *
 * This hook is invoked after the video_recording_type is inserted into the database.
 *
 * @param ExampleAiringType $video_recording_type
 *   The video_recording_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_video_recording_type_insert(ExampleAiringType $video_recording_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('video_recording_type', $video_recording_type),
      'extra' => print_r($video_recording_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a video_recording_type being inserted or updated.
 *
 * This hook is invoked before the video_recording_type is saved to the database.
 *
 * @param ExampleAiringType $video_recording_type
 *   The video_recording_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_video_recording_type_presave(ExampleAiringType $video_recording_type) {
  $video_recording_type->name = 'foo';
}

/**
 * Responds to a video_recording_type being updated.
 *
 * This hook is invoked after the video_recording_type has been updated in the database.
 *
 * @param ExampleAiringType $video_recording_type
 *   The video_recording_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_video_recording_type_update(ExampleAiringType $video_recording_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($video_recording_type, TRUE)))
    ->condition('id', entity_id('video_recording_type', $video_recording_type))
    ->execute();
}

/**
 * Responds to video_recording_type deletion.
 *
 * This hook is invoked after the video_recording_type has been removed from the database.
 *
 * @param ExampleAiringType $video_recording_type
 *   The video_recording_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_video_recording_type_delete(ExampleAiringType $video_recording_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('video_recording_type', $video_recording_type))
    ->execute();
}

/**
 * Define default video_recording_type configurations.
 *
 * @return
 *   An array of default video_recording_type, keyed by machine names.
 *
 * @see hook_default_video_recording_type_alter()
 */
function hook_default_video_recording_type() {
  $defaults['main'] = entity_create('video_recording_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default video_recording_type configurations.
 *
 * @param array $defaults
 *   An array of default video_recording_type, keyed by machine names.
 *
 * @see hook_default_video_recording_type()
 */
function hook_default_video_recording_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
