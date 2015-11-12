(function ($) {

  Drupal.behaviors.facetapi_jquerymenu = {
    attach: function(context) {
      $('.facetapi-facetapi-jquerymenu-links .facetapi-inactive').siblings('.parent.open').click();
    }
  }

})(jQuery);
