/**
 * @file
 * Javascript for the module.
 */

/**
 * Provide a slider for the slider widget.
 */
(function ($) {
  Drupal.behaviors.numeric_interval_slider = {
    attach: function(context) {
      // Hide the wrapper.
      $("div.field-widget-numeric-interval-slider div.form-type-textfield").hide();

      // Add the sider.
      $("div.field-widget-numeric-interval-slider .fieldset-wrapper").slider({
        range: true,
        min: 0,
        max: 500,
        values: [ 75, 300 ],
        slide: function( event, ui ) {
          $('input.edit-field-numeric-interval:first', $(this)).val(ui.values[0]);
          $('input.edit-field-numeric-interval:last', $(this)).val(ui.values[1]);
        }
      });
    }
  }
})(jQuery);