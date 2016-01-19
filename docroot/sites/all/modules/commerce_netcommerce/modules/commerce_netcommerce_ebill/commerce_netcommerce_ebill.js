(function($) {

  /**
   * Automatically submit the payment redirect form.
   */
  Drupal.behaviors.commerceNetCommerceEbillRedirect = {
    attach: function (context, settings) {
      $('form#netcommerce-ebill-schedule-cancel-redirect', context).submit();
    }
  }
})(jQuery);
