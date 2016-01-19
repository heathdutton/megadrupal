(function($) {
/**
 * Attaches a Projekktor element by using the Projekktor Javascript Library
 */
Drupal.behaviors.projekktor = {
  attach: function(context, settings) {
    if ($(settings.projekktor.instances).length) {
      $('video[id^="projekktor-"]', context).once('projekktor', function() {
        var id = $(this).prop('id');
        var jsoptions = settings.projekktor.instances[id];
        // Create an id for a carousel that would go with this player instance.
        var jcarouselId = 'projekktor-jcarousel-' + id.substr(11);
        // Use the carousel if those options have been passed.
        if ($(settings.projekktorJcarousel).length) {
          // Initializes the jcarousel.
          var ops = settings.projekktorJcarousel[jcarouselId];
          $('#' + jcarouselId).jcarousel(ops);
          var carousel = $('#' + jcarouselId).data("jcarousel");
          // readyListener function to get player item info.
          var readyListener = function(value, ref) {
            // Adds active class to current jcarousel slide.
            $('#' + jcarouselId).children('li.jcarousel-item').each(function() {
              if ($(this).index('#' + jcarouselId + ' > li.jcarousel-item') == (ref.getItemIdx())) {
                $(this).addClass('active');
                carousel.scroll(ref.getItemIdx(), true);
              }
              else {
                $(this).removeClass('active');
              }
            });
          }
          // The projekktor instance object for the player with jcarousel.
          projekktor('#' + id, jsoptions, function(player) {
            // Listener to get item info from the player.
            player.addListener('item', readyListener);
            // Click function to play a video with the carousel.
            $('#' + jcarouselId).children('li.jcarousel-item').each(function(index) {
              $(this).css('cursor','pointer').click(function(){
                player.setActiveItem(index);
                return false;
              })
            });
          });
        }
        // Else generate just a player object for the instance (no jcarousel).
        else {
          projekktor('#' + id, jsoptions);
        }
      });
    }
    // Optionally attaches projekktor to generic video tag.
    if ($(settings.projekktor.presetAll).length) {
      $('video', context).once('projekktor', function() {
        var jsoptions = settings.projekktor.presetAll;
        projekktor('video', jsoptions);
      });
    }
  }
};

})(jQuery);
