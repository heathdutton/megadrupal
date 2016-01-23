<?php
/**
 * @file
 * Contains the theme settings form elements for the wetkit extension.
 */


/**
 * Implements hook_extension_EXTENSION_theme_settings_form_alter().
 */
function wetkit_omega_extension_wetkit_settings_form($element, &$form, $form_state) {

  // Overrides.
  $element['wetkit_overrides'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Overrides'),
  );
  $element['wetkit_overrides']['wetkit_render_mb_main_link'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable rendering of the mb main link inside the mega menu'),
    '#default_value' => omega_theme_get_setting('wetkit_render_mb_main_link', FALSE),
    '#description'   => t('Specify whether or not the mega menu should include the main link.'),
  );
  $element['wetkit_overrides']['wetkit_render_no_link'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable rendering of menu items with <nolink> as path'),
    '#default_value' => omega_theme_get_setting('wetkit_render_no_link', FALSE),
    '#description'   => t('Specify whether or not menu links with <nolink> as path should render an a href. (This can break certain versions of WET)'),
  );
  $element['wetkit_overrides']['wetkit_sub_site'] = array(
    '#type' => 'textfield',
    '#title' => t('Intranet web site name'),
    '#default_value' => omega_theme_get_setting('wetkit_sub_site', FALSE),
    '#description' => t('The display name for the Intranet web site'),
  );
  $element['wetkit_overrides']['wetkit_sub_site_alt_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Alternate Site URL'),
    '#default_value' => omega_theme_get_setting('wetkit_sub_site_alt_url', FALSE),
    '#description'   => t('Alternate Site URL to be used with Site Name.'),
  );
  $element['wetkit_overrides']['wetkit_search_indexing_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Indexing tag for search'),
    '#default_value' => omega_theme_get_setting('wetkit_search_indexing_start', FALSE),
    '#description' => t('Custom Markup to wrap around content to assist in indexing.'),
  );
  $element['wetkit_overrides']['wetkit_search_indexing_stop'] = array(
    '#type' => 'textfield',
    '#title' => t('Stop Indexing tag for search'),
    '#default_value' => omega_theme_get_setting('wetkit_search_indexing_stop', FALSE),
    '#description' => t('Custom Markup to wrap around content to assist in indexing.'),
  );

  // Breadcrumb.
  $element['wetkit_breadcrumbs'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('WetKit Settings'),
  );
  $element['wetkit_breadcrumbs']['wetkit_breadcrumb_display'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable the WetKit breadcrumb logic.'),
    '#default_value' => omega_theme_get_setting('wetkit_breadcrumb_display', FALSE),
    '#description'   => t('Specify whether or not WetKit will be leveraging the breadcrumb system.'),
  );
  $element['wetkit_breadcrumbs']['wetkit_breadcrumb_home'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable the home link to appear inside the breadcrumb'),
    '#default_value' => omega_theme_get_setting('wetkit_breadcrumb_home', FALSE),
    '#description'   => t('Specify whether or not the home link will be rendered inside the breadcrumb trail.'),
  );
  $element['wetkit_breadcrumbs']['wetkit_breadcrumb_title'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable the title to appear inside the breadcrumb'),
    '#default_value' => omega_theme_get_setting('wetkit_breadcrumb_title', FALSE),
    '#description'   => t('Specify whether or not the title will be rendered inside the breadcrumb trail.'),
  );
  $element['wetkit_breadcrumbs']['wetkit_breadcrumb_separator'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Text to use as the breadcrumb separator'),
    '#default_value' => omega_theme_get_setting('wetkit_breadcrumb_separator', FALSE),
    '#description'   => t('Specify the HTML you would like to use for the breadcrumb separator'),
  );

  // Accessibility.
  $element['wetkit_accessibility'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Accessibility Settings'),
  );
  $element['wetkit_accessibility']['wetkit_skip_link_text_1'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Text for the “skip link” 1'),
    '#default_value' => t('Skip to main content'),
    '#description'   => t('For example: <em>Jump to navigation</em>, <em>Skip to content</em>'),
  );
  $element['wetkit_accessibility']['wetkit_skip_link_id_1'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Anchor ID for the “skip link” 1'),
    '#default_value' => omega_theme_get_setting('wetkit_skip_link_id_1', FALSE),
    '#field_prefix'  => '#',
    '#description'   => t('Specify the HTML ID of the element that the accessible-but-hidden “skip link” should link to. (<a href="!link">Read more about skip links</a>.)', array('!link' => 'http://drupal.org/node/467976')),
  );
  $element['wetkit_accessibility']['wetkit_skip_link_text_2'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Text for the “skip link” 2'),
    '#default_value' => t('Skip to footer'),
    '#description'   => t('For example: <em>Jump to navigation</em>, <em>Skip to content</em>'),
  );
  $element['wetkit_accessibility']['wetkit_skip_link_id_2'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Anchor ID for the “skip link” 2'),
    '#default_value' => omega_theme_get_setting('wetkit_skip_link_id_2', FALSE),
    '#field_prefix'  => '#',
    '#description'   => t('Specify the HTML ID of the element that the accessible-but-hidden “skip link” should link to. (<a href="!link">Read more about skip links</a>.)', array('!link' => 'http://drupal.org/node/467976')),
  );
  return $element;
}
