(function($) {
  var timeline;
  var lastMove = 0;
  Drupal.behaviors.cm_agenda_editor = {
    attach : function(context, settings) {
      var items = new vis.DataSet(Drupal.settings.cm_agenda_editor.items);
      var groups = new vis.DataSet(Drupal.settings.cm_agenda_editor.groups);
      var container = document.getElementById('timeline');
      var options = {
        stack : false,
        snap : null,
        min : '1978-11-19 00:00:00',
        max : '1978-11-19 23:59:59',
        showMajorLabels : false,
        showCustomTime : true,
        format : {
          minorLabels : {
            millisecond : 'HH:mm:ss:SSS',
            second : 'HH:mm:ss',
            minute : 'HH:mm:ss',
            hour : 'HH:mm:ss',
            weekday : 'ddd D',
            day : 'D',
            month : 'MMM',
            year : 'YYYY'
          },
          majorLabels : {
            millisecond : 'HH:mm:ss',
            second : 'D MMMM HH:mm',
            minute : 'ddd D MMMM',
            hour : 'ddd D MMMM',
            weekday : 'MMMM YYYY',
            day : 'MMMM YYYY',
            month : 'YYYY',
            year : ''
          },
          height : '300px',
        },
        editable : {
          updateTime : true,
          updateGroup : false,
          add : false,
          remove : true,
        },
      };
      timeline = new vis.Timeline(container, items, groups, options);
      timeline.addCustomTime('1978-11-19 00:00:00', 1);

      timeline.on('timechange', function(event, properties) {
        Drupal.settings.popcornPlayer[0].popcorn.currentTime(cm_agenda_date_to_sec(event.time));
      });

      items.on('*', function(event, properties) {
        console.log('Update');
        console.log(event);
        console.log(properties);
        for ( index = 0; index < properties.data.length; ++index) {
          var e = properties.data[index];
          console.log(e);
          $('#event_' + e.id + ' .newstart').html(cm_agenda_date_to_his(e.start));
          $('#event_' + e.id + ' .newend').html(cm_agenda_date_to_his(e.end));
        }
      });

      $('#cm-agenda-editor-save').bind('click', function() {
        var itemsdata = items.get({
          type : {
            start : 'ISODate',
            end : 'ISODate'
          }
        });
        $.ajax({
          type : 'POST',
          url : '/cm-agenda-editor/ajax/save/' + Drupal.settings.cm_agenda_editor.agendanid,
          dataType : 'json',
          success : cm_agenda_save_completed,
          data : {
            agendanid : Drupal.settings.cm_agenda_editor.agendanid,
            mediaid : Drupal.settings.cm_agenda_editor.mediaid,
            items : itemsdata
          },
        });
      });
      setTimeout(function() {
        Drupal.settings.popcornPlayer[0].popcorn.on("timeupdate", function(e) {
          if (Date.now() - lastMove > 40) {
            var result = cm_agenda_sec_to_his(Drupal.settings.popcornPlayer[0].popcorn.currentTime());
            time = '1978-11-19 ' + result;
            timeline.setCustomTime(time, 1);
            //timeline.moveTo(time, false);
            lastMove = Date.now();
          }
        }, false);

      }, 2000);
    }
  };

  function cm_agenda_sec_to_his(sec) {
    var hours = parseInt(sec / 3600) % 24;
    var minutes = parseInt(sec / 60) % 60;
    var seconds = parseInt(sec % 60);
    var result = (hours < 10 ? "0" + hours : hours) + ":" + (minutes < 10 ? "0" + minutes : minutes) + ":" + (seconds < 10 ? "0" + seconds : seconds);
    return result;
  }

  function cm_agenda_date_to_sec(date) {
    var start = new Date('1978-11-19 00:00:00');
    var time = new Date(date);
    var newTime = parseInt((time.getTime() - start.getTime()) / 1000);
    return newTime;
  }

  function cm_agenda_date_to_his(date) {
    return cm_agenda_sec_to_his(cm_agenda_date_to_sec(date));
  }

  function cm_agenda_save_completed(data) {
    $("#cm-agenda-editor-message-saved").remove();
    $("#cm-agenda-editor-message").html('<div id="cm-agenda-editor-message-saved">' + data.saveMessage + '</div>');
    $("#cm-agenda-editor-message-saved").fadeIn();
    $("#cm-agenda-editor-message-saved").fadeOut(4000);
  }

  function logEvent(event, properties) {
    var log = document.getElementById('log');
    var msg = document.createElement('div');
    msg.innerHTML = 'event=' + JSON.stringify(event) + ', ' + 'properties=' + JSON.stringify(properties);
    log.firstChild ? log.insertBefore(msg, log.firstChild) : log.appendChild(msg);
  }

})(jQuery);

