/**
 * @file
 * Required functions to use the Pirobox.
 *
 * Extend the Pirobox jQuery plugin and fix any CSS problems.
 */

(function ($) {
  Drupal.behaviors.initPirobox = {
    attach: function(context, settings) {
      // Add class .piro_overlay (with comma) if you want overlay click close Pirobox.
      var piroClose = '.piro_close';
      if (settings.pirobox.overlayClose == 1) {
        piroClose = '.piro_close, .piro_overlay';
      }
      // Enable/disable slideshow function; true = slideshow on, false = slideshow off
      var piroSlideshow = false;
      if (settings.pirobox.slideshow == 1) {
        piroSlideshow = true;
      }
      // Which image effect is used.
      // The default effect is 'fade in' and can changed to 'blur gauss'.
      var imgBlur = false;
      if (settings.pirobox.imgEffect == 'blurgauss') {
        imgBlur = true;
      }
      // Use Pirobox settings with the configured module settings.
      $().piroBox({
        my_speed: settings.pirobox.animationSpeed,
        bg_alpha: settings.pirobox.overlayOpacity,
        slideShow: piroSlideshow,
        slideSpeed: settings.pirobox.slideshowSpeed,
        close_all: piroClose,
        linkToShow: settings.pirobox.linkToShow,
        current: Drupal.t('of'),
        previous: Drupal.t('previous image'),
        next: Drupal.t('next image'),
        close: Drupal.t('close'),
        slideshowStart: Drupal.t('play slideshow'),
        slideshowStop: Drupal.t('stop slideshow'),
        linkTo: Drupal.t('Open Image in a new window'),
        seemsError: Drupal.t('There seems to be an Error'),
        errorClose: Drupal.t('Close Pirobox'),
        imgBlur: imgBlur,
        imgBlurRadius: settings.pirobox.imgBlurRadius,
        blurTime: settings.pirobox.blurTime
      });
      // Use configured overlay background; image or color.
      var overlayStyleAttr = $('.piro_overlay').attr('style');
      if (settings.pirobox.overlayColor != 'transparent') {
        overlayStyleAttr = overlayStyleAttr + ' background-color: ' + settings.pirobox.overlayColor + ';';
      }
      else {
        overlayStyleAttr = overlayStyleAttr + ' background: url(/' + settings.pirobox.overlayBgImage + ') repeat 0 center transparent;';
      }
      $('.piro_overlay').attr('style', overlayStyleAttr);
      // Fix theme CSS, such as example the Bartik theme.
      $('.pirobox_content td').attr('style', 'padding: 0; border: 0');
      // Fix theme CSS, such as example the Bartik theme, if used gallery covering with link output.
      $('.file img').attr('style', 'margin: 0');
      // Ensure correct gallery covering if images displayed as link. Hide file icons.
      $('.gallerycover-no').prev('.file-icon').addClass('gallerycover-no');
      // Ensure that images are displayed side by side.
      $('.pirobox-item-sbs').parent('.field-item').addClass('pirobox-field-item-sbs');
    }
  };
})(jQuery);
