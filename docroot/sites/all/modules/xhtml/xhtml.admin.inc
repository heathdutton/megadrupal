<?php /* -*- mode: php; indent-tabs-mode: nil; tab-width: 2; -*- */

function xhtml_admin_settings($form, &$form_state) {
  return system_settings_form(array(
    'xhtml_html_pages' => array(
      '#title' => t('HTML Pages'),
      '#description' => t('Enter the paths to pages which should <em>not</em> be delivered as XHTML.'
                          . ' Any page listed here will be delivered as HTML instead, regardless of any following settings.'),
      '#type' => 'textarea',
      '#default_value' => variable_get('xhtml_html_pages', ''),
    ),
    'xhtml_test_accept' => array(
      '#title' => t('Check User-Agent\'s Accept: header'),
      '#description' => t('Disable delivering XHTML to any user-agent when its Accept: header requests text/html.'
                          . ' Disabling this will make users of IE8 and older unhappy.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('xhtml_test_accept', TRUE),
    ),
  ));
}
