(function ($) {
  Drupal.Responsive = Drupal.Responsive || {};
  Drupal.Responsive.screens = [0.5, 479.5, 768.5, 979.5, 1049.5];
  Drupal.Responsive.mobileMaxWidth = 979.5;
  Drupal.Responsive.currentSize = false;
  Drupal.Responsive.IE8 = $.browser.msie && parseInt($.browser.version, 10) === 8;
  Drupal.Responsive.toolbar = false;
  
  Drupal.behaviors.actionResponsive = {
    attach: function (context) {
      $(window).load(function() {
        $('.btn-btt').smoothScroll({speed: 800});
        $(window).scroll(function() {
          if($(window).scrollTop() > 200) {
            $('.btn-btt').show();
          }
          else {
            $('.btn-btt').hide();
          }
        }).resize(function(){
          if($(window).scrollTop() > 200) {
            $('.btn-btt').show();
          }
          else {
            $('.btn-btt').hide();
          }
        });              
        Drupal.Responsive.toolbar = $('#toolbar').length ? $("#toolbar") : false;
        Drupal.Responsive.initResponsiveMenu();
        Drupal.Responsive.currentSize = $(window).width();
        Drupal.Responsive.updateResponsiveMenu(Drupal.Responsive.currentSize > Drupal.Responsive.mobileMaxWidth);
        $(window).resize(function(){
          $('body').css({'padding-top': Drupal.Responsive.toolbar ? (Drupal.Responsive.toolbar.height() - (Drupal.Responsive.IE8 ? 10 : 0)) : 0});
          var new_size = $(window).width();
          if((new_size - Drupal.Responsive.mobileMaxWidth) * (Drupal.Responsive.currentSize - Drupal.Responsive.mobileMaxWidth) < 0) {
            Drupal.Responsive.updateResponsiveMenu(new_size > Drupal.Responsive.mobileMaxWidth);
            Drupal.Responsive.currentSize = new_size;
          }
        });
      });
    }
  };
  
  Drupal.Responsive.updateResponsiveMenu = function(show_menu){
    if(show_menu) {
      $('.region-menu-bar').show();
      $('.responsive-menu-button').hide();
    } 
    else {
      $('.region-menu-bar').hide();
      $('.responsive-menu-button').show();
    }
  }

  Drupal.Responsive.initResponsiveMenu = function(){
    $('.responsive-menu-button').bind('click',function(e){
      var target = $('.region-menu-bar');
      if(target.css('display') == 'none') {
        target.css({display: 'block'});
      }
      else {
        target.css({display: 'none'});
      }
    });
    Drupal.Responsive.updateResponsiveMenu();
  }
})(jQuery);
