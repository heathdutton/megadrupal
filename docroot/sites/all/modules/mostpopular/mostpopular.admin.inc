<?php
// $Id$
/*
 * Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
 * Copyright © 2009-2012 New Signature
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * You can contact New Signature by electronic mail at labs@newsignature.com -or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 */
/**
 * @file
 * Defines the main administration forms for the Most Popular module.
 */

/* ----------------------------------------------------------------------------
 * Settings Form
 * --------------------------------------------------------------------------*/
function mostpopular_settings_form() {
  $form = array();
  $form['block'] = array(
    '#type' => 'fieldset',
    '#collapsible' => true,
    '#title' => t('Style settings'),
    '#description' => t('Configure the look and feel of the Most Popular block.'),
  );
  $form['block']['mostpopular_styling'] = array(
    '#type' => 'radios',
    '#title' => t('Stylesheet'),
    '#description' => '<p>' . t(
"Choose how much styling to apply to the Most Popular block.
You can add additional styling in your own theme.") . '</p>' .
      '<p>' . t(
"For help, look at the <a href='@basic'>basic stylesheet</a>, which turns the
service and interval links into tabs, and the <a href='@full'>full stylesheet</a>,
which adds fonts, colors, formatting, and layouts.", array(
        '@basic' => url(drupal_get_path('module', 'mostpopular') . '/css/mostpopular-basic.css'),
        '@full' => url(drupal_get_path('module', 'mostpopular') . '/css/mostpopular-full.css'),
      )) . '</p>' .
      '<p>' . t(
"When creating your own styles for the Most Popular block, we recommend you
start with our full stylesheet and override it using drupal_set_css().") . '</p>',

    '#options' => array(
      MOSTPOPULAR_STYLE_NONE => t('No styling'),
      MOSTPOPULAR_STYLE_BASIC => t('Basic styling'),
      MOSTPOPULAR_STYLE_FULL => t('Full styling'),
    ),
    '#default_value' => variable_get('mostpopular_styling', MOSTPOPULAR_STYLE_FULL),
  );

  // Add a fieldset for configuring Drupal paths
  $form['paths'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Drupal Paths'),
  );
  
  // Get the configured base paths
  $site_base = url('', array('absolute' => TRUE));
  $path_base = url('');
  $basepaths = variable_get('mostpopular_basepaths', array($site_base, $path_base));

  $form['paths']['mostpopular_basepaths'] = array(
    '#type' => 'textarea',
    '#rows' => 6,
    '#title' => t('Base Paths'),
    '#default_value' => implode("\n", $basepaths),
    '#description' => t(<<<EOT
<p>These base URLs will be stripped from the beginning of any full page URLs returned
by the various services. This allows the most popular content to work across several
site configurations.  This will only work, however, if the node nids are shared
between all sites.</p>
<p>Put each base URL on a separate line. Each must end with a slash.</p>
EOT
    ),
  );

  // Get the configured exclude paths
  $excludepaths = variable_get('mostpopular_exclude_paths', MOSTPOPULAR_DEFAULT_EXCLUDES);
  $form['paths']['mostpopular_exclude_paths'] = array(
    '#type' => 'textarea',
    '#rows' => 10,
    '#title' => t('Paths to exclude'),
    '#default_value' => $excludepaths,
    '#description' => t(<<<EOT
<p>These Drupal pages will be excluded from the most popular results for any 
services that return nodes.  The homepage will automatically be excluded, but
you can use this field to hide landing pages or other non-content pages.</p>
<p>Each URL should be an internal Drupal path with no leading slash, and can
point either to node/%d or to an alias. Put each path on a separate line and 
use '*' to denote wildcards.</p>
EOT
    ),
  );
  
  $form['mostpopular_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log debug messages from the services?'),
    '#default_value' => variable_get('mostpopular_debug', FALSE),
  );

  $form['#validate'][] = 'mostpopular_settings_form_validate';
  $form = system_settings_form($form);
  return $form;
}

function mostpopular_settings_form_validate($form, &$form_state) {
  // Change the encoding for the path fields
  $basepaths = explode("\n", $form_state['values']['mostpopular_basepaths']);
  $form_state['values']['mostpopular_basepaths'] = array();
  foreach ($basepaths as $path) {
    $path = trim($path);
    if (!empty($path)) {
      $form_state['values']['mostpopular_basepaths'][] = $path;
    }
  }
}
