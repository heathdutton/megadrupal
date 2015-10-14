<?php
/**
 * @file
 * Storing core function for layout module
 */

/**
 * Function to build layout suggestion
 * This function will fetch the .layout file
 * and fallback to default page.layout if nothing
 * fancy is found
 *
 * .layout file will follow Drupal per path file
 * naming system.
 * Example: front page will be page--front.layout
 *
 * Additional feature outside theme_get_suggestions()
 * is per node type layout.
 * Example: node type = my_node_type
 * then the layout file will be page--node--my-node-type.layout
 *
 * Processing which layout suggestion to use will
 * always follow this rules :
 *
 * Layout plugin layouts reside in layout plugin directory
 * will be always the default page.layout to fallback
 *
 * Other plugins that have layout file in their plugin
 * directory will override Layout Plugin default page.layout
 *
 * Theme implementing layout must follow this directory
 * structure :
 * theme_folder/layouts/theme_default - for storing default
 * per theme layouts updating theme usually will override
 * all layout stored in this folder
 *
 * theme_folder/layouts/theme_custom - for storing custom
 * layout created by user, it is generally a good idea
 * to keep files stored in this folder to read only
 * so when updating core those files won't got overriden
 *
 * theme_folder/layouts/theme_dynamic - This will be used
 * by other plugins to store their per theme layouts
 *
 * Layouts file stored in theme_folder/layouts/theme_default
 * will overrride the Layout plugins and the other plugin layout
 * file
 *
 * Layout files stored in theme_folder/layouts/theme_custom
 * will override the files stored in theme_folder/layouts/theme_default
 * and the Layout plugins and the other plugin layout file
 *
 * Layout files stored in theme_folder/layouts/theme_dynamic
 * will override the files stored in theme_folder/layouts/theme_default
 * and theme_folder/layouts/theme_custom and also  the Layout plugins
 * and the other plugin layout file
 *
 * Layouts stored in variable table will override any layouts stored
 * as file if both are in the same suggestion level. File stored layouts
 * will prevail against layout stored in variable table, if it has higher
 * suggestion level.
 *
 */
function _layout_read_layout_files($theme_key, $variables = array()) {
  global $vtcore;

  // Build suggestions
  $suggestions = theme_get_suggestions(arg(), 'page');

  // Add suggestions from theme_hook_suggestions by merging it.
  if (!empty($variables['theme_hook_suggestions'])
      && is_array($variables['theme_hook_suggestions'])) {

     $suggestions = drupal_array_merge_deep($suggestions, $variables['theme_hook_suggestions']);
  }

  // Add default Page
  array_unshift($suggestions, 'page');

  // Added per node type template suggestions
  if (isset($variables['node'])) {
    // If the node type is "my_type" layout file will be "page--my-type.layout".
    $suggestions[] = 'page__node__'. $variables['node']->type;
  }

  // Allow other plugin to alter the suggestions
  vtcore_alter_process('layout_file_suggestions', $suggestions, $theme_key, $variables);

  // Load dynamic stored value based on theme key
  $dynamic = vtcore_get_plugin_setting('layouts', 'layouts', FALSE, $theme_key);

  // Try to use dynamic value first
  $suggest = '';
  if (!empty($dynamic)) {
    foreach ($suggestions as $suggestion) {
      $suggestion = str_replace('_', '-', $suggestion);
      if (isset($dynamic[$suggestion])) {
        $dynamic_variable = $dynamic[$suggestion];
        $suggest = $suggestion;
      }
    }

    if (!empty($dynamic_variable) && $suggest != 'page') {
      return $dynamic_variable;
    }
  }

  // Nothing found in dynamic storage, parse the .layout files
  static $files;
  if (!isset($files)) {
    $files = &drupal_static(__FUNCTION__);
    $dir = drupal_get_path('theme', $theme_key);
    $options = array('key' => 'uri');
    $files = file_scan_directory($dir, '/.*\.layout$/', $options);
  }

  // Get plugin metadata for its weight sorting later on
  $plugins = vtcore_scan_plugin();
  $theme_types = array(
    'theme_default',
    'theme_custom',
    'theme_dynamic',
  );

  $plugins_key = array_keys($plugins);

  // Build layouts array
  $layouts = array();
  foreach ($files as $path => $file) {
    $key = array_flip(explode('/', $path));

    // Plugin
    if (isset($key['plugins']) && $theme_key == BASE_THEME) {
      $name = explode('/', $path);
      // Fix for http://drupal.org/node/1739998 bug.
      // Relying on fullpath and intersect the array against all
      // available plugin name.
      // This is still not perfect if we got the same plugin name as theme name
      // or any other folder name in the path then this will break badly.
      $plugin_name = array_intersect($name, $plugins_key);
      $plugin_name = array_shift($plugin_name);
      $layouts['plugin'][$plugin_name][$file->name] = $file;
      $layouts['plugin'][$plugin_name]['weight'] = $plugins[$plugin_name]['weight'];
    }

    // Theme
    if (!isset($key['plugins'])) {
      $theme_type_key = $key['layouts'] + 1;
      $key_value = array_flip($key);
      $theme_type = 'theme_default';
      if (in_array($key_value[$theme_type_key], $theme_types)) {
        $theme_type = $key_value[$theme_type_key];
      }

      $layouts['theme'][$theme_type][$file->name] = $file;
    }
  }

  // Case where user using subtheme
  if (empty($layouts['plugin'])) {
    static $plugin_files;
    if (!isset($plugin_files)) {
      $plugin_files = &drupal_static(__FUNCTION__);
      $dir = drupal_get_path('theme', BASE_THEME);
      $options = array('key' => 'uri');
      $plugin_files = file_scan_directory($dir, '/.*\.layout$/', $options);
    }

    foreach ($plugin_files as $path => $file) {
      $key = array_flip(explode('/', $path));
      // Plugin
      if (isset($key['plugins'])) {
        $name = explode('/', str_replace($vtcore->plugin_path . '/', '', $path));
        $plugin_name = array_shift($name);
        $layouts['plugin'][$plugin_name][$file->name] = $file;
        $layouts['plugin'][$plugin_name]['weight'] = $plugins[$plugin_name]['weight'];
      }
    }
  }


  // Sort the plugin to respect meta weight
  uasort($layouts['plugin'], 'drupal_sort_weight');

  // Set to layout plugin default layout
  $file = $layouts['plugin']['layout']['page']->uri;
  $suggest = 'page';

  // Try to find layout as suggestions
  foreach ($suggestions as $suggestion) {
    $suggestion = str_replace('_', '-', $suggestion);

    // Grab the plugins first
    foreach ($layouts['plugin'] as $plugin_data) {
      if (isset($plugin_data[$suggestion])) {
        $file = $plugin_data[$suggestion]->uri;
        $suggest = $suggestion;
      }
    }
    // Grab the theme override
    foreach ($theme_types as $theme_type) {
      if (isset($layouts['theme'][$theme_type]) && isset($layouts['theme'][$theme_type][$suggestion])) {
        $file = $layouts['theme'][$theme_type][$suggestion]->uri;
        $suggest = $suggestion;
      }
    }
  }

  // Special conditions, if no file aside from 'page'
  // found and dynamic 'page' found previously
  // return dynamic array instead of default page
  if ($suggest == 'page' && !empty($dynamic_variable)) {
    return $dynamic_variable;
  }

  return drupal_parse_info_file($file);
}

/**
 * Function to grab layout blocks meta options
 */
function _layout_read_blocks_meta($block_name) {
  global $vtcore;
  $file = $vtcore->plugin_path . '/layout/blocks/' . $block_name . '/' . $block_name . '.setting';

  $meta = array();
  if (file_exists($file)) {
    $meta = drupal_parse_info_file($file);
  }
  return $meta;
}

/**
 * Function to build block renderable array
 * This function is for layout special blocks only
 */
function _layout_read_block_renderable_arrays($block_name, $options, &$variables) {
  global $vtcore;

  $file = $vtcore->plugin_path . '/layout/blocks/' . $block_name . '/' . $block_name . '.block';
  $arrays = array();
  if (file_exists($file)) {
    // Load the block files.
    require_once($file);

    // Invoke the block renderable arrays functions
    $function = $block_name . '_block_renderable_array';
    if (function_exists($function)) {
      $arrays = $function($options, $variables);
    }
  }

  return $arrays;
}

/**
 * Core function to scan all available blocks and static cache it
 */
function _layout_scan_blocks() {
  // Auto scanning all files in blocks directory
  static $layout_blocks;
  if (!isset($layout_blocks)) {
    global $vtcore;
    $layout_blocks = &drupal_static(__FUNCTION__);
    $dir = $vtcore->plugin_path . '/layout/blocks';
    $layout_blocks = file_scan_directory($dir, '/.*\.block$/', array('key' => 'name'), $depth = 1);
  }

  return $layout_blocks;
}

/**
 * Helper function to return the proper column class
 */
function _layout_column_class($column) {
  $column_class = array(
    '1' => 'onecol', '2' => 'twocol', '3' => 'threecol', '4' => 'fourcol',
    '5' => 'fivecol', '6' => 'sixcol', '7' => 'sevencol', '8' => 'eightcol',
    '9' => 'ninecol', '10' => 'tencol', '11' => 'elevencol', '12' => 'twelvecol',
  );

  return $column_class[$column];
}

/**
 * Helper function to add first and last class of an array
 */
function _layout_add_first_last_class(array &$arrays) {
  if (empty($keys)) {
    return;
  }

  $first_key = $keys[0];
  $last_key = end($keys);

  if ($first_key != $last_key) {
    $arrays[$first_key]['#attributes']['class'][] = 'first';
    $arrays[$last_key]['#attributes']['class'][] = 'last';
  }
}

/**
 * Function to process the areas
 */
function _layout_process_area(&$layouts, $layout, $variables) {
  // Sort and process each layouts
  foreach ($layout['area'] as $areaname => $areadata) {
    // Skip on disabled areas
    if (!isset($areadata['#enabled']) || $areadata['#enabled'] == DISABLED) {
      continue;
    }

    $layouts[$areaname] = $areadata;
    $layouts[$areaname]['#delta'] = $areaname;
    $layouts[$areaname]['#theme'] = 'section';
    $layouts[$areaname]['theme_hook_suggestions'][] = 'section__' . $areaname;

    // Default attributes
    $attributes = array(
      'id' => $areaname,
      'class' => array('regionarea', 'region-area-' . $areaname, 'row'),
    );

    // Merge with user defined attributes in .layout file if available.
    if (isset($layouts[$areaname]['#attributes']) && is_array($layouts[$areaname]['#attributes'])) {
      $attributes = vtcore_array_merge_recursive_distinct($attributes, $layouts[$areaname]['#attributes']);
    }

    $layouts[$areaname]['#attributes'] = $attributes;

    // Default wrapper attributes
    $wrapper_attributes = array(
      'id' => $areaname . '-wrapper',
      'class' => array('areawrapper', 'center', 'clearfix'),
    );

    // Merge with user defined attributes in .layout file if available.
    if (isset($layouts[$areaname]['#attributes_wrapper']) && is_array($layouts[$areaname]['#attributes_wrapper'])) {
      $wrapper_attributes = vtcore_array_merge_recursive_distinct($wrapper_attributes, $layouts[$areaname]['#attributes_wrapper']);
    }

    $layouts[$areaname]['#attributes_wrapper'] = $wrapper_attributes;

    // Additional classes
    $layouts[$areaname]['#attributes']['class'][] = _layout_column_class($areadata['#column']);

    // Clearfix
    if ($areadata['#clearfix'] == ENABLED) {
      $layouts[$areaname]['#attributes']['class'][] = 'clearfix';
    }

    // New row
    if ($areadata['#newrow'] == ENABLED) {
      $layouts[$areaname]['#attributes']['class'][] = 'newrow';
    }

    // New row
    if ($areadata['#lastrow'] == ENABLED) {
      $layouts[$areaname]['#attributes']['class'][] = 'lastrow';
    }

    // allow other plugin to alter areas array.
    vtcore_alter_process('layout_area', $layouts, $areaname, $areadata);
  }

  // Sort by weight
  uasort($layouts, 'element_sort');

  // Add First and Last Class
  _layout_add_first_last_class($layouts);

}

/**
 * Function to process the region
 */
function _layout_process_region(&$layouts, $layout, $variables) {
  foreach($layout['region'] as $regionname => $regiondata) {

    // No parents area or invalid region data
    if (!isset($layouts[$regiondata['#parent']]) || !isset($variables['page'][$regionname])) {
      continue;
    }

    $element = $regiondata;
    $element['#region'] = $regionname;
    $element['#theme_wrappers'] = array('region');
    $element['#attributes']['class'][] = 'region';
    $element['#attributes']['class'][] = _layout_column_class($regiondata['#column']);

    // Attribute ID
    $element['#attributes']['id'] = 'region-' . str_replace('_', '-', $regionname);

    // Clearfix
    if ($regiondata['#clearfix'] == ENABLED) {
      $element['#attributes']['class'][] = 'clearfix';
    }

    // New row
    if ($regiondata['#newrow'] == ENABLED) {
      $element['#attributes']['class'][] = 'newrow';
    }

    // Last row
    if ($regiondata['#lastrow'] == ENABLED) {
      $element['#attributes']['class'][] = 'lastrow';
    }

    // Check first if parent is set
    if (isset($layouts[$regiondata['#parent']])) {
      $layouts[$regiondata['#parent']][$regionname] = $element;
    }
  }

  // Sort and add first and last class
  foreach ($layouts as $areaname => $areadata) {
    // Sort by weight
    uasort($layouts[$areaname], 'element_sort');
    // Add First and Last Class
    _layout_add_first_last_class($layouts[$areaname]);
  }

}

/**
 * Function to process the block
 */
function _layout_process_block(&$layouts, &$layout, $variables) {
  // Check if we got the right theme, Drupal will switch to
  // current theme when on its configuration page thus breaking
  // the admin theme
  global $theme_key, $vtcore;

  // Process Drupal enabled blocks
  // On admin switched mode
  // @notice : potential bug fix is by adding another
  // checking point to check if user can use admin theme.
  // If that additional checks breaks then this portion
  // need to be rebased.
  if ($vtcore->admin_page == TRUE && $vtcore->admin_theme != $theme_key && $vtcore->user_access_admin_theme) {
    $admin_blocks = vtcore_block_load_blocks($vtcore->admin_theme);
    _layout_process_admintheme_configured_blocks($layouts, $layout, $variables, $admin_blocks, $vtcore->admin_theme);
  }
  // Not on admin switched mode
  else {
    _layout_process_drupal_configured_blocks($layouts, $layout, $variables);
  }

  // Process vtcore configured blocks
  _layout_process_vtcore_configured_blocks($layouts, $layout, $variables);

  // Extra processing
  foreach ($layout['region'] as $regionname => $regiondata) {
    // Sort by weight
    if (!empty($layouts[$regiondata['#parent']][$regionname])) {
      uasort($layouts[$regiondata['#parent']][$regionname], 'element_sort');

      // Add First and Last Class
      _layout_add_first_last_class($layouts[$regiondata['#parent']][$regionname]);
    }
  }
}


/**
 * Helper function to process vtblocks and normal blocks
 * that configured via vtcore.
 */
function _layout_process_vtcore_configured_blocks(&$layouts, &$layout, $variables) {
  // Process block that is registered to the layout
  foreach($layout['block'] as $blockname => $blockdata) {
    // Skip early if no valid parent found
    if (!isset($blockdata['#parent']) || !isset($layout['region'][$blockdata['#parent']])) {
      continue;
    }

    // Get the area
    $regiondata = $layout['region'][$blockdata['#parent']];

    if (!isset($layout['area'][$regiondata['#parent']])) {
      continue;
    }

    $areadata = $layout['area'][$regiondata['#parent']];

    // Skip if we got no valid parents
    if (!isset($layouts[$regiondata['#parent']][$blockdata['#parent']])) {
      continue;
    }

    // If the block aready processed by drupal
    if (isset($blockdata['#processed'])) {
      $layouts[$regiondata['#parent']][$blockdata['#parent']][$blockname] = $blockdata;
      continue;
    }

    // Somehow block hasnt been processed by drupal
    // add them manualy based on its type
    if ($blockdata['#block_type'] == 'vtcore-block') {
      $blockdata['#delta'] = $blockname;
      $render_array = vtcore_special_block_renderable_arrays($blockdata, $variables);

      if (is_array($render_array) && !empty($render_array)) {
        $layouts[$regiondata['#parent']][$blockdata['#parent']][$blockname] = vtcore_array_merge_recursive_distinct($render_array, $blockdata);
      }

      // Debug only
      //$layouts[$regiondata['#parent']][$blockdata['#parent']][$blockname] = $blockdata;
      continue;
    }

    if ($blockdata['#block_type'] == 'block') {
      $delta = explode('_', $blockname);
      $block_object = block_load($delta[0], $delta[1]);

      // Only render if block is eligible for visible
      $blocks = array($block_object);
      if (vtcore_block_check_visibility($blocks, FALSE)) {
        $block = array($blockname => $block_object);
        $block_array = _block_render_blocks($block);
        $build = _block_get_renderable_array($block_array);

        // Fix for block got empty array, Possible scenario, block is not set to be visible
        if (isset($build[$blockname]) && is_array($build[$blockname])) {
          $layouts[$regiondata['#parent']][$blockdata['#parent']][$blockname] = vtcore_array_merge_recursive_distinct($build[$blockname], $blockdata);
        }
      }
    }
  }
}

/**
 * Helper function to process blocks that enabled
 * via Drupal block configuration page
 */
function _layout_process_drupal_configured_blocks(&$layouts, &$layout, $variables) {
  // Process blocks that is not registered in the layout
  // Possible cause is user set this block via drupal block configuration page
  $regions = element_children($variables['page']);
  foreach ($regions as $regionname) {
    if (!isset($layout['region'][$regionname])) {
      continue;
    }

    $regiondata = $layout['region'][$regionname];
    $children = element_children($variables['page'][$regionname]);

    if (empty($children)) {
      continue;
    }
    // Process Children
    foreach ($children as $block_delta) {
      // Check first if we got valid parents
      if (!isset($layouts[$regiondata['#parent']][$regionname])) {
        continue;
      }

      $blockdata = $variables['page'][$regionname][$block_delta];

      // Give default VTCore blocks configuration if this
      // block hasn't been configured yet
      if (empty($layout['block'][$block_delta])) {
        $blockdata['#name'] = $block_delta;
        $blockdata['#block_type'] = 'block';
        $blockdata['#parent'] = $regionname;
        $blockdata['#column'] = '12';
        $blockdata['#clearfix'] = '1';
        $blockdata['#newrow'] = '1';
        $blockdata['#lastrow'] = '1';
      }

      // Skip configured blocks
      if (isset($layout['block'][$block_delta])) {
        // Merge the renderable arrays
        $layout['block'][$block_delta] = vtcore_array_merge_recursive_distinct($blockdata, $layout['block'][$block_delta]);
        $layout['block'][$block_delta]['#processed'] = TRUE;
        continue;
      }

      $layouts[$regiondata['#parent']][$regionname][$block_delta] = $blockdata;
    }
  }
}

/**
 * Helper function to process blocks that is enabled
 * for admin theme, possible case is the theme configuration
 * page uses frontend theme while admin theme is not the
 * front end theme, thus we need to force load the admin
 * theme enabled block for sane block visibility.
 */
function _layout_process_admintheme_configured_blocks(&$layouts, &$layout, $variables, $admin_blocks, $theme_key) {

  foreach ($admin_blocks as $region => $blocks) {
    // Skip if region is disabled via vtcore
    // or we got phantom region
    if (!isset($layout['region'][$region])
        || (isset($layout['region'][$region]['#parent'])
        && $layout['region'][$region]['#parent'] == 'disabled')) {
      continue;
    }

    // Skip if parent area is disabled;
    $area = $layout['region'][$region]['#parent'];
    if ($layout['area'][$area]['#enabled'] == 0) {
      continue;
    }

    // Sort our block visibility manually, Drupal
    // haven't got the chance to sort this under the
    // proper theme_key
    vtcore_block_check_visibility($blocks, $theme_key);

    foreach ($blocks as $block_delta => $blockdata) {
      $delta = explode('_', $block_delta);
      $block = array($block_delta => block_load($delta[0], $delta[1]));
      $block_array = _block_render_blocks($block);
      $build = _block_get_renderable_array($block_array);

      // Fix for block got empty array, Possible scenario, block is not set to be visible
      if (isset($build[$block_delta]) && is_array($build[$block_delta])) {
        if (isset($layout['block'][$block_delta])&& is_array($layout['block'][$block_delta])) {
          $layout['block'][$block_delta] = vtcore_array_merge_recursive_distinct($build[$block_delta], $layout['block'][$block_delta]);
        }
        else {
          $layout['block'][$block_delta] = $build[$block_delta];
        }
        $layout['block'][$block_delta]['#processed'] = TRUE;
        $layouts[$area][$region][$block_delta] = $layout['block'][$block_delta];
      }
    }
  }
}

/**
 * Helper function to delete empty areas and regions
 */
function _layout_hide_empty_region(&$layouts) {
  $areas = element_children($layouts);

  foreach ($areas as $area) {
    $regions = element_children($layouts[$area]);

    // Remove theme_hook_suggestions
    if ($regions[0] == 'theme_hook_suggestions') {
      unset($regions[0]);
    }

    // Remove area if empty and set to be hidden when empty
    if (empty($regions) && $layouts[$area]['#disable_empty'] == 1) {
      unset($layouts[$area]);
      continue;
    }

    $region_count = 0;
    foreach ($regions as $region) {
      // Most probably this is not a region
      if (!isset($layouts[$area][$region]['#region'])) {
        continue;
      }

      $blocks = element_children($layouts[$area][$region]);

      // Remove theme_hook_suggestions
      if (isset($blocks[0]) && $blocks[0] == 'theme_hook_suggestions') {
        unset($blocks[0]);
      }

      // backward compatibility, before alpha 16 there are no key
      // for region disabled empty
      if (!isset($layouts[$area][$region]['#disable_empty'])) {
        $layouts[$area][$region]['#disable_empty'] = 0;
      }

      // Disable when we got no blocks under this region
      if (empty($blocks) && $layouts[$area][$region]['#disable_empty'] == 1) {
        unset($layouts[$area][$region]);
        continue;
      }

      $region_count++;
    }

    if ($region_count == 0) {
      unset($layouts[$area]);
    }
  }
}

/**
 * Helper function to move all page properties to new layout
 */
function _layout_top_level_register_properties(&$layouts, $variables) {
  // Register all element properties
  $properties = element_properties($variables['page']);
  foreach ($properties as $property_key) {
    $layouts[$property_key] = $variables['page'][$property_key];
  }
}