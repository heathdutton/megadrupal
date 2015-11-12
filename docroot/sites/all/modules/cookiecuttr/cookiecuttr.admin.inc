<?php
/**
 * @file
 * Admin settings form for CookieCuttr.
 */

/**
 * LiveChat config form.
 */
function cookiecuttr_config() {
  $form['cookiecuttr_hide_parts'] = array(
    '#type' => 'textfield',
    '#title' => 'Hide elements',
    '#description' => t("if you'd like to actively hide parts of your website set this to true, for example say you use a comments system that inserts cookies, you can put the div name in below to replace it with a cookie warning message."),
    '#default_value' => variable_get('cookiecuttr_hide_parts'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_analytics'] = array(
    '#type' => 'checkbox',
    '#title' => t('Analytics'),
    '#description' => t('if you are just using a simple analytics package you can set this to true, it displays a simple default message with no privacy policy link - this is set to true by default.'),
    '#default_value' => variable_get('cookiecuttr_analytics'),
  );

  $form['cookiecuttr_decline_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cookie Decline Button'),
    '#description' => t('if you’d like a decline button to (ironically) write a cookie into the browser then set this to true.'),
    '#default_value' => variable_get('cookiecuttr_decline_button'),
  );

  $form['cookiecuttr_accept_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide Accept Button'),
    '#description' => t('set this to true to hide the accept button, its set to false by default'),
    '#default_value' => variable_get('cookiecuttr_accept_button'),
  );

  $form['cookiecuttr_reset_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset Button'),
    '#description' => t('if you’d like a reset button to delete the accept or decline cookies then set this to true.'),
    '#default_value' => variable_get('cookiecuttr_reset_button'),
  );

  $form['cookiecuttr_overlay_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overlay enabled'),
    '#description' => t("don't want a discreet toolbar? this makes the whole message into a 100% height"),
    '#default_value' => variable_get('cookiecuttr_overlay_enabled'),
  );

  $form['cookiecuttr_policy_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Policy Link'),
    '#description' => t('if applicable, enter the link to your privacy policy in here - this is as soon as cookieAnalytics is set to false;'),
    '#default_value' => variable_get('cookiecuttr_policy_link'),
  );

  $form['cookiecuttr_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Bar Message'),
    '#description' => t('edit the message you want to appear in the cookie bar, remember to keep the {{cookiePolicyLink}} variable in tact so it inserts your privacy policy link.'),
    '#default_value' => variable_get('cookiecuttr_message'),
  );

  $form['cookiecuttr_analytics_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Analytics Message'),
    '#description' => t('edit the message you want to appear, this is the default message.'),
    '#default_value' => variable_get('cookiecuttr_analytics_message'),
  );

  $form['cookiecuttr_what_are_they_link'] = array(
    '#type' => 'textfield',
    '#title' => t('What are Cookies Link'),
    '#description' => t("edit the link for the 'What are Cookies' link."),
    '#default_value' => variable_get('cookiecuttr_what_are_they_link'),
  );

  $form['cookiecuttr_error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Error Message'),
    '#description' => t('edit the message you’d like to appear in place of the functionality'),
    '#default_value' => variable_get('cookiecuttr_error_message'),
  );

  $form['cookiecuttr_notification_location_bottom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show cookie notification at bottom'),
    '#description' => t('this is false by default, change it to true and the cookie notification bar will show at the bottom of the page instead, please note this will remain at the top for mobile and iOS devices and Internet Explorer 6.'),
    '#default_value' => variable_get('cookiecuttr_notification_location_bottom'),
  );

  $form['cookiecuttr_disable'] = array(
    '#type' => 'textfield',
    '#title' => t('Elements to disable'),
    '#description' => t('list elements comma separated in here that you want to disable, this will only work if cookieCutter is set to true.'),
    '#default_value' => variable_get('cookiecuttr_disable'),
  );

  $form['cookiecuttr_accept_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Accept Button Text'),
    '#description' => t('you can change the text of the green accept button.'),
    '#default_value' => variable_get('cookiecuttr_accept_button_text'),
  );

  $form['cookiecuttr_decline_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Decline Button Text'),
    '#description' => t('you can change the text of the red decline button.'),
    '#default_value' => variable_get('cookiecuttr_decline_button_text'),
  );

  $form['cookiecuttr_reset_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Reset Button Text'),
    '#description' => t('you can change the text of the orange reset button.'),
    '#default_value' => variable_get('cookiecuttr_reset_button_text'),
  );

  $form['cookiecuttr_what_are_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('What Are Cookies Link Text'),
    '#description' => t('you can change the text of the "What are Cookies" link shown on Google Analytics message.'),
    '#default_value' => variable_get('cookiecuttr_what_are_link_text'),
  );

  $form['cookiecuttr_policy_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cookie Policy Page'),
    '#description' => t('set this to true to display the message you want to appear on your privacy or cookie policy page.'),
    '#default_value' => variable_get('cookiecuttr_policy_page'),
  );

  $form['cookiecuttr_policy_page_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Policy Page Message'),
    '#description' => t('edit the message you want to appear on your policy page.'),
    '#default_value' => variable_get('cookiecuttr_policy_page_message'),
  );

  $form['cookiecuttr_discreet_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Discreet Link'),
    '#description' => t('false by default, set to true to enable.'),
    '#default_value' => variable_get('cookiecuttr_discreet_link'),
  );

  $form['cookiecuttr_discreet_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Discreet Link Text'),
    '#description' => t('edit the text you want to appear on the discreet option.'),
    '#default_value' => variable_get('cookiecuttr_discreet_link_text'),
  );

  $form['cookiecuttr_discreet_position'] = array(
    '#type' => 'textfield',
    '#title' => t('Discreet Link Position'),
    '#description' => t('set to topleft by default, you can also set topright, bottomleft, bottomright.'),
    '#default_value' => variable_get('cookiecuttr_discreet_position'),
  );

  $form['cookiecuttr_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Domain'),
    '#description' => t('empty by default, add your domain name in here without www. or https:// or http:// to remove Google Analytics cookies on decline.'),
    '#default_value' => variable_get('cookiecuttr_domain'),
  );

  return system_settings_form($form);
}
