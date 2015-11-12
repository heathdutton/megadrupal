(function ($) {
  Drupal.Responsive = Drupal.Responsive || {};
  Drupal.Responsive.supportedScreens = [0.5, 479.5, 719.5, 959.5, 1049.5];
  Drupal.Responsive.oldWindowWidth = 0;
  Drupal.Responsive.masonry_containers = [];
  Drupal.Responsive.masonry_widths = [];

  Drupal.Responsive.updateResponsiveMenu = function(){
    var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
    if ( $('#block-tb-megamenu-main-menu').length > 0 ) return;
    
    if(windowWidth < Drupal.Responsive.supportedScreens[3]){
      $('.region-menu-bar').css('display', 'none');
      $('.responsive-menu-button').css('display', 'block');
    }
    else{
      $('.responsive-menu-button').css('display', 'none');
      $('.region-menu-bar').css('display', 'block');
    }
  }

  Drupal.Responsive.initResponsiveMenu = function(){
    Drupal.Responsive.updateResponsiveMenu();
    $('.tb-main-menu-button').bind('click',function(e){
      var target = $('.region-menu-bar');
      if(target.css('display') == 'none') {
        window.scrollTo(0, 0);
        target.css({display: 'block'});
      }
      else {
        target.css({display: 'none'});
      }
    });
  }

  Drupal.Responsive.setSlideshowHeight = function() {
    var imgs = $('#slideshow-wrapper .view .views-field img')
    if(imgs.length) {
      if(!Drupal.Responsive.slideshowSize) {
        var img = new Image();
        img.src = $(imgs[0]).attr('src');
        Drupal.Responsive.getImageSize(img);
        setTimeout(Drupal.Responsive.setSlideshowHeight, 200)
        return;
      }

      var page_width = $('#page').width();
      if(page_width < Drupal.Responsive.supportedScreens[3]) {
        var width = Drupal.Responsive.slideshowSize.width;
        var height = Drupal.Responsive.slideshowSize.height;
        var new_height = Math.floor(page_width * height / width);
        $('#slideshow-wrapper .view .views-field img, #slideshow-wrapper .view .views_slideshow_cycle_main, #slideshow-wrapper .views-slideshow-cycle-main-frame-row, #slideshow-wrapper .views-slideshow-cycle-main-frame').css({height: new_height + "px"});
      }
      else {
        $('#slideshow-wrapper .view .views-field img, #slideshow-wrapper .view .views_slideshow_cycle_main, #slideshow-wrapper .views-slideshow-cycle-main-frame-row, #slideshow-wrapper .views-slideshow-cycle-main-frame').css({height: Drupal.Responsive.slideshowSize.height + "px"});
      }
      $('#slideshow-wrapper .views-slideshow-cycle-main-frame').cycle('destroy');
      $('#slideshow-wrapper .views-slideshow-cycle-main-frame').cycle();
    }
  }

  Drupal.Responsive.getImageSize = function(img) {
    if(img.height == 0) {
      setTimeout(function() {
        Drupal.Responsive.getImageSize(img);
      }, 200);
      return;
    }
    if(!Drupal.Responsive.slideshowSize) {
      Drupal.Responsive.slideshowSize = {height: img.height, width: img.width};
    }
  }
  
  Drupal.behaviors.actionResponsive = {
    attach: function (context) {
      $(window).load(function() {
        Drupal.Responsive.initResponsiveMenu();
        //Drupal.Responsive.initMasonry();
        $(window).resize(function(){
          var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
          if(windowWidth != Drupal.Responsive.oldWindowWidth){
            Drupal.Responsive.updateResponsiveMenu();
            Drupal.Responsive.oldWindowWidth = windowWidth;
          }
          Drupal.Responsive.setSlideshowHeight();
		  /*
          for($i = 0; $i < Drupal.Responsive.masonry_containers.length; $i ++) {
        	Drupal.Responsive.updateColumnWidth($i);        	  
            $(Drupal.Responsive.masonry_containers).masonry('reload');
          }
		  */
        });
      });
    }
  };
  
  Drupal.Responsive.updateColumnWidth = function(index) {
	container = $(Drupal.Responsive.masonry_containers[index]);
	container.addClass('masonry-reload');
    var container_width = container.width();
    console.log(container_width);
    var number_column = Math.round(container_width / Drupal.Responsive.masonry_widths[index]);
    var column_width = Math.floor(container_width / number_column);
    container.find('.block:not(.block-quicktabs .block)').css({
      width: column_width + "px"
    });
    container.data('basewidth', column_width);
    return column_width;
  }

  Drupal.Responsive.updateMasonryWidth = function(index, reload) {    
	container = $(Drupal.Responsive.masonry_containers[index]);
    container.addClass('masonry-reload');
    var options = {
      itemSelector: '.block:not(.block-quicktabs .block)',
      isResizable: false,
      isAnimated: false,
      columnWidth: function() {
    	container.removeClass('masonry-reload');
        var basewidth = container.data('basewidth');
        if(basewidth == undefined) {
          return Drupal.Responsive.updateColumnWidth(index);
        }
    	else {
          return basewidth;
        }
      }
    }
    container.masonry(options);
  }

  Drupal.Responsive.initMasonry = function() {
    var container = $('.sidebar .region');
    if(container.length) {
      for($i = 0; $i < container.length; $i ++) {
        Drupal.Responsive.masonry_containers.push(container[$i]);
        Drupal.Responsive.masonry_widths.push(300);
        Drupal.Responsive.updateMasonryWidth($i, true);
      }
    }
  };
  
  
})(jQuery);
