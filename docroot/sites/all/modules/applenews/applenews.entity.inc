<?php
/**
 * @file
 * Entity hook implementations.
 */

/**
 * Implements hook_entity_insert().
 */
function applenews_entity_insert($entity, $type) {
  if (!empty($entity->applenews['applenews_publish_flag'])) {
    _applenews_entity_push('insert', $entity, $type);
  }
}

/**
 * Implements hook_entity_update().
 */
function applenews_entity_update($entity, $type) {
  if (!empty($entity->applenews['applenews_publish_flag'])) {
    _applenews_entity_push('update', $entity, $type);
  }
}

/**
 * Push node.
 */
function _applenews_entity_push($op, $entity, $type) {
  module_load_include('inc', 'applenews');

  /** @var EntityDrupalWrapper $wrapper */
  $wrapper = entity_metadata_wrapper($type, $entity);
  $export = applenews_entity_get_export($type, $wrapper->getIdentifier());

  $el = $entity->applenews;
  foreach (applenews_channels(TRUE) as $channel_id => $channel) {
    if (!empty($el['channels']['channel-' . $channel_id])) {
      $settings = [];
      foreach ($channel['sections'] as $section_id => $section) {
        if (!empty($el['channels']['section-' . $channel_id . '-' . $section_id])) {
          $settings['metadata']['data']['links']['sections'][] = applenews_section_url($section_id);
        }
      }

      // Visible to members of the channel only.
      $settings['metadata']['data']['isPreview'] = !empty($entity->applenews['is_preview']);

      if (!applenews_op($op, $channel_id, $entity, $type, $export)) {
        drupal_set_message(
          t('Unable to push %title to %channel',
            [
              '%title'   => $wrapper->label(),
              '%channel' => filter_xss($channel->name)
            ]), 'error'
        );
      }
    }
  }
}

/**
 * Implements hook_entity_delete().
 */
function applenews_entity_delete($entity, $type) {
  module_load_include('inc', 'applenews');
  if ($channel_ids = applenews_load_article_channels($entity, $type)) {
    foreach ($channel_ids as $channel_id) {
      if (!applenews_op('delete', $channel_id, $entity, $type)) {
        $wrapper = entity_metadata_wrapper($type, $entity);
        $channel = applenews_channel($channel_id);
        drupal_set_message(
          t('Unable to delete %title from %channel',
            [
              '%title'   => $wrapper->label(),
              '%channel' => filter_xss($channel->name)
            ]), 'error'
        );
      }
    }
  }
}
