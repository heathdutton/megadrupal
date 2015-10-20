<?php

/**
 * @file
 * Hooks provided by Path Metatags module.
 */

/**
 * Expose Path Metatags settings.
 *
 * This hook is called by CTools. For this hook to work, you need
 * hook_ctools_plugin_api(). The values of this hook can be overridden
 * and reverted through the UI.
 *
 * @return array
 *   Array with importable objects.
 */
function hook_path_metatags_settings_info() {
  $path_metatags_data = array();

  $path_metatags = new stdClass();
  $path_metatags->api_version = 1;
  $path_metatags->machine_name = 'example_metatags';
  $path_metatags->name = 'Example metatags';
  $path_metatags->path = 'node/%node';
  $path_metatags->data = array(
    'metatags' => array(
      0 => 'author',
      1 => 'keywords',
      2 => 'copyright',
      3 => 'url',
      4 => 'description',
      5 => 'generator',
    ),
    'metatags_values' => array(
      0 => '%node:author:name',
      1 => '%node:field-tags:0:name, %node:field-tags:1:name, download',
      2 => '%site:name',
      3 => '%site:current-page:url',
      4 => '%node:title, node description, see all node descriptions. ',
      5 => 'Drupal 7',
    ),
    'translatable' => 1,
    'arguments' => array(
      'node' => array(
        'position' => 1,
        'argument' => 'entity_id:node',
        'settings' => array(
          'identifier' => 'Node: ID',
        ),
      ),
    ),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'article' => 'article',
            ),
          ),
          'context' => 'node',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $path_metatags->weight = -100;

  $path_metatags_data['example_metatags'] = $path_metatags;

  return $path_metatags;
}

/**
 * Provides list of available metatags.
 */
function hook_path_metatags_info() {

  $metatags['author'] = array(
    'group' => 'name',
  );

  $metatags['copyright'] = array(
    'group' => 'name',
  );

  $metatags['Content-Type'] = array(
    'group' => 'http-equiv',
  );

  return $metatags;
}

/**
 * Allows alter list of available metatags.
 *
 * @param $metatags
 *   Array of metatags
 */
function hook_path_metatags_info_alter(&$metatags) {

  $metatags['copyright'] = array(
    'group' => 'http-equip',
  );

}

/**
 * Respond to saving path metatags.
 *
 * This hook is invoked after creating new path_breadcrumbs or updating
 * existing one.
 *
 * @param object $path_metatags
 *    Object with all necessary information from saving form.
 */
function hook_path_metatags_save($path_metatags) {

}

/**
 * Respond to path metatags deletion.
 *
 * This hook is invoked before path_metatags variant is removed from
 * the database.
 *
 * @param object $path_metatags
 */
function hook_path_metatags_delete($path_metatags) {

}

/**
 * Act on a path metatags object is preparing for view.
 *
 * This hook is invoked before any token replacement.
 *
 * @param object $path_metatags
 *    Object with path breadcrumb variant loaded from database.
 * @param array $contexts
 *    Ctools contexts from current URL.
 *
 * @return object $path_metatags
 */
function hook_path_metatags_view($path_metatags, &$contexts) {

}

/**
 * Allows to alter information about site metadata before
 * it is added to HTML head.
 *
 * @param $head_metadata
 *   Array with site metadata.
 */
function hook_path_metatags_view_alter(&$head_metadata) {

  if (isset($head_meta['keywords'])) {
    $head_meta['keywords']['#attributes']['content'] .= ' keyword1, keyword2.';
  }

}
