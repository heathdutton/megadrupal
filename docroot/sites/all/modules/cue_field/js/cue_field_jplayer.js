(function ($) {

  /**
   * Attaches a seek event to each cue point link for jPlayer.
   *
   * This code assumes that the jPlayer is embedded to the page using the
   * audiofield module. The function audiofield_jplayer() uses a static
   * variable to build the player id. This results in the id
   * #jquery_jplayer_1 for the first player on each page and therefore
   * this id is used in the following.
   *
   * @todo Get the player id so we can deal with multiple player instances.
   */
  Drupal.behaviors.JPlayerSeek = {
    attach: function(context) {
      $("[id^=chapter]").each(function(index, value) {
        $(value).click(function() {
          $('#jquery_jplayer_1').jPlayer('play', parseInt(value.getAttribute('data-start')));
          // Prevent the original link to be triggered.
          return false;
        });
      });

      var chapter = decodeURI((RegExp('(#chapter.+?)(&|$)').exec(location.hash) || [,null])[1]);
      if (chapter != 'null') {
        // For some reasons jPlayer does not start unless we use this timeout.
        window.setTimeout(function() {
          $('#jquery_jplayer_1').jPlayer('play', parseInt($(chapter)[0].getAttribute('data-start')));
          $("html, body").animate({ scrollTop: $('#jquery_jplayer_1').offset().top }, 100);
        }, 0);
      }
    }
  }

})(jQuery);
