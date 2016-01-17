(function($) {

Drupal.behaviors.actionCorporate = {
  attach: function (context) {
    Drupal.Corporate.centerSlideshowControls();
    
    $(window).resize(function() {
      Drupal.Corporate.centerSlideshowControls();
      Drupal.Corporate.updateResponsiveMenu();
      //Drupal.Corporate.currentWidth = window.innerWidth ? window.innerWidth : $(window).width();
      Drupal.Corporate.currentWidth = $(window).width();
    });
    
    $(window).load(function() {
      Drupal.Corporate.initResponsiveMenu();
      //Drupal.Corporate.currentWidth = window.innerWidth ? window.innerWidth : $(window).width();
      Drupal.Corporate.currentWidth = $(window).width();
      window.setTimeout(function() {
        $('#slideshow-wrapper .views_slideshow_cycle_main').children().css({width: "100%"});
      }, 100);
    });
  }
};

Drupal.Corporate = Drupal.Corporate || {};
Drupal.Corporate.supportedScreens = [0.5, 480.5, 767.5, 980.5, 1200.5, 1500.5, 1800.5];
Drupal.Corporate.currentScreen = "";
Drupal.Corporate.currentWidth = -1;

Drupal.Corporate.initResponsiveMenu = function() {
  Drupal.Corporate.updateResponsiveMenu();
  $('#main-menu-wrapper .responsive-menu-button').click(function(e){
    var target = $('#block-system-main-menu');
    if (target.length == 0) {
      target = $('#block-superfish-1');
    }
    if(target.css('display') == 'none') {
      window.scrollTo(0, 0);
      target.css({display: 'block'});
    }
    else {
      target.css({display: 'none'});
    }
  });
};

Drupal.Corporate.updateResponsiveMenu = function(){
  var target = $('#block-system-main-menu');
  if (target.length == 0) {
    target = $('#block-superfish-1');
  }
  
  if (target.css('display') == 'none') {
    $('#main-menu-wrapper .responsive-menu-button').show();
  } else {
    $('#main-menu-wrapper .responsive-menu-button').hide();
  }
}

Drupal.Corporate.centerSlideshowControls = function() {
  var w = $('#block-views-slideshow-block').width() - $('#widget_pager_bottom_slideshow-block').width();
  $('#widget_pager_bottom_slideshow-block').css('left', w/2 + 'px');
}

})(jQuery);
