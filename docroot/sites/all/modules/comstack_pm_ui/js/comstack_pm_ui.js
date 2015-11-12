(function($) {
  Drupal.behaviors.comstack_pm_ui = {
    attach: function(context, settings) {
      if (typeof Drupal.settings.comstack_pm_ui != 'undefined' && typeof Drupal.settings.comstack_pm_ui.tooltips != 'undefined' && $.isFunction($.fn.tooltip)) {
        $('[data-toggle="tooltip"]', context).tooltip();
      }
    }
  };
})(jQuery);
