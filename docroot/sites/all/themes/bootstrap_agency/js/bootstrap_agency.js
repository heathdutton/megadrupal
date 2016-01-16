(function ($) {
Drupal.behaviors.agencyTheme = {
  attach: function (context) {
    // Replace cbpAnimatedHeader.js
    $(window).scroll(function(){
        if ($(this).scrollTop() > 200){
            $('.navbar-default').addClass('navbar-shrink');
        }
        else{
            $('.navbar-default').removeClass('navbar-shrink');
        }
    });

    // Hide the responive menu when clicking in a menu item
    $('#navbar .navbar-collapse ul li a').click(function() {
        $('#navbar button.navbar-toggle:visible').click();
    });
  }
};
})(jQuery);
