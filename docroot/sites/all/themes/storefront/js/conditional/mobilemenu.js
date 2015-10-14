(function ($) {
  // Create the dropdown base for Main Menu
  var header = undefined;
  var menu = undefined;
  var menuButton = undefined;

  $(document).ready(function(){
    header = $("#primary-menu-bar");
    menu = $("#primary-menu-bar nav ul");
    menuButton = $("<div class='mobile-menu clearfix'><a id='menu-toggle' href='#'>Menu</a></div>");
    header.prepend(menuButton);
    menuButton.click(showMenu);
  })

  function showMenu (event) {
    if (menu.is(":visible"))
      $('.mobile-menu a').removeClass('menu-down'),
      menu.slideUp({complete:function(){$(this).css('display','')}});
    else
      $('.mobile-menu a').addClass('menu-down'),
      menu.slideDown();
  }

})(jQuery);
