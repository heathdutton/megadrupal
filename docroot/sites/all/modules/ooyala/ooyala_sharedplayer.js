/**
 * @file
 * JavaScript for the Views Ooyala shared player output.
 */
(function ($) {
  // If the ooyala_player.js hasn't been loaded yet we initialize the object so
  // that we can added to it.
  Drupal.ooyala = Drupal.ooyala || {'listeners': {}, 'onCreateHandlers': {}, 'players': {} };

  /**
   * Switch the currently selected video when clicking on one of the videos in
   * the list.
   */
  Drupal.behaviors.ooyalaSharedPlayer = {
    attach: function(context) {
      $.each(Drupal.settings.ooyalaSharedPlayer, function(index, id) {
        // Use the array index to get the raw DOM element.
        var video_object = $('#' + id + "-player")[0];
        // Store a reference to the highest div for this player. We can't
        // store it directly in the object tag, so we put it in the parent
        // div saving us a few ugly .parent() calls later.
        $(video_object).parent().data('superid', id);


        $('#' + id + ' .ooyala_shared_player_row:not(".ooyala-shared-player-processed")').each(function() {
          $(this).addClass("ooyala-shared-player-processed");
          $(this).click(function() {
            // We won't have a reference to the video object in our closure.
            var video_object = $('#' + id + "-player")[0];
            video_object.setQueryStringParameters({
              embedCode: $(this).data('embed-code')
            });

            // Keep track of what item is currently selected.
            $(this).parent().find(".current").removeClass("current");
            $(this).addClass("current");

            return false;
          });
        }).first().addClass("current");
      });
    }
  };

  /**
   * Automatically play the video when it has been selected from the list of
   * videos.
   *
   * @todo: This code is not compatible with the V3 player.
   */
  Drupal.ooyala.listeners.shared_player = function(player, eventName, p) {
    switch(eventName) {
      case 'playComplete':
        // Get the index of the current embed code.
        var id = $(player).parent().data('superid');
        var next = $('#' + id + ' .ooyala_shared_player_row.current').removeClass("current").next();
        var embedCode = next.data('embed-code');

        // Kick off the next video.
        player.setQueryStringParameters({
          embedCode: embedCode
        });

        next.addClass("current");
        break;

      case 'embedCodeChanged':
        if (!$(player).parent().data('first-load')) {
          player.playMovie();
        }
        else {
          // This was the initial embed code set event, so remove our guard.
          $(player).parent().data('first-load', '');
        }
        break;

      case 'playerEmbedded':
        // Set a guard to prevent automatically playing on page load.
        $(player).parent().data('first-load', true);

        // And fire off Drupal behaviours that might be on the page.
        Drupal.attachBehaviors();
        break;
    }
  }

})(jQuery);
