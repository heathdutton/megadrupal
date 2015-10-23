<?php
/**
 * @file
 * Novalnet payment method module
 * This module is used for real time processing of
 * Novalnet transaction of customers.
 *
 * Copyright (c) Novalnet AG
 *
 * Released under the GNU General Public License
 * This free contribution made by request.
 * If you have found this script useful a small
 * recommendation as well as a comment on merchant form
 * would be greatly appreciated.
 *
 * Script : commerce_novalnet_constants.inc
 *
 */

//status
define('NOVALNET_SUCCESS_STATUS', 100);
define('NOVALNET_PAYPAL_PENDING_STATUS', 90);
define('NOVALNET_DEACTIVATED_STATUS', 103);

//paths
define('NOVALNET_MODULE_BASEPATH', base_path() . drupal_get_path('module', 'commerce_novalnet'));
define('NOVALNET_IMAGES_PATH', NOVALNET_MODULE_BASEPATH . '/images/');
define('NOVALNET_CSS_PATH', NOVALNET_MODULE_BASEPATH . '/css/');
define('NOVALNET_JS_PATH', NOVALNET_MODULE_BASEPATH . '/js/');

define('NOVALNET_COMMERCE_MODULE_VERSION', '10.0.0');
define('NOVALNET_ACCOUNT_HOLDER', 'Novalnet AG');

//fraud modules
define('NOVALNET_FRAUD_MODULES_EMAIL', 'EMAIL');
define('NOVALNET_FRAUD_MODULES_CALLBACK', 'CALLBACK');
define('NOVALNET_FRAUD_MODULES_SMS', 'SMS');
define('NOVALNET_FRAUD_MODULES_ALLOWED_COUNTRIES', serialize(array('DE', 'AT', 'CH')));
define('NOVALNET_FRAUD_MODULES_ALLOWED_PAYMENTS', serialize(array('commerce_novalnet_cc', 'commerce_novalnet_sepa', 'commerce_novalnet_invoice')));

//callbackscript
define('NOVALNET_CALLBACK_ALLOWED_IP', serialize(array('195.143.189.210', '195.143.189.214')));
define('NOVALNET_CALLBACK_PROCESS_STATUS', 100);

