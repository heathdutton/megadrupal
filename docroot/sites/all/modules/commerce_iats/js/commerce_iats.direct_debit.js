/**
 * @file
 * Commerce iATS Direct Debit JavaScript functionality.
 */

(function ($) {

  Drupal.behaviors.commerce_iats_direct_debit = {
    attach: function(context, settings) {
      $('input#edit-print', context).unbind('click').bind('click', function() {
        window.print();
        return false;
      });
    }
  }

})(jQuery);
