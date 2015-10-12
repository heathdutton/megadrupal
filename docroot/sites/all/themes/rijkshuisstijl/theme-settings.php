<?php
/**
 * @file
 * Functionality for the admin form of the theme.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function rijkshuisstijl_form_system_theme_settings_alter(&$form, $form_state) {

  $form['branding'] = array(
    '#type' => 'fieldset',
    '#title' => t('Branding'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => -20,
  );

  $form['branding']['branding_type'] = array(
    '#type' => 'radios',
    '#title' => t('Branding type'),
    '#default_value' => theme_get_setting('branding_type'),
    '#options' => array(
      'none' => t('None'),
      'rijkshuisstijl' => t('Government logo'),
      'neutral' => t('Neutral wordmark'),
    ),
    '#description' => t('Use the government logo for websites of the Dutch government. Use the neutral wordmark for projects in which the Dutch government participates with other organisations.' . '<br />' . t('More information about branding structure in "Huisstijlhandboek Afzenderschap en samenwerkingsverbanden".'), array('%system' => 'system')),
  );

  $form['branding']['authorisation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Yes, this website is authorised to use the visual identity of the State of the Netherlands.'),
    '#options' => array(
      '1' => 'yes',
      '0' => 'no',
    ),
    '#default_value' => theme_get_setting('authorisation'),
    '#description' => t("By ministerial order of 16 June 2008 (<a href=\"https://zoek.officielebekendmakingen.nl/stcrt-2008-115-p7-SC86194.html\" target=\"_blank\">Government Gazette 2008, no.115</a>), the State of the Netherlands (Ministry of General Affairs) entered a copyright reservation, as referred to in section 15b of the Copyright Act 1912, concerning the use and application of central government's logo, lay-out, color scheme and fonts. This reservation does not apply to parties to whom central government has supplied the Rijkslogo and the visual identity for use in carrying out activities or services commissioned by central government. To obtain authorisation, you should apply to the Government Information Service."),
    '#states' => array(
      'visible' => array(
        ':input[name="branding_type"]' => array(array('value' => 'neutral'), array('value' => 'rijkshuisstijl')),
      )),
  );
  
  $form['wordmark'] = array(
    '#type' => 'fieldset',
    '#title' => t('Wordmark'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => -19,
    '#states' => array(
      'visible' => array(
        ':input[name="branding_type"]' => array(array('value' => 'neutral'), array('value' => 'rijkshuisstijl')),
      ),
    ),
  );

  $form['wordmark']['project_name'] = array(
    '#type' => 'textarea',
    '#title' => t('Name'),
    '#default_value' => theme_get_setting('project_name'),
    '#rows' => 2,
    '#resizable' => FALSE,
    '#description' => t("Name of the project. Characters between '{' and '}' are printed in the main color."),
    '#states' => array(
      'visible' => array(
        ':input[name="branding_type"]' => array('value' => 'neutral'),
      ),
    ),
  );

  $form['wordmark']['sender'] = array(
    '#type' => 'textarea',
    '#title' => t('Primary sender'),
    '#default_value' => theme_get_setting('sender'),
    '#rows' => 2,
    '#resizable' => FALSE,
    '#description' => t("Official sender used in the logo as the main sender, e.g. 'National Archives'."),
    '#states' => array(
      'visible' => array(
        ':input[name="branding_type"]' => array('value' => 'rijkshuisstijl'),
      ),
    ),
  );

  $form['wordmark']['secondary_sender'] = array(
    '#type' => 'textarea',
    '#title' => t('Secondary sender'),
    '#default_value' => theme_get_setting('secondary_sender'),
    '#rows' => 2,
    '#resizable' => FALSE,
    '#description' => t("Official mother organisation, usually the ministry, e.g. 'Ministry of Education'Printed in the logo under the main sender."),
    '#states' => array(
      'visible' => array(
        ':input[name="branding_type"]' => array('value' => 'rijkshuisstijl'),
      ),
    ),
  );
  
  $form['pay_off'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pay-off'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => -18,
    '#states' => array(
      'visible' => array(
        ':input[name="branding_type"]' => array(array('value' => 'neutral'), array('value' => 'rijkshuisstijl')),
      ),
    ),
  );

  $form['pay_off']['pay_off_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Main bar pay-off'),
    '#default_value' => theme_get_setting('pay_off_header'),
    '#rows' => 1,
    '#resizable' => FALSE,
    '#description' => t("Text above the main navigation bar."),
  );
  $form['pay_off']['pay_off_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer pay-off'),
    '#default_value' => theme_get_setting('pay-off_footer'),
    '#rows' => 2,
    '#resizable' => FALSE,
    '#description' => t("Text beneath the content next to the visual reference ."),
    '#states' => array(
      'visible' => array(
        ':input[name="branding_type"]' => array('value' => 'rijkshuisstijl'),
      )),
  );

}
