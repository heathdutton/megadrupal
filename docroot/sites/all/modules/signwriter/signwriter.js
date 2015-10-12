/* $Id $ */
(function ($) {

  $(document).ready(function() {
    // Add signwriter-hover class to the menus that signwriter is being used for.
    $('img.signwriter-hover')
          .parents('ul.menu')
          .addClass('signwriter-hover');
  
    $('img.signwriter-hover')
  
    set_signwriter_active_top();
  
    // Add over class so it will work in IE.
    $('ul.menu.signwriter-hover li a:not(.active)').hover(function() {
      $(this).addClass('over');
      set_signwriter_over_top(this);
    }, function() {
      $(this).removeClass('over');
      $(this).children('img.signwriter-hover').css('top', 0);
    });
  
    // Set css top values for hover images based on the height of the image
    function set_signwriter_over_top(elem) {
      var image_height =  $(elem).height();
      var over_top = 0 - image_height;
  
      $(elem).children('img.signwriter-hover').css('top', over_top);
    }
  
    // Set css top values for active images based on the height of the image
    function set_signwriter_active_top() {
      var image_height =  $('a.active img.signwriter-hover').height();
      var active_top = 0 - ((image_height / 3) * 2);
  
      $('ul.menu.signwriter-hover').find('a.active img.signwriter-hover').css('top', active_top);
    }
  });

})(jQuery);
