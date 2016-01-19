(function($) {
/*
 * Custom behavior
 */
Drupal.behaviors.jqueryuiFormattersTabs = {
  attach: function(context, settings) {
    collapsible = settings.jqueryuiFormattersTabs.collapsible;
    tabs = $('.jqueryui-formatters.tabs');
    $.each(tabs, function(key, object) {
      instance = $(this).attr('data-instance');
      $(this).tabs({collapsible: collapsible[instance]});
    });
  }
};

})(jQuery);
