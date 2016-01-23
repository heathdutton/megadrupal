/**
 * @file
 * message.js
 */

(function($) {
  Drupal.behaviors.CommerceCreditsTransaction = {
    attach: function(context, settings) {
      $(document).delegate('#page', 'commerceCreditsTransactionNotify', function(e, data) {

        if ($('#messages').length) {
          return false;
        }

        // Conditionally add drupal status message to page based on elements
        // that exist beginning with the breadcrumb per Drupal core themes.
        if ($('#breadcrumb').length) {
          $('#breadcrumb').after(data.message);
        }
        else if ($('#navigation').length) {
          $('#navigation').after(data.message);
        }
        else if ($('#header').length) {
          $('#header').after(data.message);
        }
        else {
          $('#page').append(data.message);
        }
      });
    }
  }
})(jQuery);
