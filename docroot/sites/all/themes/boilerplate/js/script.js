/* Author: Dan Linn */
(function($) {
  $(window).resize(function(){
    if(!$(".mobileselect").length) {
      createMobileMenu();
    } else if ($(window).width()>=480) {
      $('#navigation ul').show();
      $('.mobileselect').hide();
    } else {
      $('#navigation ul').hide();
      $('.mobileselect').show();
    }
  });
  function createMobileMenu(){
    $('#navigation ul').mobileSelect({
      autoHide: true, // Hide the ul automatically
      defaultOption: "Navigation", // The default select option
      deviceWidth: 480, // The select will be added for screensizes smaller than this
      appendTo: '', // Used to place the drop-down in some location other than where the primary nav exists
      className: 'mobileselect', // The class name applied to the select element
      useWindowWidth: true // Use the width of the window instead of the width of the screen
    });
  }
  Drupal.behaviors.mobileMenu = {
    attach: function (context) {
      createMobileMenu();
    }
  }
})(jQuery);





