<?php
/*  Drupal starter theme provided by gazwal.com - Freelance Drupal Development Services  */

/*
 * Here we override the default HTML output of drupal.
 * refer to http://drupal.org/node/550722
 */


// Auto-rebuild the theme registry during theme development. (from Zen)
if (theme_get_setting('clear_registry')) {
  system_rebuild_theme_data();
}


//******************  preprocess_html  *****************************************

function fueldeluxe_preprocess_html(&$variables) {

  // Add conditional stylesheets for IE (from Bartik)
  drupal_add_css(path_to_theme() . '/css/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));

  // Add admin class
  if ($variables['is_admin']) {
    $variables['classes_array'][] = 'admin';
  }

  // Add classes for the font styles settings
  $variables['classes_array'][] = theme_get_setting('font_family');
  $variables['classes_array'][] = theme_get_setting('font_size');


  // Add unique classes for each page and website section
  if (!$variables['is_front']) {
    $path = drupal_get_path_alias($_GET['q']);
    $temp = explode('/', $path, 2);
    $section = array_shift($temp);
    $page_name = array_shift($temp);

    if (isset($page_name)) {
      $variables['classes_array'][] = fueldeluxe_id_safe('page-'. $page_name);
    }

    $variables['classes_array'][] = fueldeluxe_id_safe('section-'. $section);

    // node add/edit/delete classes
    $variables['theme_hook_suggestions'][] = "page__section__" . $section;
    $variables['theme_hook_suggestions'][] = "page__" . $page_name;

    // add section-node-add & 
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($body_classes); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-add'; // Add 'section-node-add'
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($body_classes); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-'. arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }


} // end preprocess_html



//******************  preprocess_page  *****************************************

function fueldeluxe_preprocess_page(&$variables) {

  // Page template suggestions related to node type
  // If the node type is "custom_blog" the template suggestion will be page--type--custom_blog.tpl.php.
  if (isset($variables['node'])) {
    $variables['theme_hook_suggestions'][] = 'page__type__'. $variables['node']->type;
  }

} // end preprocess_page



//******************  process_page  *****************************************

/**
 * Override or insert variables into the page template.
 */
function fueldeluxe_process_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.(from Bartik)
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;

  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.(from Bartik)
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.(from Bartik)
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }

} // end process_page



//******************  preprocess_node  *****************************************

/**
 * Override or insert variables into the node template.
 */
function fueldeluxe_preprocess_node(&$variables) {
  // Add a $submitted variable.(from Bartik)
  $variables['submitted'] = t('published by !username on !datetime', array('!username' => $variables['name'], '!datetime' => $variables['date']));
  // Add a striping class.(from Basic)
  $variables['classes_array'][] = 'node-' . $variables['zebra'];
  // Add node-full class.(from Bartik)
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }

} // end preprocess_node



//******************  preprocess_block  *****************************************

function fueldeluxe_preprocess_block(&$variables) {
  // Add a striping class.
  $variables['classes_array'][] = 'block-' . $variables['zebra'];

} // end preprocess_block



//******************  functions  *****************************************

/**
 * Implements theme_menu_tree().(from Bartik)
 */
// theme_menu_tree($variables) / see Navigation menu en Management menu
// http://api.drupal.org/api/drupal/includes--menu.inc/function/theme_menu_tree/7
function fueldeluxe_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}



/**
 * Implements theme_field__field_type().(from Bartik)
 */
function fueldeluxe_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h3 class="field-label">' . $variables['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}



/**
 * Return a themed breadcrumb trail.
 *
 * @param unknown $breadcrumb
 *   An array containing the breadcrumb links.
 * @return
 *   A string containing the breadcrumb output.
 */
function fueldeluxe_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  // Determine if we are to display the breadcrumb.
  $show_breadcrumb = theme_get_setting('fdl_breadcrumb');
  if ($show_breadcrumb == 'yes' || $show_breadcrumb == 'admin' && arg(0) == 'admin') {

    // Optionally get rid of the homepage link.
    $show_breadcrumb_home = theme_get_setting('fdl_breadcrumb_home');
    if (!$show_breadcrumb_home) {
      array_shift($breadcrumb);
    }

    // Return the breadcrumb with separators.
    if (!empty($breadcrumb)) {
      $breadcrumb_separator = theme_get_setting('fdl_breadcrumb_separator');
      $trailing_separator = $title = '';
      if (theme_get_setting('fdl_breadcrumb_title')) {
        if ($title = drupal_get_title()) {
          $trailing_separator = $breadcrumb_separator;
        }
      }
      elseif (theme_get_setting('fdl_breadcrumb_trailing')) {
        $trailing_separator = $breadcrumb_separator;
      }
      return '<div class="breadcrumb">' . implode($breadcrumb_separator, $breadcrumb) . "$trailing_separator$title</div>";
    }
  }
  // Otherwise, return an empty string.
  return '';
}


/*
 * 	Converts a string to a suitable html ID attribute.
 *
 * 	 http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
 * 	 valid ID attribute in HTML. This function:
 *
 * 	- Ensure an ID starts with an alpha character by optionally adding an 'n'.
 * 	- Replaces any character except A-Z, numbers, and underscores with dashes.
 * 	- Converts entire string to lowercase.
 *
 * 	@param $string
 * 	  The string
 * 	@return
 * 	  The converted string
 */

function fueldeluxe_id_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9_-]+/', '-', $string));
  // If the first character is not a-z, add 'n' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id'. $string;
  }
  return $string;
}


/**
 * Generate the HTML output for a menu link and submenu.
 *
 * @param unknown $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */

function fueldeluxe_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  // Adding a class depending on the TITLE of the link (not constant)
  $element['#attributes']['class'][] = fueldeluxe_id_safe($element['#title']);
  // Adding a class depending on the ID of the link (constant)
  $element['#attributes']['class'][] = 'mid-' . $element['#original_link']['mlid'];
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


/*
 * 	Customize the PRIMARY and SECONDARY LINKS, to allow the admin tabs to work on all browsers
 */

function fueldeluxe_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link['localized_options']['html'] = TRUE;
  return '<li' . (!empty($variables['element']['#active']) ? ' class="active"' : '') . '>' . l('<span class="tab">' . $link['title'] . '</span>', $link['href'], $link['localized_options']) . "</li>\n";
}


/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs. (from ZEN)
 */
function fueldeluxe_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}


/**
 * Implements hook_form_alter() for form_search_block.
 *
 * Theme customization of the search box. (from Pop)
 */
function fueldeluxe_form_search_block_form_alter(&$form, &$form_state) {
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

