(function ($) {
  $(document).ready(function() {
    // The height of the content block when it's not expanded
    var adjustheight = 80;
    // The "more" link text
    var moreText = "More";
    // The "less" link text
    var lessText = "Less";
    
    $(".project-information .project-description").each(function(index) {
      if ($(this).height() > adjustheight)
      {
        $(this).css('height', adjustheight).css('overflow', 'hidden');
        $(this).parents(".project-information").append('<a href="#" class="show-more"></a>');
      }
    });
    
    $("a.show-more").text(moreText);
    
    $(".show-more").toggle(function() {
        $(this).parents("div:first").find(".project-description").css('height', 'auto').css('overflow', 'visible');
        $(this).text(lessText);
      }, function() {
        $(this).parents("div:first").find(".project-description").css('height', adjustheight).css('overflow', 'hidden');
        $(this).text(moreText);
    });
  });
})(jQuery);
