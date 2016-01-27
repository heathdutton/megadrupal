<?php

/**
 * @file
 * Contains entity property information for the party activity entity.
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function party_activity_entity_property_info_alter(&$property_info) {
  $info = entity_get_info('party_activity');
  $props =& $property_info['party_activity']['properties'];

  $props['title']['setter callback'] = 'entity_property_verbatim_set';
  $props['type']['required'] = TRUE;
  $props['type']['setter callback'] = 'entity_property_verbatim_set';
  $props['type']['description'] = t('The :activity type.', array(':activity' => $info['label']));
  $props['created']['type'] = 'date';
  $props['created']['description'] = t('When the :activity was created.', array(':activity' => $info['label']));
  $props['modified']['type'] = 'date';
  $props['modified']['description'] = t('When the :activity was last changed.', array(':activity' => $info['label']));
  $props['user']['setter callback'] = 'entity_property_verbatim_set';
}
