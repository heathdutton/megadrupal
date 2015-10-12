(function($) {
  Drupal.behaviors.cm_agenda_live = {
    attach : function(context, settings) {
      if ( typeof Drupal.settings.cm_agenda_live.countdown != 'undefined') {
        checkCountdown();
        setInterval(checkCountdown, 10000);
        setInterval(function() {
          start = Drupal.settings.cm_agenda_live.startTime;
          now = new Date();
          now = new Date(now.getTime() + Drupal.settings.cm_agenda_live.diff);
          viewerstart = new Date(start.getTime() - (Drupal.settings.cm_agenda_live.viewer_offset * 1000) + 1000);
          // One extra second for server to be totally ready becuase of clock syncronisation
          if (now > viewerstart) {
            window.location.href = "/node/" + Drupal.settings.cm_agenda_live.agenda_nid + '/live?time=' + now.getTime();
            return;
          }
          var difference = (start - now) / 1000;
          var days = parseInt(difference / (3600 * 24));
          var hours = parseInt(difference / 3600) % 24;
          var minutes = parseInt(difference / 60) % 60;
          var seconds = parseInt(difference % 60);
          var result = (days < 1 ? "" : "<div class=\"countdown-item\"><b>" + (days < 10 ? "0" : "") + days + "</b>" + Drupal.t("days") + "</div>") + (hours < 1 ? "" : "<div class=\"countdown-item\"><b>" + (hours < 10 ? "0" : "") + hours + "</b>" + Drupal.t("hours") + "</div>") + (minutes < 1 ? "" : "<div class=\"countdown-item\"><b>" + (minutes < 10 ? "0" : "") + minutes + "</b>" + Drupal.t("minutes") + "</div>") + "<div class=\"countdown-item\"><b>" + (seconds < 10 ? "0" : "") + seconds + "</b>" + Drupal.t("seconds") + "</div>";
          $('#countdown').html(result);
        }, 1000);
      } else {
        updatePresenter(true);
        setInterval(updatePresenter, 1000);
        setInterval(googlePing, 30000);
      }
    }
  };

  function checkCountdown() {
    now = new Date();
    $.ajax({
      url : '/ajax/agenda/' + Drupal.settings.cm_agenda_live.agenda_nid + '/countdown?time=' + now.getTime(),
      success : function(data) {
        Drupal.settings.cm_agenda_live.startTime = new Date(data.start);
        nowserver = new Date(data.now);
        nowclient = new Date();
        Drupal.settings.cm_agenda_live.diff = (nowserver.getTime() - nowclient.getTime());
        Drupal.settings.cm_agenda_live.viewer_offset = data.viewer_offset;
      }
    });
  }

  function updatePresenter(force) {
    now = new Date();
    force = typeof force !== 'undefined' ? force : false;
    $.ajax({
      url : '/ajax/agenda/' + Drupal.settings.cm_agenda_live.agenda_nid + '?time=' + now.getTime(),
      success : function(data) {
        now = new Date();
        if (data.state == 'post_live' || data.state == 'vod') {
          if (!data.admin) {
            window.location.href = "/node/" + Drupal.settings.cm_agenda_live.agenda_nid + '?time=' + now.getTime();
            return;
          } else {
            if ( typeof Drupal.settings.cm_agenda_live.admin_message == 'undefined') {
              alert('Normal users will be redirected via javascript');
              Drupal.settings.cm_agenda_live.admin_message = true;
            }
          }
        }
        setTimeout(function() {
          if(data.speaker && data.speaker !="") {
            $('#cm-agenda-speaker-0').addClass("active");
          } else {
            $('#cm-agenda-speaker-0').removeClass("active");
          }
          $('#cm-agenda-speaker-name').html(data.speaker);
          var currentScroll = $(".cm-agenda-chapter-container .cm-agenda-chapter-list")[0].scrollTop;
          $('.cm-agenda-chapter-container').html(data.chapter);
          $(".cm-agenda-chapter-container .cm-agenda-chapter-list")[0].scrollTop = currentScroll;
          $('.cm-agenda-info-container .cm-agenda-info-title').html(data.background_title);
          $('.cm-agenda-info-container .cm-agenda-info-text').html(data.background_text);
          if (data.background_title != '') {
            console.log('active');
            $('.cm-agenda-info').addClass("active");
          } else {
            $('.cm-agenda-info').removeClass("active");
            console.log('notactive');
          }
          // Adding script for scrollbar customization with enscroll
          // $('.scrollbox').enscroll({
          //  addPaddingToPane: false
          // });
        }, force ? 0 : data.delay * 1000);
      }
    });
  }

  function googlePing() {
    if ( typeof (_gaq) !== 'undefined') {
      _gaq.push(['_trackEvent', 'cm_agenda', 'Ping30s', Drupal.settings.cm_agenda_live.agenda_title + ' [' + Drupal.settings.cm_agenda_live.agenda_nid + ']']);
      console.log('ping');
    }
  }
})(jQuery);
