/**
 * @file
 * Renders ad blocks.
 */

(function ($) {


/**
 * Here we render our ad blocks via the jquery.googleDfp.plugin.js
 */
Drupal.behaviors.googleDfpRender = {
  attach: function (context, settings) {

    if (Drupal.settings.googleDfps !== undefined) {

      $('.ad-placement', context).once(function(){
        id = $(this).attr('id');
        opts = Drupal.settings.googleDfps[id];
        if ($(this).googleDfp({
          screenSize: opts.screenSize,
          placement: opts.placement
        }, Drupal.settings.googleDfpBreakpoints || [])) {
          $(this).show();
        }
      })
    }

  }
};

}(jQuery));
