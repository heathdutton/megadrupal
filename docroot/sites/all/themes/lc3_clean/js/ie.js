/* vim: set ts=2 sw=2 sts=2 et: */

/**
 * Hacks and tweaks for IE6 and IE7 browsers
 */

/**
 * jQuery fix of "IE Z-Index bug" for ".menu-tree" menus
 *
 * See more info on this bug at http://css-discuss.incutio.com/?page=OverlappingAndZIndex
 */
$(function() {
  var zIndexNumber = 999999;
  $('.menu li').each(function() {
    $(this).css('zIndex', zIndexNumber--);
  });
});

/**
 * Emulation of CSS "border-spacing" for IE6 & IE7
 */
$(function() {
  $('table.products-grid').attr('cellspacing', '20px');
});

