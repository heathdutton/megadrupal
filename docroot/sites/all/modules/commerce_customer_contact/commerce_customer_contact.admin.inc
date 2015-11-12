<?php
/**
 * @file
 *   Admin UI code for the "Commerce Customer Contact" module.
 *
 *   © 2015 RedBottle Design, LLC. All rights reserved.</p>
 *
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 */
require_once('commerce_customer_contact.constants.inc');

/**
 * Form builder for the customer contact settings page.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 *
 * @return array
 *   The order contact settings form.
 */
function commerce_customer_contact_admin_form(array $form, array &$form_state) {
  $form[COCUCO_VARIABLE_APPEARANCE_OPTIONS] = array(
    '#title'    => t('Location(s) for the customer contact form'),
    '#type'     => 'checkboxes',
    '#options'  => array(
      COCUCO_SHOW_ON_VIEW_TAB      => t('Show on "View" tab on orders.'),
      COCUCO_SHOW_ON_PAYMENTS_TAB   => t('Show on "Payments" tab on orders.'),
      COCUCO_SHOW_ON_OWN_TAB    => t('Show on a new "Contact customer" tab on orders.'),
    ),
    '#default_value' => commerce_customer_contact_get_appearance_locations(),
  );

  return system_settings_form($form);
}