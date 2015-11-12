/**
 * @file
 * Instagram Feeds main js.
 */

(function($) {
  Drupal.behaviors.instagramFeedsEmptyImages = {
    attach: function() {
      $('.inst-image img').error(function() {
        var bgImg = 'url(' + $(this).data('empty') + ')';
        $(this).closest('.inst-image').find('.instf-standard-img').css('background-image', bgImg);
      });
      $('.inst-image img[src$="image_removed.svg"]').once(function() {
        var bgImg = 'url(' + $(this).data('empty') + ')';
        $(this).closest('.inst-image').find('.instf-standard-img').css('background-image', bgImg);
      });
    }
  };

  // Disabling title hidding for colorbox popups.
  Drupal.behaviors.initColorboxDefaultStyle = false;
  Drupal.behaviors.initColorboxPlainStyle = false;

})(jQuery);
