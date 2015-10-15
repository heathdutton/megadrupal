(function ($) {
  /**
   * Provides the Face Detection Drupal behavior.
   */
  Drupal.behaviors.face_detection = {
    attach: function (context, settings) {
      $('img.face_detection').each(function(){
        $(this).load(function(){
        var coords = $(this).faceDetection({
				  error:function(img, code, message) {
				    console.log(message);
				  }
			  });
			  
		    for (var i = 0; i < coords.length; i++) {
			    $('<div>', {
				    'class':'face',
					  'css': {
					    'left':		coords[i].positionX +'px',
					    'top':		coords[i].positionY +'px',
					    'width': 	coords[i].width		+'px',
					    'height': 	coords[i].height	+'px',
					   }
				  })
				  .appendTo($(this).parent());
	      }
      })
      });
    }
  };
})(jQuery);
