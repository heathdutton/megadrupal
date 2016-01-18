/**
 * @file
 * Change the click process to open a modal.
 */

(function ($) {

Drupal.behaviors.party_activity_popup = {
  attach: function (context, settings) {
    $.each(Drupal.settings.fullcalendar, function (id, cal) {
      cal.weights[2] = "party_activity_popup";
    });
  }
}

Drupal.fullcalendar.plugins.party_activity_popup = {
  options: function (fullcalendar, settings) {
    var options = {
      eventClick: function (calEvent, jsEvent, view) {
        Drupal.CTools.Modal.show();
      },
      eventAfterRender: function (calEvent, element, view) {
        var base = calEvent.url;
        var ajaxSettings = {
          url: calEvent.url,
          event: 'click',
          progress: { type: 'throbber' }
        };
        Drupal.ajax[base] = new Drupal.ajax(base, element, ajaxSettings);
      },
      selectable: true,
      selectHelper: true,
      select: function ( startDate, endDate, allDay, jsEvent, view ) {
        Drupal.CTools.Modal.show();
        // NOTE: getTimezoneOffset will always use the local timezone of the
        // browser and not any other setting.
        var element = this.element[0];
        var base = "/"+Drupal.settings.party_activity_popup.add_path;

        // Work out query args
        base += "?fc=1&views_view="+$(element).parents('.fullcalendar').attr('views_view')+"&views_display="+$(element).parents('.fullcalendar').attr('views_display');
        base += "&startDate="+(startDate.getTime()/1000)+"&endDate="+(endDate.getTime()/1000)+"&tz="+startDate.getTimezoneOffset();
        $.each(Drupal.fullcalendar.plugins.party_activity_popup.add_path_hooks, function (i, func) {
          base = func(base, startDate, endDate, allDay, jsEvent, view);
        });
        var ajaxSettings = {
          url: base,
          event: base,
          progress: { type: 'throbber' }
        }
        Drupal.ajax[base] = new Drupal.ajax(base, element, ajaxSettings);
        $(element).trigger(base);
      }
    };

    return options;
  },
  add_path_hooks: {}
};

$(function() {
  Drupal.party_activity = {
    updateEvent: function($calendar, $event, response) {
      $.each(response, function(attr, value) {
        if (attr == 'command') {
          return;
        }

        $event.attr(attr, value);
      });
      $calendar.fullCalendar('refetchEvents');
    },
    createEvent: function($calendar, response) {
      var event = document.createElement("div");
      $(event).addClass('fullcalendar-event');
      $(event).append('<h3 class="title">'+response.title+'<h3>');
      $(event).append('<div class="fullcalendar-instance"><a class="fullcalendar-event-details"><span class="date-display-single">{time}</span></a></div>');
      var $event = $(event).find('a.fullcalendar-event-details');
      $event.attr('entity_type', response.entity_type);
      $event.attr('eid', response.eid);
      $.each(response, function(attr, value) {
        if (attr == 'command') {
          return;
        }

        $event.attr(attr, value);
      });
      $calendar.siblings('.fullcalendar-content').append(event);
      $calendar.fullCalendar('refetchEvents');
    }
  };

  Drupal.ajax.prototype.commands.fullcalendar_activity_add = function(ajax, response, status) {
    $(response.fullcalendar_selector).each(function() {
      if ($(this).siblings('.fullcalendar-content').find(".fullcalendar-event-details[entity_type='"+response.entity_type+"'][eid='"+response.eid+"']").length > 0) {
        var $event = $(this).siblings('.fullcalendar-content').find(".fullcalendar-event-details[entity_type='"+response.entity_type+"'][eid='"+response.eid+"']");
        Drupal.party_activity.updateEvent($(this), $event, response);
      }
      else {
        Drupal.party_activity.createEvent($(this), response);
      }
    });
  };
  Drupal.ajax.prototype.commands.fullcalendar_activity_update = function(ajax, response, status) {
    $(response.fullcalendar_selector).each(function() {
      var $event = $(this).siblings('.fullcalendar-content').find(".fullcalendar-event-details[entity_type='"+response.entity_type+"'][eid='"+response.eid+"']");
      Drupal.party_activity.updateEvent($(this), $event, response);
    });
  };
  Drupal.ajax.prototype.commands.fullcalendar_activity_delete = function(ajax, response, status) {
    $(response.fullcalendar_selector).each(function() {
      var $event = $(this).siblings('.fullcalendar-content').find(".fullcalendar-event-details[entity_type='"+response.entity_type+"'][eid='"+response.eid+"']");
      $event.remove();
      $(this).fullCalendar('refetchEvents');
    });
  };
});

}(jQuery));
