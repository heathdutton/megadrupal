
(function ($) {

$(document).ready(function () {

  // limit the height of main content (scrollbars added with css)
/* needs work for mobile compatibility
  $('#region-content').css('max-height', Math.max( $(window).height(), $('#region-sidebar-first').height(), $('#region-sidebar-second').height() ));
  $(window).resize(function() {
    $('#region-content').css('max-height', Math.max( $(window).height(), $('#region-sidebar-first').height(), $('#region-sidebar-second').height() ));
  });
*/
  
  /* Fixes */
  // number fields should stay ltr
  $('.field-type-number-integer .field-item').css('direction', 'ltr');
  
});

})(jQuery);
