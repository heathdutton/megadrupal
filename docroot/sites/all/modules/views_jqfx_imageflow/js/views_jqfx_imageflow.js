/**
 *  @file
 *  This will pass the settings and initiate the ImageFlow.
 */
(function($) {
Drupal.behaviors.viewsJqfxImageFlow = {
  attach: function (context) {
    for (id in Drupal.settings.viewsJqfxImageFlow) {
      $('#' + id + ':not(.viewsJqfxImageFlow-processed)', context).addClass('viewsJqfxImageFlow-processed').each(function () {
	    // Create a new instance and get our basics
	    var imageflow = $(this).attr('id');
	    imageflow = new ImageFlow();
	    var _settings = Drupal.settings.viewsJqfxImageFlow[$(this).attr('id')];
	    
	    // Need to pass opacityArray as an array
	    if (_settings['opacityArray']) {
	      var flowOpacityArray = _settings['opacityArray'];
          eval("_settings['opacityArray'] = " + flowOpacityArray);
        }
	    
	    // Need to pass onClick as a function
	    if (_settings['onClick']) {
	      var flowClick = _settings['onClick'];
          eval("_settings['onClick'] = " + flowClick);
        }

        // Load ImageFlow
        imageflow.init(_settings);
     });
    }
  }
}

})(jQuery);
