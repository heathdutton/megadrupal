/**
 * @file
 * 
 * Javascript file
 * 
 * Provides AJAX functionality for retrieving contacts by a specific node. 
 * 
 * Used when retreiving contacts by org or event. For this script to work 
 * correctly the tables used to display the retrieved results must have a 
 * certain structure.  This is achieved by modifying the relevant view's 
 * table using javascript. @see file 'namecards.js' for this initialization 
 * script. 
 */

(function ($) {
  
//Module namespace:
var Namecards = Namecards || {};

Namecards.displayedRows = new Array();
Namecards.reloadRowsURI = '';

/**
 * Determines which rows are currently visible to the user.
 */
Namecards.getDisplayedRows = function() {
  // Clear existing values from array.
  Namecards.displayedRows.length = 0;
  Namecards.reloadRowsURI = '/namecards/set_displayed_rows_session_var';	
	
  $('td[id^="namecards-get-contacts-by-nid-cell-"]').each(function(i) {
    if ($(this).html() != '') {
      var nodeId = $(this).attr('id').split('-').pop();
      Namecards.reloadRowsURI += '/' + nodeId;
    }
  });
  // Update session variable on server contain via ajax.
  $.post(Namecards.reloadRowsURI);
};

/**
 * Attaches toggle event to links which retrieve ajax data 
 */
Drupal.behaviors.namecardsContactsByNode = {
  attach: function (context, settings) {
    // Clear existing values from array on page load. Ensures array is 
    // empty when a page is being loaded, to eliminate any vestigel 
    // values.
    Namecards.displayedRows.length = 0;
    // Reset session var on server on page load as no rows are visible on 
    // page load. 
    Namecards.getDisplayedRows();
    
    $('a.namecards-node-link:not(.namecards-node-link-processed)', context).addClass('namecards-node-link-processed').each(function (i) {
    	 // This function will get exceuted after the ajax request is completed successfully
    	 var currentLink = $(this);
    	 var currentNid = currentLink.attr('id').split('-').pop();
    	
    	 // Create new table row below existing row, to display ajax data. 
      var newRow = $('<tr id="namecards-get-contacts-by-nid-row-' + currentNid + '"><td id="namecards-get-contacts-by-nid-cell-' + currentNid + '" colspan="2"></td></tr>');
      // Add row directly below to display ajax results
      currentLink.parents('tr').after(newRow);
      // Hide new row since doesn't yet contain any data.
      $('#namecards-get-contacts-by-nid-row-' + currentNid).hide();
    	
      // Add click event.
      currentLink.click(function() {
    	   // Toggle on. Checks if row to display ajax data is visible.
    	   //if ($('#namecards-get-contacts-by-nid-row-' + currentNid).is(':hidden')) { 
        if ($('#namecards-get-contacts-by-nid-cell-' + currentNid).html() == '') { 
          var updateContacts = function(data) {
            // The data parameter is a JSON object. The “contacts” property is the list of products items that was returned from the server response to the ajax request.
            var destinationTableCell = $('#namecards-get-contacts-by-nid-cell-' + currentNid);
            
            // Disable throbber.
            //destinationTableCell.empty()
            //$('#namecards-get-contacts-by-nid-cell-' + currentNid).html();
            
            // Make the nid available globally for use in toggle function
            destinationTableCell.html(data.contacts).slideDown('fast');
            $('#namecards-get-contacts-by-nid-row-' + currentNid).show(function() {
              // Determine which ajax rows are now displayed. Used when 
              // reloading page to ensure that already displayed rows are 
              // reloaded when modalframe is closed.
          	  Namecards.getDisplayedRows();
            });
        	   // Determine which ajax rows are now displayed. Used when reloading 
            // page to ensure that already displayed rows are reloaded when 
            // modalframe is closed.
            Namecards.getDisplayedRows();
            Drupal.attachBehaviors();
          };
          
          // Add throbber.
          $('#namecards-get-contacts-by-nid-row-' + currentNid).show();
          $('#namecards-get-contacts-by-nid-cell-' + currentNid).html($('<div class="namecards-throbber">&nbsp;</div>'));
          
          // Send ajax request.
          $.ajax({
            type: 'POST',
            url: this.href, // Which url should be handle the ajax request. This is the url defined in the <a> html tag
            error: function() {
              $('#namecards-get-contacts-by-nid-row-' + currentNid).show();
              $('#namecards-get-contacts-by-nid-cell-' + currentNid).html($('<div class="">An error has occurred while retrieving the list of contacts.  Please try again.  If the problem persists, please inform your website administrator.</div>'));
            },
            success: updateContacts, // The js function that will be called upon success request
            dataType: 'json', //define the type of data that is going to get back from the server
            data: 'js=1' //Pass a key/value pair
          });
          // return false so the navigation stops here and not continue to the page in the link.
          return false; 
        }
        // Toggle off.
    	   else { 
      	  // Hide the current cell containing ajax content and empty the cell of any content.
    	    $('#namecards-get-contacts-by-nid-cell-' + currentNid).slideUp(function() { 
      	    // Remove ajax content from cell and hide parent row.
      	    $(this).empty().parents('#namecards-get-contacts-by-nid-row-' + currentNid).hide(); 
           // Determine which ajax rows are now displayed. Used when reloading 
      	    // page to ensure that already displayed rows are reloaded when 
      	    // modalframe is closed.
           Namecards.getDisplayedRows(); 
      	  });
      	  // return false so the navigation stops here and not continue to the page in the link
         return false; 
        }
      });
    });
  }
};

}(jQuery));