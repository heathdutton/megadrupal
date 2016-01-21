(function($) {
/*
 * Custom behavior
 */
Drupal.behaviors.jqueryuiFormattersAccordion = {
  attach: function(context, settings) {
    header = settings.jqueryuiFormattersAccordion.header;
    autoHeight = settings.jqueryuiFormattersAccordion.autoHeight;
    accordion = $('.jqueryui-formatters.accordion');
    $.each(accordion, function(key, object) {
      instance = $(this).attr('data-instance');
      $(this).accordion({collapsible:true, active:false, header: header[instance], autoHeight: autoHeight[instance]});
    });
  }
};

})(jQuery);
