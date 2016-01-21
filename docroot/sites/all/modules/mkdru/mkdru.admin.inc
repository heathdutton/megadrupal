<?php
/**
 * @file
 * Admin settings form.
 */
function mkdru_admin_settings($form, &$form_state) {
  require_once(realpath( dirname( __FILE__ )) . '/defaults.inc' );
  $libinfo = libraries_detect('pazpar2-js');
  $placeholders = array(
    '@libjs-pz2-url' => 'http://www.indexdata.com/pazpar2-js',
    '@libraries-api-url' => 'https://drupal.org/project/Libraries',
  );
  if (isset($libinfo['version'])) {
    $placeholders['@version'] = $libinfo['version'];
    drupal_set_message(t(
      'Using <a href="@libjs-pz2-url">Pazpar2 client library</a> via <a href="@libraries-api-url">Libraries API</a>. Version detected: <strong>@version</strong>',
      $placeholders), 'status');
  }
  else {
    drupal_set_message(t(
      '<a href="@libjs-pz2-url">Pazpar2 client library</a> unavailable via <a href="@libraries-api-url">Libraries API</a>, falling back to configured URL',
      $placeholders), 'warning');
  }
  $form['pz2_js_path']=array(
    '#type' => 'textfield',
    '#title' => t('Fallback URL for Pazpar2 client library'),
    '#description' => t('If pazpar2-js is not available via the library api, MkDru will attempt to load it from here.'),
    '#default_value' => variable_get('pz2_js_path', $mkdru_admin_defaults['pz2_js_path']),
    '#required' => TRUE
  );
  return system_settings_form($form);
}
