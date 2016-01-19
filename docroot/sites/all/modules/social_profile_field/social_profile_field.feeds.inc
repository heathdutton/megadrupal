<?php
/**
 * @file
 * Integration with the Feeds module.
 */

/**
 * Adds mapping targets for processors.
 *
 * This hook allows additional target options to be added to the processors
 * mapping form.
 *
 * If the key in $targets[] does not correspond to the actual key on the node
 * object ($node->key), real_target MUST be specified. See mappers/link.inc
 *
 * For an example implementation, see mappers/text.inc
 *
 * @param string $entity_type
 *   The entity type of the target, for instance a 'node' entity.
 * @param string $bundle
 *   The entity bundle to return targets for.
 *
 * @return array
 *   Array containing the targets to be offered to the user. This function must
 *   return an array, even an empty one.
 */
function social_profile_field_feeds_processor_targets($entity_type, $bundle) {
  $targets = array();
  foreach (field_info_instances($entity_type, $bundle) as $name => $instance) {
    if ($entity_type == 'node' && isset($instance['widget']['module']) && $instance['widget']['module'] == 'social_profile_field') {
      $targets[$instance['field_name']] = array(
        'name' => t('Social profile field'),
        'description' => t('The URl of Social profile field.'),
        'callback' => 'social_profile_field_set_target',
      );
    }
  }
  return $targets;
}

/**
 * Alter mapping sources.
 *
 * Use this hook to add additional mapping sources for any parser. Allows for
 * registering a callback to be invoked at mapping time.
 *
 * @see my_source_get_source().
 * @see locale_feeds_parser_sources_alter().
 */
function social_profile_field_feeds_processor_targets_alter (&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if (in_array($info['type'], array('social_profile_field_url'))) {
      $targets[$name] = array(
        'name' => $instance['label'],
        'callback' => 'social_profile_field_set_target',
        'description' => t('The @label field of the node.', array('@label' => $instance['label'])),
      );
    }
  }
}

/**
 *  * Callback function for mapping social_profile_field.
 *
 * This function is invoked via hook_feeds_processor_targets_alter().
 * Here is where the actual mapping happens.
 *
 * @param \FeedsSource $source
 *   Source object for processing on.
 * @param $entity
 *   Entity been processed.
 * @param $target
 *   The name of the field the user has decided to map to.
 * @param array $values
 *   The value of the feed item element the user has picked as a source.
 * @param array $mapping
 *   Array of predefined mappings.
 */
function social_profile_field_set_target(FeedsSource $source, $entity, $target, array $values, array $mapping) {
  if(isset($mapping['target']) && !empty($mapping['target'])) {
    foreach ($values as $key => $value) {
      $entity->{$target}[$entity->language][$key]['url'] = $value;
    }
  }
}
