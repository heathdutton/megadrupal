jQuery(document).ready(function() {
	wildfire_list_init();
	
	// Add a click handler for each list, so when clicked, the hidden element is
  // updated correctly.
  jQuery('ul#wildfire-list-selector li').click(function() {
    // If the <li> is not selected, select it. Otherwise, unselect it.
    if (jQuery(this).hasClass('unselected')) {
      // Find the list ID by splitting the <li>'s id attribute up.
      var pieces = jQuery(this).attr('id').split('-');
      jQuery('#edit-list').val(pieces[2]);
      
      // Make the active <li> the selected one, unselecting others.
      jQuery(this).addClass('selected').removeClass('unselected');
      jQuery(this).siblings().removeClass('selected').addClass('unselected').fadeOut();
      
      // Show the link to change the selected item.
      jQuery('div#wildfire-list-remove').show();
    }
    else {
      jQuery(this).removeClass('selected').addClass('unselected');
      jQuery(this).siblings().fadeIn();
      jQuery('#edit-list').val('');
      jQuery('div#wildfire-list-remove').hide();
      jQuery('#wildfire-list-count').text('0');
    }
  });
  
  // Add a click handler to remove the current list selection.
  jQuery('div#wildfire-list-remove a').click(function() {
    jQuery('ul#wildfire-list-selector li').removeClass('selected').addClass('unselected').fadeIn();
    jQuery('#edit-list').val('');
    
    return false;
  });
});

/**
 * Make sure the right list is selected by default when the page loads.
 * 
 * This is used when the form is being edited, if a list is already selected,
 * otherwise it forces the user to choose the list again, when a form validation
 * error occurs, for example.
 */
function wildfire_list_init() {
  if (jQuery("#edit-list").val() > 0) {
    jQuery("#wildfire-list-" + jQuery("#edit-list").val()).addClass("selected");
  }
}
