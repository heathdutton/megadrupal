(function ($) {
  Drupal.behaviors.chardinContext = {
    attach:function (context, settings) {
      if(!Drupal.settings.chardinContext) return false;
			var auto_start = Drupal.settings.chardinContext.auto_start;
      if(auto_start == 0) {
				$('body').chardinJs();
			} else {
				$('body').chardinJs('start');
			}
			
			var elements = Drupal.settings.chardinContext.elements;
			
			$.each(elements, function (key, chardin) {
				// add chardin attributes to elements set in the context
				if(chardin.position != 'none'){
					$(chardin.element).attr({
						'data-intro': chardin.intro,
						'data-position': chardin.position,
					});
				}
				
				if(chardin.toggle == 1){
					$(chardin.element).attr({
						'data-toggle': 'chardinjs',
					});
				}
			});
			
			$('a[data-toggle="chardinjs"]').on('click', function(e) {
				e.preventDefault();
				return ($('body').data('chardinJs')).toggle();
			});
    }
  };
}(jQuery));