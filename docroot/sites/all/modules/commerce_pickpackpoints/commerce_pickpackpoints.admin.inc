<?php

/**
 * @file
 * Administration form to pickpackpoints modul.
 */

/**
 * Form builder function for module settings.
 */
function commerce_pickpackpoints_settings() {
  $form = array();

  $form['pickpackpoints_list_url'] = array(
    '#type' => 'textfield',
    '#title' => t('The URL of Pick-Pack point XML file'),
    '#default_value' => variable_get('pickpackpoints_list_url', 'http://partner.pickpackpont.hu/stores/validboltlista.xml'),
    '#description' => t("Place of XML file from generating the list of Pick-Pack Point.<br />Default is the original place from website Pick-Pack Point: http://partner.pickpackpont.hu/stores/validboltlista.xml"),
  );

  $form['button']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild Pick-pack point list'),
    '#submit' => array('commerce_pickpackpoints_admin_list_reset_submit'),
  );

  return system_settings_form($form);
}

/**
 * Rebuild Pick-pack point list manualy.
 */
function commerce_pickpackpoints_admin_list_reset_submit($form, &$form_state) {
  commerce_pickpackpoints_cron();
}
