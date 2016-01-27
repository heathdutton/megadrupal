<?php


/**
 * Form builder function
 */
function dqx_adminmenu_settings_form($form, &$form_state) {

  $siblings = variable_get('dqx_adminmenu_sibling_sites', array());
  $rows = array();
  foreach ($siblings as $sibling) {
    $rows[] = $sibling;
  }

  $form['sibling_sites'] = array(
    '#title' => t('Sibling sites'),
    '#description' => t('Links to development, staging and live sites. One per row.'),
    '#type' => 'textarea',
    '#default_value' => implode("\n", $rows),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


function dqx_adminmenu_settings_form_submit($form, &$form_state) {
  $rows = explode("\n", $form_state['values']['sibling_sites']);
  $siblings = array();
  foreach ($rows as $row) {
    $row = trim($row);
    // Remove trailing slash.
    if (preg_match('#^(.+)/$#', $row, $m)) {
      $row = $m[1];
    }
    if (preg_match('#^(http|https)://(.+)$#', $row, $m)) {
      list(,$protocol,$row) = $m;
    }
    else {
      $protocol = 'http';
    }
    $sibling = "$protocol://$row/";
    $siblings[$sibling . 'admin'] = $sibling;
  }
  variable_set('dqx_adminmenu_sibling_sites', $siblings);
  // set a timestamp for client-side cache
  variable_set('dqx_adminmenu_timestamp', time());
}
