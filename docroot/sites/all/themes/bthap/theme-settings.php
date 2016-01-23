<?php

/**
 * Implements hook_form_FORM_ID_alter().
 * @see system_theme_settings();
 */
function bthap_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['bthap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings specific to the Base theme for Highly Attractive People'),
    '#weight' => -10,
  );

  $form['bthap']['bthap_footer_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Move JavaScript to the Bottom'),
    '#description' => t('This will move all javascript to the bottom of the page. This can be overridden on an individual basis by setting the <pre>\'force header\' => true</pre> option in <pre>drupal_add_js</pre> or by using <pre>hook_js_alter</pre> to add the option to other JavaScript files.'),
    '#default_value' => theme_get_setting('bthap_footer_js'),
  );

  $humans_txt_exists = file_exists(DRUPAL_ROOT . '/humans.txt');
  $form['bthap']['bthap_humanstxt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link to humans.txt'),
    '#description' => $humans_txt_exists
      ? t('!alert. This checkbox will allow you to link to it from the page source.', array('!alert' => '<strong>' . t('humans.txt was found') . '</strong>'))
      : t('!alert. Create a humans.txt in the Drupal root (or not..) Read !link for more info.', array('!alert' => '<strong>' . t('humans.txt is missing') . '</strong>', '!link' => '<a href="http://humanstxt.org/" target="_blank">humanstxt.org</a>')),
    '#disabled' => !$humans_txt_exists,
    '#default_value' => $humans_txt_exists && theme_get_setting('bthap_humanstxt'),
  );

  $form['bthap']['bthap_ie_chrome_compatibility'] = array(
    '#type' => 'checkbox',
    '#title' => t('Forces "Edge" render system and Google Chrome Frame for Internet Explorer'),
    '#default_value' => theme_get_setting('bthap_ie_chrome_compatibility'),
    '#description' => t('This is a debugging option. You should only really uncheck this box if you are interested in debugging IE7 using debugging tools from a more modern version of IE.'),
  );

  $form['bthap']['bthap_suppress_ie6_image_toolbar'] = array(
    '#type' => 'checkbox',
    '#title' => t('Suppresses IE6 image toolbar'),
    '#default_value' => theme_get_setting('bthap_suppress_ie6_image_toolbar'),
    '#description' => t('See more info !link.', array('!link' => '<a href="http://technet.microsoft.com/en-us/library/dd361901.aspx" target="_blank">here</a>')),
  );

  $form['bthap']['ie9_ps'] = array(
    '#type' => 'fieldset',
    '#title' => t('IE9 Pinned Sites'),
    '#description' => t('Windows Internet Explorer 9 introduces pinned sites, a feature with which you can integrate your websites with the Windows 7 desktop. More info can be seen !link.', array('!link' => 'a< href="http://msdn.microsoft.com/en-us/library/gg131029.aspx" target="_blank">here</a>')),
  );

  $form['bthap']['ie9_ps']['bthap_ie9_ps_app_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Name'),
    '#description' => t('The name of the shortcut. If missing, the title of the site is used instead.'),
    '#default_value' => theme_get_setting('bthap_ie9_ps_app_name') ? theme_get_setting('bthap_ie9_ps_app_name') : variable_get('site_name', 'Drupal'),
  );

  $form['bthap']['ie9_ps']['bthap_ie9_ps_tooltip'] = array(
    '#type' => 'textarea',
    '#title' => t('Application Tooltop'),
    '#description' => t('Optional text that is displayed as a tooltip when the mouse pointer hovers over the pinned site shortcut icon in the Windows Start menu or desktop.'),
    '#rows' => 1,
    '#default_value' => theme_get_setting('bthap_ie9_ps_tooltip'),
  );

  $form['bthap']['ie9_ps']['bthap_ie9_ps_starturl'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Start URL'),
    '#description' => t('The start path of the application. If missing, the address of the current page (when pinned) is used instead.'),
    '#default_value' => theme_get_setting('bthap_ie9_ps_starturl'),
  );

  $form['bthap']['ie9_ps']['bthap_ie9_ps_starturl_https'] = array(
    '#type' => 'checkbox',
    '#title' => t('Application Start URL uses secure protocol'),
    '#default_value' => theme_get_setting('bthap_ie9_ps_starturl_https'),
  );

  $form['bthap']['ie9_ps']['bthap_ie9_ps_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Application navigation button color'),
    '#description' => t('The color of the Back and Forward buttons in the pinned site browser window. Any named color, or hex color value as defined by Cascading Style Sheets (CSS), Level 3 (CSS3) is valid.'),
    '#default_value' => theme_get_setting('bthap_ie9_ps_color'),
  );
}
