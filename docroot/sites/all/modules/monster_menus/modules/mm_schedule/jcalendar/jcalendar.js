(function ($) {
  Drupal.behaviors.jcalendar = {
    attach: function (context, settings) {
      // I am assuming that all of the links are refering to an internal node

      // add the attribute rel=facebox to all of the links I want to have a popup
      $('div.views-field > .field-content > a,tr.all-day .view-item a,.month-view .view-item a,.view .view-content .view-field > a').filter(':not(.no-jcalendar)').attr('class', 'jcpopup');

      var closeMe = function() {
        $('div#calpopup').fadeOut('fast', function() { $(this).remove(); });
      };

      // on click of a link
      $('a.jcpopup').click(function() {
        // Get NodeID and ItemID
        var ids = $(this).closest('div[class^="calendar."]').attr('class').match(/(calendar\..*?)(\s|$)/);
        if (ids.length >= 2) {
          ids = ids[1];
          var nid = ids.split(".")[1];

          // Make sure that other stuff is closed. This seems to cause a problem in Fx2 and IE7.
          $('div#calpopup').remove();

          // create div to hold data and add it to the end of the body
          var div = $('<div id="calpopup"><div id="popup-close"><img id="popup-close-img" src="' + Drupal.settings.jcalendarPath + '/images/cross.png" /></div><div id="calpopup-body"><img src="' + Drupal.settings.jcalendarPath + '/images/throbber.gif" id="popthrobber" /></div></div>').attr('style','display: none');
          div.appendTo(document.body);

          // Locate Popup
          var offset = $(this).offset();
          // Check position with window width.
          var offset_left = offset.left + 5;
          if ($(window).width() < $('#calpopup').width() + offset.left) {
            offset_left -= $('#calpopup').width() + 5;
            if (offset_left < 0) {
              offset_left = 0;
            }
          }
          var offset_top = offset.top + 25;
          if ($(window).height() < $('#calpopup').height() + offset_top) {
            offset_top -= $('#calpopup').height() + 25;
            if (offset_top < 0) {
              offset_top = 0;
            }
          }
          $('#calpopup').css({
            left: offset_left,
            top:  offset_top}
          ).fadeIn('slow');

          // fill the div with data
          $.ajax({
            type: "GET",
            url: Drupal.settings.basePath + "?q=jcalendar/getnode/" + nid + "/" + ids,
            success: function(msg) {
              $('#calpopup-body').html(msg);
            }
          });

          // On click of the close image
          $('img#popup-close-img').click(closeMe);

          $(document).bind("click.jcalendar", function(y) {
            var $tgt = $(y.target);
            if (!$tgt.parents().is('div#calpopup')) {
              closeMe();
              $(document).unbind("click.jcalendar");
            }
          });

          // Don't Follow the real link
          return false;
        }
      });
    }
  };
})(jQuery);
