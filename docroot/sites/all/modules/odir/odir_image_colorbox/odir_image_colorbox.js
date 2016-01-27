(function ($) {
Drupal.behaviors.odir_image_colorbox = {
  attach: function (context, settings) {
      if (typeof jQuery('a.odir_colorbox_slideshow').colorbox == 'function') { 
    	  jQuery('a.odir_colorbox_slideshow').colorbox({rel:"odir_colorbox_slideshow", scalePhotos: true, width: "100%", height: "100%"}); 
	  }
    }
  };
})(jQuery);
