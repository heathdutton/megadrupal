(function ($) {

/**
 * Custom summary for the module vertical tab.
 */
Drupal.behaviors.exportableSchedulerVerticalTabs = {
  attach: function (context) {

    $('fieldset#edit-exportable-scheduler-settings', context).drupalSetSummary(function (context) {
      var summary = '';
      $('.field-exportable-scheduler-date', context).each(function (context) {
        date = Drupal.checkPlain($(this).val());
        if (date != '') {
          type = Drupal.checkPlain($(this).closest('tr').find('.field-exportable-scheduler-type').val());
          summary += date + ' (' + type + ')<br/>';
        }
      });
      return summary;
    });

  }
};

})(jQuery);
