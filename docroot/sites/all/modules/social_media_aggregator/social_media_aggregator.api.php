<?php

/**
 * @file
 * Global API functions to provide a social media source.
 */

/**
 * Declare a source for Social media items.
 *
 * @return array
 *   An array defining the source.
 */
function hook_social_media_aggregator_source_info() {
  $source['my_source'] = array(
    'name' => 'my_source',
    'label' => t('My Source'),
    'base' => 'social_media_aggregator_my_source',
  );
  return $source;
}

/**
 * Alter the declared social media sources.
 *
 * @param array $sources
 *   The array of social media sources.
 *
 * @see hook_social_media_aggregator_source_info()
 */
function hook_social_media_aggregator_sources_info_alter(array &$sources) {
  $sources['my_source'] = array(
    'name' => 'my_source',
    'label' => t('My Source'),
    'base' => 'social_media_aggregator_my_source',
  );
}
