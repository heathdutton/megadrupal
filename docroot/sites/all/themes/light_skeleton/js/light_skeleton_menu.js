(function($) {

  // This will fire when document is ready:
  $(document).ready(function() {


    $("table.views-table").addClass("u-full-width");

    $(window).resize(function() {
      // This will fire each time the window is resized:
      if ($(window).width() <= 649) {


        $(".breadcrumb").prev('.navbar').remove();

        // Removing Menu Skeleton For Mobile
        $(".navbar").removeClass("navbar")
        $('nav#navigation').removeAttr('id');
        $(".navbar-list").removeClass("navbar-list")
        $(".navbar-item").removeClass("navbar-item")
        $(".navbar-link").removeClass("navbar-link")

        if (!$("#show-menu").hasClass("stop-exit")) {
          // This will prevent an infinite loop on adding the mobile menu.
          $("ul#menu").before('<label for="show-menu" class="show-menu">Menu</label> <input type="checkbox" id="show-menu" class="stop-exit" role="button">');

        }
      }

      if ($(window).width() >= 650) {

        // Breadcrumb Alignment
        $("div.breadcrumb").parent().css({
          "display": "inline"
        });

        // Edit Tabs Alignment
        $("ul.tabs").children().css({
          "display": "inline",
          "padding": "0px 9px"
        });


        // Adding Skeleton Menu
        $('nav').attr('id', 'navigation');
        $("nav#navigation.replaceme").addClass("navbar");
        $("ul#menu").addClass("navbar-list");
        $("ul.navbar-list li.leaf").addClass("navbar-item");
        $("li.navbar-item a.a-class").addClass("navbar-link");

        // Removing Mobile Menu
        $("ul#menu").prev('label.show-menu').remove();
        $("ul#menu").prev('input#show-menu').remove();


      }

    }).resize(); // This will simulate a resize to trigger the initial run.
  })

})(jQuery);
