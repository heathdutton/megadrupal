/**
 * @file
 * Live track allows track the current time at a given task.
 */

(function($) {

Drupal.timetracker = Drupal.timetracker || {};
Drupal.timetracker.liveTrack = Drupal.timetracker.liveTrack || {};

/**
 * Create and attach the live track buttons.
 */
Drupal.behaviors.liveTrack = {
  attach: function(context, settings) {
    var $form = $('#timetracker-table-form', context);
    var $liveTrack = $('<div id="timetracker-livetrack"></div>');
    var $controls = $('<div id="timetracker-livetrack-controls"/></div>').appendTo($liveTrack);
    var $startButton = $('<button id="timetracker-livetrack-start">' + Drupal.t('Start') + '</button>').appendTo($controls);
    var $stopButton = $('<button id="timetracker-livetrack-stop">' + Drupal.t('Stop') + '</button>').appendTo($controls);
    var $display = $('<div id="timetracker-livetrack-display">00:00</div>').appendTo($liveTrack);
    var cycle = settings.timetracker.livetrack.cycle * 60;

    Drupal.timetracker.liveTrack.ringAudio = new Audio(settings.basePath + settings.timetracker.path + '/sounds/ring.wav');

    $liveTrack.prependTo($form);

    $startButton.bind('click', function(event) {
      if (!$startButton.is('[disabled]')) {
        Drupal.timetracker.liveTrack.start($liveTrack, cycle);
      }
      event.preventDefault();
    });

    $stopButton.bind('click', function(event) {
      if (!$stopButton.is('[disabled]')) {
        Drupal.timetracker.liveTrack.stop();
      }
      event.preventDefault();
    });

    $controls
      .bind('enable', function() {
        $startButton.removeAttr('disabled');
        $stopButton.removeAttr('disabled');
      })
      .bind('disable', function() {
        $startButton.attr('disabled', 'disabled');
        $stopButton.attr('disabled', 'disabled');
      });

    $liveTrack.data('$display', $display);
  },
};

/**
 * Start tracker.
 *
 * @param $track
 *   The live track jQuery object.
 * @param stop
 *   Time to end the tracker in seconds.
 */
Drupal.timetracker.liveTrack.start = function($track, stop) {
  var time = arguments[0] || 1800;
  var interval = 250;
  var liveTrack;

  if (!Drupal.timetracker.liveTrack.data) {
    Drupal.timetracker.liveTrack.data = {};
    liveTrack = Drupal.timetracker.liveTrack.data;

    liveTrack.$track = $track;
    liveTrack.init = new Date().getTime();
    liveTrack.timer = 0;

    Drupal.timetracker.liveTrack.displaySet(0);

    liveTrack.interval = setInterval(Drupal.timetracker.liveTrack.interval, interval);
    liveTrack.timeout = setInterval(function() {
      if (liveTrack.timer > stop) {
        Drupal.timetracker.liveTrack.stop();
      }
    }, interval);

    $track.addClass('running');
  }
};

/**
 * Update track dislay by interval basis.
 */
Drupal.timetracker.liveTrack.interval = function() {
  var liveTrack = Drupal.timetracker.liveTrack.data;
  liveTrack.timer = ((new Date().getTime()) - liveTrack.init) / 1000;

  Drupal.timetracker.liveTrack.displaySet(liveTrack.timer);
};

/**
 * Set a new value to tracker display.
 *
 * @param time
 *   Updated time in seconds.
 */
Drupal.timetracker.liveTrack.displaySet = function(time) {
  var liveTrack = Drupal.timetracker.liveTrack.data;
  var seconds = Math.floor(time);
  var minutes = seconds ? Math.floor(seconds / 60) : 0;
  var hours = minutes ? Math.floor(minutes / 60) : 0;
  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours < 10 ? '0' + hours : hours;
  minutes = minutes < 10 ? '0' + minutes : minutes;
  seconds = seconds < 10 ? '0' + seconds : seconds;

  liveTrack.$track.data('$display').text((hours > 0 ? hours + ':' : '') + minutes + ':' + seconds);
};

/**
 * Clear display and reset track timer.
 */
Drupal.timetracker.liveTrack.stop = function() {
  var liveTrack = Drupal.timetracker.liveTrack.data;

  liveTrack.$track.removeClass('running');

  Drupal.timetracker.liveTrack.end(liveTrack.timer);

  clearInterval(liveTrack.interval);
  clearInterval(liveTrack.timeout);
  delete Drupal.timetracker.liveTrack.data;
};

/**
 * End the timer to submit the tracked time.
 *
 * @param time
 *   The total tracked time in seconds.
 */
Drupal.timetracker.liveTrack.end = function(time) {
  var now = new Date();
  var start = new Date(now.getTime() - (time * 1000));

  Drupal.timetracker.liveTrack.ring();
  Drupal.timetracker.liveTrack.reportDialog(start, time, Drupal.timetracker.liveTrack.report);
};

/**
 * Report given time.
 *
 * @param start
 *   Date object with the start date of the report.
 * @param time
 *   Reported time in seconds.
 */
Drupal.timetracker.liveTrack.report = function(start, time) {
  var startDayMinutes = (start.getHours() * 60) + (start.getMinutes() * 1);
  var end = new Date(start.getTime() + (time * 1000));
  var endDayMinutes = (end.getHours() * 60) + (end.getMinutes() * 1);

  $('#timetracker-table-form .timetracker-table-form .today td').each(function() {
    var time = $(this).attr('class').replace(/.*time-([0-9]{4})/, '$1');
    var hours = time[0] + time[1];
    var minutes = time[2] + time[3];
    var dayMinutes = (hours * 60) + (minutes * 1);

    if (dayMinutes >= startDayMinutes && dayMinutes < endDayMinutes) {
      $(this).find('label').trigger('click');
    }
  });
};

/**
 * Play ring sound.
 */
Drupal.timetracker.liveTrack.ring = function() {
  if (Drupal.timetracker.liveTrack.ringAudio) {
    console.log(Drupal.timetracker.liveTrack.ringAudio);
    Drupal.timetracker.liveTrack.ringAudio.play();
  }
};

/**
 * Show a dialog asking to save a report.
 */
Drupal.timetracker.liveTrack.reportDialog = function(start, time, callback) {
  var startHours = start.getHours() > 9 ? start.getHours() : '0' + start.getHours();
  var startMinutes = start.getMinutes() > 9 ? start.getMinutes() : '0' + start.getMinutes();
  var startText = startHours + ':' + startMinutes;

  var timeHours = Math.floor(time / (60 * 60));
  var timeMinutes = Math.floor(time / 60);
  var timeText = [];

  var cancelCallback = arguments.length > 3 ? arguments[3] : null;
  var liveTrack = Drupal.timetracker.liveTrack.data;
  var $controls = liveTrack.$track.find('#timetracker-livetrack-controls');

  var report, $dialog, $confirm, $cancel;

  if (timeHours > 0) {
    timeText.push(timeHours > 1 ? timeHours + ' hours' : timeHours + ' hour');
  }
  if (timeMinutes > 0) {
    timeText.push(timeMinutes > 1 ? timeMinutes + ' minutes' : timeMinutes + ' minute');
  }
  timeText = timeText.join(', ');

  $dialog = $('<div id="timetracker-livetrack-report-dialog">' + Drupal.t('tracked since <strong>@start</strong>. Save time?', {
    '@time': timeText,
    '@start': startText
  }) + '</div>');
  $cancel = $('<button id="timetracker-livetrack-report-dialog-cancel">' + Drupal.t('Discard') + '</button>').appendTo($dialog);
  $confirm = $('<button id="timetracker-livetrack-report-dialog-confirm">' + Drupal.t('Save') + '</button>').appendTo($dialog);

  $controls.trigger('disable');

  $cancel.bind('click', function() {
    if (typeof cancelCallback == 'function') {
      cancelCallback.call(this, start, time);
    }

    $controls.trigger('enable');
    $dialog.remove();
  });

  $confirm.bind('click', function() {
    callback.call(this, start, time);

    $controls.trigger('enable');
    $dialog.remove();
  });

  $dialog.hide().prependTo(liveTrack.$track).fadeIn();
};

})(jQuery);
