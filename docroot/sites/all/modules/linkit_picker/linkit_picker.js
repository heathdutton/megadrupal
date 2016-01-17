(function($){

	Drupal.behaviors.linkit_picker_buttons = {
		attach: function(context) {
			$('a.linkit-picker-button:not(.linkit-processed)').addClass('linkit-processed').click(function() {
			  $('#linkit-picker-container a.linkit-picker-button').removeClass('selected');
			  $(this).addClass('selected');
				var viewname = $(this).data('viewname');
				$('#linkit-picker-container .view-container').hide();
				$('#linkit-picker-container .view-linkit-picker-' + viewname).toggle();
				return false;
			});
			$('a.linkit-picker-button:first', context).click();
		}
	}

	Drupal.behaviors.linkit_picker = {
		attach: function(context) {
			$('#linkit-picker-container .view-container tr td:not(.linkit-row-processed)').addClass('linkit-row-processed').click(function() {
				var result = $(this).parent().find('.views-field-linkit-picker');
				result = jQuery.parseJSON($.trim(result.text()));
				Drupal.linkit.populateFields({
          path: result.path
        });
        Drupal.settings.linkit.currentInstance.linkContent = result.title;
        $('.linkit-path-element', context).focus();
				$('html, body').animate({scrollTop:0}, 200);
			});
		}
	}

})(jQuery);