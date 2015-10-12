/**
 * @file
 * Launch TIMETABLE.
 */

(function ($) {
  Drupal.behaviors.field_timetable = {
    attach: function (context) {
      $.each(Drupal.settings.field_timetable, function (selector) {
        $(selector).timetable({
          header: {
		  },
          resources: this.resources,
          editable: this.editable,
          selectable: this.selectable,
          minTime: this.minTime,
          maxTime: this.maxTime,
          slotMinutes: this.slotMinutes,
          selectHelper: this.selectHelper,
          events: this.events,
        });
      });
    }
  };
})(jQuery);
