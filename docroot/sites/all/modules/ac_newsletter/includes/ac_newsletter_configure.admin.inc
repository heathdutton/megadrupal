<?php
/**
 * @file
 * Page callback: ActiveCampaign configuration settings.
 *
 * @see ac_newsletter_menu()
 */

/**
 * Form constructor for the ac_newsletter admin configure form.
 *
 * @see ac_newsletter_configure_admin_form_validate()
 *
 * @ingroup forms
 */
function ac_newsletter_configure_admin_form($form, &$form_state) {

  // Load icons.
  $icons = _ac_newsletter_icons();
  $logo = $icons['ac-logo-icon'];
  $conf = $icons['configure-ac-icon'];

  // Add CSS file.
  $form['#attached']['css'] = array(drupal_get_path('module', 'ac_newsletter') . '/css/ac_newsletter.css');

  // Add fieldset.
  $form['active_campaign'] = array(
    '#type' => 'fieldset',
    '#title' => t('ActiveCampaign API configuration settings'),
  );

  // Display ActiveCampaign logo and Settings wheel icon.
  $ac_newsletter_url = check_url(variable_get('ac_newsletter_api_url', AC_NEWSLETTER_SITE_URL));
  $form['active_campaign']['ac_newsletter_logo'] = array(
    '#markup' => t('<div class="ac-newsletter-logo">!logo_url</div>', array(
      '!logo_url' => l($logo, AC_NEWSLETTER_SITE_URL, array('attributes' => array('target' => '_blank'), 'html' => TRUE)))),
  );
  $form['active_campaign']['ac_newsletter_conf'] = array(
    '#markup' => t('<div class="ac-newsletter-comm">!conf_url</div>', array(
      '!conf_url' => l($conf, $ac_newsletter_url, array('attributes' => array('target' => '_blank'), 'html' => TRUE)))),
  );

  // Get options(ActiveCampaign Lists).
  $options = _ac_newsletter_get_lists();

  // Field to select/enable lists on the site.
  $form['active_campaign']['ac_newsletter_site_lists'] = array(
    '#type' => 'select',
    '#title' => t('Enable Lists on site'),
    '#options' => $options,
    '#default_value' => variable_get('ac_newsletter_site_lists', array()),
    '#multiple' => TRUE,
    '#required' => TRUE,
    '#description' => t('Provides lists to be enabled on the site.'),
  );

  // Optionals, Field to use Briteverify API on subscription form.
  $form['active_campaign']['ac_newsletter_use_briteverify_api'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Briteverify API'),
    '#default_value' => variable_get('ac_newsletter_use_briteverify_api', FALSE),
    '#description' => t('Provides email verification using Briteverify API.'),
  );

  // Field to provide API key of the Briteverify.
  $form['active_campaign']['ac_newsletter_briteverify_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Briteverify API Key'),
    '#attributes' => array('placeholder' => t('Enter Briteverify API key')),
    '#size' => 50,
    '#default_value' => variable_get('ac_newsletter_briteverify_api_key', ''),
    '#description' => t('Provide an valid API key of Briteverify.'),
    '#states' => array(
      'invisible' => array(
        'input:[name="ac_newsletter_use_briteverify_api"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Optionals, Field to enable Double Opt-in on subscription form.
  $form['active_campaign']['ac_newsletter_enable_double_opt_in'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Double Opt-in'),
    '#default_value' => variable_get('ac_newsletter_enable_double_opt_in', FALSE),
    '#description' => t('Opt-in emails are now directly related to subscription forms only. You may only add “unconfirmed” subscribers (subscribers that would be sent an opt-in) from a subscription form.'),
  );

  // Field to provide the subscription form id.
  $form['active_campaign']['ac_newsletter_subscription_form_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription Form ID'),
    '#attributes' => array('placeholder' => t('Enter Form ID')),
    '#size' => 20,
    '#default_value' => variable_get('ac_newsletter_subscription_form_id', ''),
    '#description' => t('Provide an subscription form <em>form_id</em> where Double Opt-in is enabled'),
    '#states' => array(
      'invisible' => array(
        'input:[name="ac_newsletter_enable_double_opt_in"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Optionals, Field to provide alter text for the lists.
  $form['active_campaign']['ac_newsletter_alternative_text'] = array(
    '#title' => t('Alternative Text for the lists'),
    '#type' => 'textarea',
    '#attributes' => array('placeholder' => t('Enter Alternative text or leave blank')),
    '#default_value' => variable_get('ac_newsletter_alternative_text', ''),
    '#description' => t('Provide alternative text per line for the enabled lists to display on my-account page. For Example: Daily Strip|The Daily Email (sent daily)'),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for ac_newsletter_configure_admin_form().
 */
function ac_newsletter_configure_admin_form_validate($form, &$form_state) {

  // Get all values.
  $fs_values = $form_state['values'];

  // Validate the Briteverify credentials.
  if ($fs_values['ac_newsletter_use_briteverify_api']) {
    if (empty($fs_values['ac_newsletter_briteverify_api_key'])) {
      form_set_error('ac_newsletter_briteverify_api_key', t('Provide a valid BriteVerify API key'));
    }
    else {
      $response_data = _ac_use_brite_verify(trim($fs_values['ac_newsletter_briteverify_api_key']));
      if ($response_data['code'] != 200) {
        $msg = $response_data['errors']['user'];
        form_set_error('ac_newsletter_briteverify_api_key', t('bpi.briteverify.com : @msg', array('@msg' => $msg)));
      }
      else {
        drupal_set_message(t('bpi.briteverify.com : Your API key authorization successful!'));
      }
    }
  }

  // Validate the ActiveCampaign Form ID.
  if ($fs_values['ac_newsletter_enable_double_opt_in'] && empty($fs_values['ac_newsletter_subscription_form_id'])) {
    form_set_error('ac_newsletter_subscription_form_id', t('Provide a valid ActiveCampaign Form ID'));
  }
}
