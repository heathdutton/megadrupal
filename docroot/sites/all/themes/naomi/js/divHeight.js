(function ($) {

  Drupal.behaviors.autoHeightAdjust = {
    attach: function (context, settings) {
      Drupal.eight.dynamicHeight();
      
      $(window).resize(function() {
        Drupal.eight.dynamicHeight();
      });  

    }
  };

//Initialize settings array.
Drupal.eight = {};
Drupal.eight.dynamicHeight = {};

Drupal.eight.dynamicHeight = function() {
  var windowHeight = $(window).height(),
    headerHeight = $('#section-header').height(),
    footerHeight = $('#section-footer').height();

  var contentHeight = windowHeight - ( headerHeight + footerHeight );
  $('#section-content').css('height',contentHeight);
  $('#openlayers-container-openlayers-map').css('height',contentHeight);
  $('#openlayers-map').css('height',contentHeight); 
}

})(jQuery);
