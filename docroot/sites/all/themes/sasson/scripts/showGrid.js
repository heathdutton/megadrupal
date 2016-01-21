/*
 * ##### Sasson - Advanced Drupal Theming. #####
 *
 * This script will add a grid background "image", for easy element aligning,
 * made with CSS3 and SASS to fit any grid.
 *
 */

(function($) {

  Drupal.behaviors.showGrid = {
    attach: function(context, settings) {

      $('body.grid-background').once('grid').each(function() {
        var body = $(this),
            gridToggle = $('<div class="toggle-switch toggle-grid off" ><div>' + Drupal.t('Grid') + '</div></div>');
        body.append(gridToggle);
        gridToggle.click(function() {
          $('body').toggleClass('grid-visible grid-hidden');
          $(this).toggleClass("off");
        });
      });

    }
  };

})(jQuery);
