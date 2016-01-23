(function ($) {
  Drupal.behaviors.jqueryUIRegionareaInit = { 
    attach: function() {
      $('#regionarea-workarea').once('region-area').regionarea();
   }
  };
  
  $.fn.regionarea = function (options) {
   var defaults = {
        wrapperClass: 'ui-corner-all ui-widget-content ui-state-active',
        areaClass: 'ui-corner-all ui-widget-content ui-state-default',
        areaHeaderClass : 'ui-corner-all ui-widget-header',
        regionClass: 'ui-corner-all ui-widget-content ui-state-hover',
        regionHeaderClass: 'ui-corner-all ui-widget-content ui-state-hover',
        blockClass: 'ui-corner-all ui-widget-content ui-state-default',
        configClass: 'ui-corner-all ui-widget-content',
        emptySlotClass: 'ui-corner-all ui-state-error',
        dragIconClass: 'ui-icon ui-icon-arrow-4 ui-helper-float-left',
        emptyIconClass: 'ui-icon ui-icon-info ui-helper-float-left'
      };
    options = $.extend(defaults, options);
    
    icons = jQuery('<span />').addClass(options.dragIconClass);
    emptyIcons = jQuery('<span />').addClass(options.emptyIconClass);
    
    return this.each(function () {
      self = $(this);
      self.find('.layout-parent').addClass(options.wrapperClass).Fhover();
      self.find('.element-wrapper-area').addClass(options.areaClass).Fhover()
          .children('.region-title').addClass(options.areaHeaderClass).Fhover();
      self.find('.element-wrapper-region').addClass(options.regionClass).Fhover();
      self.find('.element-wrapper-block').addClass(options.blockClass).Fhover();
      self.find('.region-configuration').addClass(options.configClass);
      self.find('.emptyslot').addClass(options.emptySlotClass)
          .prepend(emptyIcons);
      self.find('.region-title:not(h3)').prepend(icons);
    });
  };
})(jQuery);
