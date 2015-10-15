(function ($) {
  Drupal.behaviors.scrollbar = {
    attach: function (context, settings) {
      var $element = settings.scrollbar.element;
      $($element + ", .demo-class").jScrollPane({
        // jScrollpane needs clear true or false, not quoted text so we add this if statement
        showArrows: (settings.scrollbar.showArrows === 'true'),
        arrowScrollOnHover: (settings.scrollbar.arrowScrollOnHover === 'true'),
        mouseWheelSpeed: settings.scrollbar.mouseWheelSpeed,
        arrowButtonSpeed: settings.scrollbar.arrowButtonSpeed,
        arrowRepeatFreq: settings.scrollbar.arrowRepeatFreq,
        horizontialGutter: settings.scrollbar.horizontialGutter,
        verticalGutter: settings.scrollbar.verticalGutter,
        verticalDragMinHeight: settings.scrollbar.verticalDragMinHeight,
        verticalDragMaxHeight: settings.scrollbar.verticalDragMaxHeight,
        verticalDragMinWidth: settings.scrollbar.verticalDragMinWidth,
        verticalDragMaxWidth: settings.scrollbar.verticalDragMaxWidth,
        horizontialDragMinHeight: settings.scrollbar.horizontialDragMinHeight,
        horizontialDragMaxHeight: settings.scrollbar.horizontialDragMaxHeight,
        horizontialDragMinWidth: settings.scrollbar.horizontialDragMinWidth,
        horizontialDragMaxWidth: settings.scrollbar.horizontialDragMaxWidth,
        verticalArrowPositions: settings.scrollbar.verticalArrowPositions,
        horizontialArrowPositions: settings.scrollbar.horizontialArrowPositions,
        autoReinitialise: (settings.scrollbar.autoReinitialise === "true"),
        autoReinitialiseDelay: settings.scrollbar.autoReinitialiseDelay
      });
    // Uncomment the line below for debugging.
    // console.log(settings.scrollbar);
    }
  };
}(jQuery));
