
(function ($) {

// Calendar datepicker filter
Drupal.behaviors.calendarFilter = {
  attach: function (context, settings) {
    var listing = $('.view-tv-schedules', context);

    listing.once('calendar-filter', function () {
      $('.date-heading h3', listing).append('<input id="calendar-datepicker" type="hidden"></input>');
      var calendarDatepicker = $('#calendar-datepicker', listing);

      calendarDatepicker.datepicker({
        autoPopUp: 'click',
        changeMonth: true,
        changeYear: true,
        closeAtTop: false,
        dateFormat: 'yy-mm-dd',
        defaultDate: '0y',
        firstDay: 0,
        fromTo: false,
        onSelect: function (dateText, inst) {
          window.location = settings.basePath + settings.cm_tv_schedules.uri + '/' + dateText;
        },
        speed: 'immediate',
        yearRange: '-1:+1',
        showAnim: 'slideDown',
        showOptions: { direction: 'down' },
        showButtonPanel: true,
        showOn: 'both',
        buttonImage: settings.basePath + settings.cm_tv_schedules.path + '/images/icon-calendar.png',
        buttonImageOnly: true,
        gotoCurrent: true
      });

      calendarDatepicker.parent().click(function(e) {
        e.preventDefault();
        calendarDatepicker.datepicker('show');
      })
    });
  }
};

// TV Schedule sticky header
Drupal.behaviors.scheduleHeader = {
  attach: function (context, settings) {
    if (!$.support.positionFixed) {
      return;
    }

    $('.view-tv-schedules', context).once('stickyscheduleheader', function () {
      var calendarHeader = $(this).children('.view-header');
      var originalTable = $(this).find('.view-content .calendar-calendar table');

      var originalTableHeader = originalTable.children('thead');
      var originalTableCol = originalTable.children('colgroup');
      var stickyTable = $('<table></table>');

      originalTableCol.clone(true).appendTo(stickyTable);
      originalTableHeader.clone(true).appendTo(stickyTable);
      calendarHeader.append(stickyTable);
      stickyTable.hide();

      var stickyHeaderTop = $(this).offset().top;

      $(window).scroll(function() {
        if ($(document).scrollTop() > stickyHeaderTop) {
          var origWidth = calendarHeader.width();
          calendarHeader.width(origWidth).addClass('sticky');
          stickyTable.show();
        }
        else {
          calendarHeader.removeClass('sticky');
          stickyTable.hide();
        }
      });
    });
  }
};

}(jQuery));
