
"use strict";

(function(window, $) {

  /**
   * Notify users about new log entries.
   */
  Drupal.behaviors.amocrmWidgetInitPageLoad = {
    attach: function (context, settings) {
      var panels = $('.panel-amo-widget');

      if (panels.length) {
        panels.each(function() {
          var widget = $(this),
            widgetHead = widget.find('.panel-amo-widget-head'),
            widgetBody = widget.find('.panel-amo-widget-body');

          widgetHead.click(function(event){
            event.preventDefault();

            if (!widget.hasClass('panel-amo-widget-active')) {
              widget.addClass('panel-amo-widget-active');
            }
            else {
              widget.removeClass('panel-amo-widget-active');
            }
          });
        });
      }
    }
  };

})(window, jQuery);
