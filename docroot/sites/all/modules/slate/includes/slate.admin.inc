<?php
/**
 * @file
 */


/**
 * List page.
 *
 * @todo add more info! models and view mode.
 */
function slate_list_page($form, &$form_state) {
  $slates = slate_get_slates();

  $header = array(
    t('Name'),
    t('File'),
  );

  $rows = array();

  foreach ($slates as $slate => $slate_info) {
    $row = array();

    $row[] = $slate_info['name'];
    $row[] = format_string(
      "@path/<strong>@file</strong>",
      array(
        '@path' => $slate_info['path'],
        '@file' => $slate_info['file']
      )
    );

    $rows[] = $row;
  }

  $form['slates'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $form;
}

/**
 * Settings page.
 */
function slate_settings_page($form, &$form_state) {
  $form['slate_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug mode'),
    '#default_value' => variable_get('slate_debug', FALSE),
  );

  return system_settings_form($form);
}
