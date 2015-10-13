<?php
/*
/**
 * Implementation of hook_features_export_options()
 */
function cta_selector_features_export_options() {
  $options = array();
  $selectors = (array) cta_selectors();
  foreach ($selectors as $name => $selector) {
    $options[$name] = $selector['description'];
  }
  return $options;
}

/**
 * Implementation of hook_features_export()
 */
function cta_selector_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies'][] = 'bean';

  foreach ((array) $data as $name) {
    $selector = cta_selector_load($name);
    if (is_string($selector['data'])) {
      $selector['data'] = unserialize($selector['data']);
    }
    if (!empty($selector)) {
      $export['features']['cta_selector'][$name] = $name;
      if (isset($selector['module']) && !in_array($selector['module'], $export['dependencies'])) {
        $export['dependencies'][] = $selector['module'];
      }
    }
  }

  return $pipe;
}

/**
 * Implementation of hook_features_export_render()
 */
function cta_selector_features_export_render($module_name, $data) {
  $selectors = array();

  foreach ($data as $name) {
    $selector = cta_selector_load($name);

    if (!empty($selector)) {
      if (!is_array($selector['data'])) {
        $selector['data'] = unserialize($selector['data']);
      }
      // remove local id
      unset($selector['sid']);
      $selectors[$name] = $selector;
    }
  }
  $code = '  $data = ' . features_var_export($selectors, '  ') . ';' . PHP_EOL;
  $code .= '  return $data;';

  return array('cta_selector_defaults' => $code);
}

/**
 * Implementation of hook_features_rebuild()
 */
function cta_selector_features_rebuild($module) {
  cta_selector_features_revert($module);
}

/**
 * Implementation of hook_features_revert()
 */
function cta_selector_features_revert($module) {
  if ($data = features_get_default('cta_selector', $module)) {
    foreach ($data as $name => $selector) {

      cta_selector_save($selector);
    }
  }
}

/**
 * Implementation of hook_features_export_options()
 */
function cta_block_setting_features_export_options() {
  $options = array();
  $query = db_select('cta_block_selector', 's')
    ->fields('s');
  $result = $query->execute();
  $block_info = array();
  while ($row = $result->fetchObject()) {
    $module = $row->module;
    $delta = $row->delta;
    $func = $module . '_block_info';
    $infos = $func();
    $info = isset($infos[$delta]) ? $infos[$delta] : '';

    // transform id deltas to use uuids
    cta_get_block_uuid_by_id($module, $delta);

    $name = $row->module . ':' . $delta;
    $options[$name] = is_array($info) ? $info['info'] : '';
  }
  return $options;
}

/**
 * Implementation of hook_features_export()
 */
function cta_block_setting_features_export($data, &$export, $module_name = '') {
  $pipe = array();
//dsm($data);
  foreach ((array) $data as $name) {
    list($module, $delta) = explode(':', $name);
    $export['features']['cta_block_setting'][$name] = $name;
  }

  return $pipe;
}

/**
 * Implementation of hook_features_export_render()
 */
function cta_block_setting_features_export_render($module_name, $data) {
  $metas = array();
  foreach ($data as $name) {
    list($module, $delta) = explode(':', $name);

    // convert any deltas with uuids back to id form
    cta_get_block_id_by_uuid($module, $delta);

    $meta = cta_block_meta_load($module, $delta);

    if (!empty($meta)) {
      $metas[$name] = $meta;
    }
  }
  $code = '  $data = ' . features_var_export($metas, '  ') . ';' . PHP_EOL;
  $code .= '  return $data;';

  return array('cta_block_setting_defaults' => $code);
}

/**
 * Implementation of hook_features_rebuild()
 */
function cta_block_setting_features_rebuild($module) {
  cta_block_setting_features_revert($module);
}

/**
 * Implementation of hook_features_revert()
 */
function cta_block_setting_features_revert($module) {
  if ($data = features_get_default('cta_block_setting', $module)) {
    foreach ($data as $name => $meta) {
      list($mod, $delta) = explode(':', $name);
      // convert any deltas with uuids back to id form
      if (cta_get_block_id_by_uuid($mod, $delta)) {
        cta_block_meta_save($meta, $mod, $delta);
      }
    }
  }
}
