/**
 * Theme Enable Sticky Navigation
 * @author Pitabas Behera
*/
jQuery(function() {
  function bootPressStickyHeader() {
    if (jQuery(window).scrollTop() >= origOffsetY) {
      jQuery('#main-navigation').addClass('sticky');
      jQuery('#main-navigation.sticky').css({'top' : toolbarHeight+'px'});
    } else {
      jQuery('#main-navigation').removeClass('sticky');
      jQuery('#main-navigation').css({'top' : 'auto'});
    }
  }
  function adminToolbarHeight(toolbarSelector) {
    if(jQuery(toolbarSelector).length) {
      var toolbarHeight = jQuery(toolbarSelector).outerHeight(true);
    } else {
      var toolbarHeight = 0;
    }
    return toolbarHeight;
  }
  jQuery(window).on("load resize scroll",function(e){
    toolbarHeight = adminToolbarHeight("#toolbar");
    jQuery('#main-navigation.sticky').css({'top' : toolbarHeight+'px'});
  });    
  toolbarHeight = adminToolbarHeight("#toolbar");
  var stickyMenu = jQuery('#main-navigation');
  var origOffsetY = (stickyMenu.offset().top) - (toolbarHeight);
  document.onscroll = bootPressStickyHeader;
});