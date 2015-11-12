<?php

/**
 * @file
 * Feeds Default Template
 */

/**
 * Returns Feeds Importer object.
 *
 * @param string $name
 *   Name of importer.
 *
 * @param string $entity_type
 *   Entity type (node, user, etc.). Used for importer description only.
 *
 * @param string $bundle
 *   Entity_id or bundle_id (article, page, etc.).
 *
 * @param string $processor
 *   Processor (FeedsNodeProcessor, etc.).
 */
function mirrors_feeds_default_template($name, $entity_type, $bundle, $processor) {
  $vars = array(
    '@entity_type' => $entity_type,
    '@bundle' => $bundle,
  );
  if ($entity_type == $bundle) {
    $description = t('Import @entity_type entities from a CSV file.', $vars);
  }
  else {
    $description = t('Import @entity_type entities of type @bundle from a CSV file.', $vars);
  }

  $feeds_default_config = array(
    'name' => $name,
    'description' => $description,
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'direct' => FALSE,
        'allowed_extensions' => 'txt csv tsv xml opml',
        'directory' => 'public://feeds',
        'allowed_schemes' => array(
          0 => 'public',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ';',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => $processor,
      'config' => array(
        'bundle' => $bundle,
        'update_existing' => 1,
        'expire' => '-1',
        'input_format' => 'plain_text',
        'author' => 0,
      ),
    ),
    'update' => 0,
    'update_existing' => 2,
    'import_period' => '-1',
    'expire_period' => 0,
    'import_on_create' => 1,
  );

  return $feeds_default_config;
}
