(function ($) {
  Drupal.behaviors.er_plus = {
    attach: function (context, settings) {
      $('.form-autocomplete').change(function() {
        var fValue = $(this).attr("view_mode_id");
        console.log(fValue);
        $('#view-mode-' + fValue).trigger('test');
      });

    }
  }
})(jQuery);