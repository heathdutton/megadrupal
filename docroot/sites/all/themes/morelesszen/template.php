<?php

/**
 * @file
 * Contains theme override functions and process & preprocess functions for Zentropy
 */

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('morelesszen_clear_registry')) {
  // Rebuild .info data.
  system_rebuild_theme_data();
  // Rebuild theme registry.
  drupal_theme_rebuild();
}

/**
 * Implements template_html_head_alter();
 *
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function morelesszen_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * Implements template_proprocess_search_block_form().
 *
 * Changes the search form to use the HTML5 "search" input attribute
 */
function morelesszen_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}

/**
 * Implements template_preprocess().
 */
function morelesszen_preprocess(&$vars, $hook) {
  $vars['morelesszen_path'] = base_path() . path_to_theme();
}

/**
 * Implements template_preprocess_html().
 */
function morelesszen_preprocess_html(&$vars) {
  
  $vars['doctype'] = _morelesszen_doctype();
  $vars['rdf'] = _morelesszen_rdf($vars);

  // Since menu is rendered in preprocess_page we need to detect it here to add body classes
  $has_main_menu = theme_get_setting('toggle_main_menu');
  $has_secondary_menu = theme_get_setting('toggle_secondary_menu');

  /* Add extra classes to body for more flexible theming */

  if ($has_main_menu or $has_secondary_menu) {
    $vars['classes_array'][] = 'with-navigation';
  }

  if ($has_secondary_menu) {
    $vars['classes_array'][] = 'with-subnav';
  }

  if (!empty($vars['page']['featured'])) {
    $vars['classes_array'][] = 'featured';
  }

  if (!empty($vars['page']['triptych_first'])
    || !empty($vars['page']['triptych_middle'])
    || !empty($vars['page']['triptych_last'])) {
    $vars['classes_array'][] = 'triptych';
  }

  if (!empty($vars['page']['footer_firstcolumn'])
    || !empty($vars['page']['footer_secondcolumn'])
    || !empty($vars['page']['footer_thirdcolumn'])
    || !empty($vars['page']['footer_fourthcolumn'])) {
    $vars['classes_array'][] = 'footer-columns';
  }

  if ($vars['is_admin']) {
    $vars['classes_array'][] = 'admin';
  }

  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    $temp = explode('/', $path, 2);
    $section = array_shift($temp);
    $page_name = array_shift($temp);

    if (isset($page_name)) {
      $vars['classes_array'][] = morelesszen_id_safe('page-' . $page_name);
    }

    $vars['classes_array'][] = morelesszen_id_safe('section-' . $section);

    // add template suggestions
    $vars['theme_hook_suggestions'][] = "page__section__" . $section;
    $vars['theme_hook_suggestions'][] = "page__" . $page_name;

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-add'; // Add 'section-node-add'
      } elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-' . arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }

}

function _morelesszen_menu_links($menu) {
  $data = menu_tree_page_data($menu, 1);
  if (module_exists('i18n_menu')) {
    $data = i18n_menu_localize_tree($data);
  }
  return menu_tree_output($data);
}

/**
 * Implements template_preprocess_page().
 */
function morelesszen_preprocess_page(&$vars) {
  $vars['main_menu'] =      theme_get_setting('toggle_main_menu') ?
    _morelesszen_menu_links(variable_get('menu_main_links_source', 'main-menu')) : array();
  $vars['secondary_menu'] = theme_get_setting('toggle_secondary_menu') ?
    _morelesszen_menu_links(variable_get('menu_secondary_links_source', 'user-menu')) : array();

  if (isset($vars['node_title'])) {
    $vars['title'] = $vars['node_title'];
  }

  // Adding classes wether #navigation is here or not
  if (!empty($vars['main_menu']) or !empty($vars['sub_menu'])) {
    $vars['classes_array'][] = 'with-navigation';
  }

  if (!empty($vars['secondary_menu'])) {
    $vars['classes_array'][] = 'with-subnav';
  }

  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($vars['title_suffix']['add_or_remove_shortcut']) && $vars['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $vars['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $vars['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $vars['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
  
  if(!theme_get_setting('morelesszen_feed_icons')) {
    $vars['feed_icons'] = '';
  }
}

/**
 * Implements template_preprocess_maintenance_page().
 */
function morelesszen_preprocess_maintenance_page(&$vars) {
  // Manually include these as they're not available outside template_preprocess_page().
  $vars['rdf_namespaces'] = drupal_get_rdf_namespaces();
  $vars['grddl_profile'] = 'http://www.w3.org/1999/xhtml/vocab';

  $vars['doctype'] = _morelesszen_doctype();
  $vars['rdf'] = _morelesszen_rdf($vars);

  if (!$vars['db_is_active']) {
    unset($vars['site_name']);
  }

  drupal_add_css(drupal_get_path('theme', 'morelesszen') . '/css/maintenance-page.css');
}

/**
 * Implements template_preprocess_node().
 *
 * Adds extra classes to node container for advanced theming
 */
function morelesszen_preprocess_node(&$vars) {
  // Striping class
  $vars['classes_array'][] = 'node-' . $vars['zebra'];

  // Node is published
  $vars['classes_array'][] = ($vars['status']) ? 'published' : 'unpublished';

  // Node has comments?
  $vars['classes_array'][] = ($vars['comment']) ? 'with-comments' : 'no-comments';

  if ($vars['sticky']) {
    $vars['classes_array'][] = 'sticky'; // Node is sticky
  }

  if ($vars['promote']) {
    $vars['classes_array'][] = 'promote'; // Node is promoted to front page
  }

  if ($vars['teaser']) {
    $vars['classes_array'][] = 'node-teaser'; // Node is displayed as teaser.
  }

  if ($vars['uid'] && $vars['uid'] === $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }
  
  $vars['submitted'] = t('Submitted by !username on ', array('!username' => $vars['name']));
  $vars['submitted_date'] = t('!datetime', array('!datetime' => $vars['date']));
  $vars['submitted_pubdate'] = format_date($vars['created'], 'custom', 'Y-m-d\TH:i:s');
  
  if ($vars['view_mode'] == 'full' && node_is_page($vars['node'])) {
    $vars['classes_array'][] = 'node-full';
  }
}

/**
 * Implements template_preprocess_block().
 */
function morelesszen_preprocess_block(&$vars, $hook) {
  // Add a striping class.
  $vars['classes_array'][] = 'block-' . $vars['zebra'];

  // In the header region visually hide block titles.
  if ($vars['block']->region == 'header') {
    $vars['title_attributes_array']['class'][] = 'element-invisible';
  }
}

/**
 * Implements theme_menu_tree().
 */
function morelesszen_menu_tree($vars) {
  return '<ul class="menu clearfix">' . $vars['tree'] . '</ul>';
}

/**
 * Implements theme_field__field_type().
 */
function morelesszen_field__taxonomy_term_reference($vars) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<h3 class="field-label">' . $vars['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ( $vars['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($vars['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $vars['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $vars['classes'] . (!in_array('clearfix', $vars['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}

/**
 *  Return a themed breadcrumb trail
 */
function morelesszen_breadcrumb($vars) {
  
  $breadcrumb = isset($vars['breadcrumb']) ? $vars['breadcrumb'] : array();
  
  if (theme_get_setting('morelesszen_breadcrumb_hideonlyfront')) {
    $condition = count($breadcrumb) > 1;
  } else {
    $condition = !empty($breadcrumb);
  }
  
  if(theme_get_setting('morelesszen_breadcrumb_showtitle')) {
    $title = drupal_get_title();
    if(!empty($title)) {
      $condition = true;
      $breadcrumb[] = $title;
    }
  }
  
  $separator = theme_get_setting('morelesszen_breadcrumb_separator');

  if (!$separator) {
    $separator = '»';
  }
  
  if ($condition) {
    return implode(" {$separator} ", $breadcrumb);
  }
}

/**
 * Determine whether to output Google Analytics tracking code
 *
 * @return bool
 */
function morelesszen_ga_enabled() {
  if (!theme_get_setting('morelesszen_ga_enable')) {
    return FALSE;
  }

  global $user;
  $roles_orig = theme_get_setting('morelesszen_ga_trackroles');

  // theme_get_setting() doesn't allow specifying default values so provide one here
  if (!is_array($roles_orig)) {
    $roles_orig = array();
  }

  // remove roles with permission
  $roles = array_filter($roles_orig);

  // get intersection of user's roles and roles without permission
  $intersect = array_intersect(array_keys($user->roles), array_keys($roles));

  return empty($intersect);
}

/**
 * Determine whether to show floating tabs
 *
 * @return bool
 */
function morelesszen_tabs_float() {
  $float = (bool) theme_get_setting('morelesszen_tabs_float');
  $float_node = (bool) theme_get_setting('morelesszen_tabs_node');
  $is_node = (arg(0) === 'node' && is_numeric(arg(1)));

  if ($float) {
    return ($float_node) ? $is_node : TRUE;
  }

  return FALSE;
}

/*
 * 	Converts a string to a suitable html ID attribute.
 *  Taken from "basic"
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

function morelesszen_id_safe($string) {
  // Strip accents
  $accents = '/&([A-Za-z]{1,2})(tilde|grave|acute|circ|cedil|uml|lig);/';
  $string = preg_replace($accents, '$1', htmlentities($string));
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9_-]+/', '-', $string));
  // If the first character is not a-z, add 'n' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id' . $string;
  }
  return $string;
}

/**
 * Generate doctype for templates
 */
function _morelesszen_doctype() {
  return (module_exists('rdf')) ? '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN"' . "\n" . '"http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">' : '<!DOCTYPE html>' . "\n";
}

/**
 * Generate RDF object for templates
 *
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 *
 * @param array $vars
 */
function _morelesszen_rdf($vars) {
  $rdf = new stdClass();

  if (module_exists('rdf')) {
    $rdf->version = 'version="HTML+RDFa 1.1"';
    $rdf->namespaces = $vars['rdf_namespaces'];
    $rdf->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $rdf->version = '';
    $rdf->namespaces = '';
    $rdf->profile = '';
  }

  return $rdf;
}

/**
 * Generate the HTML output for a menu link and submenu.
 *
 * @param $vars
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function morelesszen_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  // Adding a class depending on the TITLE of the link (not constant)
  $element['#attributes']['class'][] = morelesszen_id_safe(strip_tags($element['#title']));
  // Adding a class depending on the ID of the link (constant)
  $element['#attributes']['class'][] = 'mid-' . $element['#original_link']['mlid'];
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Override or insert variables into theme_menu_local_task().
 */
function morelesszen_preprocess_menu_local_task(&$vars) {
  $link = & $vars['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}

/**
 *  Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function morelesszen_menu_local_tasks(&$vars) {
  $output = '';

  if (!empty($vars['primary'])) {
    $vars['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $vars['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $vars['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['primary']);
  }

  if (!empty($vars['secondary'])) {
    $vars['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $vars['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $vars['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['secondary']);
  }

  return $output;
}

/**
 * Implementation of hook_preprocess_menu_link().
 *
 * This allows menus that are not primary/secondary menus to get
 * the "active" and "active-trail" class assigned to them. This assumes they are
 * using theme('menu_link') for the menu rendering to html.
 */
function morelesszen_preprocess_menu_link(&$variables) {
  if (!function_exists('context_active_contexts'))
    return;
  if( ($contexts = context_active_contexts()) ){
    foreach($contexts as $context){
      if((isset($context->reactions['menu']))){
        if ($variables['element']['#href'] == $context->reactions['menu']) {
          $variables['element']['#localized_options']['attributes']['class'][] = 'active';
          $variables['element']['#localized_options']['attributes']['class'][] = 'active-trail';
          $variables['element']['#attributes']['class'][] = 'active';
          $variables['element']['#attributes']['class'][] = 'active-trail';
        }
      }
    }
  }
}

/**
 * Implements template_preprocess_field() for main pic/vid
 */
 
function morelesszen_field__image($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }

  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    if (isset($item['#bundle'])) {
      $classes .= ' media-type-' . $item['#bundle'];
    }
    $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * helper function: returns TRUE if context $cname is active
 */
function morelesszen_context_is_active($cname) {
  if (!function_exists('context_active_contexts'))
    return FALSE;
  $contexts = context_active_contexts();
  if (isset($contexts[$cname])) {
    return TRUE;
  }
  foreach ($contexts as $context) {
    if ($context->name == $cname) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Implements theme_field__field_type().
 */
function morelesszen_field__field_heading($variables) {
  $output = '';

  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    $output .= '<h2 class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</h2>';
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Prepares variables for webform view
 * to set classes indicating the current webform page.
 *
 * Default function: theme_webform_view().
 */
function morelesszen_preprocess_webform_view(&$vars) {
  $page_num = $vars['webform']['#form']['details']['page_num']['#value'];
  $page_count = $vars['webform']['#form']['details']['page_count']['#value'];

  $vars['webform']['#form']['#attributes']['class'][] = 'page-' . $page_num;
  if ($page_num == $page_count) {
    $vars['webform']['#form']['#attributes']['class'][] = 'page-last';
  }
}
