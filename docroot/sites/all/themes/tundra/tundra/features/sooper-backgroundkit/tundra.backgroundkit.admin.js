/**
 * @file
 * Bind the colorpicker event to the Colors form element.
 */

(function ($) {
if(typeof $.farbtastic == 'function') {
  Drupal.behaviors.colors = {
    attach: function() {

      $(".form-item-gradient-opacity").before('<div id="farb-bgkit"></div>');
      var farb = $.farbtastic("#farb-bgkit");

      // Loop over each calendar_color type.
      $('#edit-gradient-color-top,#edit-gradient-color-bottom').each(function() {

        // Set the background colors of all of the textfields appropriately.
        farb.linkTo(this);

        // When clicked, they get linked to the associated farbtastic colorpicker.
        $(this).click(function () {
          farb.linkTo(this);
        });
      });
    }
  };
}


Drupal.behaviors.sliders = {
  attach: function() {

    $( "#gradient-opacity-slider" ).slider({
			value: $( "#edit-gradient-opacity" ).val(),
			slide: function( event, ui ) {
				$( "#edit-gradient-opacity" ).val(ui.value);
			}
		});
    $( "#image-opacity-slider" ).slider({
			value: $( "#edit-image-opacity" ).val(),
			slide: function( event, ui ) {
				$( "#edit-image-opacity" ).val(ui.value);
			}
		});
  }
};


})(jQuery);
