(function ($) {

  workDaysCalendar = {
    /**
     * Returns all days in month.
     */
    getDaysInMonth : function(month, year) {
      var date = new Date(year, month, 1);
      var days = [];
      while (date.getMonth() === month) {
        days.push(new Date(date));
        date.setDate(date.getDate() + 1);
      }
      return days;
    },

    /**
     * Highlight work days.
     */
    hightlightWorkDays : function(year, month, datepicker) {
      var days = workDaysCalendar.getDaysInMonth(month - 1, year);

      var exclusions = workDaysCalendar.getAdEx(datepicker, 'ex');
      var additions = workDaysCalendar.getAdEx(datepicker, 'ad');

      var work_days = [];
      $.each(days, function(index, day) {
        // Format as mm/dd/YYYY.
        var fday = ("0" + (day.getDate())).slice(-2);
        var fmonth = ("0" + (day.getMonth() + 1)).slice(-2);
        var formatted_date = fmonth + '/' + fday + '/' + day.getFullYear();

        var isWorkDay = $.datepicker.noWeekends(day);
        // If it's work day and not excluded OR it's addition weekend day.
        if ((isWorkDay[0] && $.inArray(formatted_date, exclusions) == -1) || $.inArray(formatted_date, additions) != -1) {
          work_days[work_days.length] = day;
        }
      });

      // Bind datepicker.
      if (work_days.length) {
        $('#' + datepicker.id).multiDatesPicker('addDates', work_days);
      }
      else {
        // Prevent wrong highlighting.
        $('#' + datepicker.id).multiDatesPicker('resetDates', 'disabled');
      }

      workDaysCalendar.disableDayClick(datepicker);
    },

    /**
     * Event handler, which called when user click on one of days.
     */
    selectDate : function(date, datepicker) {
      var dateObj = new Date(date);
      var isWorkDay = $.datepicker.noWeekends(dateObj);

      var activeDates = $('#' + datepicker.id).multiDatesPicker('getDates');
      var isActive = $.inArray(date, activeDates) == -1 ? false : true;

      // If it's work day then it's exclusion, otherwise it's addition.
      var mode = isWorkDay[0] ? 'ex' : 'ad';
      workDaysCalendar.toggleAdEx(datepicker, mode, isActive, date);
    },

    /**
     * Disable click on day for each
     */
    disableDayClick : function(datepicker) {
      if (Drupal.settings.workDays && Drupal.settings.workDays.disableSelection) {
        setTimeout(function() {
          $('#' + datepicker.id + ' td').each(function() {
            $(this).unbind('click');
            $(this).bind('click', function(e) {e.preventDefault();});
          });
        }, 100)
      }
    },

    /**
     * Add or remove exclusion.
     */
    toggleAdEx : function(datepicker, mode, isActive, date) {
      var data = workDaysCalendar.getAdEx(datepicker, mode) || [];
      if (mode == 'ex') {
        // Remove exclusion.
        if (isActive) {
          var index = $.inArray(date, data);
          data.splice(index, 1);
        }
        // Add exclusion.
        else {
          data[data.length] = date;
        }
      }
      else {
        // Add addition.
        if (isActive) {
          data[data.length] = date;
        }
        // Remove addition.
        else {
          var index = $.inArray(date, data);
          data.splice(index, 1);
        }
      }
      workDaysCalendar.saveAdEx(datepicker, mode, data);
    },

    /**
     * Returns all additions/exclusions.
     */
    getAdEx : function(datepicker, mode) {
      var textElement = workDaysCalendar.getElement(datepicker, mode);
      if (textElement) {
        return JSON.parse($(textElement).val());
      }
    },

    /**
     * Write additions/exclusions into textfield.
     */
    saveAdEx : function(datepicker, mode, data) {
      var textElement = workDaysCalendar.getElement(datepicker, mode);
      if (textElement) {
        $(textElement).val(JSON.stringify(data));
      }
    },

    /**
     * Returns element.
     */
    getElement : function(datepicker, type) {
      if (type == 'ex') {
        return $('#' + datepicker.id).parents('.field-type-work-days').find('.work-days-calendar-exclusions');
      }
      else {
        return $('#' + datepicker.id).parents('.field-type-work-days').find('.work-days-calendar-additions');
      }
    },

    /**
     * Init calendar.
     */
    init : function(item) {
      var date = new Date();
      workDaysCalendar.hightlightWorkDays(date.getFullYear(), date.getMonth() + 1, {id : $(item).attr('id')});
    }
  };


  Drupal.behaviors.WorkDaysCalendar = {
    attach: function (context) {
      $(context).find('.work-days-calendar').each(function(index, item) {
        $(item).once(function() {
          $(item).multiDatesPicker({
            onChangeMonthYear: workDaysCalendar.hightlightWorkDays,
            onSelect: workDaysCalendar.selectDate
          });
          workDaysCalendar.init(item);
        });
      });
    }
  }
})(jQuery);
