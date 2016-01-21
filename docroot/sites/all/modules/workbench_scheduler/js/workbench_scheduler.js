/**
 * @file
 *
 * The javascript functionality for workbench Scheduler.
 * Sets the summary for Workbench Scheduler on vertical tabs.
 */

(function ($) {

    // Hiding start and end date fields.
    Drupal.behaviors.workbenchSchedulerOptions = {
        attach: function (context) {

            var type_input = $('input[name="workbench_scheduler_sid"]:checked');
            var start_date = $('input[name="workbench_scheduler_start_date[date]"]');
            var start_time = $('input[name="workbench_scheduler_start_date[time]"]');
            var end_date = $('input[name="workbench_scheduler_end_date[date]"]');
            var end_time = $('input[name="workbench_scheduler_end_date[time]"]');
            var schedules = Drupal.settings.workbench_scheduler.schedules;
            var type = false;

            if (type_input.val() in schedules) {
                type = schedules[type_input.val()];
            }

            // Hiding start date field, if exists.
            if (type && (type.start_state == '')) {
                start_date.val('');
                start_time.val('');
                $(".form-item-workbench-scheduler-start-date").hide();
            }
            else {
                $(".form-item-workbench-scheduler-start-date").show();
            }

            // Hiding end date field.
            if (type && (type.end_state == '')) {
                end_date.val('');
                end_time.val('');
                $(".form-item-workbench-scheduler-end-date").hide();
            }
            else {
                $(".form-item-workbench-scheduler-end-date").show();
            }

            // This works but if there a more elegant way to do it. . .
            $("input[name=workbench_scheduler_sid]").change(function () {
                Drupal.behaviors.workbenchSchedulerOptions.attach(context);
            });
        }
    };

    // Vertical tabs.
    Drupal.behaviors.workbenchSchedulerSettingsSummary = {
        attach: function (context) {

            $('#edit-workbench-scheduler', context).drupalSetSummary(function (context) {
                var vals = [];

                // Schedule type.
                var type_input = $('input[name="workbench_scheduler_sid"]:checked');
                var type = Drupal.settings.workbench_scheduler.schedules[type_input.val()];

                vals.push(type.label);

                // If schedule exists.
                if (type_input.val() > 0) {

                    // Start Date and Time.
                    var start_date = $('input[name="workbench_scheduler_start_date[date]"]');
                    var start_time = $('input[name="workbench_scheduler_start_date[time]"]');
                    var start_text = $('label[for="edit-workbench-scheduler-start-date"]');
                    if (start_date.val() || start_time.val()) {
                        vals.push(Drupal.t('@start_text: @start_date @start_time', {'@start_text': start_text.text(), '@start_date':start_date.val(), '@start_time':start_time.val()}));
                    }

                    // End Date and Time.
                    var end_date = $('input[name="workbench_scheduler_end_date[date]"]');
                    var end_time = $('input[name="workbench_scheduler_end_date[time]"]');
                    var end_text = $('label[for="edit-workbench-scheduler-end-date"]');
                    if (end_date.val() || end_time.val()) {
                        vals.push(Drupal.t('@end_text: @end_date @end_time', {'@end_text': end_text.text(), '@end_date':end_date.val(), '@end_time':end_time.val()}));
                    }
                }

                return vals.join("<br />");

            });
        }
    };

})(jQuery);
