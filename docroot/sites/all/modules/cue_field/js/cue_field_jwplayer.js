(function ($) {

  /**
   * Attaches a seek event to each cue point link for jw_player.
   *
   * @todo Get the player id so we can deal with multiple player instances.
   */
  Drupal.behaviors.JWPlayerSeek = {
    attach: function(context) {
      $("[id^=chapter]").each(function(index, value) {
        $(value).click(function() {
          jwplayer().seek(value.getAttribute('data-start'))
          // Prevent the original link to be triggered.
          return false;
        });
      });

      var chapter = decodeURI((RegExp('(#chapter.+?)(&|$)').exec(location.hash)||[,null])[1]);
      if (chapter != 'null') {
        jwplayer().onReady(function (event) {
          // For some reasons jwPlayer does not start unless we use this
          // timeout.
          window.setTimeout(function() {
            jwplayer().seek($(chapter)[0].getAttribute('data-start'));
          }, 1);
        })
      }
    }
  };

})(jQuery);
