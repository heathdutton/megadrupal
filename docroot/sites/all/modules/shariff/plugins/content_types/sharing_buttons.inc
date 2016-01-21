<?php

/**
 * @file
 * Plugin to provide a Shariff sharing buttons pane for the respective template.
 */

$plugin = array(
  'title' => t('Shariff sharing buttons'),
  'single' => TRUE,
  'description' => t('Displays Shariff sharing buttons.'),
  'category' => t('Widgets'),
  'all contexts' => TRUE,
);

/**
 * Output function for the 'shariff_buttons' content type.
 */
function shariff_sharing_buttons_content_type_render($subtype, $conf, $args, $contexts) {
  $block = new stdClass();

  // Load and prepare settings.
  if (!$conf['default_settings']) {
    global $language;
    $lang = $language->language;
    $supported_languages = _shariff_supported_languages();

    foreach ($conf['services'] as $service) {
      if ($service) {
        $services[] = $service;
      }
    }

    // For some settings we allow context specific substitutions.
    foreach (array('twitter_via', 'referrer_track', 'mail_url', 'url', 'media_url', 'mail_subject', 'mail_body') as $setting) {
      if (!empty($contexts) && !empty($conf[$setting])) {
        $conf[$setting] = ctools_context_keyword_substitute($conf[$setting], array(), $contexts);
      }
    }

    $theme_args = array(
      'services'        => $services,
      'theme'           => (empty($conf['theme'])) ? NULL : $conf['theme'],
      'orientation'     => (empty($conf['orientation'])) ? NULL : $conf['orientation'],
      'twitter_via'     => (empty($conf['twitter_via'])) ? NULL : $conf['twitter_via'],
      'referrer_track'  => (empty($conf['referrer_track'])) ? NULL : $conf['referrer_track'],
      'backend_url'     => (empty($conf['backend_url'])) ? NULL : $conf['backend_url'],
      'mail_url'        => (empty($conf['mail_url'])) ? NULL : $conf['mail_url'],
      'mail_subject'    => (empty($conf['mail_subject'])) ? NULL : $conf['mail_subject'],
      'mail_body'       => (empty($conf['mail_body'])) ? NULL : $conf['mail_body'],
      'lang'            => in_array($lang, $supported_languages) ? $lang : 'en',
      'url'             => (empty($conf['url'])) ? NULL : $conf['url'],
      'flattr_category' => (empty($conf['flattr_category'])) ? NULL : $conf['flattr_category'],
      'flattr_user'     => (empty($conf['flattr_user'])) ? NULL : $conf['flattr_user'],
      'media_url'       => (empty($conf['media_url'])) ? NULL : $conf['media_url'],
    );
  }

  // Prepare default settings to be used as arguments.
  else {
    $theme_args = _shariff_get_settings();
  }

  if (!empty($theme_args['services'])) {
    $block->content = theme('shariff', $theme_args);
  }

  return $block;
}

/**
 * Implements hook_content_type_admin_info().
 */
function shariff_sharing_buttons_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title = t('Shariff sharing buttons');
  $block->content = t('Displays Shariff sharing buttons.');
  return $block;
}

/**
 * Form constructor for the content type edit form.
 *
 * @see shariff_sharing_buttons_content_type_edit_form_validate()
 * @see shariff_sharing_buttons_content_type_edit_form_submit()
 *
 * @ingroup forms
 */
function shariff_sharing_buttons_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['default_settings'] = array(
    '#title' => t('Use Shariff default settings?'),
    '#description' => t('When set default Shariff settings are used. Uncheck to overwrite settings here.'),
    '#type' => 'checkbox',
    '#checked' => TRUE,
    '#default_value' => !isset($conf['default_settings']) ? TRUE : $conf['default_settings'],
  );

  $form['services'] = array(
    '#title' => t('Activated services'),
    '#description' => t('Please define for which services a sharing button should be included.'),
    '#type' => 'checkboxes',
    '#options' => array(
      'twitter' => t('Twitter'),
      'facebook' => t('Facebook'),
      'googleplus' => t('Google+'),
      'linkedin' => t('LinkedIn'),
      'pinterest' => t('Pinterest'),
      'xing' => t('Xing'),
      'whatsapp' => t('WhatsApp'),
      'addthis' => t('AddThis'),
      'tumblr' => t('Tumblr'),
      'flattr' => t('Flattr'),
      'diaspora' => t('Diaspora'),
      'reddit' => t('reddit'),
      'stumbleupon' => t('StumbleUpon'),
      'threema' => t('Threema'),
      'mail' => t('E-Mail'),
      'info' => t('Info Button'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => !isset($conf['services']) ? array('twitter', 'facebook') : $conf['services'],
  );
  $form['theme'] = array(
    '#title' => t('Theme'),
    '#description' => t('Please choose a layout option.'),
    '#type' => 'radios',
    '#options' => array(
      'colored' => t('Colored'),
      'grey' => t('Grey'),
      'white' => t('White'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => !isset($conf['theme']) ? 'colored' : $conf['theme'],
  );
  $form['orientation'] = array(
    '#title' => t('Orientation'),
    '#description' => t('Vertical will stack the buttons vertically. Default is horizontally.'),
    '#type' => 'radios',
    '#options' => array(
      'vertical' => t('Vertical'),
      'horizontal' => t('Horizontal'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => !isset($conf['orientation']) ? 'horizontal' : $conf['orientation'],
  );
  $form['twitter_via'] = array(
    '#title' => t('Twitter Via User'),
    '#description' => t('Screen name of the Twitter user to attribute the Tweets to.'),
    '#type' => 'textfield',
    '#default_value' => !isset($conf['twitter_via']) ? '' : $conf['twitter_via'],
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['url'] = array(
    '#title' => t('Share URL'),
    '#description' => t('The canonical URL of the page to share. Leave blank to use the URL from the page where the button is displayed (or og:url).'),
    '#type' => 'textfield',
    '#default_value' => !isset($conf['url']) ? '' : $conf['url'],
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['mail_url'] = array(
    '#title' => t('Mail link'),
    '#description' => t('The url target used for the mail service button. Leave it as "mailto:" to let the user
 choose an email address.'),
    '#type' => 'textfield',
    '#default_value' => !isset($conf['mail_url']) ? 'mailto:' : $conf['mail_url'],
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['mail_subject'] = array(
    '#title' => t('Mail subject'),
    '#description' => t("If a mailto: link is provided in Mail link above, then this value is used as the mail subject.
 You can add context specific substitutions like %node:title if given. Left empty the page's current (canonical) URL or og:url is used."),
    '#type' => 'textfield',
    '#default_value' => !isset($conf['mail_subject']) ? '' : $conf['mail_subject'],
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['mail_body'] = array(
    '#title' => t('Mail body'),
    '#description' => t("If a mailto: link is provided in Mail link above, then this value is used as the mail body.
 You can add context specific substitutions like %node:title if given. Left empty the page title is used."),
    '#type' => 'textarea',
    '#default_value' => !isset($conf['mail_body']) ? '' : $conf['mail_body'],
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['referrer_track'] = array(
    '#title' => t('Referrer track code'),
    '#description' => t('A string that will be appended to the share url. Disabled when empty.'),
    '#type' => 'textfield',
    '#default_value' => !isset($conf['referrer_track']) ? '' : $conf['referrer_track'],
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['backend_url'] = array(
    '#title' => t('Backend URL'),
    '#description' => t('The path to your Shariff backend. Leaving the value blank disables the backend feature and no counts will occur.'),
    '#type' => 'textfield',
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => !isset($conf['backend_url']) ? '' : $conf['backend_url'],
  );
  $form['flattr_category'] = array(
    '#title' => t('Flattr category'),
    '#description' => t('Category to be used for Flattr.'),
    '#type' => 'textfield',
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => !isset($conf['flattr_category']) ? '' : $conf['flattr_category']
  );
  $form['flattr_user'] = array(
    '#title' => t('Flattr user'),
    '#description' => t('User that receives Flattr donation.'),
    '#type' => 'textfield',
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => !isset($conf['flattr_user']) ? '' : $conf['flattr_user']
  );
  $form['media_url'] = array(
    '#title' => t('Media url'),
    '#description' => t('Media url to be shared (Pinterest). You can add context specific substitutions.'),
    '#type' => 'textfield',
    '#states' => array(
      'visible' => array(
        ':input[name="default_settings"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => !isset($conf['media_url']) ? '' : $conf['media_url']
  );

  return $form;
}

/**
 * Form validation handler for shariff_sharing_buttons_content_type_edit_form().
 *
 * @see shariff_sharing_buttons_content_type_edit_form()
 */
function shariff_sharing_buttons_content_type_edit_form_validate($form, &$form_state) {
  $backend_url = $form_state['values']['backend_url'];
  if ($backend_url && !valid_url($backend_url, TRUE)) {
    form_set_error('backend_url', t('Please enter a valid URL.'));
  }
}

/**
 * Form submission handler for shariff_sharing_buttons_content_type_edit_form().
 *
 * @see shariff_sharing_buttons_content_type_edit_form()
 */
function shariff_sharing_buttons_content_type_edit_form_submit($form, &$form_state) {

  // Storing data in conf.
  if (isset($form_state['values']['default_settings'])) {
    $form_state['conf']['default_settings'] = $form_state['values']['default_settings'];
  }

  $data_settings = array(
    'services',
    'shariff_theme',
    'orientation',
    'twitter_via',
    'referrer_track',
    'mail_url',
    'mail_subject',
    'mail_body',
    'backend_url',
    'flattr_category',
    'flattr_user',
    'media_url',
    'lang',
    'url',
  );

  foreach ($data_settings as $setting) {
    if (isset($form_state['values'][$setting])) {
      $form_state['conf'][$setting] = $form_state['values'][$setting];
    }
  }
}
