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
 * Script : disable_check_box.js
 *
 */
jQuery(document).ready(function(){
    jQuery('#novalnet_new_pin').attr('checked',false);
    if (jQuery('#novalnet_trxn_pin').val() != undefined)
        jQuery('#novalnet_trxn_pin').attr('value','');
});
