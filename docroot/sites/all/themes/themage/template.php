<?php
/**
 * @file
 * Contains theme override and preprocess functions.
 */

/**
 * Implements template_preprocess_html().
 *
 * @see https://gist.github.com/pascalduez/1417914
 */
function themage_preprocess_html(&$vars) {
  // Add Lobster font.
  drupal_add_css('http://fonts.googleapis.com/css?family=Lobster', array('type' => 'external'));

  // Site is responsive, make sure initial scale is appropriate.
  $viewport = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1',
    ),
  );
  drupal_add_html_head($viewport, 'viewport');

  $vars['html_attributes_array'] = array();
  $vars['body_attributes_array'] = array();

  // HTML element attributes.
  $vars['html_attributes_array']['lang'] = $vars['language']->language;
  $vars['html_attributes_array']['dir']  = $vars['language']->dir;

  // Adds RDF namespace prefix bindings in the form of an RDFa 1.1 prefix
  // attribute inside the html element.
  if (function_exists('rdf_get_namespaces')) {
    $vars['rdf'] = new stdClass();
    foreach (rdf_get_namespaces() as $prefix => $uri) {
      $vars['rdf']->prefix .= $prefix . ': ' . $uri . "\n";
    }
    $vars['html_attributes_array']['prefix'] = $vars['rdf']->prefix;
  }

  // BODY element attributes.
  $vars['body_attributes_array']['class'] = $vars['classes_array'];
  $vars['body_attributes_array'] += $vars['attributes_array'];
  $vars['attributes_array'] = '';
}

/**
 * Implements template_process_html().
 *
 * @see https://gist.github.com/pascalduez/1417914
 */
function themage_process_html(&$vars) {
  // Flatten out html_attributes and body_attributes.
  $vars['html_attributes'] = drupal_attributes($vars['html_attributes_array']);
  $vars['body_attributes'] = drupal_attributes($vars['body_attributes_array']);
}

/**
 * Implements hook_html_head_alter().
 *
 * @see https://gist.github.com/pascalduez/1417914
 */
function themage_html_head_alter(&$head_elements) {
  // Simplify the meta charset declaration.
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}

/**
 * Implements template_preprocess_region().
 */
function themage_preprocess_region(&$vars) {
  // Add sidebar class to sidebar regions.
  if ($vars['region'] == 'sidebar_first' || $vars['region'] == 'sidebar_second') {
    $vars['classes_array'][] = 'sidebar';
  }
}

/**
 * Implements template_preprocess_node().
 */
function themage_preprocess_node(&$vars) {
  $vars['submitted'] = themage_submitted($vars['name'], $vars['created']);
}

/**
 * Implements template_preprocess_comment().
 */
function themage_preprocess_comment(&$vars) {
  $vars['submitted'] = themage_submitted($vars['author'], $vars['comment']->created);
}

/**
 * Formats a "submitted by" statement.
 *
 * This function adds an html5 "time" element to the
 * submitted by statement, for semantics.
 *
 * @param string $author
 *   The author's name (optionally with html tags) to display.
 * @param int $datetime
 *   The creation time, in UNIX timestamp format.
 *
 * @return string
 *   To be used as "submitted by".
 */
function themage_submitted($author, $datetime) {
  $date_string = format_date($datetime, 'custom', 'F j, Y');
  $time_string = format_date($datetime, 'custom', 'c');
  return t('Submitted by !username on !datetime',
    array(
      '!username' => $author,
      '!datetime' => "<time datetime='$time_string'>$date_string</time>",
    )
  );
}

/**
 * Implements template_preprocess_block().
 */
function themage_preprocess_block(&$vars) {
  if ($vars['block']->region == 'sidebar_first' || $vars['block']->region == 'sidebar_second') {
    $vars['attributes_array']['role'] = 'complementary';
  }
  elseif ($vars['block']->module == 'search') {
    $vars['attributes_array']['role'] = 'search';
  }
}

/**
 * Implements template_breadcrumb().
 */
function themage_breadcrumb(&$vars) {
  $breadcrumb = $vars['breadcrumb'];

  // Don't show anything if there's no breadcrumb or it shouldn't be displayed.
  if (empty($breadcrumb) || !theme_get_setting('breadcrumb_display')) {
    return;
  }

  // Optionally, add title.
  if (theme_get_setting('breadcrumb_title')) {
    $breadcrumb[] = drupal_get_title();
  }

  // Create the string to display from the breadcrumb array.
  $separator = check_plain(theme_get_setting('breadcrumb_separator'));
  $breadcrumb_string = implode($separator, $breadcrumb);

  $prefix = check_plain(theme_get_setting('breadcrumb_prefix'));

  return $prefix . $breadcrumb_string;
}
