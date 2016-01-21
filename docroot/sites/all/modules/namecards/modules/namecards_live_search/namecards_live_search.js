// $Id$
/**
 * @file
 * 
 * Add live search function to pages within Namecard Module.
 * 
 * TODO: filterTable() function is not written very efficiently (very 
 * much a patch work solution). Should be overhauled.
 */ 
(function ($) {

Drupal.behaviors.namecardsLiveSearch = {
    attach: function(context) {
  
    var searchBox = $('#edit-namecards-live-search-textfield');
    var clearButton = $('#edit-namecards-live-search-clear-button');
    
    // Add keyup event handler to search textbox.
    searchBox.not('.namecards-live-search-textfield-processed').addClass('namecards-live-search-textfield-processed').keyup(function() {
  	delay(function() {
  		filterTable();
  	}, 800);
    });
  
    // Add event handler to clear button.
    clearButton.not('.namecards-live-search-clear-button-processed').addClass('namecards-live-search-clear-button-processed').click(function() {
      searchBox.val('');
      filterTable();
    });
  
    // Add wrapper for displaying view with ajax.
    $('.view:not(.namecards-view-ajax-wrapper-processed)').addClass('namecards-view-ajax-wrapper-processed').wrap($('<div id="namecards-view-ajax-wrapper" />'));
    
    // Get array of search terms from the search text box.
    var getKeywords = function () {
      var searchString = Drupal.checkPlain(searchBox.val());
      var searchTerms = new Array();
      searchTerms = searchString.split(' ');
      return searchTerms;
    };
    
    // Filters table by search terms. No need to explicitly define table as only ever one table per page.
    var filterTable = function() {
  	var searchTerms = getKeywords();
  	
      // Start throbber
  	searchBox.addClass('namecards-live-search-throbbing');
  	
  	// Determine which view is being displayed.  Necessary as view namecards_browse_contacts 
  	// and table email_list has different table structure to namecards_organizations and 
  	// namecards_events. Furthermore view namecards_browse_contacts includes a database call, 
      // where as the others are purely ajax based.
  	if ($('.view-namecards-browse-contacts').length) {
  	  // Create URL friendly string of search terms.
  	  var urlFriendlySearchTerms = searchTerms.join('/');
  	  var destURI = 'http://' + location.host + '/namecards/live_search_results/js/' + urlFriendlySearchTerms;
  
  	  var updateView = function(data) {
  		// Add view.
  		$('#namecards-view-ajax-wrapper').html(data.viewBrowseContacts);
  		// Turn off thobber.
  		searchBox.removeClass('namecards-live-search-throbbing');
  		// Reattach behaviors.
  		Drupal.attachBehaviors();
  	  };
  	  
  	  $.ajax({
  	    type: 'POST',
  	    url: destURI, // Which url should be handle the ajax request. 
  	    success: updateView, // The js function that will be called upon success request
  	    dataType: 'json', //define the type of data that is going to get back from the server
  	    data: 'js=1' //Pass a key/value pair
  	  });
  	}
  	else if ($('.namecards-create-email-list-wrapper').length) {
  	  // For searching email list.  Is implemented purely in javascript as it appears 
  	  // this method is sufficiently fast.  It is possible that speed may be affected 
  	  // in a very large list.  If this does occur then I may have to implement an 
  	  // AJAX method using the database to search for results (as has been implemented 
  	  // for "Browse" Contacts). 
  	  $('.namecards-create-email-list-wrapper tbody tr').each(function(rowIndex) {
  		var currentRow = $(this);
  		var containsSearchTerm = true;
  	    var rowText = '';
  		
  	    // Build a single string of text from cells within each row, excluding the first cell.
  		currentRow.find('td').each(function(cellIndex) {
  	      if (cellIndex != 0) {
  		    var currentCell = $(this);
  		    rowText += Drupal.checkPlain(currentCell.text());
  		  }
  		});
          
          // Check if any of the search terms are not present since all terms must be present for it to be considered a match.
  		for (var i = 0; i < searchTerms.length; i++) {
  		  if (rowText.toLowerCase().indexOf(searchTerms[i].toLowerCase()) == -1) {
  		    // Term present
  		    containsSearchTerm = false;
  		    // exit loop.
  		    break;
  		  }
  		}
  				  
  		if (containsSearchTerm == true) {
  		  currentRow.show();
  		}
  		else {
  		  currentRow.hide();
  		}
  	  });
  	  // Turn off thobber.
  	  searchBox.removeClass('namecards-live-search-throbbing');
      }
  	else if ($('.view-id-namecards_events').length || $('.view-id-namecards_organizations').length) {
  	  // For searching "organizations" and "events" views.
  	  $('a[id^="namecards-ajax-link-"]').each(function(index) {
  		var currentRow = $(this).parents('tr');
          var linkText = $(this).text();
  		var containsSearchTerm = true;
  	    var nodeId = $(this).attr('id').split('-').pop();
  	    
  	    for (var i = 0; i < searchTerms.length; i++) {
  	      if (linkText.toLowerCase().indexOf(searchTerms[i].toLowerCase()) == -1) {
  	        // Term not present
  	        containsSearchTerm = false;
  	        // exit loop.
  	        break;
    	      }
  	    }
  	    if (containsSearchTerm == true) {
  	    	currentRow.show();
            // only show row if it contains content to be shown. Otherwise keep it hidden.
            if ($('#namecards-get-contacts-by-nid-row-' + nodeId).find('td').text() != '') {
              $('#namecards-get-contacts-by-nid-row-' + nodeId).show();
            }
  	    }
  	    else {
            currentRow.hide();
            $('#namecards-get-contacts-by-nid-row-' + nodeId).hide();
          }
  	  });
  	  // Turn off thobber.
  	  searchBox.removeClass('namecards-live-search-throbbing');
  	}
    };
    
    // Function to create delay effect so event will not fire after every key press. 
    // Solution from "http://stackoverflow.com/questions/1909441/jquery-keyup-delay".
    var delay = (function () {
      var timer = 0;
      return function(callback, ms) {
        clearTimeout(timer);
        timer = setTimeout(callback, ms);
      };
    })();
  }
};

}(jQuery));