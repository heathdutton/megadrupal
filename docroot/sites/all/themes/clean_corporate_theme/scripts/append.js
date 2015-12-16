(function($){
  $(window).on('resize', function() {
    $('#search-block-form').find('input[type="search"]').attr('placeholder','Search');
    if (window.innerWidth < 1000) {
      if($("body").hasClass("logged-in")) {
        if ($(".responsive-menu-wrapper > ul #search-block-form").length + $(".responsive-menu-wrapper > ul .username").length == 0) {
          $($("#search-block-form")).clone().prependTo($(".responsive-menu-wrapper > ul"));
          if (!$("#block-logintoboggan-logintoboggan-logged-in a").hasClass()) {
            $($("#block-logintoboggan-logintoboggan-logged-in .content a")).clone().appendTo($(".responsive-menu-wrapper > ul"));
          }
        }
      }
      else {
        if ($(".responsive-menu-wrapper > ul #search-block-form").length + $(".responsive-menu-wrapper > ul #user-login").length == 0) {
          $($("#search-block-form")).clone().prependTo($(".responsive-menu-wrapper > ul"));
          $("#user-login").clone().appendTo($(".responsive-menu-wrapper > ul"));
        }
      }
    }
    else {
      $("#block-superfish-1 h2.block-title img.site-logo").remove();
      $(".responsive-menu-wrapper > ul #search-block-form").remove();
      $(".responsive-menu-wrapper > ul .username").remove();
      $(".responsive-menu-wrapper > ul #user-login").remove();
      $(".responsive-menu-wrapper > ul > a").remove();
    }
  }
  );

  window.onload = function() {
    $('#search-block-form').find('input[type="search"]').attr('placeholder','Search');
    if (window.innerWidth < 1000) {
      if($("body").hasClass("logged-in")) {
        if ($(".responsive-menu-wrapper > ul #search-block-form").length + $(".responsive-menu-wrapper > ul .username").length == 0)  {        
          $($("#search-block-form")).clone().prependTo($(".responsive-menu-wrapper > ul"));
          if (!$("#block-logintoboggan-logintoboggan-logged-in a").hasClass()) {
            $($("#block-logintoboggan-logintoboggan-logged-in .content a")).clone().appendTo($(".responsive-menu-wrapper > ul"));
          };
        }
      }
      else {
        if ($(".responsive-menu-wrapper > ul #search-block-form").length + $(".responsive-menu-wrapper > ul #user-login").length == 0) {
          $($("#search-block-form")).clone().prependTo($(".responsive-menu-wrapper > ul"));
          $("#user-login").clone().appendTo($(".responsive-menu-wrapper > ul"));
        }
      }
    }
    else {
      $(".responsive-menu-wrapper > ul #search-block-form").remove();
      $(".responsive-menu-wrapper > ul .username").remove();
      $(".responsive-menu-wrapper > ul #user-login").remove();
      $(".responsive-menu-wrapper > ul > a").remove();
      $("#block-superfish-1 h2.block-title img.site-logo").remove();
    }
  };
})(jQuery);
