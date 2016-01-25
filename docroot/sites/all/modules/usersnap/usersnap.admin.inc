<?php

/**
 * @file
 * Code related to administrative tasks for the Usersnap module.
 */


/**
 * Provides a Configuration Form for entering the Usersnap API Key.
 */
function usersnap_admin() {
  $form = array();
  $form['usersnap_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter the Usersnap API Key'),
    '#default_value' => variable_get('usersnap_apikey', ''),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t("Enter the Usersnap API Key. You can get a Key at http://www.usersnap.com."),
    '#required' => TRUE,
  );
  $form['usersnap_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget button text'),
    '#description' => t('Button text displayed on the Usersnap feedback widget'),
    '#default_value' => variable_get('usersnap_button_text', 'Feedback'),
    '#required' => TRUE,
  );
  $form['usersnap_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show widget on specific pages'),
    '#default_value' => variable_get('usersnap_visibility', USERSNAP_VISIBILITY_NOTLISTED),
    '#options' => array(
      USERSNAP_VISIBILITY_NOTLISTED => t('All pages except those listed'),
      USERSNAP_VISIBILITY_LISTED => t('Only the listed pages'),
    ),
  );
  $form['usersnap_paths'] = array(
    '#type' => 'textarea',
    '#title' => '',
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>')
    ),
    '#default_value' => variable_get('usersnap_paths', ''),
  );
  $form['usersnap_position'] = array(
    '#type' => 'select',
    '#title' => t('Widget position'),
    '#description' => t('The feedback widget will be placed in this part of the screen.'),
    '#default_value' => variable_get('usersnap_position', 'bottom_right'),
    '#options' => array(
      'bottom_right' => t('Bottom right corner'),
      'bottom_left' => t('Bottom left corner'),
      'middle_right' => t('Middle right'),
      'middle_left' => t('Middle left'),
    ),
  );
  $form['usersnap_language'] = array(
    '#type' => 'select',
    '#title' => t('Widget language'),
    '#description' => t('Preferred interface language for feedback widget.'),
    '#default_value' => variable_get('usersnap_language', 'en'),
    '#options' => usersnap_supported_languages(),
  );
  $form['usersnap_show_email_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show e-mail field'),
    '#default_value' => variable_get('usersnap_show_email_field', FALSE),
  );
  $form['usersnap_email_field_placeholder'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail field placeholder text'),
    '#description' => t('Placeholder text shown to users in the e-mail field'),
    '#default_value' => variable_get('usersnap_email_field_placeholder', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="usersnap_show_email_field"]' => array(
          'checked' => TRUE,
        ),
      ),
    )
  );
  $form['usersnap_email_field_required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require e-mail'),
    '#default_value' => variable_get('usersnap_email_field_required', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="usersnap_show_email_field"]' => array(
          'checked' => TRUE,
        ),
      ),
    )
  );
  $form['usersnap_show_comment_box'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show comment box'),
    '#default_value' => variable_get('usersnap_show_comment_box', FALSE),
  );
  $form['usersnap_comment_box_placeholder'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment box placeholder text'),
    '#description' => t('Text shown to users in the comment box'),
    '#default_value' => variable_get('usersnap_comment_box_placeholder', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="usersnap_show_comment_box"]' => array(
          'checked' => TRUE,
        ),
      ),
    )
  );
  $form['usersnap_comment_box_required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require comment'),
    '#default_value' => variable_get('usersnap_comment_box_required', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="usersnap_show_comment_box"]' => array(
          'checked' => TRUE,
        ),
      ),
    )
  );
  return system_settings_form($form);
}

/**
 * Helper function to return Usersnap supported languages and language codes.
 *
 * @see https://usersnap.com/docs#language
 */
function usersnap_supported_languages() {
  return array(
    'en' => t('English'),
    'de-informal' => t('German (informal)'),
    'de-formal' => t('German (formal)'),
    'fr' => t('French'),
    'es' => t('Spanish'),
    'pl' => t('Polish'),
    'fa' => t('Farsi'),
    'is' => t('Icelandic'),
    'jp' => t('Japanese'),
    'it' => t('Italian'),
    'hu' => t('Hungarian'),
    'ko' => t('Korean'),
    'cz' => t('Czech'),
    'da' => t('Danish'),
    'no' => t('Norwegian'),
    'sk' => t('Slovakian'),
    'nl' => t('Dutch'),
    'sv' => t('Swedish'),
    'pt' => t('Portuguese'),
    'fi' => t('Finnish'),
    'ru' => t('Russian'),
    'tr' => t('Turkish'),
  );
}
