/**
 * @file
 * Makes changes to the url path textfield.
 */

(function ($) {
  Drupal.behaviors.pathAliasPicker = {
    attach: function (context) {

      if ($.isFunction($.fn.chosen)) {
        $("select.path-alias-picker-list").chosen({search_contains: true, width: "95%"});
      }

      // Make sure all select elements are reset.
      setResetState();

      $('#edit-path-pathauto').change(function () {
        setResetState();
      });

      $("select.path-alias-picker-list").change(function () {

        appendPath = '';
        existingPath = $("#edit-path-alias").val();

        // If the existing path doesn't contain anything yet, leave out the
        // slash.
        if (existingPath.length !== 0) {
          appendPath = '/';
        }

        appendPath += $(this).val();

        $("#edit-path-alias").val(existingPath + appendPath);
        // Reset all relevant select elements.
        setResetState();
      });

      /**
       * Updates and reset all relevant select elements.
       */
      function setResetState() {
        if ($('#edit-path-pathauto').is(':checked')) {
          $("select.path-alias-picker-list").attr('disabled','disabled');
        }
        else {
          $("select.path-alias-picker-list").removeAttr('disabled');
        }
        $("select.path-alias-picker-list").val('').trigger("chosen:updated");
      }
    }
  };
})(jQuery);
