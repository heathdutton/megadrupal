/**
 * @file
 * Attaches the custom autocomplete behavior to all required fields.
 *
 * @see js/dsb_portal_autocomplete.autocomplete.js
 */

(function ($) {

// We use our own method for handling our autocomplete, but we still want to use
// most of core's functionality. This is why we override the core's attach
// handler, and substitute it with our own. We check each input. If it has the
// "dsb-portal-autocomplete" class, we use our own logic. Otherwise, we simply
// use the default, core one.

// Turn the core autocomplete behavior into a no-op.
Drupal.behaviors.autocomplete.attach = function() {};

Drupal.behaviors.dsbPortalAutocomplete = {
  attach: function (context, settings) {
    var acdb = [];
    $('input.autocomplete', context).once('autocomplete', function () {
      var $input = $('#' + this.id.substr(0, this.id.length - 13)),
          useCore = !$input.hasClass('dsb-portal-autocomplete');

      var uri = this.value;
      if (!acdb[uri]) {
        acdb[uri] = useCore ? new Drupal.ACDB(uri) : new DsbPortalAutocomplete.ACDB(uri);
      }

      $input
        .attr('autocomplete', 'OFF')
        .attr('aria-autocomplete', 'list');

      $($input[0].form).submit(Drupal.autocompleteSubmit);

      $input.parent()
        .attr('role', 'application')
        .append($('<span class="element-invisible" aria-live="assertive"></span>')
          .attr('id', $input.attr('id') + '-autocomplete-aria-live')
        );

      var jsAC = useCore ? new Drupal.jsAC($input, acdb[uri]) : new DsbPortalAutocomplete.jsAC($input, acdb[uri]);
    });
  }
};

})(jQuery);
