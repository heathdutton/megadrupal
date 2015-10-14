/**
Copyright (c) 2012 Marco Pegoraro, http://movableapp.com/

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


WHERE TO FIND MEDIA TABLE:
https://github.com/thepeg/MediaTable
http://consulenza-web.com/jquery/MediaTable/
http://www.consulenza-web.com/2012/01/mediatable-jquery-plugin/


Modified to match SigmaOne theme and Drupal.s

@knownbug Views attaching table causing double div wrapper
**/


/**
 * jQuery Bacheca
 * Plugin Dimostrativo
 */

(function($){ 
  Drupal.behaviors.ResponsiveTable = { 
      attach: function(context) {
        // Intialize table     
        $('table').not('.sticky-header', '.cke_editor').once('mediatable').mediaTable();
      }
    };
    
    
  /**
   * DOM Initialization Logic
   */
  
  var __loop = function( cfg, i ) {
    
    var $this   = $(this),
      wdg   = $this.data( 'MediaTable' );
    
    // Prevent re-initialization of the widget!
    if ( !$.isEmptyObject(wdg) ) return;
       
    // Build the widget context.
    wdg = {
      $wrap:    $('<div>'),   // Refer to the main content of the widget
      $table:   $this,      // Refer to the MediaTable DOM (TABLE TAG)
      $menu:    false,      // Refer to the column's toggler menu container
      cfg:    cfg,      // widget local configuration object
      id:     $this.attr('id')
    };
    
    // Drupal support for sticky table
    if ($this.hasClass('sticky-enabled')) {
      wdg.$stick = $this.prev('.sticky-header');
    }
    
    var tables = wdg.$table.find('thead th');
    
    if (tables.length == 0) {
      return false;
    }
    
    // Make three column marked as essential to show when window is small
    tables.filter(':lt(3)').addClass('essential');
    
    // Setup Widget ID if not specified into DOM Table.
    if ( !wdg.id ) {
      wdg.id = 'MediaTable-' + i;
      wdg.$table.attr( 'id', wdg.id );
    }
     
    // Activate the MediaTable.
    wdg.$table.addClass('activeMediaTable');
    
    // Create the wrapper.
    wdg.$wrap.addClass('mediaTableWrapper');
    
    // Place the wrapper near the table and fill with MediaTable.
    wdg.$table.before(wdg.$wrap).appendTo(wdg.$wrap);
    
    
    // Menu initialization logic.
    if ( wdg.cfg.menu ) __initMenu( wdg );
    
    // Columns Initialization Loop.
    tables.each(function(i){ __thInit.call( this, i, wdg, '');  });
    
    if (typeof wdg.$stick != 'undefined' && wdg.$stick.length != 0) {
      var sticks = wdg.$stick.find('thead th');
      sticks.each(function(i) { __thInit.call( this, i, wdg, '-sticky'); });
    }
    // Save widget context into table DOM.
    wdg.$table.data( 'MediaTable', wdg );
    
  }; // EndOf: "__loop()" ###
  
    
    var __initMenu = function( wdg ) {      
      
      if ($('#' + wdg.id + '-mediaTableMenu').length != 0) {
        $('#' + wdg.id + '-mediaTableMenu').remove();
      }
      
      // Buid menu objects
      wdg.$menu       = $('<div />');
      wdg.$menu.$header   = $('<a class="menuHeader" />');
      wdg.$menu.$list   = $('<ul />');
      
      // Setup menu general properties and append to DOM.
      wdg.$menu
        .addClass('mediaTableMenu')
        .addClass('mediaTableMenuClosed')
        .addClass('ui-widget-content ui-state-default ui-corner-all')
        .append(wdg.$menu.$header)
        .append(wdg.$menu.$list)
        .attr('id', wdg.id + '-mediaTableMenu');
      
      // Add a class to the wrapper to inform about menu presence.
      wdg.$wrap.addClass('mediaTableWrapperWithMenu');
      
      // Setup menu title (handler)
      wdg.$menu.$header.text(wdg.cfg.menuTitle).prepend('<span class="ui-icon ui-icon-circle-plus" />');
      wdg.$table.before(wdg.$menu);
      
      // Bind screen change events to update checkbox status of displayed fields.
      $(window).bind('orientationchange resize',function(){
        wdg.$menu.find('input').trigger('updateCheck');
      });
      
      // Toggle list visibility when clicking the menu title.
      wdg.$menu.$header.bind('click',function(){
        wdg.$menu.toggleClass('mediaTableMenuClosed').toggleClass('ui-state-active');
      });
      
      wdg.$table.click(function() {
          wdg.$menu.addClass('mediaTableMenuClosed');
      });
      
      // Toggle list visibilty when mouse go outside the list itself.
      wdg.$menu.$list.bind('mouseleave',function(e){
        wdg.$menu.toggleClass('mediaTableMenuClosed').toggleClass('ui-state-active');
        e.stopPropagation();
      });
      
      // Hide the weight column visibility toggle
      $('.tabledrag-toggle-weight-wrapper').hide();
      
    }; // EndOf: "__initMenu()" ###
    
    var __thInit = function( i, wdg, mode) {
      var $th   = $(this),
        id    = $th.attr('id'),
        classes = $th.attr('class'),
        thContent = $th.html(),
        colspan = $th.attr('colspan');
      
      // Skip on table header without label
      // Drupal has several table like this eg. table sort checkbox column
      if (thContent.length == 0) {
        return;
      }
      
      // Set up an auto-generated ID for the column.
      // the ID is based upon widget's ID to allow multiple tables into one page.
      if ( !id ) {
        id = wdg.id + '-mediaTableCol-' + i;
        $th.attr( 'id', id + mode);
      }
      
      // Add toggle link to the menu.
      // Bug fix : Double checkbox created.
      var liId = 'toggle-col-' + wdg.id + '-' + i;
      if ( wdg.cfg.menu && !$th.is('.persist') && wdg.$menu.$list.find('#' + liId).length == 0) {
        
        var $li = $('<li><input type="checkbox" name="toggle-cols" id="' + liId + '" value="'+id+'" /> <label for="toggle-col-'+wdg.id+'-'+i+'">'+$th.text()+'</label></li>');
        wdg.$menu.$list.append($li);
        
        __liInitActions( $th, $li.find('input'), wdg );
        
      }
      
      // Propagate column's properties to each cell.
      $('tbody tr',wdg.$table).each(function(){ __trInit.call( this, i, colspan, id, classes ); });
      
    }; // EndOf: "__thInit()" ###
    
    
    var __trInit = function( i, colspan, id, classes ) {
      
      var $cell = $(this).find('td,th').slice(i, colspan + i);
      $cell.attr( 'headers', id );
      
      if ( classes ) $cell.addClass(classes);
      
    }; // EndOf: "__trInit()" ###
    
    var __liInitActions = function( $th, $checkbox, wdg ) {
      
      var change = function() {
        
        var val   = $checkbox.val(),  // this equals the header's ID, i.e. "company"
          cols  = wdg.$table.find("#" + val + ", [headers="+ val +"]"); // so we can easily find the matching header (id="company") and cells (headers="company")
        
        // Drupal Sticky table
        if (typeof wdg.$stick != 'undefined' && wdg.$stick.length != 0) {
          stick = wdg.$stick.find("#" + val + '-sticky');
        }
        
        if ( $checkbox.is(":checked")) { 
          cols.show();
          
          // Drupal Sticky table
          if (typeof stick != 'undefined' && stick.length != 0) {
            stick.show();
            
            // Resize all the sticky headers
            wdg.$table.find('th').each(function() {
              wdg.$stick.find('#' + $(this).attr('id') + '-sticky').width($(this).width());
            });
          }
        } else { 
          cols.hide();
          
          // Drupal Sticky table
          if (typeof stick != 'undefined' && stick.length != 0) {
            stick.hide();
            
            // Resize all the sticky headers
            wdg.$table.find('th').each(function() {
              wdg.$stick.find('#' + $(this).attr('id') + '-sticky').width($(this).width());
            });
          }
        };
        
      };
      
      var updateCheck = function() {
        if ( $th.is(':visible')) {
          $checkbox.attr("checked", true);
        }
        else {
          $checkbox.attr("checked", false);
        };
      };
      
      $checkbox
        .bind('change', change )
        .bind('updateCheck', updateCheck )
        .trigger( 'updateCheck' );
    
    }; // EndOf: "__liInitActions()" ###
  
  
  
  
  
  
  
  /**
   * Widget Destroy Logic
   */
  
  var __destroy = function() {
    
    // Get the widget context.
    var wdg = $(this).data( 'MediaTable' );
    if ( !wdg ) return;
    
    
    // Remove the wrapper from the MediaTable.
    wdg.$wrap.after(wdg.$table).remove();
    
    // Remove MediaTable active class so media-query will not work.
    wdg.$table.removeClass('activeMediaTable');
    
    
    // Remove DOM reference to the widget context.
    wdg.$table.data( 'MediaTable', null );
    
  }; // EndOf: "__destroy()" ###
  
  
  
  
  
  
  
  /**
   * jQuery Extension
   */
  $.fn.mediaTable = function() {
    
    var cfg = false;
    
    // Default configuration block
    if ( !arguments.length || $.isPlainObject(arguments[0]) ) cfg = $.extend({},{
      
      // Teach the widget to create a toggle menu to declare column's visibility
      menu:   true,
      menuTitle:  Drupal.t('Columns'),
      
    t:'e'},arguments[0]);
    // -- default configuration block --
    
    
    
    // Items initialization loop: 
    if ( cfg !== false ) {
      $(this).each(function( i ){ __loop.call( this, cfg, i ); });
      
      
      
      
    // Item actions loop - switch throught actions
    } else if ( arguments.length ) switch ( arguments[0] ) {
    
      case 'destroy':
      $(this).each(function(){ __destroy.call( this ); });
      break;
    
    }
    
    
    // Mantengo la possibilitˆ di concatenare plugins.
    return this;
    
  }; // EndOf: "$.fn.mediaTable()" ###
  
  
})( jQuery );

