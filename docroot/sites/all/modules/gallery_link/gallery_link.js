(function ($) {

  Drupal.behaviors.gallery_link = {
    attach: function (context, settings) {
      // We bind a click event to all items with a "data-gallery-id" attribute
      $('a[data-gallery-id]').bind('click', function(e) {
        currentGallery = Drupal.settings.gallery_link[$(this).attr('data-gallery-id')];
        // We access the gallery with the gallery id and put it into the fancybox
        $.fancybox(currentGallery['images'], currentGallery['options']);
        // We prevend the default behavior of the link
        e.preventDefault();
      });
    }
  }

})(jQuery);
 
