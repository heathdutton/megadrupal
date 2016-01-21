(function ($) {

/**
 * Calculate max height value in pixels from number of elements.
 */
/**
 *  Applies ddcl on all exposed filters that are marked as sexy.
 */
Drupal.behaviors.sexyExposed = {
  attach: function (context, settings) {
    $.each(settings.sexyExposed, function(id, settings) {
      // Set default options.
      var options = {
        icon: {},
        width: 150
      };

      // Get select list as a jQuery object.
      $sexyElement = $(id, context);

      // Adjust max height if maximum number of visible items is set.
      if (settings.sexyNumber > 0) {
        height = sexyExposedCalculateMaxHeight($sexyElement, settings.sexyNumber);
        options.maxDropHeight = height;
      }
      // Adjust width if set.
      if (settings.sexyWidth > 0) {
        options.width = settings.sexyWidth;
      }
      // Apply ddcl.
      $sexyElement.dropdownchecklist(options);
    });
  }
};

/**
 * Calculate maxDropHeight value in pixels based on allowed number of
 * visible items by building a temporary ddcl.
 */
function sexyExposedCalculateMaxHeight (sexyElement, sexyNumber) {
  // We need to build a temporary dropdownchecklist to get height of one item.
  sexyElement.dropdownchecklist();
  var height = $('.ui-dropdownchecklist-item:first-child').outerHeight(true);
  sexyElement.dropdownchecklist("destroy");
  return sexyNumber * height;
};

})(jQuery);
