/**
 * @file
 * JavaScript specifically for CiviCRM report form
 */


Drupal.behaviors.civicrm_private_report = {
  attach: function(context, settings) {
    console.debug('my foo')
    if(settings.civicrm_private_report.has_access) {
      // Insert the "Private Report" accordion block
      cj('.crm-accordion-wrapper.crm-report_criteria-accordion').parent().after(settings.civicrm_private_report.html)

      // Move form elements from beginHookFormElements into the "Private Report"
      // accordion.
      cj('#table_save_as').append(cj('#save_as_name').closest('tr'))
      el = (cj('#do_save_as').closest('tr'));
      cj('#save_as_name').after(cj('#do_save_as'));
      el.remove();
      cj('#do_save_as').before('<BR />');
      cj('#table_save_as').append(cj('#do_promote').closest('tr'))

      // Call CiviCRM's accordion plugin to activate the "Private Report" accordion.
      cj().crmaccordions();

      // If required, hide some of the "Report Settings" form elements.
      if (settings.civicrm_private_report.is_private) {
        cj('tr.crm-report-instanceForm-form-block-email_subject').closest('table').prev('h3').remove();
        cj('tr.crm-report-instanceForm-form-block-email_subject').closest('table').remove();
        cj('tr.crm-report-instanceForm-form-block-is_navigation').closest('table').prev('h3').remove();
        cj('tr.crm-report-instanceForm-form-block-is_navigation').closest('table').remove();
      }
    }
  }
}