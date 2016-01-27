/**
 * @file
 * Javascript functions for Flux Slider module.
 */

/**
 * USAGE: see https://github.com/joelambert/Flux-Slider.
 * transition options.
 *  2D options: bars, blinds, blocks, blocks2, concentric, dissolve, slide, warp, zip.
 *  3D options: bars3d, blinds3d, cube, tiles3d, turn.
 */

(function ($) {
   Drupal.behaviors.fluxSlider = {
     attach: function (context, settings) {
       // Get the settings.
       var flux_autoplay = Drupal.settings.flux_slider.flux_autoplay;
       var flux_autoplay_bool = (flux_autoplay == 1 ? true : false);

       var flux_transition = Drupal.settings.flux_slider.flux_transition;

       var flux_delay = Drupal.settings.flux_slider.flux_delay;

       var flux_pagination = Drupal.settings.flux_slider.flux_pagination;
       var flux_pagination_bool = (flux_pagination == 1 ? true : false);

       var flux_controls = Drupal.settings.flux_slider.flux_controls;
       var flux_controls_bool = (flux_controls == 1 ? true : false);

       var flux_captions = Drupal.settings.flux_slider.flux_captions;
       var flux_captions_bool = (flux_captions == 1 ? true : false);

       var flux_width = Drupal.settings.flux_slider.flux_width;
       var flux_height = Drupal.settings.flux_slider.flux_height;

       // Initialise the slider.
        var myFlux = new flux.slider('#slider', {
          autoplay: flux_autoplay_bool,
          transitions: [flux_transition],
          delay: flux_delay,
          pagination: flux_pagination_bool,
          controls: flux_controls_bool,
          captions: flux_captions_bool,
          width: flux_width,
          height: flux_height
        });
        // Adjust container height to cope with bug http://drupal.org/node/1880556.
        // Additional height required if pagination controls are shown.
        flux_container_height = (flux_pagination_bool ? (parseInt(flux_height, 10))+40 : flux_height);
        jQuery("div#slider").css({
          "height":flux_container_height,
          "overflow":"hidden"
        });
       }
     }
 }(jQuery));