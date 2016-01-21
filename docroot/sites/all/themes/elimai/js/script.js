/**
 * @file
 * Javascript for elimai theme.
 */

(function ($) {

Drupal.behaviors.elimai = {
  attach: function (context) {
    $('.carousel').carousel();
    // Removing the pagination item-list class for twitter bootstrap.
    $('.pagination > div').removeClass('item-list');

    // Making the content div 100% for no-sidebars pages.
    if($('body').hasClass('no-sidebars')) {
      $('#main-wrapper #content').removeClass('span8');
      $('#main-wrapper #content').addClass('span12');
    }
    else{
      $('#main-wrapper #content').removeClass('span12');
      $('#main-wrapper #content').addClass('span8');
    }
  }
}
})(jQuery);
