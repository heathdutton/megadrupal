/**
 * @file
 * Entity Dialog JavaScript.
 */
 
jQuery(document).ready(function($) {
	
// Click event on the dialog close button.

// @TODO: This can be .on() if we have a dependancy on jQuery update module.
$('body').delegate('#entity-dialog .dialog-close', 'click', function(){
	$('#entity-dialog')
	  .fadeOut('fast', function(){
  	  $(this).remove();
	  });
});
	
/**
 * Entity Dialog status message AJAX framework command.
 */
Drupal.ajax.prototype.commands.edStatus = function(ajax, response, status) {
  if (status == 'success') {
    // Fade in the status message and then fade out after 5 seconds.
    $(response.selector)
      .animate({
        opacity: 1,
        top: 10
      }, 250, 'easeInOutQuad')
      .delay(5000)
      .animate({
        opacity: 0,
        top: 20
      }, 500, 'easeInOutQuad');
  }
}

/**
 * Reload page AJAX framework command.
 */
Drupal.ajax.prototype.commands.edReloadPage = function(ajax, response, status) {
	// Hide the contents of the dialog and show the spinner.
	$('#entity-dialog')
		.find('.inner')
		.addClass('loading')
		.find('.dialog-content')
			.fadeOut('slow');
	
	// Reload the page.
	window.location.reload(true);
}

});
