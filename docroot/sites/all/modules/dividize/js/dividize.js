(function ($) {

  "use strict";

  Drupal.behaviors.dividize = {
    attach: function (context, settings) {

      // Checking settings.
      if (!settings.dividize || settings.dividize.selector === '') {
        return;
      }

      Drupal.dividize.unbind('ready', document);
      Drupal.dividize.bind('ready', document);
      Drupal.dividize.unbind('resize', window);
      Drupal.dividize.bind('resize', window);
    }
  };

  // Make sure our objects are defined.
  Drupal.dividize = Drupal.dividize || {};

  // The current state of tables.
  Drupal.dividize.full = true;

  // Starting execution.
  Drupal.dividize.execute = function (settings) {
    if (settings.selector !== '') {

      // Full to compact.
      if (Drupal.dividize.full && ((settings.width === '') || dividizeWidthCheck(settings.width))) {
        Drupal.dividize.full = false;

        $(settings.selector).dividize({
          classes: settings.classes,
          removeHeaders: settings.removeHeaders,
          addLabelHeaders: settings.addLabelHeaders,
          hideLabels: settings.hideLabels,
          preserveEvents: settings.preserveEvents,
          preserveDim: settings.preserveDim,
          enableAltRows: settings.enableAltRows
        });
      }
      // todo: Compact to full.
      /*else if (!Drupal.dividize.full && ((settings.width !== '') && dividizeWidthCheck(settings.width))) {
       Drupal.dividize.full = true;

        $(settings.selector).dividize({
          classes: settings.classes,
          removeHeaders: settings.removeHeaders,
          addLabelHeaders: settings.addLabelHeaders,
          hideLabels: settings.hideLabels,
          preserveEvents: settings.preserveEvents,
          preserveDim: settings.preserveDim,
          enableAltRows: settings.enableAltRows
        });
      }*/
    }
  };

  // Bind an event handler.
  Drupal.dividize.bind = function (event, element) {
    $(element).bind(event + '.dividizeEvent', dividizeEvent);
  };

  // Unbind an event handler.
  Drupal.dividize.unbind = function (event, element) {
    $(element).unbind(event + '.dividizeEvent', dividizeEvent);
  };

  // Execute if the event is triggered.
  var dividizeEvent = function () {
    Drupal.dividize.execute(Drupal.settings.dividize);
  };

  // Check a width of the window.
  function dividizeWidthCheck(width) {
    return !isNaN(parseFloat(width)) && isFinite(width) && ($(window).width() <= width);
  }

  // Adding a jQuery method.
  $.fn.dividize = function (options) {
    var settings = $.extend({
      // Custom header row target (if not normal table).
      customHeaderTarget: 'th',
      // Remove/leave headers div.
      removeHeaders: false,
      // Add headers to each cell.
      addLabelHeaders: false,
      // Hide the labelHeader by default.
      hideLabels: true,
      // Save events cell content and restore after conversion.
      preserveEvents: false,
      // Try to keep cell widths & heights.
      preserveDim: false,
      // Add any extra classes to converted div.
      classes: '',
      // Enable alternating rows.
      enableAltRows: false
    }, options);

    // Manage the events we want to save and restore.
    var eventManager = {
      container: {
        eventContainer: {}
      },
      save: function ($elem) {
        var self = this;

        $elem.find('*').each(function (index, element) {
          // Get source events.
          var events = $(element).data('events');
          var elemIndex = 'index-' + index;

          // Make sure there are events.
          if (typeof events == 'undefined') {
            return true;
          }

          // Iterate through all event types.
          $.each(events, function (eventType, eventArray) {
            // Iterate through every bound handler.
            $.each(eventArray, function (iindex, event) {
              //take event namespaces into account
              var eventToBind = event.namespace.length > 0 ? (event.type + '.' + event.namespace) : (event.type);

              // Create new index.
              if (typeof self.container.eventContainer[elemIndex] == 'undefined') {
                self.container.eventContainer[elemIndex] = [];
              }

              // Push events to container.
              self.container.eventContainer[elemIndex].push({
                eventToBind: eventToBind,
                eventData: event.data,
                eventHandler: event.handler
              });
            });
          });
          // Add marker to element so we can map back correct event.
          $(element).attr('data-event-index', elemIndex);
        });
      },
      restore: function ($elem) {
        var self = this;

        $elem.find('*').each(function (index, element) {
          var $element = $(element);
          var indexID = $element.attr('data-event-index');

          if (typeof indexID != 'undefined') {
            var events = self.container.eventContainer[indexID];

            for (var i = 0; i < events.length; i++) {
              var event = events[i];

              // Unbind any similar events.
              $element.unbind(event.eventToBind);
              $element.bind(event.eventToBind, event.eventData, event.eventHandler);
            }
          }
        });
      }
    };

    // Save events to reattach later.
    if (settings.preserveEvents) {
      eventManager.save(this);
    }

    // Global index to make each element unique.
    var globalIndex = 0;

    this.replaceWith(function () {
      var $self = $(this);
      // Get header target.
      var $th = $self.find(settings.customHeaderTarget);
      // Get values.
      var th = $th.map(function () {
        return $(this).html();
      }).get();

      // Remove the headers
      if (settings.removeHeaders) {
        $th.closest('tr').remove();
      }

      // Our table replacement.
      var $table = $('<div>').addClass('dividize-box').addClass(settings.classes);
      // Add/first last row tags.
      var rowCount = $self.find('tr').length;

      // Iterate table rows.
      $('tr', $self).each(function (i) {
        // Add our classes and append row to table.
        var $row = $('<div>')
          .addClass('dividize-' + globalIndex++)
          .addClass('dividize-row')
          .addClass('dividize-row-' + i)
          .appendTo($table);

        // Add alternating row classes.
        if (settings.enableAltRows) {
          $row.addClass((i % 2) === 0 ? 'even' : 'odd');
        }

        // Mark first and last row.
        if (i === 0) {
          $row.addClass('first-row');
        }
        else if (i == (rowCount - 1)) {
          $row.addClass('last-row');
        }

        // Add first/last cell tags.
        var cellCount = $(this).find('td, ' + settings.customHeaderTarget).length;

        // Iterate table cells.
        $('td, ' + settings.customHeaderTarget, this).each(function (index, el) {
          var $cell = $('<div>')
            .addClass('dividize-' + globalIndex++)
            .addClass('dividize-cell')
            .addClass('dividize-cell-' + index)
            // Mark the original table header cells.
            .addClass(($(this).is(settings.customHeaderTarget) ? 'data-exth' : ''));

          // Mark first and last cell.
          if (index === 0) {
            $cell.addClass('first-cell');
          }
          else if (index == (cellCount - 1)) {
            $cell.addClass('last-cell');
          }

          // Preserve cell dimensions.
          if (settings.preserveDim) {
            $cell.addClass('dividize-dim-cell');
            $cell.css({
              'line-height': $(el).outerHeight() + 'px',
              'height': $(el).outerHeight() + 'px',
              'width': $(el).outerWidth() + 'px'
            });
          }

          // Create label with content from table header (exclude header cells).
          if (settings.addLabelHeaders && !$(this).is(settings.customHeaderTarget)) {
            var $label = $('<div>');
            $label.addClass('dividize-' + globalIndex++).addClass('dividize-label').addClass('dividize-label-' + index).html(th[index]);

            if (settings.hideLabels) {
              $label.addClass('dividize-hidden');
            }

            $cell.append($label);
          }

          var $labelData = $('<div>')
            .addClass('dividize-' + globalIndex++)
            .addClass('dividize-data')
            .addClass('dividize-data-' + index)
            .html($(this).html());

          // Preserve dimensions of content.
          if (settings.preserveDim) {
            $labelData.addClass('dividize-dim-data');
          }

          $cell.append($labelData);
          $cell.appendTo($row);
        });
      });

      // Restore events.
      if (settings.preserveEvents) {
        eventManager.restore($table);
      }

      return $table;
    });
  };

})(jQuery);
