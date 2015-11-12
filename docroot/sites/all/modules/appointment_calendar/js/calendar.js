/**
 * @file
 * Appointment Calendar Views javascript functions.
 */

jQuery(document).ready(function ($, Drupal)
{
    Drupal.behaviors.calendar = {
        attach: function ()
        {
            var date = new Date();
            var weekdays = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
            var month = new Array(12);
            month[0] = "January";
            month[1] = "February";
            month[2] = "March";
            month[3] = "April";
            month[4] = "May";
            month[5] = "June";
            month[6] = "July";
            month[7] = "August";
            month[8] = "September";
            month[9] = "October";
            month[10] = "November";
            month[11] = "December";
            var weekday = weekdays[date.getDay()];
            var dateValue = date.getDay();
            var monthvalue = month[date.getMonth()];
            var fullDate = new Date();
            var twoDigitDate = fullDate.getDate() + "";
            if (twoDigitDate.length == 1) {
                twoDigitDate;
            }
            var fullDate = new Date();
            var currentYear = fullDate.getFullYear();
            // Getting complete date as views shown date.
            var j = 0;
            var completedate = weekday + ', ' + monthvalue + ' ' + twoDigitDate + ', ' + currentYear;
            jQuery('.full > tbody  > tr .inner').each(function () {
                var prev_date = jQuery.trim(jQuery(this).text());
                jQuery("tr:first").hide();
                jQuery("tr:eq(1)").hide();
                // Making Available for all timeslots.
                if ((prev_date == '') || (prev_date.indexOf(completedate) >= 0)) {
                    jQuery(this).html('<div class="item">\n\
                        <div class="view-item view-item-calendar">\n\
                        <div class="calendar dayview">\n\
                        <div class="calendar.126.field_appointment_date.0.0 contents">\n\
                        <span class="views-field views-field-appointment-date">\n\
                        <span class="field-content">\n\
                        <span style="color:green">Available</span>\n\
                        </span>\n\
                        </span>\n\
                        </div>\n\
                        </div>\n\
                        </div>\n\
                        </div>');
                }
                j++;
            });
            // Empty TD labels.
            jQuery('.views-label-appointment-date').text('');
            // Disabling PREV if Calendar date is equal to today's Date.
            var prev_date = jQuery('.date-heading h3').html();

            if (prev_date == completedate) {
                jQuery('ul.pager li.date-prev').hide();
            }
            jQuery('.calendar-agenda-hour').css({
                width: '35%',
            })
            jQuery('.view-header > p').hide();
            var get;
            var a = 0;
            var time_slots;
            jQuery('.full > tbody:eq(1)  > tr').each(function () {
                time_slots = jQuery('.view-header > p').text();
                get = time_slots.split(',');
                a++;
            });
            get.sort();
            jQuery("tr:eq(" + (get.length + 2) + ")").hide();
            for (var i = 0; i < (a) - 1; i++) {
                var C = i + 1;
                jQuery(".full > tbody:eq(1)  > tr:eq(" + C + ") td .calendar-hour").text(get[i]);
            }
            // Adding CSS.
            jQuery("body.page-customcal > ul.pager").css({
                clear: "both",
            });
            jQuery("body.page-customcal > li.date-prev, li.date-next").css({
                background: "none repeat scroll 0 0 rgba(0, 0, 0, 0) !important",
                clear: "both !important",
                position: "relative !important",
                right: "0px !important"
            });
        }
    };
}(jQuery, Drupal));
