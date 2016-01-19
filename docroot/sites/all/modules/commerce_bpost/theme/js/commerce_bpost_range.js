/**
 * @file
 * Script file to attach a range bar to a form element.
 */

(function ($) {
  // Attach range events on input field with "range-slider" class.
  Drupal.behaviors.attachRange = {
    attach: function (context, settings) {
      var $minWeight = $(".field-name-bpost-min-weight input");
      var $maxWeight = $(".field-name-bpost-max-weight input");
      var $rangeSlider = $("#bpost-range-slider");

      if (!$maxWeight || !$minWeight || !$rangeSlider) {
        return;
      }

      $rangeSlider.slider({
        range: true,
        min: 0,
        max: 50,
        step: 0.25,
        values: [$minWeight.val(), $maxWeight.val()],
        slide: function (event, ui) {
          $minWeight.val(ui.values[0]);
          $maxWeight.val(ui.values[1]);
        },
        start: function (event, ui) {
          // Colorize fields background when moving slider cursors.
          var $cursorCollection = $rangeSlider.children("a");
          if (ui.handle == $cursorCollection[0]) {
            $minWeight.addClass('colorize');
          }
          else {
            if (ui.handle == $cursorCollection[1]) {
              $maxWeight.addClass('colorize');
            }
          }
        },
        stop: function (event, ui) {
          // Remove *colorize* class on both elements.
          $minWeight.removeClass('colorize');
          $maxWeight.removeClass('colorize');
        }
      });

      $minWeight.val($rangeSlider.slider("values", 0));
      $maxWeight.val($rangeSlider.slider("values", 1));

      // Add events to sync slider bar and fields values.
      $minWeight.change(function () {
        if (!isNaN($(this).val())) {
          $rangeSlider.slider('values', [
            $(this).val(),
            $rangeSlider.slider('values', 1)
          ]);
        }
      });
      $maxWeight.change(function () {
        if (!isNaN($(this).val())) {
          $rangeSlider.slider('values', [
            $rangeSlider.slider('values', 0),
            $(this).val()
          ]);
        }
      });

    }
  }
})(jQuery);
