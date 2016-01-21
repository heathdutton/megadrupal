/**
 * Core behavior for E-Hawk.
 *
 * Test whether we're on the E-Hawk config form. If so, disable invalid options.
 */

Drupal.behaviors.ehawk = {
  attach: function (context, settings) {
    // Funny how nothing at https://www.drupal.org/node/304258 suggests this is necessary.
    $ = jQuery
    // Are we on edit form?
    if ($('#ehawk-form-configure', context).length) {
      // Disable any invalid options.
      if ($('input[name="disabled_options"]').length) {
        var disabledIDs = $('input[name="disabled_options"]').val().split(',');
        $('#edit-to-ehawk option').each(function() {
          if ($.inArray($(this).val(), disabledIDs) != -1) {
            $(this).attr('disabled','disabled').data('always-disabled', true);
          }
        });
      }

      // Disable options as they're selected, in each section.
      // Using 'bind' instead of 'on' since D7 comes with 1.4.4.
      $('select', context).bind('change', function() {

        var $this = $(this),
        $fieldset = $this.closest('fieldset'),
        selectedOptions = [],
        selectID = $this.attr('id');

        // Build array of currently-selected values.
        $('select', $fieldset).each(function() {
          if ($(this).val() != '') {
            selectedOptions.push($(this).val());
          }
        });

        // Now go thru all options and disable/enable based on what's selected/frozen.
        $('option', $fieldset).each(function(i) {
          var $thisopt = $(this);
          if ($thisopt.attr('value') == $thisopt.parent('select').val()) { return true; }
          if ($thisopt.attr('value') !== undefined && $.inArray($thisopt.attr('value'), selectedOptions) != -1) {
            $thisopt.attr('disabled','disabled');
          } else if ($thisopt.attr('value') && $thisopt.data('always-disabled') != true) {
            $thisopt.removeAttr('disabled');
          }
        });
      });

      $('#edit-to-ehawk select:first, #edit-from-ehawk select:first').trigger('change');

    }
  }
};
