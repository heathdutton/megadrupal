<?php

/**
 * @file
 * Provide integration with other MaPS Suite modules.
 */

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_links\Mapping\Mapper\Link as LinkMapper;

/**
 * Implements hook_maps_import_operations().
 *
 * Add the link mapping operation to the list.
 */
function maps_links_maps_import_operations(Profile $profile) {
  $instance = new LinkMapper($profile);

  return array(
    $instance->getType() => array(
      'title' => $instance->getTitle(),
      'description' => $instance->getDescription(),
      'class' => get_class($instance),
    ),
  );
}

/**
 * Implements hook_maps_import_mapping_finished().
 */
function maps_links_maps_import_mapping_finished(Profile $profile, array $ids) {
  LinkMapper::deleteMappedRelations($profile, $ids);
}
