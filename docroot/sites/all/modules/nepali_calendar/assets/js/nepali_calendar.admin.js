/**
 * @file
 * nepali_calendar.admin.js
 */
(function ($) {

  Drupal.behaviors.nepalicalendarSetSummaries = {
    attach: function (context, settings) {
      var $context = $(context);

      // Nepali date
      $context.find('#edit-general-nepali-date').drupalSetSummary(function () {
        var summary = [];
        // Date format
        var dateFormat = $context.find('select[name="nepali_calendar_nepali_date_format"] :selected').text();
        summary.push('<strong>' + Drupal.t('Date format') + '</strong><br>' + dateFormat);

        // Show label
        if ($context.find('input[name="nepali_calendar_show_date_label"]').is(':checked')) {
          summary.push(Drupal.t('Date label enabled'));
        }
        else {
          summary.push(Drupal.t('Date label disabled'));
        }
        return summary.join('<br><br>');
      });

      // Nepal time
      $context.find('#edit-general-nepal-time').drupalSetSummary(function () {
        var summary = [];
        // Show time
        if ($context.find('input[name="nepali_calendar_show_nepal_time"]').is(':checked')) {
          summary.push(Drupal.t('Time enabled'));
          // Time format
          var timeFormat = $context.find('select[name="nepali_calendar_nepal_time_format"] :selected').text();
          summary.push('<strong>' + Drupal.t('Time format') + '</strong><br>' + timeFormat);
        }
        else {
          summary.push(Drupal.t('Time disabled'));
        }
        return summary.join('<br><br>');
      });

    }
  };
})(jQuery);
