<?php

/**
 * @file
 * Admin page callback file for the socialloginandsocialshare module.
 */

/**
 * Function that build an abstract representation of a HTML form for Social Login settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array A renderable form array for the respective request.
 */
function socialloginandsocialshare_admin_settings($form, &$form_state) {
  if (!variable_get('socialloginandsocialshare_apikey', FALSE) || !variable_get('socialloginandsocialshare_apisecret', FALSE)) {
    drupal_set_message(t('To activate the social login, insert LoginRadius API Key and Secret in the social login section below.'), 'warning');
  }

  drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/css/socialloginandsocialshare.admin.css');
  drupal_add_js('jQuery(document).ready(function() {
    jQuery("#edit-thanks-block").css({"background-color":"#FFFFE0"});
   });', array('type' => 'inline', 'scope' => 'header', 'weight' => 5));
  $get_socialloginandsocialshare = '';

  if (!variable_get('socialloginandsocialshare_apikey', FALSE)) {
    $get_socialloginandsocialshare = t('For creating a social login account, please visit <a href="@get_socialloginandsocialshare" target="_blank">LoginRadius</a>', array('@get_socialloginandsocialshare' => 'http://www.Loginradius.com/'));
  }

  $thanks_text = t('To activate the module, you will need to first configure it (manage your desired social networks, etc.) from your LoginRadius account. If you do not have an account, click <a href="@loginradius" target="_blank">here</a>.<br/>We also offer Social Plugins for
  <a href="@joomla" target="_blank">Joomla</a>,
  <a href="@wordpress" target="_blank">Wordpress</a>,
  <a href="@vbulletin" target="_blank">vBulletin</a>,
  <a href="@vanillaforum" target="_blank">VanillaForum</a>,
  <a href="@magento" target="_blank">Magento</a>,
  <a href="@oscommerce" target="_blank">OSCommerce</a>,
  <a href="@prestashop" target="_blank">Prestashop</a>,
  <a href="@xcart" target="_blank">X-Cart</a>,
  <a href="@zencart" target="_blank">Zen-Cart</a>,
  <a href="@phpbb" target="_blank">phpBB</a>,
  <a href="@smf" target="_blank">SMF</a> and
  <a href="@dotnetnuke" target="_blank">DotNetNuke</a>  <br/><a href="@loginradius" target="_blank"><br/><input class="form-submit" type="button" value="Set up my account!"></a> (<a href="@get_sociallogin" target="_blank">How to set up an account?</a>)', array(
    '@loginradius' => 'http://ish.re/4',
    '@joomla' => 'http://ish.re/8PE6',
    '@wordpress' => 'http://ish.re/96IF',
    '@vbulletin' => 'http://ish.re/8PED',
    '@vanillaforum' => 'http://ish.re/8PEE',
    '@magento' => 'http://ish.re/8PEC',
    '@oscommerce' => 'http://ish.re/8PEG',
    '@prestashop' => 'http://ish.re/8PEH',
    '@xcart' => 'http://ish.re/8PFQ',
    '@zencart' => 'http://ish.re/96IC',
    '@dotnetnuke' => 'http://ish.re/96IA',
    '@phpbb' => 'http://ish.re/96I8',
    '@smf' => 'http://ish.re/96I9',
    '@get_sociallogin' => 'http://ish.re/INI1'
  ));
  $form['thanks_block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thank you for installing the LoginRadius Social Module!'),
    '#description' => $thanks_text,
  );
  $setup_group = 'setup';
  $form[$setup_group] = array(
    '#type' => 'fieldset',
    '#title' => t('LoginRadius API Settings'),
    '#id' => 'setup',
    '#collapsible' => TRUE,
    '#description' => $get_socialloginandsocialshare,
  );

  $form[$setup_group]['socialloginandsocialshare_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('socialloginandsocialshare_apikey', ''),
    '#description' => t('To activate the module, insert LoginRadius API Key ( <a href="@get_socialloginandsocialshare" target="_blank">How to get it?</a> )', array('@get_socialloginandsocialshare' => 'http://ish.re/INI1')),
  );
  $form[$setup_group]['socialloginandsocialshare_apisecret'] = array(
    '#type' => 'textfield',
    '#title' => t('API Secret'),
    '#default_value' => variable_get('socialloginandsocialshare_apisecret', ''),
    '#description' => t('To activate the module, insert LoginRadius API Secret ( <a href="@get_socialloginandsocialshare" target="_blank">How to get it?</a> )', array('@get_socialloginandsocialshare' => 'http://ish.re/INI1')),
  );
  $basic_group = 'basic';
  $form[$basic_group] = array(
    '#type' => 'fieldset',
    '#title' => t('SOCIAL LOGIN BASIC SETTINGS'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
    $form[$basic_group]['socialloginandsocialshare_disable_traditional_login'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want users to log in using social login only?'),
        '#default_value' => variable_get('socialloginandsocialshare_disable_traditional_login', 0),
        '#options' => array(
            1 => t('Yes, I want to disable the  Drupal native login form.(In this case, admin can login with his/her linked social account or via /user [example(clean url): site-url/user or example(not clean url): site-url/?q=user])'),
            0 => t('No, I want my users to have both options at their disposal'),
        ),
    );
  $form[$basic_group]['socialloginandsocialshare_display_username'] = array(
    '#type' => 'radios',
    '#title' => t('How would you like username to be created? Select the username syntax as per your preference.'),
    '#default_value' => variable_get('socialloginandsocialshare_display_username', 0),
    '#options' => array(
      0 => t('Firstname Lastname [Ex: John Doe]'),
      1 => t('Firstname-Lastname [Ex: John-Doe]'),
      2 => t('Email Address [Ex: johndoe@abc.com ]'),
    )
  );
  $form[$basic_group]['socialloginandsocialshare_userlogin_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('Redirection settings after login'),
    '#default_value' => variable_get('socialloginandsocialshare_userlogin_redirect', 0),
    '#options' => array(
      0 => t('Redirect to same page of site'),
      1 => t('Redirect to profile page of site'),
      2 => t('Redirect to custom page of site (If you want user to be redirected to specific URL after login)'),
    ),
  );
  $form[$basic_group]['socialloginandsocialshare_userlogin_redirect']['socialloginandsocialshare_custom_redirection'] = array(
    '#type' => 'textfield',
    '#weight' => 50,
    '#default_value' => variable_get('socialloginandsocialshare_custom_redirection'),
  );
  $form[$basic_group]['socialloginandsocialshare_userregister_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('Redirection settings after registration'),
    '#default_value' => variable_get('socialloginandsocialshare_userregister_redirect', 0),
    '#options' => array(
      0 => t('Redirect to same page of site'),
      1 => t('Redirect to profile page of site'),
      2 => t('Redirect to custom page of site (If you want user to be redirected to specific URL after registration)'),
    ),
  );
  $form[$basic_group]['socialloginandsocialshare_userregister_redirect']['socialloginandsocialshare_custom_register_redirection'] = array(
    '#type' => 'textfield',
    '#weight' => 50,
    '#default_value' => variable_get('socialloginandsocialshare_custom_register_redirection', ''),
  );
    $linking_group = 'linking';
    $form[$linking_group] = array(
        '#type' => 'fieldset',
        '#title' => t('SOCIAL ACCOUNT LINKING SETTINGS'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form[$linking_group]['socialloginandsocialshare_account_linking'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to enable social account linking at user profile page?'),
        '#default_value' => variable_get('socialloginandsocialshare_account_linking', 1),
        '#options' => array(
            1 => t('YES'),
            0 => t('NO'),
        )
    );
    $form[$linking_group]['socialloginandsocialshare_identity_label_string'] = array(
        '#type' => 'textfield',
        '#title' => t('What heading do you want to display to the users to link multiple social networks to one account?'),
        '#default_value' => variable_get('socialloginandsocialshare_identity_label_string', 'Link to an existing social account'),
        '#description' => t('This text will be displayed just above social login add more identities interface.'),
    );
  // Submit handler.
  $form['#submit'][] = 'socialloginandsocialshare_admin_settings_submit';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}
function socialloginandsocialshare_lr_advance_settings($form, &$form_state){
  drupal_add_js('jQuery(document).ready(function() {
 var val="' . variable_get("socialloginandsocialshare_send_user_credentials") . '";
 if(val == 1){
  jQuery(".form-item-socialloginandsocialshare-email-content").show();
 }
 else {
  jQuery(".form-item-socialloginandsocialshare-email-content").hide();
 }
  });', array(
    'type' => 'inline',
    'scope' => 'header',
    'weight'=> 4,

  ));
    drupal_add_css(drupal_get_path('module', 'socialloginandsocialshare') . '/css/socialloginandsocialshare.admin.css');
    //interface settings section
    $interface_group = 'interface_settings';
    $form[$interface_group] = array(
        '#type' => 'fieldset',
        '#title' => t('Social Login Interface customization'),
        '#id' => 'interface_settings',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form[$interface_group]['interface']['socialloginandsocialshare_label_string'] = array(
        '#type' => 'textfield',
        '#title' => t('What text do you want to display above the Social Login interface?'),
        '#default_value' => variable_get('socialloginandsocialshare_label_string', 'Login with Social ID'),
        '#description' => t('Leave empty for no text'),
    );
    $form[$interface_group]['socialloginandsocialshare_interface_size'] = array(
        '#type' => 'radios',
        '#title' => t('Select the icon size to use in the Social Login interface'),
        '#default_value' => variable_get('socialloginandsocialshare_interface_size', 0),
        '#options' => array(
            0 => t('Small'),
            1 => t('Large'),
        ),
    );
    $form[$interface_group]['socialloginandsocialshare_interface_columns'] = array(
        '#type' => 'textfield',
        '#title' => t('How many social icons would you like to be displayed per row?'),
        '#size' => 7,
        '#default_value' => variable_get('socialloginandsocialshare_interface_columns', ''),
    );
    $form[$interface_group]['socialloginandsocialshare_interface_bgcolor'] = array(
        '#type' => 'textfield',
        '#title' => t('What background color would you like to use for the Social Login interface?'),
        '#default_value' => variable_get('socialloginandsocialshare_interface_bgcolor', ''),
        '#description' => t('Leave empty for transparent. You can enter hexa-decimal code as well as name of the color.'),
    );
    //interface display settings section
    $display_group = 'interface_display_settings';
    $form[$display_group] = array(
        '#type' => 'fieldset',
        '#title' => t('Social Login Interface Display Settings'),
        '#id' => 'interface_display_settings',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form[$display_group]['interface']['socialloginandsocialshare_attach_login_form'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to show the Social Login interface on the user login block?'),
        '#default_value' => variable_get('socialloginandsocialshare_attach_login_form', 1),
        '#options' => array(
            1 => t('Yes'),
            0 => t('No'),
        ),
    );
    $form[$display_group]['interface']['socialloginandsocialshare_loginpage_position'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to show the Social Login interface with Drupal`s native login form?'),
        '#default_value' => variable_get('socialloginandsocialshare_loginpage_position', 1),
        '#options' => array(
            1 => t('Yes'),
            0 => t('No'),
        ),
    );
    $form[$display_group]['interface']['socialloginandsocialshare_registerpage_position'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to show the Social Login interface with Drupal`s native user registration form?'),
        '#default_value' => variable_get('socialloginandsocialshare_registerpage_position', 1),
        '#options' => array(
            1 => t('Yes'),
            0 => t('No'),
        ),
    );
    $form[$display_group]['interface']['socialloginandsocialshare_commenting_enable'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to enable the Social Login interface on the commenting form?'),
        '#default_value' => variable_get('socialloginandsocialshare_commenting_enable', 1),
        '#options' => array(
            1 => t('Yes'),
            0 => t('No'),
        )
    );
    $form[$display_group]['interface']['socialloginandsocialshare_showicon_location'] = array(
        '#type' => 'radios',
        '#title' => t('How do you want the Social Login interface to be displayed on your Drupal traditional registration/login/commenting form?'),
        '#default_value' => variable_get('socialloginandsocialshare_showicon_location', 0),
        '#options' => array(
            0 => t('Above the native registration/login/commenting form'),
            1 => t('Below the native registration/login/commenting form'),
        ),
    );
    //basic user settings section
    $user_settings_group = 'user_settings';
    $form[$user_settings_group] = array(
        '#type' => 'fieldset',
        '#title' => t('Social Login User settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form[$user_settings_group]['socialloginandsocialshare_enable_user_registration'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to enable Social Login functionality when the Administrators only option is checked?'),
        '#default_value' => variable_get('socialloginandsocialshare_enable_user_registration', 0),
        '#options' => array(
            1 => t('Yes'),
            0 => t('No'),
        ),
    );
    $form[$user_settings_group]['socialloginandsocialshare_force_registration'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want users to get registered automatically or you want them to manually complete the registration process?'),
        '#default_value' => variable_get('socialloginandsocialshare_force_registration', 1),
        '#options' => array(
            1 => t('Automatically register a user and create a new user account'),
            0 => t('Let the user submit the user registration form after social login process.(Users will be redirected to registration page with user fields auto filled and users finally submit the form to create an account with your website.)'),
        ),
    );
    $form[$user_settings_group]['socialloginandsocialshare_email_required'] = array(
        '#type' => 'radios',
        '#title' => t('A few network providers do not supply user email address as part of user profile data. Do you want users to provide their email address before completing the registration process?'),
        '#default_value' => variable_get('socialloginandsocialshare_email_required', 1),
        '#options' => array(
            1 => t('Yes, get real email address from the users (Ask users to enter their email addresses in a pop-up)'),
            0 => t('No, just auto-generate random email IDs for the users'),
        )
    );
    $form[$user_settings_group]['socialloginandsocialshare_emailrequired_popup_top'] = array(
        '#type' => 'textarea',
        '#title' => t('Please enter the title of the pop-up asking users to enter their email address'),
        '#rows' => 2,
        '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_top', 'Please fill the following details to proceed'),
    );
    $form[$user_settings_group]['socialloginandsocialshare_emailrequired_popup_text'] = array(
        '#type' => 'textarea',
        '#title' => t('Please enter the message to be displayed to the user in the pop-up asking for their email address'),
        '#rows' => 2,
        '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_text', 'Unfortunately we could not retrieve your email from your @provider account Please enter your email in the form below in order to continue.'),
        '#description' => t('You may use @provider, it will be replaced by the Provider name.'),
    );
    $form[$user_settings_group]['socialloginandsocialshare_emailrequired_popup_wrong'] = array(
        '#type' => 'textarea',
        '#title' => t('Please enter the message to be shown to the user in case of an already registered email'),
        '#rows' => 2,
        '#default_value' => variable_get('socialloginandsocialshare_emailrequired_popup_wrong', 'This email is already registered. Please enter another email or link this account via account linking on your profile page'),
    );
    $form[$user_settings_group]['socialloginandsocialshare_send_user_credentials'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to send a welcome email to new users after successful registration?'),
        '#default_value' => variable_get('socialloginandsocialshare_send_user_credentials', 1),
        '#options' => array(
            1 => t('Yes'),
            0 => t('No'),
        ),
        '#description' => t('Note: It will work only with networks which give email address like Facebook, linkedin.'),
      '#attributes' => array('onclick' => 'if(this.value == 1){
      jQuery(".form-item-socialloginandsocialshare-email-content").show();}
      else{
      jQuery(".form-item-socialloginandsocialshare-email-content").hide();
      }'),
    );
    $form[$user_settings_group]['socialloginandsocialshare_email_content'] = array(
        '#type' => 'textarea',
        '#title' => t('Edit the welcome e-mail messages sent to new member accounts'),
        '#default_value' => socialloginandsocialshare_email_content_text(),
        '#rows' => 10,
    );
    $form[$user_settings_group]['socialloginandsocialshare_update_profile'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to update the user profile data in your database everytime a user logs into your website?'),
        '#default_value' => variable_get('socialloginandsocialshare_update_profile', 0),
        '#options' => array(
            1 => t('Yes'),
            0 => t('No'),
        )
    );
    module_load_include('inc', 'socialloginandsocialshare', 'socialloginandsocialshare.field');
    //field mapping section
    $field_map_group = 'fieldmap';
    $form[$field_map_group] = array(
        '#type' => 'fieldset',
        '#title' => t('Social Login Field Mapping'),
        '#description' => t('Each of your <a href="!url">fields that are attached to users</a> are listed below. Map social provider data to field by choosing from select list and configure your user fields from <a href="!url"> FIELD MAPPING </a>section.<br/><br/> For a list of all fields according to the providers:<a href="!data_url" target="_blank"> https://secure.loginradius.com/datapoints </a>', array(
            '!url' => url('admin/config/people/accounts/fields'),
            '!data_url' => url('http://ish.re/7O90')
        )),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form[$field_map_group]['socialloginandsocialshare_user_fields'] = array(
        '#title' => 'user fields',
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#weight' => 5,
        '#collapsible' => TRUE,
    );
    $properties = socialloginandsocialshare_field_user_properties();
    $property_options = array();

    foreach ($properties as $property => $property_info) {
        if (isset($property_info['field_types'])) {
            foreach ($property_info['field_types'] as $field_type) {
                $property_options[$field_type][$property] = $property_info['label'];
            }
        }
    }

    $field_defaults = variable_get('socialloginandsocialshare_user_fields', array());
    $instances = field_info_instances('user', 'user');

    foreach ($instances as $field_name => $instance) {
        $field = field_info_field($instance['field_name']);

        if (isset($property_options[$field['type']])) {
            $options = array_merge(array('' => t('- Do not import -')), $property_options[$field['type']]);
            $form[$field_map_group]['socialloginandsocialshare_user_fields'][$field_name] = array(
                '#title' => check_plain(t($instance['label'])),
                '#type' => 'select',
                '#options' => $options,
                '#default_value' => isset($field_defaults[$field_name]) ? $field_defaults[$field_name] : '',
            );
        }
        else {
            $form[$field_map_group]['socialloginandsocialshare_user_fields'][$field_name] = array(
                '#title' => check_plain(t($instance['label'])),
                '#type' => 'form_element',
                '#children' => '<em>' . t('Not any mappable properties.') . '</em>',
                '#theme_wrappers' => array('form_element'),
            );
        }
    }

    if (module_exists('profile2')) {
        $form[$field_map_group]['socialloginandsocialshare_profile_fields'] = array(
            '#title' => 'Profile2 fields',
            '#type' => 'fieldset',
            '#tree' => TRUE,
            '#weight' => 5,
            '#collapsible' => TRUE,
        );

        $properties = socialloginandsocialshare_field_user_properties();
        $property_options = array();

        foreach ($properties as $property => $property_info) {
            if (isset($property_info['field_types'])) {
                foreach ($property_info['field_types'] as $field_type) {
                    $property_options[$field_type][$property] =  $property_info['label'];
                }
            }
        }

        $field_defaults = variable_get('socialloginandsocialshare_profile_fields', array());

        foreach (field_info_instances('profile2') as $field_name => $instances) {
            $form[$field_map_group]['socialloginandsocialshare_profile_fields'][$field_name] = array(
                '#type' => 'fieldset',
                '#title' => check_plain(t($field_name)),
                '#tree' => TRUE,
                '#weight' => 5,
                '#collapsible' => TRUE,
            );

            foreach ($instances as $instance => $array) {
                $field = field_info_field($array['field_name']);

                if (isset($property_options[$field['type']])) {
                    $options = array_merge(array('' => t('- Do not import -')), $property_options[$field['type']]);
                    $form[$field_map_group]['socialloginandsocialshare_profile_fields'][$field_name][$array['field_name']] = array(
                        '#title' => check_plain(t($array['label'])),
                        '#type' => 'select',
                        '#options' => $options,
                        '#default_value' => isset($field_defaults[$field_name][$array['field_name']]) ? $field_defaults[$field_name][$array['field_name']] : '',
                    );
                }
                else {
                    $form[$field_map_group]['socialloginandsocialshare_profile_fields'][$field_name][$array['field_name']] = array(
                        '#title' => check_plain(t($array['label'])),
                        '#type' => 'form_element',
                        '#children' => '<em>' . t('Not any mappable properties.') . '</em>',
                        '#theme_wrappers' => array('form_element'),
                    );
                }
            }
        }
    }

    //debug mode section
    $debug_group = 'debug';
    $form[$debug_group] = array(
        '#type' => 'fieldset',
        '#title' => t('Debug'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form[$debug_group]['socialloginandsocialshare_debug_mode'] = array(
        '#type' => 'radios',
        '#title' => t('Do you want to enable Debugging mode?'),
        '#default_value' => variable_get('socialloginandsocialshare_debug_mode', 0),
        '#options' => array(
            1 => t('Yes'),
            0 => t('No'),
        )
    );
    return system_settings_form($form);
}
/**
 * Save settings to database.
 *
 * @param array $form_state A keyed array containing the current state of the form
 */
function socialloginandsocialshare_save_settings(&$form_state) {
  //Validate APIs.
  $api_key = isset($form_state['values']['socialloginandsocialshare_apikey']) ? trim($form_state['values']['socialloginandsocialshare_apikey']) : trim(variable_get('socialloginandsocialshare_apikey', ''));
  $api_secret = isset($form_state['values']['socialloginandsocialshare_apisecret']) ? trim($form_state['values']['socialloginandsocialshare_apisecret']) : trim(variable_get('socialloginandsocialshare_apisecret', ''));
  $data = socialloginandsocialshare_get_authentication($api_key, $api_secret);

  if (isset($data['status']) && $data['status'] != 'status') {
    drupal_set_message($data['message'], $data['status']);
    return FALSE;
  }
  form_state_values_clean($form_state);

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }

  drupal_set_message(t('The configuration options have been saved.'));
  // Clear the cached pages and blocks.
  cache_clear_all();
  menu_rebuild();
  return TRUE;
}

/**
 * Submit Social Login tab settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 */
function socialloginandsocialshare_admin_settings_submit($form, &$form_state) {
  if (!socialloginandsocialshare_save_settings($form_state)) {
    return FALSE;
  }
    $settings = array(
        'socialloginandsocialshare_label_string' => 'Login with Social ID',
        'socialloginandsocialshare_interface_size' => '0',
        'socialloginandsocialshare_attach_login_form' => '1',
        'socialloginandsocialshare_loginpage_position' => '1',
        'socialloginandsocialshare_registerpage_position' => '1',
        'socialloginandsocialshare_showicon_location' => '0',
        'socialloginandsocialshare_commenting_enable' => '1',
        'socialloginandsocialshare_force_registration' => '1',
        'socialloginandsocialshare_email_required' => '1',
        'socialloginandsocialshare_emailrequired_popup_top' => 'Please fill the following details to proceed',
        'socialloginandsocialshare_emailrequired_popup_text' => 'Unfortunately we could not retrieve your email from your @provider account Please enter your email in the form below in order to continue.',
        'socialloginandsocialshare_emailrequired_popup_wrong' => 'This email is already registered. Please enter another email or link this account via account linking on your profile page',
        'socialloginandsocialshare_send_user_credentials' => '1',
        'socialloginandsocialshare_update_profile' => '0',
    );

    foreach ($settings as $key => $values) {
        if (!variable_get($key, FALSE) && variable_get($key, '') != '0') {
            variable_set($key, $values);
        }
    }
}

/**
 * Function validates against GUID format of keys
 *
 * @param string $key LoginRadius API key/secret
 * @return boolean If valid - true, else - false
 */
function socialloginandsocialshare_isvalid_key($key) {
  return preg_match('/^\{?[A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}\}?$/i', $key);
}



/**
 * Check API credentials and API connection methos is working or not
 *
 * @param string $apikey LoginRadius API Key
 * @param string $apisecret LoginRadius API Secret
 * @param string $api_handler API connection method curl/drupal_http_request
 * @return array Contain message and status of data
 */
function socialloginandsocialshare_get_authentication($apikey, $apisecret) {
  if (isset($apikey)) {
  $data = array();
    $obj_auth = new LoginRadius();

    try {
      $validate_url = 'https://' . LR_DOMAIN . '/api/v2/app/validate?apikey=' . rawurlencode(trim($apikey)) . '&apisecret=' . rawurlencode(trim($apisecret));
      $result = json_decode($obj_auth->loginradius_get_api_response($validate_url, FALSE));

      if (empty($result)) {
        $data['message'] = check_plain('please check your php.ini settings to enable CURL or FSOCKOPEN');
        $data['status'] = 'error';
      }
      elseif (isset($result->Status) && !$result->Status) {
        $error = array(
          "API_KEY_NOT_VALID" => "LoginRadius API key is invalid. Get your LoginRadius API key from <a href='http://www.loginradius.com' target='_blank'>LoginRadius</a>",
          "API_SECRET_NOT_VALID" => "LoginRadius API Secret is invalid. Get your LoginRadius API Secret from <a href='http://www.loginradius.com' target='_blank'>LoginRadius</a>",
          "API_KEY_NOT_FORMATED" => "LoginRadius API Key is not formatted correctly",
          "API_SECRET_NOT_FORMATED" => "LoginRadius API Secret is not formatted correctly"
        );

        foreach ($result->Messages as $value) {
          $data['message'] = $error["$value"];
          $data['status'] = 'error';
        }
      }

      return $data;
    }
    catch (Exception $e) {
      watchdog_exception('type', $e);
    }
  }
}