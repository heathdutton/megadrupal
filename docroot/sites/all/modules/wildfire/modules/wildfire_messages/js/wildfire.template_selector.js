jQuery(document).ready(function() {
	wildfire_template_init();
	
	// Add a click handler for each template, so when clicked, the hidden element
  // is updated correctly.
  jQuery('#wildfire-template-selector li a').click(function() {
    var pieces = jQuery(this).parent().attr('id').split('-');
    jQuery('#edit-template').val(pieces[2]);
    
    // Select the current template and deselect the rest.
    jQuery(this).parent().addClass('selected').removeClass('unselected');
    jQuery(this).parent().siblings().addClass('unselected').removeClass('selected');
    jQuery('.unselected').fadeTo('fast', 0.60);
    jQuery('.selected').fadeTo('fast', 1.00);
    
    //Add the parameters back into the activated a tag
    jQuery('.edit-no-preview').hide();
    jQuery('#edit-preview').fadeTo('fast', 1.00);
    
    // Alter the preview link based on the current template name, assuming the
    // preview link is on the page, of course.
    var urlChunk = jQuery('#edit-preview').attr('href');
    
    if (urlChunk) {
    	var url = urlChunk.split('/');
	    url[url.length-1] = pieces[2];
	    jQuery('#edit-preview').attr('href', url.join('/'));
    }
    
    return false;
  });
  
  // Add a rollover handler for some nice fading buttons
  jQuery('#wildfire-template-selector li a').hover(
    function () {
      jQuery(this).parent('.unselected').stop().fadeTo('fast',1.00);
      jQuery(this).parent().siblings('.unselected').stop().fadeTo('fast',0.60);
    }, 
    function () {
      jQuery(this).parent('.unselected').stop().fadeTo('fast',0.60);
      jQuery(this).parent().siblings('.unselected').stop().fadeTo('fast',0.60);
    }
  );
  
});

/**
 * Make sure the right template is selected by default when the page loads.
 * 
 * This is used when the form is being edited, if a template is already
 * selected, otherwise it forces the user to choose the template again, when a
 * form validation error occurs, for example.
 */
function wildfire_template_init() {
  if (jQuery("#edit-template").val() != "") {
    jQuery("#wildfire-template-selector li").removeClass("selected").addClass("unselected");
    jQuery("#wildfire-template-" + jQuery("#edit-template").val()).removeClass("unselected").addClass("selected");
    
    jQuery('.unselected').fadeTo('fast', 0.60);
    jQuery('.selected').fadeTo('fast', 1.00);
  }
}