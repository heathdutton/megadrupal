/**
 * @file
 * A JavaScript file for Webform Auto Address.
 */

(function ($, Drupal, window, document, undefined) {
    $.fn.webformAutoAddressInject = function (arguments) {
      // Parse the JSON argument.
      var data = JSON.parse(arguments);

      // Set the street name when the user hasn't filled in anything yet. To
      // prevent the user adding to the text field, lose its (possible) focus.
      if (!$("input.ajax-callback-street-name--" + data.formKey).val()) {
        $("input.ajax-callback-street-name--" + data.formKey).blur();
        $("input.ajax-callback-street-name--" + data.formKey).val(data.street);
      }

      // Set the city when the user hasn't filled in anything yet. To
      // prevent the user adding to the text field, lose its (possible) focus.
      if (!$("input.ajax-callback-city--" + data.formKey).val()) {
        $("input.ajax-callback-city--" + data.formKey).blur();
        $("input.ajax-callback-city--" + data.formKey).val(data.city);
      }
    }
})(jQuery, Drupal, this, this.document);
