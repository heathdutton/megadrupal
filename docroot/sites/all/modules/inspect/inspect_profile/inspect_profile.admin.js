
(function ($) {
  'use strict';

  Drupal.behaviors.inspectProfileAdmin = {
    attach: function (context) {
      $('input#inspect_profile_statsnow', context).click(function() {
        var sFrom = $.trim($('input#edit-inspect-profile-statsfrom').val()), sTo = $.trim($('input#edit-inspect-profile-statsto').val()),
          from = 1, to = 0;

        if (sFrom) {
          if (!/^\d{4}\-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$/.test(sFrom)) {
            alert(Drupal.t('From time is not a valid date (YYYY-MM-DD HH:II:SS).'));
            return;
          }
          from = new Date();
          from.setFullYear(parseInt(sFrom.substr(0, 4)));
          from.setMonth(parseInt(sFrom.substr(5, 2)) - 1);
          from.setDate(parseInt(sFrom.substr(8, 2)));
          from.setHours(parseInt(sFrom.substr(11, 2)));
          from.setMinutes(parseInt(sFrom.substr(14, 2)));
          from.setSeconds(parseInt(sFrom.substr(17, 2)));
          from = Math.floor(from / 1000);
        }

        if (sTo) {
          if (!/^\d{4}\-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$/.test(sTo)) {
            alert(Drupal.t('To time is not a valid date (YYYY-MM-DD HH:II:SS).'));
            return;
          }
          to = new Date();
          to.setFullYear(parseInt(sTo.substr(0, 4)));
          to.setMonth(parseInt(sTo.substr(5, 2)) - 1);
          to.setDate(parseInt(sTo.substr(8, 2)));
          to.setHours(parseInt(sTo.substr(11, 2)));
          to.setMinutes(parseInt(sTo.substr(14, 2)));
          to.setSeconds(parseInt(sTo.substr(17, 2)));
          to = Math.floor(to / 1000);
        }

        $.get('/admin/reports/inspect_profile/statistics/make/' + from + '/' + to);
      });
    }
  };
})(jQuery);
