<?php
/**
 * Helper function to save data as layout files
 */
function _regionarea_save_layout_file($filepath, $file, $data) {
  $file .= '.layout';
  $info = vtcore_build_info_file($data, $prefix = FALSE);
  $save = file_unmanaged_save_data($info, $filepath . '/' . $file, $replace = FILE_EXISTS_REPLACE);
  if ($save != FALSE) {
    drupal_set_message(t('@file saved.', array('@file' => $file)));
  }
}

/**
 * Helper function to build parent select options array
 */
function _regionarea_build_parent_select_options($data) {
  $output = array();
  foreach ($data as $key => $value) {
    $output[$key] = $value['#name'];
  }

  return $output;
}

/**
 * Helper function to grab all available layout files
 * per theme as specified by $theme_key
 */
function _regionarea_get_all_layout_files($theme_key) {
  $dir = drupal_get_path('theme', $theme_key);
  $files = file_scan_directory($dir, '/.*\.layout$/', array('key' => 'uri'));
  $options = array();
  foreach ($files as $key => $file) {
    // Don't show master layout file.
    if ($file->filename == 'master.layout') {
      continue;
    }
    $options[$file->uri] = $file->filename . ' - ' . $file->uri;
  }
  return $options;
}

/**
 * Helper function to grab and build all available
 * layout that are stored in database
 *
 * @return an array to be used in select element
 */
function _regionarea_get_all_layout_database($theme_key) {
  $output = array();
  $dynamic = variable_get('theme_' . $theme_key . '_settings');
  if (isset($dynamic['layouts']) && !empty($dynamic['layouts'])) {
    foreach ($dynamic['layouts'] as $key => $value) {
      $output[$key] = t('!key - Stored in database', array('!key' => $key));
    }
  }
  return $output;
}

/**
 * Helper function to move data out from $form_state
 * to $data.
 *
 * @see regionarea_submit().
 */
function _regionarea_build_data_for_saving($key, &$data, $form_state) {
  foreach ($form_state['values'][$key] as $type => $typedata) {
    // Skip on disabled blocks or region
    if (isset($typedata['parent']) && $typedata['parent'] == 'disabled') {
      continue;
    }

    // Check for block parent
    if ($key == 'block') {
      // Skip if region parent is disabled
      if (!isset($form_state['values']['region'][$typedata['parent']])
          || $form_state['values']['region'][$typedata['parent']]['parent'] == 'disabled') {
        continue;
      }

      $region_parent = $form_state['values']['region'][$typedata['parent']]['parent'];

      // Skip if area parent is disabled
      if (!isset($form_state['values']['area'][$region_parent])
          || $form_state['values']['area'][$region_parent]['enabled'] == 0) {
        continue;
      }
    }

    // Check for region parent
    if ($key == 'region') {
       if (!isset($form_state['values']['area'][$typedata['parent']])
           || $form_state['values']['area'][$typedata['parent']]['enabled'] == 0) {
        continue;
      }
    }

    // Check for area
    if ($key == 'area') {
      if (!isset($typedata['enabled']) || $typedata['enabled'] == 0) {
        continue;
      }
    }

    foreach ($typedata as $setting => $settingdata) {
      $data[$key][$type]['#' . $setting] = $settingdata;
    }
  }

  uasort($data[$key], 'element_sort');
  uasort($form_state['values'][$key], 'element_sort');
  if ($key != 'area') {
    uasort($data[$key], 'regionarea_sort_by_parent');
  }

}

/**
 * Helper function to sort element by #parent
 * @todo need improvement to sort by parent then weight.
 */
function regionarea_sort_by_parent($a, $b) {
  return strcasecmp($a['#parent'], $b['#parent']);
}

/**
 * Helper function to grab the correct layout data
 * for original and perpath layout
 *
 * Currently it will alter the $original and $layout variable
 * depending on build mode
 */
function _regionarea_build_layouts_array($build_mode, $theme_key) {

  // Try use per theme default page layout
  $original = drupal_get_path('theme', $theme_key) . '/layouts/theme_default/page.layout';

  // This is a master layout that theme should
  // supply as a template when creating new layout
  $master_layout = drupal_get_path('theme', $theme_key) . '/layouts/theme_default/master.layout';
  if (file_exists($master_layout)) {
    $original = $master_layout;
  }

  // Fallback to vtcore layout plugin page layout
  if (!file_exists($original)) {
    $original = drupal_get_path('theme', BASE_THEME) . '/vtcore/plugins/layout/layouts/page.layout';
  }

  // Use original layout for new template
  if ($build_mode == 'create_new') {
    $file = $original;
  }

  // Use user selected layout as template for page layout
  else {
    // File layout default fallback as user selected
    $file = $build_mode;

    // Try to override with dynamic database stored layout
    $dynamic_name = basename($file);
    str_replace('.layout', '', $dynamic_name);
    $dynamic = variable_get('theme_' . $theme_key . '_settings');
    if (isset($dynamic['layouts'][$dynamic_name])) {
      $layout_data = $dynamic['layouts'][$dynamic_name];
    }
  }

  // No template loaded yet, load layout file template
  // This can be loading for original on create new or
  // user selected file based stored layout file.
  if (empty($layout_data)) {
    $layout_data = drupal_parse_info_file($file);
  }

  // Load original file template
  $original_data = drupal_parse_info_file($original);

  // Fix for missing layout plugin blocks when
  // it is not defined in the theme layout file
  $layout_blocks = array(
    'action',
    'breadcrumb',
    'help',
    'logo',
    'messages',
    'title',
    'tabs',
  );

  $layout_core_file = drupal_get_path('theme', BASE_THEME) . '/vtcore/plugins/layout/layouts/page.layout';
  if ($original != $layout_core_file) {
    $layout_plugin_layout = drupal_parse_info_file($layout_core_file);
    foreach ($layout_blocks as $delta) {
      if (isset($original_data['block'][$delta])) {
        continue;
      }

      if (isset($layout_plugin_layout['block'][$delta])) {
        $original_data['block'][$delta] = $layout_plugin_layout['block'][$delta];
      }
    }
  }

  // Fix for special blocks missing if not defined in layout file
  // Load additional vtcore-blocks, this is where special blocks
  // is defined by plugins but not in the layout files
  $vtcore_blocks = vtcore_special_block_register();

  if (!empty($vtcore_blocks)) {
    foreach ($vtcore_blocks as $delta => $name) {
      if (isset($original_data['block'][$delta])) {
        continue;
      }

      $meta = vtcore_get_default($delta);
      if (!empty($meta)) {
        $original_data['block'][$delta] = $meta;
      }
    }
  }

  // Loop into the original data and set everything to disabled first
  foreach ($original_data as $type => $areas) {
    foreach ($areas as $area => $area_setting) {
      if (isset($area_setting['#enabled'])) {
        $original_data[$type][$area]['#enabled'] = 0;
      }

      if (isset($area_setting['#parent'])) {
        $original_data[$type][$area]['#parent'] = 'disabled';
      }
    }
  }


  // Get drupal regions
  $regions = system_region_list($theme_key);

  // Set everything to disabled first
  $region_data = array();
  foreach ($regions as $region => $region_name) {
  	$region_data['region'][$region] = array(
      '#name' => $region_name,
      '#parent' => 'disabled',
      '#weight' => 0,
      '#column' => 12,
      '#clearfix' => 0,
      '#newrow' => 0,
      '#lastrow' => 0,
    );
  }

  // Get drupal blocks
  $blocks = _block_load_blocks();
  $block_data = array();
  foreach ($blocks as $region => $region_blocks) {
    foreach ($region_blocks as $block => $data) {
      $block_data['block'][$block] = array(
        '#name' => $block,
      	'#block_type' => 'block',
        '#weight' => $data->weight,
        '#parent' => $data->region,
        '#column' => 12,
        '#clearfix' => 0,
        '#newrow' => 0,
        '#lastrow' => 0,
      );
    }
  }

  // Now we merge all data into a single layout
  $layouts = vtcore_array_merge_recursive_distinct($original_data, $region_data);
  $layouts = vtcore_array_merge_recursive_distinct($layouts, $block_data);
  $layouts = vtcore_array_merge_recursive_distinct($layouts, $layout_data);

  // Sort by weight
  foreach ($layouts as $type => $value) {
    uasort($layouts[$type], 'element_sort');
  }

  return $layouts;

}
