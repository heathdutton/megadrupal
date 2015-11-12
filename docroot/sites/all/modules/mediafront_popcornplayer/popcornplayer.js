(function($) {

  /**
   * Launch player, insert speakers and convert chapter links
   */
  Drupal.behaviors.popcornPlayer = {
    attach : function(context, settings) {
      $.each(settings.popcornPlayer || {}, function(index, options) {
        //console.log(options.speakers);
        //alert(options.id);
        // process all media youtube cue field
        $('#media-popcornjs-' + options.id, context).once('popcornPlayer', function() {
          // initialize popcorn
          var video = $(this);
          var popcorn = Popcorn.smart('#media-popcornjs-' + options.id, options.url);
          options.popcorn = popcorn;
          for (var i in options.classactivator) {
            var c = options.classactivator[i];
            popcorn.classactivator({
              start : c.start,
              end : c.end,
              target : c.target,
              classname : c.classname,
            });
          }
          popcorn.play();
        });

        // attach chapter to popcorn
        $('[data-popcorn-chapter-source*="' + options.id + '"]', context).once('popcornPlayer', function() {
          var chapter = $(this);
          // jump to the cue point on click
          chapter.on('click', function(e) {
            e.preventDefault();
            var start = chapter.data('popcorn-start');
            var p = options.popcorn;

            // do the jump & play
            p.currentTime(start).play();
          });
        });
      });

      // Adding script for scrollbar customization with enscroll
      //$('.scrollbox').enscroll({});
    }
  };

})(jQuery);
