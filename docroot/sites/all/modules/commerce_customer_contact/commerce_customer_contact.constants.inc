<?php
/**
 * @file
 *   Constants for the "Commerce Customer Contact" module.
 *
 *   © 2015 RedBottle Design, LLC. All rights reserved.</p>
 *
 * @author  Guy Paddock (guy.paddock@redbottledesign.com)
 */

/**
 * Constant for the permission that allows a user to contact customers about
 * orders.
 *
 * @var string
 */
define('COCUCO_PERM_CONTACT_CUSTOMERS', 'contact customers');

/**
 * Constant for the permission that allows a user to control contact settings,
 * including where the contact form appears.
 *
 * @var string
 */
define('COCUCO_PERM_ADMIN_CONTACT_SETTINGS', 'administer customer contact settings');

/**
 * Constant for the name of the setting that controls where the contact form
 * appears.
 *
 * @var string
 */
define('COCUCO_VARIABLE_APPEARANCE_OPTIONS', 'commerce_customer_contact_appearances');

/**
 * Constant for the option to show the contact form on the "View" tab of orders.
 *
 * @param int
 */
define('COCUCO_SHOW_ON_VIEW_TAB', 1);

/**
 * Constant for the option to show the contact form on the "Payments" tab of
 * orders.
 *
 * @param int
 */
define('COCUCO_SHOW_ON_PAYMENTS_TAB', 2);

/**
 * Constant for the option to show the contact form on its own standalone tab
 * on orders.
 *
 * @param int
 */
define('COCUCO_SHOW_ON_OWN_TAB', 3);