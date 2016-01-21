<?php

/**
 * @file
 * Rijksoverheid Cookie Opt-in admin functions.
 */

/**
 * Settings page callback function.
 */
function ro_cookie_opt_in_settings_form($form, &$form_state) {
  $form = array();

  // General settings.
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0,
  );
  $form['general']['ro_cookie_opt_in_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Rijksoverheid Cookie Opt-in'),
    '#description' => t('Enable the Rijksoverheid Cookie Opt-in functionality.'),
    '#default_value' => variable_get('ro_cookie_opt_in_enabled', TRUE),
    '#weight' => 0,
  );
  $form['general']['ro_cookie_opt_in_log_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Rijksoverheid Cookie Opt-in log'),
    '#description' => t('Enable logging of all responses to the Drupal database. If you disable this, you will have to implement other ways of keeping the responses to conform to the Dutch cookie law. You can keep all apache access logs for example, but be aware that you have to keep them for at least five years.'),
    '#default_value' => variable_get('ro_cookie_opt_in_log_enabled', TRUE),
    '#weight' => 5,
  );
  $form['general']['cleanup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Log cleanup'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 10,
    '#states' => array(
      'invisible' => array(
        ':input[name="ro_cookie_opt_in_log_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['general']['cleanup']['ro_cookie_opt_in_log_cleanup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Periodically cleanup the Rijksoverheid Cookie Opt-in log'),
    '#description' => t('Enable periodic log cleanup and remove entries older than the number of configured days.'),
    '#default_value' => variable_get('ro_cookie_opt_in_log_cleanup', FALSE),
    '#weight' => 0,
  );
  $form['general']['cleanup']['ro_cookie_opt_in_log_days_to_keep'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#description' => t('Number of days to keep cookielog entries before they are cleaned up.'),
    '#default_value' => variable_get('ro_cookie_opt_in_log_days_to_keep', 1825),
    '#weight' => 5,
    '#states' => array(
      'invisible' => array(
        ':input[name="ro_cookie_opt_in_log_cleanup"]' => array('checked' => FALSE),
      ),
    ),
  );
  // Library settings.
  $form['general']['library_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rijksoverheid Cookie Opt-in library settings'),
    '#description' => t('Settings for the Rijksoverheid Cookie Opt-in library.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 15,
  );
  $form['general']['library_settings']['cookie_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie settings'),
    '#description' => t('Settings for the cookie.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 0,
  );
  $form['general']['library_settings']['cookie_settings']['ro_cookie_opt_in_cookie_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie name'),
    '#description' => t('The name of the cookie in which the consent is stored.'),
    '#default_value' => variable_get('ro_cookie_opt_in_cookie_name', 'consent_for_cookies'),
    '#weight' => 0,
  );
  $form['general']['library_settings']['cookie_settings']['ro_cookie_opt_in_cookie_accept'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie accept value'),
    '#description' => t('The value that is stored in the cookie if consent is given.'),
    '#default_value' => variable_get('ro_cookie_opt_in_cookie_accept', 'yes'),
    '#weight' => 5,
  );
  $form['general']['library_settings']['cookie_settings']['ro_cookie_opt_in_cookie_deny'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie deny value'),
    '#description' => t('The value that is stored in the cookie if consent is not given.'),
    '#default_value' => variable_get('ro_cookie_opt_in_cookie_deny', 'no'),
    '#weight' => 10,
  );
  $form['general']['library_settings']['ro_cookie_opt_in_add_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Rijkshuisstijl CSS'),
    '#description' => t('Add the Rijkshuisstijl CSS to the output of the site.'),
    '#default_value' => variable_get('ro_cookie_opt_in_add_css', TRUE),
    '#weight' => 15,
  );
  $form['general']['library_settings']['ro_cookie_opt_in_add_resp_fix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Responsive popup fix CSS'),
    '#description' => t('By default the cookie popup is not responsive. Add extra CSS make the cookie popup responsive.'),
    '#default_value' => variable_get('ro_cookie_opt_in_add_resp_fix', FALSE),
    '#weight' => 20,
    '#states' => array(
      'invisible' => array(
        ':input[name="ro_cookie_opt_in_add_css"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['text'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['text']['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General text settings'),
    '#description' => t('General texts.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 0,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['general']['ro_cookie_opt_in_close'] = array(
    '#type' => 'textfield',
    '#title' => t('Close link'),
    '#description' => t('The close link text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_close', 'Close this menu bar'),
    '#weight' => 0,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['question'] = array(
    '#type' => 'fieldset',
    '#title' => t('Question texts'),
    '#description' => t('Texts that are used when asking for consent.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 5,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['question']['ro_cookie_opt_in_question_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The title text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_question_title', 'Cookies on Example.com'),
    '#weight' => 0,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['question']['ro_cookie_opt_in_question_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#description' => t('The introduction text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_question_intro', 'May Example.com place <a href="/cookies">cookies</a> on your computer to make the site easier for you to use?'),
    '#weight' => 5,
    '#group' => 'text_vert_tabs',
  );
  $qlm_default = variable_get('ro_cookie_opt_in_question_learn_more', array(
    'value' => '<p>I first want to know more ...</p><ul><li><a href="/cookies">What do cookies do?</a></li><li><a href="/privacy">What about my privacy?</a></li></ul>',
    'format' => NULL,
  ));
  $form['text']['question']['ro_cookie_opt_in_question_learn_more'] = array(
    '#type' => 'text_format',
    '#title' => t('Learn more links'),
    '#description' => t('The learn more links text. Please note that the Rijksoverheid Cookie Opt-in library expects a paragraph followed by an unordered list!'),
    '#default_value' => $qlm_default['value'],
    '#format' => $qlm_default['format'],
    '#weight' => 10,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['change'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change texts'),
    '#description' => t('Texts that are used when changing consent.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 10,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['change']['ro_cookie_opt_in_change_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The title text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_change_title', 'Changing your cookie preferences on Example.com'),
    '#weight' => 0,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['change']['ro_cookie_opt_in_change_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#description' => t('The introduction text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_change_intro', ''),
    '#weight' => 5,
    '#group' => 'text_vert_tabs',
  );
  $clm_default = variable_get('ro_cookie_opt_in_change_learn_more', array(
    'value' => '<p>I first want to know more ...</p><ul><li><a href="/cookies">What do cookies do?</a></li><li><a href="/privacy">What about my privacy?</a></li></ul>',
    'format' => NULL,
  ));
  $form['text']['change']['ro_cookie_opt_in_change_learn_more'] = array(
    '#type' => 'text_format',
    '#title' => t('Learn more links'),
    '#description' => t('The learn more links text. Please note that the Rijksoverheid Cookie Opt-in library expects a paragraph followed by an unordered list'),
    '#default_value' => $clm_default['value'],
    '#format' => $clm_default['format'],
    '#weight' => 10,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['accept'] = array(
    '#type' => 'fieldset',
    '#title' => t('Accept texts'),
    '#description' => t('Texts that are used when consent is given.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 15,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['accept']['ro_cookie_opt_in_accept_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Button'),
    '#description' => t('The accept button text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_accept_button', 'Yes, I accept cookies'),
    '#weight' => 0,
    '#group' => 'text_vert_tabs',
  );
  $ae_default = variable_get('ro_cookie_opt_in_accept_extras', array(
    'value' => '<ul><li>Example.com will collect anonymous information about your visits to help improve the website.</li></ul>',
    'format' => NULL,
  ));
  $form['text']['accept']['ro_cookie_opt_in_accept_extras'] = array(
    '#type' => 'text_format',
    '#title' => t('Extras'),
    '#description' => t('The extra button text. Please note that the Rijksoverheid Cookie Opt-in library expects an unordered list'),
    '#default_value' => $ae_default['value'],
    '#format' => $ae_default['format'],
    '#weight' => 5,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['accept']['ro_cookie_opt_in_accept_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The title text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_accept_title', 'You have chosen to accept cookies from Example.com'),
    '#weight' => 10,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['accept']['ro_cookie_opt_in_accept_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#description' => t('The introduction text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_accept_intro', 'Thank you for permitting Example.com to place cookies on your computer. You can always <a href="#" class="ck-change">change your mind</a>.'),
    '#weight' => 15,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['accept']['ro_cookie_opt_in_accept_current'] = array(
    '#type' => 'textarea',
    '#title' => t('Current'),
    '#description' => t('The current selection text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_accept_current', 'You now accept cookies from Example.com. You can change your preference below. May Example.com continue to place <a href="/cookies">cookies</a> on your computer to make the site easier for you to use?'),
    '#weight' => 20,
    '#group' => 'text_vert_tabs',
  );
  $alm_default = variable_get('ro_cookie_opt_in_accept_learn_more', array(
    'value' => '<ul><li><a href="/cookies">What do cookies do?</a></li><li><a href="/privacy">What about my privacy?</a></li></ul>',
    'format' => NULL,
  ));
  $form['text']['accept']['ro_cookie_opt_in_accept_learn_more'] = array(
    '#type' => 'text_format',
    '#title' => t('Learn more links'),
    '#description' => t('The learn more links text. Please note that the Rijksoverheid Cookie Opt-in library expects an unordered list'),
    '#default_value' => $alm_default['value'],
    '#format' => $alm_default['format'],
    '#weight' => 25,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['deny'] = array(
    '#type' => 'fieldset',
    '#title' => t('Deny texts'),
    '#description' => t('Texts that are used when consent is not given.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 20,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['deny']['ro_cookie_opt_in_deny_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Button'),
    '#description' => t('The deny button text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_deny_button', "No, I don't accept cookies"),
    '#weight' => 0,
    '#group' => 'text_vert_tabs',
  );
  $de_default = variable_get('ro_cookie_opt_in_deny_extras', array(
    'value' => '<ul><li>Example.com will not use cookies to collect anonymous information about your visits.</li></ul>',
    'format' => NULL,
  ));
  $form['text']['deny']['ro_cookie_opt_in_deny_extras'] = array(
    '#type' => 'text_format',
    '#title' => t('Extras'),
    '#description' => t('The extra button text. Please note that the Rijksoverheid Cookie Opt-in library expects an unordered list'),
    '#default_value' => $de_default['value'],
    '#format' => $de_default['format'],
    '#weight' => 5,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['deny']['ro_cookie_opt_in_deny_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The title text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_deny_title', 'Example.com respects your choice'),
    '#weight' => 10,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['deny']['ro_cookie_opt_in_deny_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#description' => t('The introduction text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_deny_intro', 'This website will not use cookies to collect information about your visits.'),
    '#weight' => 15,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['deny']['ro_cookie_opt_in_deny_current'] = array(
    '#type' => 'textarea',
    '#title' => t('Current'),
    '#description' => t('The current selection text.'),
    '#default_value' => variable_get('ro_cookie_opt_in_deny_current', 'You do not accept cookies from Example.com. You can change this preference below. May Example.com place cookies on your computer to make the site easier for you to use?'),
    '#weight' => 20,
    '#group' => 'text_vert_tabs',
  );
  $dlm_default = variable_get('ro_cookie_opt_in_deny_learn_more', array(
    'value' => '<ul><li><a href="/cookies">What do cookies do?</a></li><li><a href="/privacy">What about my privacy?</a></li><li><a href="#" class="ck-change">Change your cookie preferences</a></li></ul>',
    'format' => NULL,
  ));
  $form['text']['deny']['ro_cookie_opt_in_deny_learn_more'] = array(
    '#type' => 'text_format',
    '#title' => t('Learn more links'),
    '#description' => t('The learn more links text. Please note that the Rijksoverheid Cookie Opt-in library expects an unordered list'),
    '#default_value' => $dlm_default['value'],
    '#format' => $dlm_default['format'],
    '#weight' => 25,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['pref_block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie preferences block texts'),
    '#description' => t('Texts that are used for the cookie preferences block.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 25,
    '#group' => 'text_vert_tabs',
  );
  $form['text']['pref_block']['ro_cookie_opt_in_pre_block_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie preferences link text'),
    '#description' => t('The text to use for the cookie preferences link.'),
    '#default_value' => variable_get('ro_cookie_opt_in_pre_block_link_text', 'Change your cookie preferences'),
  );

  // Show that tokens can be used and if the Token module is installed, show the
  // available replacement patterns. Otherwise show a messages that tokens can
  // be used and to get suggestions the Token module must be installed.
  if (module_exists('token')) {
    $form['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#description' => t('In all text settings above you can use tokens as per the replacement patterns below.'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array(),
    );
  }
  else {
    $form['token_help'] = array(
      '#markup' => '<div>' . t('In all text settings above you can use tokens. To get suggestions about which tokens are available, install the Token module.') . '</div>',
    );
  }

  return system_settings_form($form);
}
