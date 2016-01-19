/**
 * @file wiredocs.js
 * We initalize the Java applet silently and start it when the edit button is clicked.
 */
(function ($) { 
/**
 * Attach behaviors for triggering observed file editing
 */
Drupal.behaviors.WireDocsFileEdit = {
  attach: function (context, settings) { 	  
    $('.wiredocs-edit-button').click(function(ev) {		  
	  ev.preventDefault(); // suppress default behavior, e. g. form submission
	  $fid = $(this).siblings('.wiredocs-fid').val(); // get value by wiredocs class
	  var applet = $("#wiredocs-file-" + $fid); // get applet object
	  applet[0].edit(); // start processing, i. e. file download
	  return false; // suppress default behavior, e. g. form submission
	}).addClass('ajax-processed');	
  }
};
})(jQuery);