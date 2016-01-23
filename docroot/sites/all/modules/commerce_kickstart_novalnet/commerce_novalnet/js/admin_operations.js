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
 * Script : admin_operations.js
 *
 */
function is_number_key(event) {
    var keycode = ('which' in event) ? event.which : event.keyCode;
    var reg = /^(?:[0-9]+$)/;
    return (reg.test(String.fromCharCode(keycode)) || keycode == 0 || keycode == 8 || (event.ctrlKey == true && keycode == 114))? true : false;
}
function confirm_message() {
            if (jQuery('#nn_process').val() == 'amount_update_management') {
                return confirm(jQuery('#amount_update_message').val());
            }
}
