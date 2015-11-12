(function ($) {	
	Drupal.behaviors.custom = {
  	attach: function (context) {
			$('input#edit-search-block-form--2').attr("placeholder","Search");
			$('input#edit-name').attr("placeholder","Name");	
			$('input#edit-mail').attr("placeholder","Email");	
			$('input#edit-subject').attr("placeholder","Subject");	
			$('#edit-message').attr("placeholder","Message");
			$('#edit-comment-body-und-0-value').attr("placeholder","Message");
		}
	};
})(jQuery);
