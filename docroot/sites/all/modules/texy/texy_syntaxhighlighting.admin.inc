<?php

/**
 * @file
 * Admin page callbacks for the Texy Syntaxhighlighting.
 */

/**
 * Form builder; Configure Syntaxhighlighting settings for Texy!.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function texy_form_syntaxhighlighting_settings() {
  $module_path = drupal_get_path('module', 'texy_syntaxhighlighting');

  $form['highlightingset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Syntax highlighting'),
    '#collapsible' => FALSE,
    '#weight' => -1,
  );
  $form['highlightingset']['texy_syntaxhighlighting_use'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use source code syntax highlighting'),
    '#description' => t('Check this option if you would like to highlight entered source code.'),
    '#default_value' => variable_get('texy_syntaxhighlighting_use', FALSE),
  );
  $form['highlightingset']['highlighterset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Syntax highlighther settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['highlightingset']['highlighterset']['texy_syntaxhighlighting_name'] = array(
    '#type' => 'textfield',
    '#size' => '20',
    '#title' => t('Syntax highlighter\'s name'),
    '#description' => t('Name of a tool which you wish to use for syntax highlighting. For example, if you plan to use <a href="http://code.google.com/p/fshl/">FSHL</a> you should enter <code>fshl</code> and install the tool into the <code>!module_path/lib/fshl</code> directory, together with a corresponding <code>!module_path/lib/fshl.php</code> file which would be a PHP implementation of the highlighter.', array('!module_path' => $module_path)),
    '#default_value' => variable_get('texy_syntaxhighlighting_name', 'fshl'),
  );
  $form['highlightingset']['highlighterset']['texy_syntaxhighlighting_css_path'] = array(
    '#type' => 'textfield',
    '#title' => t("Syntax highlighter's stylesheet"),
    '#description' => t('Path to a stylesheet wich will be used together with the syntax highlighter. For example, if you plan to use <a href="http://code.google.com/p/fshl/">FSHL</a> you should enter <code>fshl/styles/COHEN_style.css</code>.'),
    '#default_value' => variable_get('texy_syntaxhighlighting_css_path', 'fshl/styles/COHEN_style.css'),
  );

  return system_settings_form($form);
}
