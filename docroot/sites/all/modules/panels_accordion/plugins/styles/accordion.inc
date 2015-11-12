<?php


/**
 * @file accordion.inc
 * Definition of the collapsible panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Accordion'),
  'description' => t('Allows the panel pane to be collapsed.'),
  'render region' => 'panels_accordion_render_region',
  'render pane' => 'panels_accordion_render_pane',
  'settings form' => 'panels_accordion_region_settings_form',
  'pane settings form' => 'panels_accordion_pane_settings_form',
);

/**
 * Settings form callback for region settings.
 */
function panels_accordion_region_settings_form($style_settings) {
  $form = array();

  $animated_options = array(
      'none' => t('None'),
      'slide' => t('Slide'),
      'swing' => t('Swing'),
      'linear' => t('Linear'),
      'easeInQuart' => t('easeInQuart'),
      'easeOutQuart' => t('easeOutQuart'),
      'easeInOutQuart' => t('easeInOutQuart'),
      'easeInExpo' => t('easeInExpo'),
      'easeOutExpo' => t('easeOutExpo'),
      'easeInOutExpo' => t('easeInOutExpo'),
      'easeInBack' => t('easeInBack'),
      'easeOutBack' => t('easeOutBack'),
      'easeInOutBack' => t('easeInOutBack'),
      'easeInQuad' => t('easeInQuad'),
      'easeOutQuad' => t('easeOutQuad'),
      'easeInOutQuad' => t('easeInOutQuad'),
      'easeInQuint' => t('easeInQuint'),
      'easeOutQuint' => t('easeOutQuint'),
      'easeInOutQuint' => t('easeInOutQuint'),
      'easeInCirc' => t('easeInCirc'),
      'easeOutCirc' => t('easeOutCirc'),
      'easeInOutCirc' => t('easeInOutCirc'),
      'easeInBounce' => t('easeInBounce'),
      'easeOutBounce' => t('easeOutBounce'),
      'easeInOutBounce' => t('easeInOutBounce'),
      'easeInCubic' => t('easeInCubic'),
      'easeOutCubic' => t('easeOutCubic'),
      'easeInOutCubic' => t('easeInOutCubic'),
      'easeInSine' => t('easeInSine'),
      'easeOutSine' => t('easeOutSine'),
      'easeInOutSine' => t('easeInOutSine'),
      'easeInElastic' => t('easeInElastic'),
      'easeOutElastic' => t('easeOutElastic'),
      'easeInOutElastic' => t('easeInOutElastic'),
    );

  $form['region_accordion_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Accordion Id'),
    '#description' => t('Accordion ID to be used. Please use machine name format.'),
    '#required' => TRUE,
    '#default_value' => (isset($style_settings['region_accordion_id'])) ? $style_settings['region_accordion_id'] : ''
  );

  $form['description'] = array(
    '#type' => 'markup',
    '#value' => 'See <a href="http://docs.jquery.com/UI/Accordion">http://docs.jquery.com/UI/Accordion</a> for a list of possible values for these options',
  );

  // http://docs.jquery.com/UI/Accordion for how to implement these attributes
  $form['active'] = array(
    '#type' => 'textfield',
    '#title' => t('active'),
    '#default_value' => isset($style_settings['active']) ? $style_settings['active'] : $style_settings['active'] = 0,
    '#description' => t('Selector for the active element. Set to false to display none at start. Needs collapsible: true.'),
  );

  $form['collapsible'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow all accordian sections to be collapsible'),
      '#default_value' => isset($style_settings['collapsible']) ? $style_settings['collapsible'] : 1,
      '#description' => t('By default, accordions always keep one section open. 
        Check this box to allow all sections to be be collapsible. You will then
        be able to click on an open section to collapse it.'),
    );

  $form['animated'] = array(
      '#type' => 'select',
      '#title' => t('Animation effect'),
      '#default_value' => $style_settings['animated'],
      '#description' => t('Choose what animation effect you would like to see, or "None" to disable it.'),
      '#options' => $animated_options,
    );

  $form['autoHeight'] = array(
    '#type' => 'radios',
    '#title' => t('autoHeight'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
    '#default_value' => isset($style_settings['autoHeight']) ? $style_settings['autoHeight'] : 0,
    '#description' => t('For jQuery UI <= 1.8. Enable this to force the height 
      of all accordian elements to the largest height for consistent animations.
      Keep it disabled to allow for variable heights. If you are not sure, set
      this to disabled.'),
  );
  
  $form['heightStyle'] = array(
    '#type' => 'select',
    '#title' => t('heightStyle (autoHeight for jQuery UI 1.9)'),
    '#options' => array(
        'auto' => t('Auto'),
        'fill' => t('Fill'),
        'content' => t('Content')),
    '#default_value' => isset($style_settings['heightStyle']) ? $style_settings['heightStyle'] : 'content',
    '#description' => t('For jQuery UI 1.9. This does the same thing as
      autoHeight - choose whether to automatically adjust the height of each
      accordian element. 
      Note:- Setting heightStyle: "content" allows the accordion panels to keep their native height.
      See http://jqueryui.com/accordion'),
  );
  $form['header'] = array(
    '#type' => 'select',
    '#title' => t('Header selector'),
    '#options' => array('h2' => t('H2'), 'h3' => t('H3'), 'h4' => t('h4'), 'h5' => t('h5'), 'h6' => t('h6')),
    '#default_value' => isset($style_settings['header']) ? $style_settings['header'] : 'h2',
    '#description' => t('Selector for accordion section header elements.'),
  );
  return $form;
}

/**
 * Settings form callback for pane settings.
 */
function panels_accordion_pane_settings_form($style_settings) {
  $form = array();
  $form['pane_collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsed by default'),
    '#default_value' => (isset($style_settings['pane_collapsed'])) ? $style_settings['pane_collapsed'] : FALSE
  );
  return $form;
}

/**
 * Render callback for regions.
 *
 * @ingroup themeable
 */
function theme_panels_accordion_render_region($vars) {
  $panes = $vars['panes'];
  $display = $vars['display'];

  //Either from Display Settings or from panel content area settings
  $settings = $vars['settings'] ? $vars['settings'] : $display->panel_settings['style_settings']['default'];

  //support for previous versions
  $default_settings = array(
      'heightStyle' => 'content',
      'autoHeight' => 0,
      'collapsible' => 1,
      'header' => 'h2');
  $settings = array_merge($default_settings, $settings);

  // Force a true boolean value here as jQuery UI Accordian does not seem to respect 0 or 1
  $settings['autoHeight'] = $settings['autoHeight'] == 0 ? false : true;
  
  $content = implode('', $vars['panes']);
  drupal_add_library('system', 'ui.accordion');
  drupal_add_library('system', 'effects.core');

  //Calculate default active depending on each pane settings.
  $active = 0;
  foreach ( $display->content as $pane ) {
    if (!$pane->style['settings']['pane_collapsed']) {
      $settings['active'] = $active;
      break;
    }
    $active++;
  }
  
  $settings['header'] = !empty($settings['header']) ? $settings['header'] : 'h2';
  drupal_add_js( array('panels_accordion' => array(
                  $settings['region_accordion_id'] => $settings,
              )
          ), 'setting');
  drupal_add_js(drupal_get_path('module', 'panels_accordion') . '/js/panels_accordion.js');
  
  if (empty($settings['region_title'])) {
    $settings['region_title'] = '<empty>';
  }

  //Markup adjustments as per selected accordion header
  if ($settings['header']!='h2' && !empty($settings['header'])) {
    $content = str_replace('<h2 ', '<' . $settings['header'] . ' ', $content);
  }
  
  $title = ctools_context_keyword_substitute($settings['region_title'], array(), $vars['display']->context);
  $output = theme('panels_accordion_region', 
          array( 'handle' => $title, 'content' => $content, 'settings' => $settings));

  return $output;
}

/**
 * Render callback for panes.
 *
 * @ingroup themeable
 */
function theme_panels_accordion_render_pane($vars) {

  $content = $vars['content'];
  $pane = $vars['pane'];
  $display = $vars['display'];
  $settings = $vars['settings'];

  if (empty($content->content)) {
    return;
  }
  return theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $display));
}

/**
 * Render callback for region.
 *
 * @ingroup themeable
 */
function theme_panels_accordion_region($vars) {
  $pane_id = $vars['settings']['region_accordion_id'];
  $output = '<div id="' . $pane_id . '">';
  $output .= $vars['content'] . '</div>';
  return $output;
}
