(function ($) {
  // we need this because Drupal Commerce has ajax script on product display page
  // without this the bubbling will failed
  // @todo : convert this to proper jQuery plugin
  //         Utilize context and settings so multiple image field wont crash one another
  Drupal.behaviors.vtCommerceImage = { 
    attach: function(context, settings) {
      var option = settings.vtCommerceImage;
      var clouds = $('.cloud-zoom'),
          parents = $('.vt-commerce-image-wrapper'), // your parent element
          wrap = parents.find('.wrap'),
          clubthumbs = $('.cloud-thumbwrapper'),
          cHeight = clouds.find('img').outerHeight(true),
          cWidth = clouds.find('img').outerWidth(true),
          z = clouds.find('img').length;
      
      // initialization
      // This is very important steps because jQuery cloud js need the element
      // to be visible when intialized, so we cannot just plain hide the
      // large element, instead we use z-index to hide the element and 
      // show the first element on load
      
      // reorder the zindex so the first element will be always shown first
      // hide all wrap except the first one;
      // @todo Make this as a $.fn function
      if (option.zoom == 1) {
        clouds.children('img').load(function() {
          clouds.once('cloud-init').CloudZoom().each(function() {
            $(this).parent().css('z-index',  z ).hide();
            z--;       
          }).eq(0).parent().show();
        });
      }
      else {
        // Build manually the wrapper class and re-register
        // The clouds so the rest of the script won't break.
        parents.find('img').wrap('<div class="wrap" />').wrap('<div class="cloud-zoom" />');
        clouds = $('.cloud-zoom');
        clouds.once('cloud-init').each(function() {
          $(this).parent().css('z-index',  z ).hide();
          z--;       
        }).parent().eq(0).show();
      }

      
      // set the large image wrapper height
      $('.vt-commerce-image-large').height(cHeight);
      
    
      // only build thumbnail if more than 1 images found
      // @todo Make this as a $.fn function
      if (clouds.length > 1 && clubthumbs.length == 0) {
        
        // create the thumbnail wrapper so we can add thumbnail child here later on
        parents.append('<div class="cloud-thumbnail"/>');
        
        // cloning the large image to create a thumbnail navigation
        clouds.each(function() {
          var thumbClone = $(this).find('img').clone().width(option.previewWidth).height(option.previewHeight);
          thumbClone.appendTo('.cloud-thumbnail').addClass('thumbsmall').wrap('<div class="cloud-thumbwrapper" />');      
        });
        
        // declare the newly cloned thumb as a new variable
        var thumbSmall = $('.thumbsmall');
        
        // function for the thumbnail to show the right large image
        thumbSmall.each(function(){      
          $(this).unbind('click').click(function() {
            var index = thumbSmall.index(this);
            clouds.parent().stop().hide(0, function() { 
              clouds.eq(index).parent().show();
            }); 
            thumbSmall.removeClass('active');
            $(this).addClass('active');         
          });
        }).eq(0).addClass('active');

        // declare the thumbwrapper
        var thumbWrapper = $('.cloud-thumbwrapper');
        if (option.hoverZoom == 1) {
          thumbWrapper.width(option.previewWidth).height(option.previewHeight).imageEnlarge({
            multiple: option.multiple,
            mouseInSpeed : option.mouseOutSpeed,
            mouseOutSpeed : option.mouseInSpeed,
            zindex : option.zindex,
            topOffset : option.topOffset,
            leftOffset : option.leftOffset
          });
        }
      }
    
      // Integrate colorbox if requested.
      // @todo Make this as a $.fn function
      // If user wants colorbox with zooming
      if (option.colorbox == 1) {        
        $('a.cloud-zoom').colorbox(option);
        
        // emulate click if both colorbox and cloud zoom enabeld
        if (option.colorbox == 1 && option.zoom == 1) {
          $('.cloud-zoom-lens').live('click', function() {
              $(this).parent('a').click();
          });
          $('.mousetrap').live('click', function() {
            $(this).parent().find('a.cloud-zoom').click();
          });
        }
      }
    }
      
  };
})(jQuery);