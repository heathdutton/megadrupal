<?php
/**
 * @file template.php
 * Pop base theme.
 */

/*
 * Tip: Use devel module to clean theme registry during development.
 */

/**
 * Implements hook_html_head_alter().
 *
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function pop_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );

}

/**
 * Implements hook_preprocess_html().
 *
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 *
 * Added mobile viewport.
 */
function pop_preprocess_html(&$vars) {
  // RDFa support
  if (module_exists('rdf')) {
    $vars['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $vars['rdf']->version = 'version="HTML+RDFa 1.1"';
    $vars['rdf']->namespaces = $vars['rdf_namespaces'];
    $vars['rdf']->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $vars['doctype'] = '<!DOCTYPE html>' . "\n";
    $vars['rdf']->version = '';
    $vars['rdf']->namespaces = '';
    $vars['rdf']->profile = '';
  }

  // Mobile viewport optimization
  $elements = array(
    'mobile_handheldfriendly' => array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name'    => 'HandheldFriendly',
        'content' => 'True',
      ),
    ),
    'mobile_mobileoptimized' => array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name'    => 'MobileOptimized',
        'content' => '320',
      ),
    ),
    'mobile_viewport' => array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name'    => 'viewport',
        'content' => 'width=device-width',
      ),
    ),
  );
  foreach ($elements as $k => $element) {
    drupal_add_html_head($element, $k);
  }

  // Grid overlay
  if (theme_get_setting('enable_dev_settings')) {
    $vars['classes_array'][] = (theme_get_setting('grid_overlay')) ? 'grid-overlay-enabled' : '';
  }

  // Sidebars layout
  if (theme_get_setting('enable_layout_settings')) {
    $vars['classes_array'][] = theme_get_setting('sidebars_layout');
    if (theme_get_setting('equal_heights_sidebars')) {
      $vars['classes_array'][] = "pop-equal-heights";
    }
  }

  // Add conditional stylesheets for IE
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Implements hook_preprocess_search_block_form().
 *
 * Changes the search form to use the HTML5 "search" input attribute
 */
function pop_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}

/**
 * Implements hook_form_alter() for form_search_block.
 *
 * Theme customization of the search box.
 */
function pop_form_search_block_form_alter(&$form, &$form_state) {
  if (theme_get_setting('enable_search_box_settings')) {
    if (theme_get_setting('search_box_label_inline')) {
      // Enable search box HTML placeholder text
      $form['search_block_form']['#attributes']['placeholder'] = t(theme_get_setting('search_box_label_text'));
    }
    else {
      // Modify label of the search form
      $form['search_block_form']['#title'] = t(theme_get_setting('search_box_label_text'));
    }

    if (theme_get_setting('search_box_tooltip_text') != '<none>') {
    // Search box tooltip text
      $form['search_block_form']['#attributes']['title'] = t(theme_get_setting('search_box_tooltip_text'));
    }
    else {
      unset($form['search_block_form']['#attributes']['title']);
    }

    if (theme_get_setting('search_box_button')) {
      // Change the text on the submit button
      $form['actions']['submit']['#value'] = t(theme_get_setting('search_box_button_text'));
    }
    else {
      unset($form['actions']['submit']);
    }
  }
}

/*--- Helper functions ---*/
/**
 * Include templates files in theme, under templates subfolder
 */
function _theme_load_include($type, $theme, $name = NULL) {
  if (empty($name)) {
    $name = $theme;
  }

  $file = './'. _get_pop_theme_path($theme) ."/$name.$type";

  if (is_file($file)) {
    require_once $file;
    return TRUE;
  }
  else {
    return FALSE;
  }
}
