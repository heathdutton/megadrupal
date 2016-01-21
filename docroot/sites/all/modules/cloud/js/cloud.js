// $Id$

/**
 * @file
 * Used for this module
 * 
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 * 
 */

/**
  * NAME: Bootstrap 3 Triple Nested Sub-Menus
  * This script will active Triple level multi drop-down menus in Bootstrap 3.*
  */
/*
$('ul.dropdown-menu [data-toggle=dropdown]').on('click', function(event) {
    // Avoid following the href location when clicking
    event.preventDefault(); 
    // Avoid having the menu to close when clicking
    event.stopPropagation(); 
    // Re-add .open to parent sub-menu item
    $(this).parent().addClass('open');
    $(this).parent().find("ul").parent().find("li.dropdown").addClass('open');
});
*/

/**
 * Renders Action Block on Mouse Over for table data with action buttons
 *
 */

(function ($) {

Drupal.behaviors.showActionButtons = {
  attach : function(context, settings) {

    if ($('table.sticky-header .action-column')) {
      //adds CSS to the table to hide the action column in the header
      $('table.sticky-header').addClass('hide-action-column');
    }
    
    if ($('table.sticky-enabled .action-column')) {
          
      //adds CSS to the table to hide the action column      
      $('table.sticky-enabled').addClass('hide-action-column');
      
      //gets nickname table cell offset position
      if ($('table.sticky-enabled th.nickname-column')) {
          var nickname_col = $('table.sticky-enabled th.nickname-column');
          //action_column_left_position_displacement = nickname_col[0].clientWidth - 30; //offsets action tooltip to the left
      }
    
      //append action toggle icon DOM to every nickname cell
      if ( $('table.sticky-enabled tbody tr td.nickname-column a').length > 0 ) {
        $('table.sticky-enabled tbody tr td.nickname-column a').each(function() {
          if (!$(this).hasClass('lockIcon')) {
            //Create Action Table show/hide icon
            var action_toggle_icon = document.createElement('div');
            action_toggle_icon.className="action_toggle_icon";
      
            $(action_toggle_icon).insertBefore($(this));
          }
        });
      
      } else if ( $('table.sticky-enabled tbody tr td.nickname-column span.anchor').length > 0 ) {
          $('table.sticky-enabled tbody tr td.nickname-column span.anchor').each(function() {
  
            if (!$(this).hasClass('lockIcon')) {
                  
              //Create Action Table show/hide icon
              var action_toggle_icon = document.createElement('div');
              action_toggle_icon.className="action_toggle_icon";
        
              $(action_toggle_icon).insertBefore($(this));
            }
          });
      }
      
  
      //Adding On Click behavior to Action Buttons
      $('table.sticky-enabled tbody tr .action_toggle_icon').each(function(index) {
          $(this).click(function(e) {
            toggleActionButtons(this);
          });
      });
  
      //Adding On MouseOut behavior to row of Action Buttons
      $('table.sticky-enabled tbody tr').each(function(index) {
        $(this).bind('mouseleave', function() {
            if ($(this).find('.action_toggle_icon'   ).hasClass('action_toggle_icon_on')) {
                $(this).find('.action_toggle_icon_on').removeClass('action_toggle_icon_on');
                $(this).find('.action-column'        ).css('display', 'none');      
            }
          });
      });
    }
  }
};
 
toggleActionButtons = function(evt) {
    //action button tooltip displacement values
    var action_column_left_position_displacement = "25px";      
    var row_height_displacement = "-3"; //used by IE, Safari and Chrome

// Obsolete after ver.0.9
//    if ($.browser.mozilla)
//      row_height_displacement = "2";
  
    var parent_row = $(evt).parent().parent();
    //var parent_row = parent_row_obj[0];
                
    //Dom Interaction
    if ($(evt).hasClass('action_toggle_icon_on')){  //Hide Action Buttons 
  
      $(evt).removeClass('action_toggle_icon_on');
      $(parent_row).find('.action-column').css('display', 'none');;
    
    } else {   //Show Action Buttons
  
      $(evt).addClass('action_toggle_icon_on');
    
      //get row height
      var row_height = $(parent_row).height() - row_height_displacement;
    
      //toggle display to block to show action column
      $(parent_row).find('.action-column')
          .css('display'   , 'block')
          .css('left'      , action_column_left_position_displacement)
          .css('margin-top', row_height);
    }  
};

})(jQuery);
