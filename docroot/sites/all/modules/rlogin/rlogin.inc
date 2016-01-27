<?php
// $Id: rlogin.admin.inc,v 1.2 2010/07/30 18:45:56 chris Exp $

/**
 * @file
 * Admin page callbacks for the rlogin module.
 */


/**
 * Primary endpoint menu callback.
 */
function rlogin_endpoint() {
  global $base_url;
  global $user;
  if ($user->uid) {
    // already logged in, don't show form, but instead a link to the site.
    print "<div id='rlogin-link'><a href='$base_url' target='_top'>";
    print variable_get('rlogin_link_text', t('Visit ') . variable_get('site_name', check_plain($base_url)) );
    print "</a></div>\n";
    exit;
  }

  if (isset($_POST) AND is_array($_POST)) {
    // in theory, we just received our own form.  call the real user login.
    $_GET['q'] = $base_url . "?destination=".variable_get('rlogin_landing_page', 'node');
  }

  $form = drupal_get_form('user_login_block');
  $form['#action'] = variable_get('rlogin_landing_page', 'node');
  if ($url = variable_get('rlogin_css_url', '')) {
    print sprintf("<link rel='stylesheet' type='text/css' href='%s' />\n", $url);
  }
  else {
    $css_file = __DIR__ . '/rlogin.css';
    print '<style>'.file_get_contents($css_file).'</style>';
  }

  // output the div and form HTML
  echo "\n<div id='rlogin-block'>\n";
  echo drupal_render($form) . "\n";
  echo "</div>\n";
}


/**
 * Implementation of hook_settings().
 */
function rlogin_admin_settings() {
  global $base_url;
  $form['rlogin_css_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Style sheet URL'),
    '#default_value' => variable_get('rlogin_css_url', ''),
    '#size' => 60,
    '#maxlength' => 244,
    '#description' => t('Specify the full URL of the cascading style sheet (CSS) you want to have applied to the HTML remote login form.'),
  );

  $form['legend'] = array(
    '#value' => t('Below is a sample HTML form which is output when referencing your site\'s URL ' . $base_url .'/rlogin.  You may use this to learn the HTML elements, classes and identifiers used in the form which can be referenced and altered by your style sheet.'),
  );
  $form['html'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sample HTML form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $loginform = drupal_get_form('user_login_block');
  $form['html']['form'] = array(
    '#markup' => drupal_render($loginform),
  );
  $form['rlogin_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Logged-in link text'),
    '#default_value' => variable_get('rlogin_link_text', t('Visit ') . variable_get('site_name', check_plain($base_url)) ),
    '#size' => 32,
    '#maxlength' => 60,
    '#description' => t('When the user has already logged into your site, it does not make sense to present them with a login form on the remote site.  Instead, a link to your home page is presented.  This setting allows you to choose the text presented in the link.'),
  );
  $form['rlogin_landing_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Landing page'),
    '#default_value' => variable_get('rlogin_landing_page', 'node'),
    '#size' => 32,
    '#maxlength' => 32,
  );

  return system_settings_form($form);
}
