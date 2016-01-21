/**
 * @file
 * 
 * Allows the selection of multiple checkboxes within the column of a table.
 * 
 */

/*
 * Attach to Drupal behaviours.
 */
(function ($) {
  
Drupal.behaviors.namecardsTableColumnSelect = {
  attach: function(context) {
    $('form table:has(th.namecards-select-table-column):not(.namecards-select-table-column-processed)', context).each(Drupal.namecardsTableColumnSelect);
  }
};

/**
 * Main function
 */
Drupal.namecardsTableColumnSelect = function() {
  // Do not add a "Select column" checkbox if there are no rows with checkboxes in the table
  if ($('td input:checkbox', this).size() == 0) {
    return;
  }
  
  //Keep track of the table, which checkbox is checked and alias the settings.
  var table = this;
  var lastChecked;
  var strings = {
    'selectColumn': Drupal.t('Select all rows in this column'),
    'selectNone': Drupal.t('Deselect all rows in this column')
  };
  var checkboxes = new Array();
  var updateSelectColumn = function(selectedElement, state) {
    $(selectedElement).attr('title', state ? strings.selectNone : strings.selectColumn);
  };

  // Find all <th> with class namecards-select-table-column, and insert the check column checkbox.
  $('th.namecards-select-table-column', table).prepend($('<input type="checkbox" class="form-checkbox" />').attr('title', strings.selectColumn));

  // Differentiate checkboxes from each column by assigning a column specific class.
  $(table).find('tr').each(function(i) {
    $(this).find('input:checkbox').each(function(index) {
      $(this).addClass('namecards-column-' + index);
    });
  });
  
  // Build array of checkboxes objects for each column. 
  $('th.namecards-select-table-column input:checkbox', table).each(function(index) {
    var pattern = new RegExp("namecards-column-[0-9]{1,}");
    var columnClass = pattern.exec($(this).attr('class'));
    checkboxes[index] = $('.' + columnClass);
  });

  // Add click event to <th> checkboxes.
  $('th.namecards-select-table-column input:checkbox', table).click(function(event) {
  // Set mouse pointer to busy.  Required to give user feedback since this function can take several seconds to complete depending on length of list.
    $('body').css('cursor', 'wait');
    //alert('start');

    setTimeout(function() {
      if ($(event.target).is('input:checkbox')) {
        // Loop through all checkboxes for that column and set their state to the select column checkbox' state.
        var selectColumnCheckbox = $(event.target);
            
        // Get column number of selected column.
        var pattern = new RegExp("namecards-column-[0-9]{1,}");
        var columnClass = pattern.exec(selectColumnCheckbox.attr('class'));
        var pattern2 = new RegExp("[0-9]{1,}");
        var columnNumber = pattern2.exec(columnClass);
        checkboxes[columnNumber].each(function() {
          if ($(this).parents('tr:first').css('display') != 'none') {
            this.checked = event.target.checked;
            // Ensure that the checkboxs' change event is fired. Used to add
            // css row highlighting @see file 'namecards.js'.
            $(this).change();
          }
          // Update the title and the state of the check column box.
          updateSelectColumn(this, event.target.checked);
        });
        
      } 
      // Return mouse pointer to normal state.
      $('body').css('cursor', 'auto');
    }, 0);
  });
  
  //For each of the checkboxes within first column of the table.
  $('.namecards-column-0, .namecards-column-1').click(function(e) {
    // Either add or remove the selected class based on the state of the check column checkbox.
    //$(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');
  
    // If this is a shift click, we need to highlight everything in the range.
    // Also make sure that we are actually checking checkboxes over a range and
    // that a checkbox has been checked or unchecked before.
    if (e.shiftKey && lastChecked && lastChecked != e.target) {
      // We use the checkbox's parent TR to do our range searching.
      Drupal.tableSelectRange($(this).attr('class'), $(e.target).parents('tr')[0], $(lastChecked).parents('tr')[0], e.target.checked);
    }
  
    // If all checkboxes are checked, make sure the select-all one is checked too, otherwise keep unchecked.
    updateSelectColumn((checkboxes.length == $(checkboxes).filter(':checked').length));
  
    // Keep track of the last checked checkbox.
    lastChecked = e.target;
  });   
  
  //For each of the checkboxes within second column of the table.
  $('.namecards-column-1').click(function(e) {
    // Either add or remove the selected class based on the state of the check column checkbox.
    //$(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');
  
    // If this is a shift click, we need to highlight everything in the range.
    // Also make sure that we are actually checking checkboxes over a range and
    // that a checkbox has been checked or unchecked before.
    if (e.shiftKey && lastChecked && lastChecked != e.target) {
      // We use the checkbox's parent TR to do our range searching.
      Drupal.tableSelectRange($(this).attr('class'), $(e.target).parents('tr')[0], $(lastChecked).parents('tr')[0], e.target.checked);
    }
  
    // If all checkboxes are checked, make sure the select-all one is checked too, otherwise keep unchecked.
    updateSelectColumn((checkboxes.length == $(checkboxes).filter(':checked').length));
  
    // Keep track of the last checked checkbox.
    lastChecked = e.target;
  });   

  // Finished processing table. Indicate by adding 'processed' class.
  $(this).addClass('namecards-select-table-column-processed');
};

/*
 * Enables selection of multiple checkboxes using mouse and left shift key.
 */
Drupal.tableSelectRange = function(selectorClass, from, to, state) {
  // We determine the looping mode based on the the order of from and to.
  var mode = from.rowIndex > to.rowIndex ? 'previousSibling' : 'nextSibling';

  // Traverse through the sibling nodes.
  for (var i = from[mode]; i; i = i[mode]) {
    // Make sure that we're only dealing with elements.
    if (i.nodeType != 1) {
      continue;
    }

    // Either add or remove the selected class based on the state of the target checkbox.
    // Only checks those checkboxes which are visible.
    var pattern = new RegExp("namecards-column-[0-9]{1,}");
    var columnClass = '.' + pattern.exec(selectorClass);

    $(columnClass, i).filter(":visible").each(function() {
      this.checked = state;
      // Ensure that the checkboxs' change event is fired. Used to add
      // css row highlighting @see file 'namecards.js'.
      $(this).change();
    });

    if (to.nodeType) {
      // If we are at the end of the range, stop.
      if (i == to) {
        break;
      }
    }
    // A faster alternative to doing $(i).filter(to).length.
    else if (jQuery.filter(to, [i]).r.length) {
      break;
    }
  }
};

}(jQuery));