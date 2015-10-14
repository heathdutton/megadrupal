jQuery(document).ready(function ($) {
  // Move the color selector as user scroll down
  $(window).scroll(function() {
    if ($('#color_scheme_form:visible').length == 0) {
      return;
    }
    var containerHeight = $('.color-form').outerHeight(true),
        previewPos = $('#palette').position(),
        winPos = $(window).scrollTop();

    if (winPos > containerHeight) {
      winPos = containerHeight;
    } 
    
    else if (winPos < previewPos.top) {
      winPos = previewPos.top;
    }
    
    $('#placeholder').css('top', winPos);  
  }); 
});