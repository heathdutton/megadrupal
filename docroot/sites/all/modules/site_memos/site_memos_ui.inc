<?php

/**
 * @file
 * Site_memos UI functions.
 *
 * A user with the 'view_site_reports' permission can change the
 * site_memos settings.
 *
 * The UI presents a form to enable the user to do that task.
 *
 */

/**
 * Site_memos configuration form builder.
 *
 * @param $form
 *  The multiform form argument.
 * @param $form_state
 *  The multiform state argument.
 *
 */
function site_memos_configuration_form($form, &$form_state) {
  $settings = variable_get('site_memos_settings', FALSE);
  $url = ($settings && isset($settings['url'])) ? $settings['url'] : NULL;

  // Set the page heading based on the user-provided URL.
  global $base_url;
  if (!$url) {
    $heading = '<h3>' . t('Site Memos URL Not Set.') . '</h3>';
  }
  elseif (strpos($url, $base_url) === 0) {
    // URL is local, try to work with it.
    $path = substr($url, strlen($base_url . '/'));
    $path = drupal_get_normal_path($path);
    $nid = str_replace('node/', '', $path);
    $page_info = node_load($nid);
    if ($page_info && isset($page_info->title)) {
      $heading = '<h3>' . t('Your Site Memos Page:') . ' ' . l($page_info->title, "$url") . '</h3>';
    }
    else {
      $heading = '<h3>' . t('Site Memos URL Not Accessible:') . ' ' . l($url, "$url") . '</h3>';
    }
  }
  else {
    $heading = '<h3>' . t('Site Memos URL Is External:') . ' ' . l($url, "$url") . '</h3>';
  }
  $form['heading'] = array(
    '#type' => 'item',
    '#markup' => $heading,
  );

  // Last access date.
  if (isset($page_info) && isset($page_info->changed)) {
    $changed = t('Your memos page was last changed:') . ' ' . format_date($page_info->changed, 'medium');
    $form['changed'] = array(
      '#type' => 'item',
      '#markup' => $changed,
    );
  }

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Memos URL'),
    '#tree' => TRUE,
  );
  $form['settings']['help'] = array(
    '#type' => 'markup',
    '#markup' => t("Enter the full URL of your memos page.<br>For example: <code>http://mysite.example.com/site_memos</code>"),
  );
  $form['settings']['url'] = array(
    '#type'  => 'textfield',
    '#title' => 'Site Memos URL',
    '#default_value' => ($url) ? $url : '',
  );
  $form['settings']['submit'] = array(
    '#type'  => 'submit',
    '#value' => 'Save URL',
  );

  $help_link = l(t('View Site Memos help'), 'admin/help/site_memos');
  $form['help'] = array(
    '#type' => 'item',
    '#markup' => $help_link,
  );

  return $form;
}

/**
 * Submit inputs for site_memos configuration form.
 *
 */
function site_memos_configuration_form_submit(&$form, &$form_state) {
  $url = $form_state['values']['settings']['url'];
  $settings = variable_get('site_memos_settings', FALSE);
  if (!$settings) {
    $settings = array();
  }
  $settings['url'] = $url;
  variable_set('site_memos_settings', $settings);
  return;
}
