(function ($) {
  Drupal.TBSirate = Drupal.TBSirate || {};
  Drupal.TBSirate.top = false;
  Drupal.TBSirate.slideshowWrapperTop = false;
  Drupal.TBSirate.socialShareTop = false;
  Drupal.TBSirate.slideshowSize = false;
  Drupal.TBSirate.supportedScreens = [0.5, 479.5, 719.5, 959.5, 1049.5];
  
  Drupal.TBSirate.setEqualHeight = function(){
    var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
    if(windowWidth > Drupal.TBSirate.supportedScreens[2]){
      //	jQuery('key1, key2, key3, ...').equalHeightColumns();
      jQuery('#panel-fourth-wrapper .panel-column > .grid-inner').clearMinHeight();
      jQuery('#mass-bottom-wrapper .views-row.row-1 .grid-inner').clearMinHeight();
      jQuery('#mass-bottom-wrapper .views-row.row-2 .grid-inner').clearMinHeight();
      jQuery('#panel-first-wrapper .block-inner').clearMinHeight();
      jQuery('#panel-third-wrapper .block-inner').clearMinHeight();
	  jQuery('#panel-fourth-wrapper .panel-column > .grid-inner').matchHeights();
      jQuery('#mass-bottom-wrapper .views-row.row-1 .grid-inner').matchHeights();
      jQuery('#mass-bottom-wrapper .views-row.row-2 .grid-inner').matchHeights();
      jQuery('#panel-first-wrapper .block-inner').matchHeights();
      jQuery('#panel-third-wrapper .block-inner').matchHeights();
    }else{
      jQuery('#panel-fourth-wrapper .panel-column > .grid-inner').clearMinHeight();
      jQuery('#mass-bottom-wrapper .views-row.row-1 .grid-inner').clearMinHeight();
      jQuery('#mass-bottom-wrapper .views-row.row-2 .grid-inner').clearMinHeight();
      jQuery('#panel-first-wrapper .block-inner').clearMinHeight();
      jQuery('#panel-third-wrapper .block-inner').clearMinHeight();
    }
  }
  
  Drupal.TBSirate.makeMovableSocialShare = function(){
    if(jQuery("#page").offset()) {
      Drupal.TBSirate.top = jQuery("#page").offset().top;
      Drupal.TBSirate.slideshowWrapperTop = jQuery("#slideshow-wrapper").offset() ? jQuery("#slideshow-wrapper").offset().top : jQuery("#main-wrapper").offset().top;
      Drupal.TBSirate.socialShareTop = jQuery("#social-share-wrapper").offset().top;
      var div = jQuery('#header-wrapper div.container');
	  var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
	  if (windowWidth > 1048) {
	    jQuery('#social-share-wrapper').css({'left': div.width() + div.offset().left + 1 + "px"});
      }
	  else {
	    jQuery('#social-share-wrapper').css({'left': div.width() + div.offset().left - 27 + "px"});
	  }
      Drupal.TBSirate.scrollPage();
    }
  }
  
  Drupal.TBSirate.scrollPage = function() {
    var current_top = jQuery(document).scrollTop();
    Drupal.TBSirate.top = jQuery("#page").offset().top;
    Drupal.TBSirate.slideshowWrapperTop = jQuery("#slideshow-wrapper").offset() ? jQuery("#slideshow-wrapper").offset().top : jQuery("#main-wrapper").offset().top;
    if(current_top + Drupal.TBSirate.top < Drupal.TBSirate.slideshowWrapperTop) {
      jQuery('#social-share-wrapper').css({'top': Drupal.TBSirate.slideshowWrapperTop + "px"});
    }
    else {
      jQuery('#social-share-wrapper').css({'top': (current_top + Drupal.TBSirate.top) + "px"});
    }
    var div = jQuery('#header-wrapper div.container');
    var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
	if (windowWidth > 1048) {
	  jQuery('#social-share-wrapper').css({'left': div.width() + div.offset().left + 1 + "px"});
    }
	else {
	  jQuery('#social-share-wrapper').css({'left': div.width() + div.offset().left - 27 + "px"});
	}
  }
  
  Drupal.TBSirate.setSlideshowHeight = function() {
    var imgs = $('#slideshow-wrapper .view .views-field img')
    if(imgs.length) {
      if(!Drupal.TBSirate.slideshowSize) {
        var img = new Image();
        img.src = $(imgs[0]).attr('src');
        Drupal.TBSirate.getImageSize(img);
        setTimeout(Drupal.TBSirate.setSlideshowHeight, 200)
        return;
      }

      var page_width = $('#page').width();
      if(page_width < Drupal.TBSirate.supportedScreens[3]) {
        var width = Drupal.TBSirate.slideshowSize.width;
        var height = Drupal.TBSirate.slideshowSize.height;
        var new_height = Math.floor(page_width * height / width);
        $('#slideshow-wrapper .view .views-field img, #slideshow-wrapper .view .views_slideshow_cycle_main, #slideshow-wrapper .views-slideshow-cycle-main-frame-row, #slideshow-wrapper .views-slideshow-cycle-main-frame').css({height: new_height + "px"});
      }
      else {
        $('#slideshow-wrapper .view .views-field img, #slideshow-wrapper .view .views_slideshow_cycle_main, #slideshow-wrapper .views-slideshow-cycle-main-frame-row, #slideshow-wrapper .views-slideshow-cycle-main-frame').css({height: Drupal.TBSirate.slideshowSize.height + "px"});
      }
      $('#slideshow-wrapper .views-slideshow-cycle-main-frame').cycle('destroy');
      $('#slideshow-wrapper .views-slideshow-cycle-main-frame').cycle();
    }
  }

  Drupal.TBSirate.getImageSize = function(img) {
    if(img.height == 0) {
      setTimeout(function() {
          Drupal.TBSirate.getImageSize(img);
      }, 200);
      return;
    }
    if(!Drupal.TBSirate.slideshowSize) {
      Drupal.TBSirate.slideshowSize = {height: img.height, width: img.width};
    }
  }

  Drupal.TBSirate.setGalleryFormatterHeight = function(){
    $('#block-system-main .gallery-slides').height($('#block-system-main .gallery-slide').eq(0).height() - 15);
  }
  
  Drupal.behaviors.actionTBSirate = {
    attach: function (context) {     
      Drupal.TBSirate.setEqualHeight();
      Drupal.TBSirate.makeMovableSocialShare();
      Drupal.TBSirate.setGalleryFormatterHeight();
      Drupal.TBSirate.setSlideshowHeight();
      jQuery(window).scroll(Drupal.TBSirate.scrollPage);
	  $(window).load(function() {
      	Drupal.TBSirate.toolbar = $('#toolbar').length ? $("#toolbar") : false;
        jQuery(window).resize(function(){
          $('body').css({'padding-top': Drupal.TBSirate.toolbar ? (Drupal.TBSirate.toolbar.height() - (Drupal.TBSirate.IE8 ? 10 : 0)) : 0});
          Drupal.TBSirate.scrollPage();
          Drupal.TBSirate.setSlideshowHeight();
          Drupal.TBSirate.setEqualHeight();
          Drupal.TBSirate.setGalleryFormatterHeight();
        });
	  });
    }
  };
  
})(jQuery);