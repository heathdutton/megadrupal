(function ($) {

/**
* @file
* Javascript support files.
*
*/

Drupal.behaviors.bootstrap_calendar = {
  attach: function (context, settings) {
    // Read URL paramters
//    var urlParams = bootstrap_calendar_readURLparams();

//    var URLvariables = bootstrap_calendar_url_params(urlParams);

    var URLvariables = Drupal.settings.bootstrap_calendar.args;
    var baseURL = Drupal.settings.bootstrap_calendar.base;

    var firstday = Drupal.settings.bootstrap_calendar.first_day;
    firstday = firstday.length ? parseInt(value) : null;

    "use strict";

    var options = {
      events_source: baseURL + 'ajax/bootstrap_calendar/events' + URLvariables,
      view: Drupal.settings.bootstrap_calendar.default_view,
      views: Drupal.settings.bootstrap_calendar.views,
      first_day: firstday,
      tmpl_path: baseURL + Drupal.settings.bootstrap_calendar.path + '/tmpls/',
      tmpl_cache: false,
      time_start: Drupal.settings.bootstrap_calendar.time_start,
      time_end: Drupal.settings.bootstrap_calendar.time_end,
      time_split: Drupal.settings.bootstrap_calendar.time_split,
      day: Drupal.settings.bootstrap_calendar.date,
      language: 'i18n',
      holidays: {},
      onAfterViewLoad: function(view) {
        var p = this.options.position.start;
        $('span.month-title').text(this.locale['m' + p.getMonth()]);
        $('span.year-title').text(p.getFullYear());
        $( ".select-year select" ).val(p.getFullYear());
        $('h3.calendar-title').text(this.getTitle());
        $('.btn-group button').removeClass('active');
        $('button[data-calendar-view="' + view + '"]').addClass('active');
      },
      classes: {
        months: {
          general: 'label'
        }
      }
    };

    var calendar = $('#calendar').calendar(options);

    $('.btn-group button[data-calendar-nav]').each(function() {
      var $this = $(this);
      $this.click(function() {
        calendar.navigate($this.data('calendar-nav'));
      });
    });

    $('.btn-group button[data-calendar-view]').each(function() {
      var $this = $(this);
      $this.click(function() {
        calendar.view($this.data('calendar-view'));
      });
    });

    $( ".select-year select" ).change(function() {
        var newyear = $(this).val();
        calendar.options.position.start.setFullYear(newyear);
//        var p = calendar.options.position.start;
        calendar.options.day = calendar.options.position.start.getFullYear() + '-' + calendar.options.position.start.getMonthFormatted() + '-' + calendar.options.position.start.getDateFormatted();
        calendar.view();
    });

    calendar.setOptions({modal: parseInt(Drupal.settings.bootstrap_calendar.open_modal)});

    $('#events-modal .modal-header, #events-modal .modal-footer').click(function(e){
      //e.preventDefault();
      //e.stopPropagation();
    });
  }
}

function bootstrap_calendar_readURLparams() {
  (window.onpopstate = function () {
    var match,
      pl     = /\+/g,  // Regex for replacing addition symbol with a space
      search = /([^&=]+)=?([^&]*)/g,
      decode = function (s) { return decodeURIComponent(s.replace(pl, " ")); },
      query  = window.location.search.substring(1);

    urlParams = {};
    while (match = search.exec(query))
      urlParams[decode(match[1])] = decode(match[2]);
  })();
  return urlParams;
}

function bootstrap_calendar_url_params(params) {
  var URLvariables = "";
  if (params.og) URLvariables += '/og/' + params.og.replace(" ", "+");
  if (params.tid) URLvariables += '/tid/' + params.tid.replace(" ", "+");
  if (params.eid) URLvariables += '/eid/' + params.eid.replace(" ", "+");
  return URLvariables;
}

}(jQuery));

