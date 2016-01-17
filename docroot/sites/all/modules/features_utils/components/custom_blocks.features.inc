<?php

/**
 * Implementation of hook_features_export()
 */
function custom_blocks_features_export($data, &$export, $module_name) {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['features_utils'] = 'features_utils';
  $export['dependencies']['block'] = 'block';
  foreach ($data as $component) {
    $export['features']['custom_blocks'][$component] = $component;
  }
  return array();
}

/**
 * Implementation of hook_features_export_options()
 */
function custom_blocks_features_export_options() {
  $blocks = array();
  foreach (features_utils_custom_blocks_load() as $bid => $block) {
    $blocks["custom_block_$bid"] = "$bid: {$block->info}";
  }
  return $blocks;
}

/**
 * Implementation of hook_features_export_render()
 */
function custom_blocks_features_export_render($module, $data, $export = NULL) {
  $code = array();
  $code[] = '  $custom_blocks = array();';

  foreach ($data as $name) {
    $bid = str_replace('custom_block_', '', $name);
    $val = array(
      'block' => features_utils_custom_blocks_load($bid),
      'assignments' => features_utils_custom_blocks_assignments_load($bid)
    );
    $code[] = "  \$custom_blocks['{$name}'] = " . features_var_export($val) . ';';
  }

  $code[] = '  return $custom_blocks;';
  $code = implode("\n", $code);
  return array('custom_blocks_defaults' => $code);
}

/**
 * Implementation of hook_features_revert()
 */
function custom_blocks_features_revert($module) {
  return custom_blocks_features_rebuild($module);
}

/**
 * Implementation of hook_features_rebuild()
 */
function custom_blocks_features_rebuild($module) {
  $data = features_get_default('custom_blocks', $module);

  // if we had a problem loading the data, bail
  if (!$data) return;

  foreach ($data as $block) {
    // rebuld the custom block data
    db_delete('block_custom')->condition('bid', $block['block']['bid'])->execute();
    db_insert('block_custom')->fields($block['block'])->execute();

    // delete and rebuild the assignments
    db_delete('block')->condition('module', 'block')->condition('delta', $block['block']['bid'])->execute();
    foreach ($block['assignments'] as $assignment) {
      db_insert('block')->fields($assignment)->execute();
    }
  }

  return TRUE;
}

/**
 * load custom_block data from database
 */
function features_utils_custom_blocks_load($bid = NULL) {
  $blocks = &drupal_static(__FUNCTION__, NULL);

  // only run query once
  if (!$blocks) {
    $result = db_select('block_custom', 'b')->fields('b')->orderBy('bid')->execute()->fetchAll();
    $blocks = array();
    foreach ($result as $row) {
      $blocks[$row->bid] = $row;
    }
  }

  if (!$bid) {
    return $blocks;
  }

  return (!empty($blocks[$bid])) ? $blocks[$bid] : NULL;
}

/**
 * load block assignments/configurations from db
 */
function features_utils_custom_blocks_assignments_load($bid) {
  $block_assignments = &drupal_static(__FUNCTION__, NULL);

  // query and build this data once
  if (!$block_assignments) {
    $result = db_select('block', 'b')->fields('b')->condition('module', 'block')->orderBy('delta')
        ->orderBy('theme')->execute()->fetchAll();
    $block_assignments = array();
    foreach ($result as $row) {
      $block_assignments[$row->delta][] = $row;
    }
  }

  return (!empty($block_assignments[$bid])) ? $block_assignments[$bid] : array();
}