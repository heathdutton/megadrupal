<?php

/**
 * @file
 * Hooks provided by the Apple News module.
 */

/**
 * Registers your Apple News classes and defines defaults.
 *
 * @return array
 *   An associative array with the following keys:
 *   - api (required): Always 1 for any module implementing the Apple News 1
 *     API.
 *   - exports: Associative array of exports defined by this module, keyed on
 *     export machine name (matching /^[a-zA-Z0-9-_]$/), with keys:
 *     -  class (required): Class name to instantiate for the export.
 *     -  arguments: Array of arguments to pass to class constructor.
 *     -  name
 *     -  description
 *     -  default: callback allowing modifications to newly instantiated object.
 *   - sources:
 *     -  class (required): Class name to instantiate for the source.
 *     -  name
 *     -  description
 *   - destinations:
 *     -  class (required): Class name to instantiate for the source.
 *     -  arguments: Array of arguments to pass to class constructor.
 *     -  name
 *     -  description
 *
 * @see hook_applenews_api_alter()
 */
function hook_applenews_api() {
  return [
    'api' => 1,
    'exports' => [
      'article' => [
        'class' => 'AppleNewsExportArticle',
        'arguments' => [],
        'name' => t('Articles'),
        'description' => t('Export articles as defined by default install profile.'),
      ],
    ],
  ];
}

/**
 * Alter information from all implementations of hook_applenews_api().
 *
 * @param array $info
 *   An array of results from hook_applenews_api(), keyed by module name.
 *
 * @see hook_applenews_api()
 */
function hook_applenews_api_alter(array &$info) {
  // Override the class for another module's migration - say, to add some
  // additional preprocessing in prepareRow().
  $key = _applenews_export_id('MODULE_NAME', 'MACHINE_NAME');
  if (isset($info['exports'][$key]['key'])) {
    $info['exports'][$key]['key'] = 'new value';
  }
}

/**
 * Invoke Apple News insert operation hooks.
 *
 * @param string $article_id
 *   Apple News article ID.
 * @param string $article_revision_id
 *   Apple News article revision ID.
 * @param string $channel_id
 *   Apple News channel ID.
 * @param EntityDrupalWrapper $entity_wrapper
 *   Entity wrapper object.
 * @param string $entity_type
 *   Entity type.
 * @param array $data
 *   An array of article assets, used in insert and update operations.
 *
 * @see applenews_op()
 */
function hook_applenews_insert($article_id, $article_revision_id, $channel_id, EntityDrupalWrapper $entity_wrapper, $entity_type, array $data) {
}

/**
 * Invoke Apple News update operation hooks.
 *
 * @param string $article_id
 *   Apple News article ID.
 * @param string $article_revision_id
 *   Apple News article revision ID.
 * @param string $channel_id
 *   Apple News channel ID.
 * @param EntityDrupalWrapper $entity_wrapper
 *   Entity wrapper object.
 * @param string $entity_type
 *   Entity type.
 * @param array $data
 *   An array of article assets, used in insert and update operations.
 *
 * @see applenews_op()
 */
function hook_applenews_update($article_id, $article_revision_id, $channel_id, EntityDrupalWrapper $entity_wrapper, $entity_type, array $data) {
}

/**
 * Invoke Apple News delete operation hooks.
 *
 * @param string $article_id
 *   Apple News article ID.
 * @param string $channel_id
 *   Apple News channel ID.
 * @param EntityDrupalWrapper $entity_wrapper
 *   Entity wrapper object.
 * @param string $entity_type
 *   Entity type.
 *
 * @see applenews_op()
 */
function hook_applenews_delete($article_id, $channel_id, EntityDrupalWrapper $entity_wrapper, $entity_type) {
}
