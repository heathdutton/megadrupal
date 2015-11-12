<?php

/**
 * Implements hook_felix_acces().
 *
 * @param string $op
 *   Operation: 'add', 'edit', 'delete' or 'move'.
 * @param string $region
 *   Machine name for Felix region.
 * @param object $block
 *   Block (only for 'edit' and 'delete').
 * @return int
 *   FELIX_ALLOW or FELIX_DENY
 */
function hook_felix_access($op = 'add', $region = NULL, $block = NULL) {
  return FELIX_ALLOW;
}

/**
 * Implements hook_felix_hash().
 *
 * Region content is differentiated by hash. When block content should be
 * different per node type, for example, the nodetype has to be added
 * to the hash.
 *
 * @param object $region
 * @param string $path
 * @return string
 */
function hook_felix_hash($region, $path) {
  $hash = '';

  if (in_array('path', $region->data['hash'])) {
    $hash .= $path;
  }

  return $hash;
}

/**
 * Implements hook_felix_hash_options().
 *
 * Define the hash options which can be handled by this module. Modules
 * implementing this hook should also implement hook_felix_hash().
 *
 * @return array
 */
function hook_felix_hash_options() {
  return array(
    'path' => t('Path'),
    'nodetype' => t('Nodetype'),
  );
}

/**
 * Implements hook_felix_hash_config().
 *
 * Provide configuration options for the given hash option.
 *
 * @param string $option
 * @param array $values
 * @return array
 */
function felix_taxonomy_felix_hash_config($option, $values) {
  if ($option == 'taxonomy') {
    return array(
      'depth' => array(
        '#type' => 'select',
        '#title' => t('Depth'),
        '#options' => array(0 => t('Any'), 1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5),
        '#default_value' => isset($values['depth']) ? $values['depth'] : 0,
        '#description' => t('Only use terms with the specified depth.'),
      ),
    );
  }
}

/**
 * Implements hook_felix_block_view().
 *
 * Alter a block before it is displayed.
 *
 * @param object $block
 */
function hook_felix_block_view(&$block) {
  if (isset($block->data['foo'])) {
    $block->subject = 'bar';
  }
  else {
    $block->status = 0;
  }
}

/**
 * Implements hook_felix_block_insert().
 *
 * Alter a block before it is inserted written to the database.
 *
 * @param object $block
 * @param string $path
 */
function hook_felix_block_insert(&$block, $path) {

}

/**
 * Implements hook_felix_block_update().
 *
 * Alter a block before update.
 *
 * @param object $block
 * @param array $values
 *   Form values from the attributes form.
 */
function hook_felix_block_update(&$block, $values) {
  $block->data['foo'] = 'bar';
}

/**
 * Implements hook_felix_block_delete().
 *
 * Act upon block deletion.
 *
 * @param object $block
 */
function hook_felix_block_delete($block) {

}

/**
 * Implements felix_block_config().
 *
 * Provide form options for block configuration.
 *
 * @param object $block
 */
function hook_felix_block_config($block) {
  return module_invoke($block->module, 'block_configure', $block->delta);
}

/**
 * Implements hook_block_config_save().
 *
 * Save form options from formfields defined by hook_felix_block_config().
 *
 * Instead of saving the data itself, it may return an array with the values.
 * That array is available in $block->data['config'] and will be passed as
 * second argument to hook_block_view(). Note that this value is not saved
 * when it is not an array.
 *
 * @param object $block
 * @param array $values
 */
function hook_felix_block_config_save($block, $values) {
  return module_invoke($block->module, 'block_save', $block->delta, $values);
}

/**
 * Alter results in autocomplete widget.
 *
 * @param array $results
 *   Array of autocomplete results. Each element has the keys 'nid', 'title'
 *   and 'description',
 */
function hook_felix_autocomplete_alter(&$results) {
  for ($i = 0; $i < count($results); ++$i) {
    $nid = $results[$i]['nid'];
    $results[$i]['description'] = t('Path: @path', array('@path' => drupal_get_path_alias("node/$nid")));
  }
}

/**
 * Alter recent content table on add node page.
 *
 * @param array $table
 *   Renderable array of the recent content table.
 */
function hook_felix_recent_content_alter(&$table) {
  $table['#header'][] = t('Node id');
  for ($i = 0; $i < count($table['#nodes']); ++$i) {
    $table['#rows'][$i][] = $table['#nodes'][$i]->nid;
  }
}
