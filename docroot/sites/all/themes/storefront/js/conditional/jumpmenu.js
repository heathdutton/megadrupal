(function ($) {

  $(document).ready(function(){
    // Create the dropdown base for Main Menu
    $("<select class='menu' />").appendTo("#nav-wrapper nav");

    // Create default option "Go to..."
    $("<option />", {
      "selected": "selected",
      "value"   : "",
      "text"    : "Go to..."
    }).appendTo("#nav-wrapper nav select");

    // Populate dropdown with menu items
    $("#nav-wrapper nav a").each(function() {
      var el = $(this);
      $("<option />", {
        "value"   : el.attr("href"),
        "text"    : el.text()
      }).appendTo("#nav-wrapper nav select");
    });

    $("#nav-wrapper nav select").change(function() {
      window.location = $(this).find("option:selected").val();
    });

    // Create the dropdown base for Secondary Menu
    $("<select class='mobile-menu' />").appendTo("#secondary-content-wrapper nav .block-content");

    var menuTitle = $("#secondary-content-wrapper nav .block-title").html();
    // Create default option "Go to..."
    $("<option />", {
      "selected": "selected",
      "value"   : "",
      "text"    : menuTitle + "..."
    }).appendTo("#secondary-content-wrapper nav .block-content select");

    // Populate dropdown with menu items
    $("#secondary-content-wrapper nav .block-content a").each(function() {
      var el = $(this);
      $("<option />", {
        "value"   : el.attr("href"),
        "text"    : el.text()
      }).appendTo("#secondary-content-wrapper nav .block-content select");
    });

    $("#secondary-content-wrapper nav .block-content select").change(function() {
      window.location = $(this).find("option:selected").val();
    });
    $("#nav-wrapper nav").prepend("<a href='#block-commerce-cart-cart' class='cart-link'>Skip to Cart</a>");
  });
})(jQuery);
