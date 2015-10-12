/**
 * @file
 * JavaScript specifically for CiviCRM private report listing page.
 */

Drupal.behaviors.civicrm_private_report_list = {
  attach: function(context, settings) {
    // Remove onclick handler from delete links. This page is built from
    // the CiviCRM template CRM/Report/Page/InstanceList.tpl, which adds an
    // onclick handler invoking an "are you sure" JavaScript confirmation.
    // Since we're using Drupal confirm_form for the confirmation, the
    // JavaScript confirmation is redundant.
    jQuery('td.crm-report-instanceList-deleteUrl a').attr('onclick', '');
  }
}

