<?php

/**
 * @file
 * Provides the administration page for PNG Behaviors.
 */
  
/**
 * Administration settings page
 */ 
function pngbehave_admin() {
  $form = array();
  $form['pngbehave_css_classes'] = array(
    // Chris Paul (mrjeeves): changed #type to textarea to allow for very long selector lists
    '#type' => 'textarea',
    '#title' => t('CSS Classes/Selectors to <em>Include</em>'),
    '#default_value' => variable_get('pngbehave_css_classes', ''),
    '#description' => t('Comma seperated list of CSS classes/selectors to make BEHAVE!. To include all img, ul, and li use: <code>img, ul, li</code> or a specific class using <code>.star</code> or combination <code>img, .star, ul, li, ul ul, div.star</code> etc... RECOMMENDED SELECTORS: <code>img, ul, li, span</code> .'),
  );
  // Chris Paul (mrjeeves): added option for specific exclusion
  $form['pngbehave_css_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS Classes/Selectors to <em>Exclude</em>'),
    '#default_value' => variable_get('pngbehave_css_exclude', ''),
    '#description' => t('Comma seperated list of CSS classes/selectors to <em>specifically ignore</em>. To exclude all img, ul, and li use: <code>img, ul, li</code> or a specific class using <code>.star</code> or combination <code>img, .star, ul, li, ul ul, div.star</code> etc...'),
  );
  return system_settings_form($form);
} // function pngbehave_admin
