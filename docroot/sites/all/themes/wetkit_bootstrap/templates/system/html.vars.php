<?php
/**
 * @file
 * html.vars.php
 *
 * @see html.tpl.php
 */

/**
 * Implements hook_preprocess_html().
 */
function wetkit_bootstrap_preprocess_html(&$variables, $hook) {
  global $theme_key;
  global $language;

  // WxT Settings.
  $wxt_active_orig = variable_get('wetkit_wetboew_theme', 'wet-boew');
  $library_path = libraries_get_path($wxt_active_orig, TRUE);
  $wxt_active = str_replace('-', '_', $wxt_active_orig);
  $wxt_active = str_replace('theme_', '', $wxt_active);

  // Return early, so the maintenance page does not call any of the code below.
  if ($hook != 'html') {
    return;
  }

  // Initializes attributes which are specific to the html and body elements.
  $variables['html_attributes_array'] = array();
  $variables['body_attributes_array'] = array();

  // Serialize RDF Namespaces into an RDFa 1.1 prefix attribute.
  if ($variables['rdf_namespaces']) {
    $prefixes = array();
    foreach (explode("\n  ", ltrim($variables['rdf_namespaces'])) as $namespace) {
      // Remove xlmns: and ending quote and fix prefix formatting.
      $prefixes[] = str_replace('="', ': ', substr($namespace, 6, -1));
    }
    $variables['rdf_namespaces'] = ' prefix="' . implode(' ', $prefixes) . '"';
  }

  // Modify html attributes.
  $variables['html_attributes_array']['lang'][] = $language->language;

  // Add the default body id needed
  // WetKit Layouts may have already set this variable.
  if (empty($variables['wetkit_col_array'])) {
    $variables['wetkit_col_array'] = 'wb-body';
  }

  // Add a body class for the active theme name.
  $variables['classes_array'][] = drupal_html_class($theme_key);

  // Add the active WxT theme into body class.
  $variables['classes_array'][] = drupal_html_class($wxt_active_orig);

  // Assign skip link variables.
  $variables['wetkit_skip_link_id_1'] = theme_get_setting('wetkit_skip_link_id_1');
  $variables['wetkit_skip_link_text_1'] = t('Skip to main content');
  $variables['wetkit_skip_link_id_2'] = theme_get_setting('wetkit_skip_link_id_2');
  $variables['wetkit_skip_link_text_2'] = t('Skip to footer');

  // Default Bootstrap configuration.
  switch (theme_get_setting('bootstrap_navbar_position')) {
    case 'fixed-top':
      $variables['classes_array'][] = 'navbar-is-fixed-top';
      break;

    case 'fixed-bottom':
      $variables['classes_array'][] = 'navbar-is-fixed-bottom';
      break;

    case 'static-top':
      $variables['classes_array'][] = 'navbar-is-static-top';
      break;
  }

  // Splash Page.
  if (current_path() == 'splashify-splash') {
    if ($wxt_active == 'gcweb') {
    $variables['classes_array'][] = 'splash';
    }
    else {
    $variables['classes_array'][] = 'wb-sppe';
    }
  }
  if ($wxt_active == 'gcweb' && drupal_is_front_page()) {
    $variables['classes_array'][] = 'home';
  }
}

/**
 * Implements hook_process_html().
 */
function wetkit_bootstrap_process_html(&$variables) {
  // Flatten attributes arrays.
  $variables['html_attributes'] = empty($variables['html_attributes_array']) ? '' : drupal_attributes($variables['html_attributes_array']);
}
