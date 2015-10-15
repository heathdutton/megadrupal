(function ($) {

Drupal.timetracker = Drupal.timetracker || {};
Drupal.timetracker.sendingStack = [];
Drupal.timetracker.loadingCall = false;

Drupal.behaviors.timetrackerTableFormCheck = {
  attach: function (context, settings) {
    $form = $('#timetracker-table-form', context),
    $table = $form.find('.timetracker-table-form'),
    $trs = $table.find('tbody tr');


    $trs.each(function() {
      var $tr = $(this);

      $tr.find('label').each(function() {
        var $label = $(this),
            $input = $('#' + $label.attr('for'));

        $input.appendTo($label);
      });

      $tr
        .click(function(e) {
          var $target = $(e.target),
              $td = $(e.target).is('td') ? $(e.target) : $(e.target).parents('td');

          if (!$target.is('input') && $td.length && !$td.is('.disabled') && $td.find('input[type="checkbox"]:not(:disabled)').length) {
            var $focus = $tr.find('td.focus').removeClass('focus');
            if (e.shiftKey && $focus.length) {
              var $range = $tr.getRange($focus, $td);
              $range = $range.add($td);

              if ($focus.is('.active')) {
                $range.addClass('active');
                $range.each(function() {
                  $(this).not('.disabled').find('input.form-checkbox:not(:disabled):not(:checked)').click();
                });
              }
              else {
                $range.removeClass('active').removeClass('rejected');
                $range.each(function() {
                  $(this).not('.disabled').find('input.form-checkbox:not(:disabled):checked').click();
                });
              }
            }
            else {
              $td.find('input.form-checkbox').click();
              $td.toggleClass('active').removeClass('rejected');
            }

            rainy_form_submit($form, $td);
            $td.addClass('focus');

            return false;
          }
        })
        .disableSelect();

    });

    function rainy_form_submit($form, $target) {
      callEvent = {
        url: $form.attr('action'),
        formdata: $form.serialize() + '&ajax=1'
      }

      Drupal.timetracker.sendingStack.push(callEvent);

      $target.addClass('load');

    }

     $(window).bind('beforeunload', function() {
       if (Drupal.timetracker.loadingCall) {
         return Drupal.t("There are reports being processed.");
       }
     });
  }
};

/**
 * Do the actual ajax calls, orderly
 */
Drupal.timetracker.interval = window.setInterval(function() {
  // if nothing is waiting for an answer AND there are stuff in the queue to send,
  // process the latest call request. We only send the most recent call made that interval
  // because it contains all the data from previous calls
  if (!Drupal.timetracker.loadingCall && Drupal.timetracker.sendingStack.length) {

    // gets the next ajax call
    var nextCall = Drupal.timetracker.sendingStack.pop();

    // reset queue
    Drupal.timetracker.sendingStack = [];

    // indicates some call is waiting
    Drupal.timetracker.loadingCall = true;

    $.ajax({
      url: nextCall.url,
      type: "POST",
      data: nextCall.formdata,
      dataType: 'json',
      success: function(data) {
        if (data.status !== 'status') {
          var link = '<a href="javascript:document.location.reload()">' + Drupal.t("Click to reload") + '</a>';
          $table.after('<div class="message">' + data.message + '<br/>' + link + '</div>');
          var $message = $table.next('.message');
          $table.slideUp('fast');
          $('.timetracker-pager').hide();
        }

        // allows other calls to be made
        Drupal.timetracker.loadingCall = false;

        $.event.trigger('timeTrackerUpdate', [data]);
      }
    });
  }

  // if nothing is being called AND nothing is waiting a call, provides feedback to the user
  if (!Drupal.timetracker.loadingCall && !Drupal.timetracker.sendingStack.length) {
    $('.load').removeClass('load');
  }
}, 1000);

Drupal.behaviors.rainyTTTableDaySticky = {
  attach: function(context) {
    var $form = $('#timetracker-table-form', context),
        $table = $form.find('.timetracker-table-form'),
        $days = $table.children('tbody').children('tr').children('th'),
        stickies = [],
        widths = [],
        width;

    $days.each(function() {
      var $day = $(this),
          sticky = '<div class="sticky">' + $day.html() + '</div>',
          $sticky = $day.wrapInner('<div class="static"/>').append(sticky).children('.sticky');

      widths.push($sticky.width());
    });

    widths.sort();
    width = widths.pop();

    $table.find('.sticky').width(width);
  }
};

/**
 * Automatically scroll to the actual time.
 */
Drupal.behaviors.timetrackerTableStartPosition = {
  attach: function(context) {
    var $form = $('#timetracker-table-form', context);

    $form.once('timetracker-table-start-position', function() {
      var $table = $form.find('.timetracker-table-form'),
          $tbody = $table.children('tbody'),
          now = new Date(),
          hour = now.getHours(),
          time = hour < 10 ? '0' + hour + '00' : hour + '00',
          $start = $tbody.find('tr:first td.time-' + time),
          $sticky = $table.find('tbody tr th div.sticky'),
          scroll = $start.offset().left - $sticky.outerWidth();

      $('html,body')
        .animate({
          scrollLeft: scroll
        }, 'normal');
    });
  }
};

})(jQuery);

jQuery.fn.getRange = function($start, $end) {
  var i = [$start.prevAll().length, $end.prevAll().length].sort(function(a,b){return a - b});

  return $start.parent().children().slice(i[0], i[1]);
};

jQuery.fn.disableSelect = function(expr) {
  // Mozilla
  if (this.css("MozUserSelect")!= "undefined") {
    this.css("MozUserSelect", "none");
  }
  // Webkit/Khtml
  if (this.css("KhtmlUserSelect") != "undefined") {
    this.css("KhtmlUserSelect", "none");
  }
  // Opera and... IE (who cares?)
  if (typeof this[0].setProperty != "undefined") {
    this[0].setProperty("unselectable","on");
  }
};
