<?php
/**
 * @file
 * Admin file for Getclicky settigns.
 */

/**
 * function returning a $form for configuring the module
 */
function getclicky_admin_settings() {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('GetClicky Account Settings'),
    '#collapsible' => FALSE,
  );

  $form['account']['getclicky_site_id_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID Number'),
    '#default_value' => variable_get('getclicky_site_id_number', ''),
    '#size' => 15,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#description' => t('Example input: 130169<br/>
    This number is unique to every website you track via Clicky(a.k.a. GetClicky).<br/>
    - If you <b>do not</b> have an account with Clicky - you can obtain one from the <a href="@url1">GetClicky</a> website. (clicking on this link shows your support for my development of this module - especially if you decide to buy a Pro account with Clicky. Here is <a href="http://getclicky.com/#theotherguys">why</a> and here is <a href="http://getclicky.com/help/pricing">accounts comparison</a>)<br/>
    - If you <b>DO</b> have a Clicky account - you can find that number here:<br/>
      - anywhere you see a link like that <a href="@url2">http://getclicky.com/<b>130169</b></a> - take only the number - this is your Clicky site ID number.
      - you can find this in your affiliate link (if you have many sites tracked by one Clicky user account - the affiliate link uses the 1st site\'s ID), <br/>
      - also in your tracking code and in your site prefs page ...',
        array('@url1' => 'http://getclicky.com/130169', '@url2' => 'http://getclicky.com/130169')),
  );

  $form['account']['getclicky_site_key_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Key Number'),
    '#default_value' => variable_get('getclicky_site_key_number', ''),
    '#size' => 100,
    '#maxlength' => 100,
    '#required' => FALSE,
    '#description' => t('Example input: 72543da7406597242<br/>
    This number is an API key used to display stats on the <a hre"">page</a>.'),
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Role Tracking'),
    '#collapsible' => TRUE,
    '#description' => t('Define what user roles should be tracked by Get Clicky. <strong>Note:</strong> Drupal Admin pages are never tracked.'),
  );

  $form['roles']['getclicky_track_roles'] = array(
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#default_value' => variable_get('getclicky_track_roles', array()),
  );

  $form['getclicky_pass_username'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pass Usernames to GetClicky'),
    '#description' => t('Associate authenticated user traffic by passing the username to Clicky as a custom variable.'),
    '#default_value' => variable_get('getclicky_pass_username', FALSE),
  );

  $form['getclicky_banner_image'] = array(
    '#type' => 'fieldset',
    '#title' => t('GetClicky Banner Image'),
    '#collapsible' => TRUE,
  );

  $form['getclicky_banner_image']['getclicky_show_banner_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inject GetClicky Banner Image in html code\'s footer? (right before the closing body tag)'),
    '#description' => t('Check this to show this in your site\'s footer automatically if you want visitors to click on your affiliate link.'),
    '#default_value' => variable_get('getclicky_show_banner_image', FALSE),
  );


  $form['getclicky_banner_image']['getclicky_banner_image_html_code_for_copying'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML Banner code (used in the block we provide)'),
    '#description' => t('This code is used in the block we provide for you - go to <a href="/admin/build/block">/admin/build/block</a> to put it into a region.<br/>
      <b>See here - <a href="http://getclicky.com/user/affiliate">http://getclicky.com/user/affiliate</a> for more banner images. Paste from there into here, and the block here will auto-update. </b>
    '),
    '#default_value' => variable_get('getclicky_banner_image_html_code_for_copying', '<a title="Clicky Web Analytics" href="http://getclicky.com/' . variable_get('getclicky_site_id_number', '') . '"><img alt="Clicky Web Analytics" src="http://static.getclicky.com/media/links/badge.gif" border="0" /></a>'),
  );

  $form['getclicky_trackfiles'] = array(
    '#type' => 'textfield',
    '#title' => t('File Extensions To Track'),
    '#default_value' => variable_get('getclicky_trackfiles', 'pdf|zip|mp3'),
    '#description' => t('A pipe seperated list of file extensions that should be tracked when clicked. Example !extensions', array('!extensions' => 'pdf|zip|mp3')),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('You can add custom Get Clicky code here.'),
  );

  $form['advanced']['getclicky_codesnippet'] = array(
    '#type' => 'textarea',
    '#title' => t('JavaScript Code'),
    '#default_value' => variable_get('getclicky_codesnippet', ''),
    '#rows' => 15,
    '#description' => t('Paste <a href="@snippets">custom code snippets here</a>. These will be added to every page that Get Clicky appears on.', array('@snippets' => 'http://drupal.org/node/39282')),
  );

  return system_settings_form($form);
}
