/**
 * @file
 * A JavaScript file for the simple workflow module.
 *
 */

(function ($) {

  /*
   * Add checkbox trigger on node edit form.
   */
  Drupal.behaviors.pureWorflow = {
    attach: function (context, settings) {
      // When published is checked, ready should be checked.
      $('input[name=status]', context).click(function() {
        var ready_checkbox = $("input[name=ready]");
        // If ready is not checked, checked.
        if (!ready_checkbox.attr("checked")) {
          ready_checkbox.attr("checked", true);
        }
      });
    }
  };

})(jQuery);
