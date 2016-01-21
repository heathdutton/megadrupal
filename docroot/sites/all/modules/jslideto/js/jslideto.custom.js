(function ($) {
	Drupal.behaviors.jslideto = {
	attach : function(context, settings) {	
    var targetID = document.location.href;
		targetID = targetID.split('#');
		if(targetID.length >= 2) {
      var target = $("#"+targetID[1]);
     	var targetOffset = target.offset().top;
    	$('html,body').animate(
				{scrollTop: targetOffset},
				parseInt(Drupal.settings.jslideto.duration),
				function() {
					if(jQuery.Color) {
            $(target).css('background-color', Drupal.settings.jslideto.background);
            target.animate({ 'background-color': 'transparent' }, "slow");
          }	
				}
			);
    }
		$(document).scrollTop(0);
    $('a[href*=#]').click(function() {
    var target = $(this.hash);
    target = target.length && target || $('[name=' + this.hash.slice(1) +']');
  	if (target.length) {
      var targetOffset = target.offset().top;
      $('html,body').animate(
        {scrollTop: targetOffset},
  			parseInt(Drupal.settings.jslideto.duration),
  			function() {
          if(jQuery.Color) {
            $(target).css('background-color', Drupal.settings.jslideto.background);
            target.animate({ 'background-color': 'transparent' }, "slow");
  				}
  			});
  	}
    return false;
  });
}}
})(jQuery);
