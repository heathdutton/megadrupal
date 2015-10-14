<?php
/**
 * @file
 * Form callback functions for the Contact Info module.
 */

/**
 * Contact Info settings form.
 */
function contactinfo_admin_settings($form, &$form_state) {
  $default_values = contactinfo_get_contactinfo();

  $form = array();
  $path = drupal_get_path('module', 'contactinfo');
  $form['#tree'] = TRUE;
  $form['#attached'] = array(
    'css' => array(
      array(
        'data' => $path . '/css/contactinfo.admin.css',
        'type' => 'file',
      ),
    ),
    'js' => array(
      $path . '/contactinfo.admin.js',
      array(
        'data' => array(
          'siteName' => check_plain(variable_get('site_name', '')),
          'siteSlogan' => check_plain(variable_get('site_slogan', '')),
        ),
        'type' => 'setting',
      ),
    ),
  );

  $form['contactinfo']['#tree'] = TRUE;
  $form['contactinfo']['type'] = array(
    '#type' => 'radios',
    '#title' => t('Contact information type'),
    '#description' => t('Is this for an individual or a business?'),
    '#options' => array(
      'personal' => t('Personal'),
      'business' => t('Organization/Business'),
    ),
    '#default_value' => $default_values['type'],
  );
  $form['contactinfo']['fn_n'] = array(
    '#type' => 'fieldset',
    '#title' => t('Full Name'),
    '#description' => t('If this site is your personal site, enter your full name here.'),
    '#states' => array(
      // Hide this fieldset if type is set to “business”.
      'invisible' => array(
        ':input[name="contactinfo[type]"]' => array('value' => 'business'),
      ),
    ),
    '#prefix' => '<div id="edit-hcard-fn-n-wrapper">',
    '#suffix' => '</div>',
  );
  $form['contactinfo']['fn_n']['given-name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#description' => t('Your first name.'),
    '#default_value' => $default_values['fn_n']['given-name'],
  );
  $form['contactinfo']['fn_n']['family-name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#description' => t('Your last name.'),
    '#default_value' => $default_values['fn_n']['family-name'],
  );
  $form['contactinfo']['org'] = array(
    '#type' => 'textfield',
    '#title' => t('Organization/Business Name'),
    '#default_value' => $default_values['org'],
    '#description' => t('The name of your organization or business.'),
    '#prefix' => '<div class="contactinfo-org-wrapper clearfix">',
  );
  $form['contactinfo']['use_site_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use site name'),
    '#default_value' => $default_values['use_site_name'],
    '#suffix' => '</div>',
  );
  $form['contactinfo']['tagline'] = array(
    '#type' => 'textfield',
    '#title' => t('Tagline'),
    '#default_value' => $default_values['tagline'],
    '#description' => t('A short tagline.'),
    '#prefix' => '<div class="contactinfo-tagline-wrapper clearfix">',
  );
  $form['contactinfo']['use_site_slogan'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use site slogan'),
    '#default_value' => $default_values['use_site_slogan'],
    '#suffix' => '</div>',
  );
  $form['contactinfo']['adr'] = array(
    '#type' => 'fieldset',
    '#title' => t('Address'),
    '#description' => t('Enter the contact address for this website.'),
  );
  $form['contactinfo']['adr']['street-address'] = array(
    '#type' => 'textfield',
    '#title' => t('Street Address'),
    '#default_value' => $default_values['adr']['street-address'],
  );
  $form['contactinfo']['adr']['extended-address'] = array(
    '#type' => 'textfield',
    '#title' => t('Extended Address'),
    '#default_value' => $default_values['adr']['extended-address'],
  );
  $form['contactinfo']['adr']['locality'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $default_values['adr']['locality'],
  );
  $form['contactinfo']['adr']['region'] = array(
    '#type' => 'textfield',
    '#title' => t('State/Province'),
    '#default_value' => $default_values['adr']['region'],
    '#size' => 10,
  );
  $form['contactinfo']['adr']['postal-code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal Code'),
    '#default_value' => $default_values['adr']['postal-code'],
    '#size' => 10,
  );
  $form['contactinfo']['adr']['country-name'] = array(
    '#type' => 'textfield',
    '#title' => t('Country'),
    '#default_value' => $default_values['adr']['country-name'],
  );
  $form['contactinfo']['location'] = array(
    '#type' => 'fieldset',
    '#title' => t('Geographical Location'),
    '#description' => t('Enter your geographical coordinates to help people locate you.'),
  );
  $form['contactinfo']['location']['longitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#default_value' => $default_values['location']['longitude'],
    '#description' => t('Longitude, in full decimal format (like -121.629562).'),
  );
  $form['contactinfo']['location']['latitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#default_value' => $default_values['location']['latitude'],
    '#description' => t('Latitude, in full decimal format (like 38.827382).'),
  );
  $form['contactinfo']['phone'] = array(
    '#type' => 'fieldset',
    '#title' => t('Phones'),
    '#description' => t('Enter the numbers at which you would like to be reached.'),
  );
  $form['contactinfo']['phone']['voice'] = array(
    '#type' => 'textfield',
    '#title' => t('Voice Phone Number(s)'),
    '#default_value' => $default_values['phone']['voice'],
    '#description' => t('Voice phone numbers, separated by commas.'),
  );
  $form['contactinfo']['phone']['fax'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax Number(s)'),
    '#default_value' => $default_values['phone']['fax'],
    '#description' => t('Fax numbers, separated by commas.'),
  );
  $form['contactinfo']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => $default_values['email'],
    '#description' => t('Enter this site’s contact email address. This address will be displayed publicly, do not enter a private address.'),
    '#element_validate' => array('contactinfo_validate_email'),
  );

  if (module_exists('invisimail')) {
    $form['contactinfo']['email']['#description'] .= ' ' . t('This address will be obfuscated to protect it from spammers.');
  }
  else {
    $form['contactinfo']['email']['#description'] .= ' ' . t('Install the <a href="!url" target="_blank">Invisimail</a> module to protect this address from spammers.', array('!url' => 'http://drupal.org/project/invisimail'));
  }

  $form = system_settings_form($form);

  $form['actions']['submit']['#value'] = t('Save information');

  return $form;
}
