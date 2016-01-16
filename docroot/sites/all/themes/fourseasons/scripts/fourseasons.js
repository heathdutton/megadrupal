(function ($) {

  Drupal.behaviors.fourseasons = {
    attach: function(context) {
		if(typeof(Drupal.settings.fourseasons) == 'undefined'){
			return false;
		}
		var $base_color = Drupal.settings.fourseasons.base_color;
		jQuery("h1, h2, h3, h4, h5").css("color", "#" + $base_color);
		jQuery("#secondary ").css("background", "#" + $base_color);
      $("#collapse-all-fieldsets").click( function () {
      $(".pseudo-fieldset-content").hide();
      $(".pseudo-fieldset").addClass("collapsed");
    });
    $("#open-all-fieldsets").click( function () {
      $(".pseudo-fieldset-content").show();
      $(".pseudo-fieldset").addClass("collapsed");
    });
    
    $(".collapsible .pseudo-fieldset-title").click( function () {
      var thisFieldset = $(this).parent();
      $(".pseudo-fieldset-content", thisFieldset).slideToggle();
      $(thisFieldset).toggleClass("collapsed");
    });
	  
    }
  };

}(jQuery));