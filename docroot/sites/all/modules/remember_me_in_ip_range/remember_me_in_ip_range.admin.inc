<?php

/**
 * @file
 * Menu callback include file for admin/settings pages.
 */

/**
 * Implementation of menu callback.
 */
function remember_me_in_ip_range_admin_settings($form, &$form_state) {

  $help = '<p> ' . t('Accepted IP Range format are:') . '</p>';
  $help .= '<ul><li>';
  $help .= t("Single IP matches like <code>123.123.123.123</code>");
  $help .= '</li><li>';
  $help .= t("Wildcards using an asterisk (<code>*</code>) in any quadrant except the first one, for example <code>123.123.123.*</code> or <code>100.*.*.*</code> etc.");
  $help .= '</li><li>';
  $help .= t("Ranges using a hyphen (<code>-</code>) in any quadrant except the first one, for example <code>123.123.123.100-200</code> etc.");
  $help .= '</li><li>';
  $help .= t("Any number of comma-separated IP addresses or ranges like <code>10.11.12.13, 123.123.123.100-200, 123.123.124-125.*</code> etc.");
  $help .= '</li></ul>';

  $vars = array(
    'remember' => array(
      '#type' => 'item',
      '#title' => t('Remember me in IP Range Enabled:'),
      '#value' => variable_get('remember_me_in_ip_range', FALSE) ? t('Yes') : t('No'),
      '#description' => t('IP range checking enabled or disabled.'),
    ),
    'current_ip' => array(
      '#type' => 'item',
      '#title' => t('Current Client IP:'),
      '#value' => remember_me_in_ip_range_get_client_ip(),
      '#description' => t('Your IP address found by network'),
    ),
  );

  $form['legend'] = array(
    '#type' => 'markup',
    '#markup' => theme('remember_me_in_ip_range_settings_display', array('vars' => $vars)),
  );

  $form['remember_me_in_ip_range'] = array(
    '#type' => 'checkbox',
    '#title' => t('IP Range'),
    '#default_value' => variable_get('remember_me_in_ip_range', FALSE),
    '#description' => t("Enables checking remembering users in IP Range"),
  );
  $form['remember_me_in_ip_range_ip_range'] = array(
    '#type' => 'textarea',
    '#title' => t('IP Range'),
    '#default_value' => variable_get('remember_me_in_ip_range_ip_range', ""),
    '#description' => remember_me_in_ip_range_help_ranges(),
  );
  return system_settings_form($form);
}

/**
 * Theme callback function to dispaly status information.
 */
function theme_remember_me_in_ip_range_settings_display($variables) {
  $rows = array(array());

  foreach ($variables['vars'] as $var) {
    $element = array(
      'element' => array(
        '#type' => 'markup',
        '#children' => $var['#value'],
        '#title' => $var['#title'],
        '#title_display' => 'before',
      ),
    );

    $output = '<div class="container-inline">' . theme('form_element', $element) . '</div>';
    $output .= '<div class="description">' . $var['#description'] . '</div>';

    $rows[0][] = array('data' => $output);
  }
  return theme('table', array('rows' => $rows));
}
