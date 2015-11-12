(function ($) {
  Drupal.Responsive = Drupal.Responsive || {};
  Drupal.Responsive.screens = [0.5, 479.5, 767.5, 979.5, 1049.5];

  Drupal.Responsive.updateResponsiveMenu = function(){
    var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
    if(windowWidth < Drupal.Responsive.screens[3]){
      $('.region-main-menu').hide();
      $('.responsive-menu-button').show();
    }
    else{
      $('.responsive-menu-button').hide();
      $('.region-main-menu').show();
    }
  }

  Drupal.Responsive.initResponsiveMenu = function(){
    $('.responsive-menu-button').bind('click',function(e){
      var target = $('.region-main-menu');
      if(target.css('display') == 'none') {
        target.css({display: 'block'});
      }
      else {
        target.css({display: 'none'});
      }
    });
    Drupal.Responsive.updateResponsiveMenu();
  }
  
  Drupal.Responsive.setSlideshowHeight = function() {
    var imgs = $('#block-views-slideshow-block .view .views-field img')
    if (imgs.length) {
    
      if(!Drupal.Responsive.slideshowSize) {
        var img = new Image();
        img.src = $(imgs[0]).attr('src');
        Drupal.Responsive.getImageSize(img);
        Drupal.Responsive.imagePaddingLeft  = parseInt(imgs.eq(0).css('padding-left'));
        Drupal.Responsive.imagePaddingRight = parseInt(imgs.eq(0).css('padding-right'));
        setTimeout(Drupal.Responsive.setSlideshowHeight, 200)
        return;
      }
      
      var page_width = $('#page').width();
      /*
      if(page_width < Drupal.Responsive.screens[3]) {
        var width = Drupal.Responsive.slideshowSize.width;
        var height = Drupal.Responsive.slideshowSize.height;
        var new_height = Math.floor(page_width * height / width);
        $('#block-views-slideshow-block .view .views-field img, #block-views-slideshow-block .view .views_slideshow_cycle_main, #block-views-slideshow-block .views-slideshow-cycle-main-frame-row, #block-views-slideshow-block .views-slideshow-cycle-main-frame, #block-views-slideshow-block > .block-inner').css({height: new_height + "px"});
      }
      else {
        $('#block-views-slideshow-block .view .views-field img, #block-views-slideshow-block .view .views_slideshow_cycle_main, #block-views-slideshow-block .views-slideshow-cycle-main-frame-row, #block-views-slideshow-block .views-slideshow-cycle-main-frame, #block-views-slideshow-block > .block-inner').css({height: Drupal.Responsive.slideshowSize.height + "px"});
      }
      $('#block-views-slideshow-block .views-slideshow-cycle-main-frame').cycle('destroy');
      $('#block-views-slideshow-block .views-slideshow-cycle-main-frame').cycle();
      */
      var w = $('#main-wrapper .container').width();
      $('#block-views-slideshow-block .views-slideshow-cycle-main-frame').css('height', 'auto');
      $('#block-views-slideshow-block .views-slideshow-cycle-main-frame-row, #block-views-slideshow-block .views-slideshow-cycle-main-frame').css('width', w);
      
      
      
      var tmp = w - Drupal.Responsive.imagePaddingLeft - Drupal.Responsive.imagePaddingRight;
      if (tmp < Drupal.Responsive.slideshowSize.width) {
        var width = Drupal.Responsive.slideshowSize.width;
        var height = Drupal.Responsive.slideshowSize.height;
        var newH = Math.floor(tmp * height / width);
        $('#block-views-slideshow-block .view .views-field img').css({
          width: tmp,
          height: newH
        });
      } else {
        $('#block-views-slideshow-block .view .views-field img').css({
          width: Drupal.Responsive.slideshowSize.width,
          height: Drupal.Responsive.slideshowSize.height
        });
      }
      
      var h = $('#block-views-slideshow-block .views-slideshow-cycle-main-frame-row').eq(0).height();
      $('#block-views-slideshow-block .views-slideshow-cycle-main-frame').css('height', h);
      
      $('#block-views-slideshow-block .views-slideshow-cycle-main-frame').cycle('destroy');
      $('#block-views-slideshow-block .views-slideshow-cycle-main-frame').cycle();
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
        Drupal.Responsive.setSlideshowHeight();
        $(window).resize(function(){
          Drupal.Responsive.updateResponsiveMenu();
          Drupal.Responsive.setSlideshowHeight();
        });
      });
    }
  };
})(jQuery);
