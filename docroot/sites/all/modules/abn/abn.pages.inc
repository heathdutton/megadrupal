<?php
/**
 * @file
 * Page callbacks for abn module
 */

/**
 * Page callback to render global settings page for ABN.
 *
 * Currently only provides a place to enter an API key (ABR GUID).
 */
function abn_admin_settings() {
  $form = array();
  $api_key = variable_get('abn_api_key', '');

  $items['abn_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Australian Business Register GUID'),
    '#required' => TRUE,
    '#default_value' => $api_key,
    '#description' => t("The ABR provides access to their web services through a GUID that acts as an API key. <br />If you don't have one you need to <a href=\"http://www.abr.business.gov.au/RegisterAgreement.aspx\">lodge an application</a> with the ABR and agree to their Terms & Conditions."),
  );

  return system_settings_form($items);
}
