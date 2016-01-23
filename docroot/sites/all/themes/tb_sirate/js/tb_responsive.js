(function ($) {
  Drupal.TBResponsive = Drupal.TBResponsive || {};
  Drupal.TBResponsive.supportedScreens = [0.5, 479.5, 719.5, 959.5, 1049.5];
  Drupal.TBResponsive.oldWindowWidth = 0;
  Drupal.TBResponsive.updateResponsiveMenu = function(){
    var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
    if(windowWidth < Drupal.TBResponsive.supportedScreens[3]){
      $('.region-menu-bar').hide();
      $('.responsive-menu-button').show();
    }
    else{
      $('.responsive-menu-button').hide();
      $('.region-menu-bar').show();
    }
  }

  Drupal.TBResponsive.initResponsiveMenu = function(){
    Drupal.TBResponsive.updateResponsiveMenu();
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

  Drupal.behaviors.actionTBResponsive = {
    attach: function (context) {
      if($('#menu-bar-wrapper .tb-megamenu').length) return;
      Drupal.TBResponsive.initResponsiveMenu();
      $(window).resize(function(){
        var windowWidth = window.innerWidth ? window.innerWidth : $(window).width();
        if(windowWidth != Drupal.TBResponsive.oldWindowWidth){
          Drupal.TBResponsive.updateResponsiveMenu();
          Drupal.TBResponsive.oldWindowWidth = windowWidth;
        }
      });
    }
  };
})(jQuery);
