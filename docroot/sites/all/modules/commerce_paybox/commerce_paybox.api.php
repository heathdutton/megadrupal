<?php

/**
 * @file
 * Documents hooks provided by the Commerce Paybox module.
 */

/**
 * Allow other modules to alter Paybox CGI module parameters.
 *
 * For a full list of allowed parameters, see ocumentation at
 * http://www1.paybox.com/telechargement_focus.aspx?cat=3
 *
 * @param array $params
 *   The Paybox CGI module parameters to be altered.
 *
 * @param object $order
 *   A fully loaded order object.
 *
 * @param array $settings
 *   The commerce payment method (paybox_offsite) settings.
 */
function hook_commerce_paybox_offsite_params_alter(&$params, $order, $settings) {
  // The paybox site will allow only credit cards payment methods.
  $params['PBX_TYPEPAIEMENT'] = 'CARTE';
  $params['PBX_TYPECARTE'] = 'CB';
}
