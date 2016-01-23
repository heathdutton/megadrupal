/*
 * ##### Sasson - Advanced Drupal Theming. #####
 *
 * This script will add a draggable overlay image you can lay over your HTML
 * for easy visual comparison.
 * Image source and opacity are set via theme settings form
 *
 */

(function($) {

  Drupal.behaviors.showOverlay = {
    attach: function(context, settings) {

      $('body.with-overlay').once('overlay-image').each(function() {
        var body = $(this);
        var overlay = $('<div id="overlay-image"><img src="'+ Drupal.settings.sasson['overlay_url'] +'"/></div>');
        var overlayToggle = $('<div class="toggle-switch toggle-overlay off" ><div>' + Drupal.t('Overlay') + '</div></div>');
        body.append(overlay);
        body.append(overlayToggle);
        overlay.css({
          'opacity': Drupal.settings.sasson['overlay_opacity'],
          'display': 'none',
          'position': 'absolute',
          'z-index': 99,
          'text-align': 'center',
          'top': $('.region-page-top').position() ? $('.region-page-top').position().top : 0,
          'left': '50%',
          'cursor': 'move'
        });
        overlayToggle.css({
          'top': '90px'
        });
        overlayToggle.click(function() {
          $('body').toggleClass('show-overlay');
          overlay.fadeToggle();
          var pull = overlay.find('img').width() / -2 + "px";
          overlay.css("marginLeft", pull);
          $(this).toggleClass("off");
        });
        overlay.draggable();

        // Move overlay with arrows.
        $(document).keydown(function(e) {
          if ($('body').is('.show-overlay')) {

            switch(e.which) {
              case 37: // left
                overlay.animate({'left': '-=1'}, 10);
              break;

              case 38: // up
                overlay.animate({'top': '-=1'}, 10);
              break;

              case 39: // right
                overlay.animate({'left': '+=1'}, 10);
              break;

              case 40: // down
                overlay.animate({'top': '+=1'}, 10);
              break;

              default: return;
            }
            e.preventDefault();
          }
        });

      });

    }
  };

})(jQuery);
