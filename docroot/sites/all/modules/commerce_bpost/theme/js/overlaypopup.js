(function ($) {
  Drupal.behaviors.bpostOverlay = {
    attach: function (context, settings) {
      // show popup when you click on the link
      $('#bpost-shipping-map .show-popup', context).once().click(function(e) {
        $('body').append('<div class="overlay-bg"></div>');
        e.preventDefault(); // disable normal link function so that it doesn't refresh the page
        var docHeight = $(window).height(); //grab the height of the page
        $('.overlay-bg').show();
        $('.overlay').show().css({'height' : docHeight}); //display your popup and set height to the page height
      });
      // hide popup when user clicks on close button
      $('.close-btn').click(function(e){
        $('.overlay').hide(); // hide the overlay
        $('.overlay-bg').remove();
      });
      // hides the popup if user clicks anywhere outside the container
      $('.overlay-bg, .overlay').click(function(e){
        e.preventDefault();
        $('.overlay').hide(); // hide the overlay
        $('.overlay-bg').remove();
      });
      // prevents the overlay from closing if user clicks inside the popup overlay
      $('.overlay-content').click(function(){
        return false;
      });
      $(document).keyup(function(e) {
        if (e.keyCode == 27) {
          $('.overlay').hide(); // hide the overlay
          $('.overlay-bg').remove();
        }
      });
    }
  };
})(jQuery);

