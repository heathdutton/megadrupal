<?php

/**
 * Loading VTCore Files
 * Without this the layout will break
 * and additional plugins won't get loaded
 */
if (!function_exists('vtcore_start')) {
  include_once('vtcore/core/vtcore.inc.php');
}

/**
 * Implements hook_theme_registry_alter()
 */
function sigmaone_theme_registry_alter(&$theme_registry) {
  $plugins = vtcore_scan_plugin();
  foreach ($theme_registry as $theme_name => $themedata) {
    foreach ($plugins as $plugin_name => $data) {
      if ($data['status'] == 0) {
        continue;
      }
      $theme_registry[$theme_name]['preprocess functions'][] = $plugin_name . '_vtcore_preprocess_' . $theme_name;
      $theme_registry[$theme_name]['process functions'][] = $plugin_name . '_vtcore_process_' . $theme_name;
    }
  }
}

/**
 * Implements hook_css_alter()
 */
function sigmaone_css_alter(&$css) {
  // create a new hook for plugin to preprocess $variables;
  vtcore_alter_process('vtcore_css', $css);
}

/**
 * Implements hook_js_alter()
 */
function sigmaone_js_alter(&$javascript) {
  // create a new hook for plugin to preprocess $variables;
  vtcore_alter_process('vtcore_js', $javascript);
}

/**
 * Implements hook_block_view_alter()
 */
function sigmaone_block_view_alter(&$data, $block) {
  // create a new hook for plugin to preprocess $variables;
  vtcore_alter_process('vtcore_block_view', $data, $block);
}

/**
 * Implements hook_form_alter()
 */
function sigmaone_form_alter(&$form, &$form_state, $form_id) {
  // create a new hook for plugin to preprocess $variables;
  vtcore_alter_process('vtcore_form', $form, $form_state, $form_id);
}

/**
 * Implements hook_theme()
 */
function sigmaone_theme($existing, $type, $theme, $path) {
  $variables = array();

  // Generic table for form
  $variables['tableform'] = array(
    'function' => 'sigma_vtcore_tableform',
    'type' => 'theme_engine',
    'render element' => 'element',
  );

  vtcore_alter_process('vtcore_theme', $variables, $path);
  return $variables;
}

/**
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function sigmaone_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * Override or insert variables into the html template.
 */
function sigmaone_preprocess_html(&$variables) {
  // Ugly fix for metatags module
  // This should be removed when metatags module fixes bug 1310780
  if (isset($variables['page']['content']['metatags'])) {
    render($variables['page']['content']['metatags']);
  }

  drupal_add_css(path_to_theme() . '/css/ie7-fixes.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the page template.
 */
function sigmaone_preprocess_page(&$variables) {
  // decode html entity in title
  $variables['title'] = html_entity_decode(drupal_get_title());
}

/**
 * Preprocess maintenance page
 *
 * Adding additional css for maintenance page
 */
function sigmaone_preprocess_maintenance_page(&$variables) {
  drupal_add_css(path_to_theme() . '/vtcore/plugins/layout/css/1140.css', 'theme');
  drupal_add_css(path_to_theme() . '/css/maintenance.css', 'theme');

  if (empty($variables['site_name'])) {
    $variables['site_name'] = variable_get('site_name', 'SigmaOne');
  }

  if (empty($variables['site_slogan'])) {
    $variables['site_slogan'] = variable_get('site_slogan', 'Theme Framework');
  }
}

/**
 * Theme fieldset
 */
function sigmaone_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));

  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend><span class="fieldset-legend">' . $element['#title'] . '</span></legend>';
  }
  $output .= '<div class="fieldset-wrapper clearfix">';
  if (!empty($element['#description'])) {
    $output .= '<div class="fieldset-description">' . $element['#description'] . '</div>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div>';
  $output .= "</fieldset>\n";
  return $output;
}



/**
 * Implements hook_process_html()
 */
function sigmaone_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

/**
 * Implements hook_process_page()
 */
function sigmaone_process_page(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
}

/**
 * Implements hook_preprocess_node().
 * This is temporary solution until a node
 * specific vtcore plugin is created.
 */
function sigmaone_preprocess_node(&$variables) {
  $node = $variables['node'];

  // Formatted date
  $variables['formatted_created'] = format_date($node->created, 'custom', 'd M Y');

  // Add owner object to the node variables
  $variables['owner'] = user_load($node->uid);

  // Improve node classes
  if ($variables['teaser'] == TRUE) {
    $variables['classes_array'][] = 'node-teaser';
  }
  else {
    $variables['classes_array'][] = 'node-not-teaser';
  }

  // This will only detect default field images
  if (!empty($variables['content']['field_image'])) {
    $variables['classes_array'][] = 'node-with-image';
  }

  // Add zebra class
  $variables['classes_array'][] = $variables['zebra'];

  // Add clearfix class
  $variables['classes_array'][] = 'clearfix';
}

/**
 * Implement hook_preprocess_comment().
 */
function sigmaone_preprocess_comment(&$variables) {
  $comment = $variables['comment'];
  $account = user_load($comment->uid);

  // Adding extra classes for each role user has.
  if (isset($account->roles) && is_array($account->roles)) {
    foreach ($account->roles as $role) {
      if ($role == 'authenticated user') {
        continue;
      }
      $variables['classes_array'][] = 'comment-by-' . str_replace(' ', '-', strtolower($role));
    }
  }
}

/**
 * Implement hook_preprocess_comment_wrapper().
 */
function sigmaone_preprocess_comment_wrapper(&$variables) {
  if (isset($variables['node']) && is_object($variables['node'])) {
    // Adding extra theme hook suggestion per node type
    // So comment-wrapper-(node type).tpl.php will work.
    $variables['theme_hook_suggestions'][] = 'comment_wrapper__' . $variables['node']->type;

    // Adding extra theme_hook_suggestion for per node nid & node type
    $variables['theme_hook_suggestions'][] = 'comment_wrapper__' . $variables['node']->type . '__' . $variables['node']->nid;
  }
}

/**
 * Addtional theme for theming form as a table
 *
 * Currently it will support if the form array structure is :
 *
 * $form['container'] = array(
 * 	'#type' => 'container',
 * 	'#theme' => 'themeform',
 * 	'#header' => array(array of headers);
 * );
 *
 * $form['container'][]['something'] = array( .... );
 * $form['container'][]['something']['something'] = array( .... );
 *
 * Note : Only tested for 2 level of nesting and only 1 containers
 *
 * @todo : add features for table id, draggable, sortable, row id, etc
 */
function sigma_vtcore_tableform(&$variables) {
  $element = $variables['element'];
  $header = $element['#header'];

  // Loop into children to build the row
  $rows = array();

  foreach (element_children($element) as $key) {
    $row = array();
    $row_element = $element[$key];

    foreach (element_children($row_element) as $cell_key) {
      $cell_element = $row_element[$cell_key];

      // This is for form element type value.
      if (isset($cell_element['#type']) && $cell_element['#type'] == 'value') {
        $row[] = $cell_element['#value'];
      }

      // This is for other form element.
      if (isset($cell_element['#type']) && $cell_element['#type'] != 'value') {
        unset($cell_element['#title']);
        $row[] = drupal_render($cell_element);
      }

      // Probably we got another children element
      // @Todo : do we need to utilize infinite looping to grab all children level?
      if (!isset($cell_element['#type'])) {
        foreach (element_children($cell_element) as $subcell_key) {
          $subcell_element = $cell_element[$subcell_key];

          // This is for form element tye value.
          if (isset($subcell_element['#type']) && $subcell_element['#type'] == 'value') {
            $row[] = $subcell_element['#value'];
          }

          // This is for other form element.
          if (isset($subcell_element['#type']) && $subcell_element['#type'] != 'value') {
            unset($subcell_element['#title']);
            $row[] = drupal_render($subcell_element);
          }
        }
      }
    }

    $rows[] = array('data' => $row);
  }

  return theme('table', array('header' => $header, 'rows' => $rows));

}