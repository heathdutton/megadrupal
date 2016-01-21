<?php
/**
 * @file
 * Stub file for "html" theme hook [pre]process functions.
 */

/**
 * Pre-processes variables for the "html" theme hook.
 *
 * See template for list of available variables.
 *
 * @see html.tpl.php
 *
 * @ingroup theme_preprocess
 */
function wetkit_bootstrap_preprocess_html(&$variables, $hook) {
  // Backport from Drupal 8 RDFa/HTML5 implementation.
  // @see https://www.drupal.org/node/1077566
  // @see https://www.drupal.org/node/1164926

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

  // HTML element attributes.
  $variables['html_attributes_array'] = array(
    'lang' => $variables['language']->language,
    'dir' => $variables['language']->dir,
  );

  // Override existing RDF namespaces to use RDFa 1.1 namespace prefix bindings.
  if (function_exists('rdf_get_namespaces')) {
    $rdf = array('prefix' => array());
    foreach (rdf_get_namespaces() as $prefix => $uri) {
      $rdf['prefix'][] = $prefix . ': ' . $uri;
    }
    if (!$rdf['prefix']) {
      $rdf = array();
    }
    $variables['rdf_namespaces'] = drupal_attributes($rdf);
  }

  // BODY element attributes.
  $variables['body_attributes_array'] = array(
    'role'  => 'document',
    'class' => $variables['classes_array'],
  );
  $variables['body_attributes_array'] += $variables['attributes_array'];

  // Navbar position.
  switch (bootstrap_setting('navbar_position')) {
    case 'fixed-top':
      $variables['body_attributes_array']['class'][] = 'navbar-is-fixed-top';
      break;

    case 'fixed-bottom':
      $variables['body_attributes_array']['class'][] = 'navbar-is-fixed-bottom';
      break;

    case 'static-top':
      $variables['body_attributes_array']['class'][] = 'navbar-is-static-top';
      break;
  }

  // Add the default body id needed
  // WetKit Layouts may have already set this variable.
  if (empty($variables['wetkit_col_array'])) {
    $variables['wetkit_col_array'] = 'wb-body';
  }

  // Add a body class for the active theme name.
  $variables['body_attributes_array']['class'][] = drupal_html_class($theme_key);

  // Add the active WxT theme into body class.
  $variables['body_attributes_array']['class'][] = drupal_html_class($wxt_active_orig);

  // Assign skip link variables.
  $variables['wetkit_skip_link_id_1'] = theme_get_setting('wetkit_skip_link_id_1');
  $variables['wetkit_skip_link_text_1'] = t('Skip to main content');
  $variables['wetkit_skip_link_id_2'] = theme_get_setting('wetkit_skip_link_id_2');
  $variables['wetkit_skip_link_text_2'] = t('Skip to footer');

  // Splash Page.
  if (current_path() == 'splashify-splash') {
    if ($wxt_active == 'gcweb') {
      $variables['body_attributes_array']['class'][] = 'splash';
    }
    else {
      $variables['body_attributes_array']['class'][] = 'wb-sppe';
    }
  }
  if ($wxt_active == 'gcweb' && drupal_is_front_page()) {
    $variables['body_attributes_array']['class'][] = 'home';
  }
}

/**
 * Processes variables for the "html" theme hook.
 *
 * See template for list of available variables.
 *
 * @see html.tpl.php
 *
 * @ingroup theme_process
 */
function wetkit_bootstrap_process_html(&$variables) {
  $variables['html_attributes'] = drupal_attributes($variables['html_attributes_array']);
  $variables['body_attributes'] = drupal_attributes($variables['body_attributes_array']);
}
