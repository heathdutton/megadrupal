(function ($) {

  Drupal.behaviors.tmgmt_server = {
    attach: function (context, settings) {
      $("input:checkbox[name=tmgmt_server_auto_lc_sync]").click(function() {
        if ($(this).attr('checked')) {
          $(settings.tmgmt_server.lang_pairs_elements_names).each(function() {
            var capabilityCheckbox = $("." + this);
            capabilityCheckbox.attr('checked', 'checked');
            capabilityCheckbox.attr('disabled', 'disabled');
          });
        }
        else {
          $(settings.tmgmt_server.lang_pairs_elements_names).each(function() {
            $("." + this).removeAttr("disabled");
          });
        }
      });

    }
  };

})(jQuery);
