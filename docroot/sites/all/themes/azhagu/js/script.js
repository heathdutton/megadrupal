/**
 * @file
 * Javascript for azhagu theme.
 */
(function ($) {

Drupal.behaviors.azhagu = {
  attach: function (context) {
    // Adding new DIV for the mobile device menus.
    $('#navigation').prepend('<div id="menu-icon">Menu</div>');
    $("#menu-icon").bind("click", function(){
      $("#navigation ul.menu").slideToggle('slow');
      $(this).toggleClass("active");
    });
  }
};

})(jQuery);
