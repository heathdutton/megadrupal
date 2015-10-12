<?php

/**
 * @file
 * table_tidy.admin.inc - Site builder's configuration settings for Table Tidy.
 */

/**
 * Menu callback for site admin settings.
 */
function table_tidy_admin_configure($form, &$form_state) {

  $form['table_tidy_main'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#title' => t('Table Tidy settings'),
  );
  $file_readme = url(drupal_get_path('module', 'table_tidy') . '/README.txt');
  $descr = t('Enter a symbol or short text to replace the content of each table cell whose content is identical to the cell above it. <br/>Most safe HTML snippets are allowed. The HTML <em>class</em> attribute is encouraged, but <em>style="..."</em> is not.');
  $examples = t('<strong>Example1:</strong> <em>"</em> &nbsp;&nbsp;(just a double quote)<br/><strong>Example2:</strong> %snippet1 &nbsp;&nbsp;(the default)<br/><strong>Example3:</strong> %snippet2 &nbsp;&nbsp;(requires Font-Awesome icon library, see the <a href="@url_readme">README</a>)', array(
    '%snippet1' => TABLE_TIDY_SAME_AS_ABOVE_TEXT,
    '%snippet2' => '<i class="fa fa-angle-double-up"></i>',
    '@url_readme' => $file_readme,
  ));
  $note = t('Enter your "Same as above" text/markup, press "Save configuration" and visit the <a href="@url_content">content listing</a> to see what it looks like. For more examples see the <a href="@url_readme">README</a>.', array(
    '@url_content' => url('admin/content'),
    '@url_readme' => $file_readme,
  ));

  $form['table_tidy_main']['table_tidy_same_as_above_text'] = array(
    '#type' => 'textfield',
    //'#size' => 60,
    '#title' => t('<em>"Same as above"</em> symbol or text and markup'),
    '#default_value' => filter_xss_admin(variable_get('table_tidy_same_as_above_text', TABLE_TIDY_SAME_AS_ABOVE_TEXT)),
    '#description' => $descr . '<br/><br/>' . $examples . '<br/><br/>' . $note,
  );
  $form['table_tidy_main']['table_tidy_included_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Table Tidy active pages'),
    '#rows' => 6,
    '#default_value' => variable_get('table_tidy_included_pages', TABLE_TIDY_DEFAULT_INCLUDED_PAGES),
    '#description' => t('Enter paths of pages that contain tables that need to be tidied up. One path per line. Do not start with a slash. You may use URL aliases and wildcards. A line containing only an asterisk means <em>all</em> pages.<br/><strong>Example:</strong> <em>content/recipes/*</em>'),
  );

  $form['table_tidy_global'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Table Tidy icon library'),
  );
  $descr = t("Enter the path to the library's main .css file.");
  $example = t("For example, for Font-Awesome, you'd probably enter %lib_css", array(
    '%lib_css' => TABLE_TIDY_DEF_FONT_ICON_LIB_CSS));
  $check = t('Check for errors on the <a href="@url_status">Status Report</a> page.', array(
    '@url_status' => url('admin/reports/status')));

  $form['table_tidy_global']['table_tidy_icon_lib_css'] = array(
    '#type' => 'textfield',
    '#title' => t('Font icon library'),
    '#default_value' => variable_get('table_tidy_icon_lib_css', ''),
    '#description' => $descr . '<br/>' . $example . '<br/>' . $check,
  );
  return system_settings_form($form);
}
