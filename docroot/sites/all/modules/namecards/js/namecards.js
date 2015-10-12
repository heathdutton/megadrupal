/**
 * @file
 * 
 * Javascript document
 * 
 * This is a general script which runs code required for the smooth 
 * running of the namecards module. You can think of it as an init 
 * or general setup script.  
 */
(function ($) {

// Any general script which should be attached to Drupal behaviors should 
// be placed here.
Drupal.behaviors.Namecards = {
  attach: function(context) {
    /* 
     * Disable email list text selection button. Is disabled because this 
     * functionality has yet to be added to the module.
     */
    $('#namecards-email-list-select-button', context).attr("disabled", "disabled");
    
    /* 
     * Add row highlighting
     * 
     * To change the row highlight color, please modify file 'namecards.css'.
     * @see class '.namecards-row-highlight-color' and '.namecards-row-selected-color' in file 'namecards.css'.
     */
    if ($('.view-id-namecards_browse_contacts').length || $('.view-id-namecards_contacts_by_org').length || $('.view-id-namecards_contacts_by_event').length || $('.namecards-create-email-list-wrapper').length || $('.namecards-import-select-contacts-to-import-list-table').length || $('.namecards-user-settings-table-wrapper').length) {
      var context;
      var highlightObject;
      var rowHighlightColor;
      var rowSelectedColor;
      
      // Define hidden object and attach to DOM. Set colors to be used to highlight rows on mouse over and
      // row selection using checkboxes. Highlighting colors can be accessed through the highlight object.
      // Doing it this way enables us to access the colors defined in namecards.css .
      highlightObject =  $('<div></div>').css('display', 'none').addClass('namecards-row-highlight-color').addClass('namecards-row-selected-color');
      highlightObject.appendTo('body');
      rowHighlightColor = highlightObject.css('background-color');
      rowSelectedColor = highlightObject.css('color');
      
      // Return row color based on checked status of checkbox.
      var getRowHighlight = function(checkedStatus) {
        if (checkedStatus) {
          //alert(rowSelectedColor);
          return rowSelectedColor;
        }
        return null;
      };
    
      // Set context based on view type.
      if ($('.view-id-namecards_browse_contacts').length) {
        context = '.view-id-namecards_browse_contacts';
      }
      else if ($('.view-id-namecards_contacts_by_org').length) {
        context = '.view-id-namecards_contacts_by_org';
      }
      else if ($('.view-id-namecards_contacts_by_event').length) {
        context = '.view-id-namecards_contacts_by_event';
      }
      else if ($('.namecards-create-email-list-wrapper').length) {
        context = '.namecards-create-email-list-table';
        
        // In case of page reload make sure that any pre-checked rows display highlighting.
        // This occurs when returning to the email selection list after going back from
        // formatted mailing list.
        $('input[type="checkbox"]', context).each(function(index) {
          $element = $(this);
          $row = $element.parents('tr');
          var checkbox = $element[0];
          var row = $row[0];
          
          // Set row highlighting for preselected rows.
          row.style.backgroundColor = getRowHighlight(checkbox.checked);
          
          // Add change event to change highlighting on checking checkbox.
          $element.bind('change', function() {
            row.style.backgroundColor = getRowHighlight(checkbox.checked);
          });
        });
      }
      else if ($('.namecards-import-select-contacts-to-import-list-table').length) {
  	    context = '.namecards-import-select-contacts-to-import-list-table';
        
        // In case of page reload make sure that any pre-checked rows display highlighting.
        $('tr td:first-child input[type="checkbox"]', context).each(function(index) {
          $element = $(this);
          $row = $element.parents('tr');
          var checkbox = $element[0];
          var row = $row[0];
          
          // Set row highlighting for preselected rows.
          row.style.backgroundColor = getRowHighlight(checkbox.checked);
          
          // Add change event to change highlighting on checking checkbox.
          $element.bind('change', function() {
            row.style.backgroundColor = getRowHighlight(checkbox.checked);
          });
        });
      }
      else if ($('.namecards-user-settings-table-wrapper').length) {
        context = '.namecards-user-settings-table-wrapper';
      }

      // Add highlighting when mouse hovers over rows.
      $('tbody tr', context).not('namecards-row-highlighting-processed').addClass('namecards-row-highlighting-processed').each(function(index) {
        var $row = $(this);
        
        // Add mouse hover event.
        $row.hover(
            function() {
              // On mouse over.
              //alert('Mouse over');
              $row[0].style.backgroundColor = rowHighlightColor;
             },
            function() {
              // On mouse out.
              $checkbox = $('input[type="checkbox"]', $row);
              if (($('.namecards-create-email-list-wrapper').length || $('.namecards-import-select-contacts-to-import-list-table').length) && $checkbox[0].checked) {
                // Add row selected highlighting if checkbox is checked.
                $row[0].style.backgroundColor = rowSelectedColor;
              }
              else {
                // Remove highlighting.
                $row[0].style.backgroundColor = '';
              }
							//alert($checkbox[0].checked);
            }
        );
      });
    }
  }
};

}(jQuery));