(function($) {
  Drupal.behaviors.salsa_donate_page = {
    attach: function(context, settings) {
      $(".donation-amount-other", context).bind("focus", function() { $("#edit-amount-").click(); });
      $(".donation-amount-index", context).bind("click", function() { $(".donation-amount-other").attr("value", ""); });
    }
  }
}(jQuery));