(function ($) {
  Drupal.behaviors.elevatedSearch = {
    attach: function(context, settings) {
      jQuery("th.views-field-status").click(function () {
        jQuery("#dialog").dialog({ modal: true });
      });
    }
  }
})(jQuery);