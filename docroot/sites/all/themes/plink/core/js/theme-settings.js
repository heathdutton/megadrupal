Drupal.behaviors.plinkthemesettings = {
  // Constructor
  attach: function(context, settings) {
  (function ($) {
        
      // Change the layout options radios into clickable things 
      plink.plink_settings_fancy_radios();
    
      // Handle Changes to the theme settings and update the preview 
      plink.plink_settings_update_master_preview_events();
    
      // Preview updates for the full width region toggles
      plink.full_width_region_events();
            
      // Master Layout
      plink.plink_settings_custom_grid_thumbnail_preview_event_handlers($("#edit-master"));
      
      // Update full width region
      plink.plink_settings_update_master_preview($("#edit-main-layout .active"));          
      
      // Media Query 1 Layout
      plink.plink_settings_custom_grid_thumbnail_preview_event_handlers($("#edit-content-layouts-mq1"));
      
      // Media Query 2 Layout
      plink.plink_settings_custom_grid_thumbnail_preview_event_handlers($("#edit-content-layouts-mq2"));
      
      // Media Query 3 Layout
      plink.plink_settings_custom_grid_thumbnail_preview_event_handlers($("#edit-content-layouts-mq3"));
      
      
      
  })(jQuery);
  }

};


// -----------------------------------------------------------------------------------------------------------------
// Plink theme settings page javascript object
var plink = {
  
  /* 
    Change the layout radio options into fancy icons
  */
  plink_settings_fancy_radios: function() {
    (function ($) {

      // Main Layout Selection Radio Options
      var radios = $(".fancy-radios .form-item");
      radios.find('input').hide();
      radios.find(":checked").parents('.form-item').addClass('active');
      
      // Click handling
      radios.click(function(e){
        e.preventDefault();
        $(this).parents('.fancy-radios:first').find('.form-item').removeClass('active');
        $(this).addClass('active');
        $(this).find('input').attr('checked','checked');
      });
            
    })(jQuery);
  }
  
  
  /* 
    Handle Changes to the theme settings and update the preview
  */
  ,plink_settings_update_master_preview_events: function(){
  (function ($) {
  
    // Master layout theme options
    var layout_options = $("#edit-main-layout .form-item"); 
    
    layout_options.click(function(e){
      e.preventDefault();
      plink.plink_settings_update_master_preview($(this));
    });
  
    // initial setup
    plink.plink_settings_update_master_preview($("#edit-main-layout .active"));
    
    
  })(jQuery);
  }
  
  
  /* 
    The update preview function for the mater layout preview
  */
  ,plink_settings_update_master_preview: function(active_selection) {
  (function ($) {  
    var target = active_selection.find('.layout-block');
    
    plink.master_preview = $(".content-region");
    plink.master_preview.html(target.html());
    plink.master_preview.attr("class","content-region");
    plink.master_preview.addClass(target.attr('class'));
  
  })(jQuery);  
  }
  
  /*
    Handle initialization and the toggling of different grid selects
    This function updates text fields with the appropriate grid options
  */
  ,plink_settings_grid_select_handlers: function() {
  (function ($) {  
        
    // store grid selections for later
    $("body").data('grid_select_columns', plink.grid_columns.find('option') );
    
    /*
      Change handlers
    */
    
    // Grid width selector
    plink.grid_width.change(function(){
      
     var opts = {
        gwidth : plink.grid_width.val()
        ,grids : plink.grid_columns.val()
      }
      
      // MAIN
      opts.field = $("#edit-main-secondary-width");
      plink.plink_settings_generate_grid_select(opts);
      opts.field = $("#edit-main-tertiary-width");
      plink.plink_settings_generate_grid_select(opts);      
      
      $('body').data('current_grids', plink.grid_columns.find(":selected").val());
      plink.grid_columns.html(''); // truncate options
      plink.grid_columns.append($('body').data('grid_select_columns'));      
      
    });
    
    
    // Media Query Grid Widths
    
    // Media Query 1
   $('#edit-mq1-grid-width').change(function(){
      
     var opts = {
        gwidth : $('#edit-mq1-grid-width').val()
        ,grids : plink.grid_columns.val()
      }
      
      opts.field = $("#edit-mq1-secondary-width");
      plink.plink_settings_generate_grid_select(opts);
      opts.field = $("#edit-mq1-tertiary-width");
      plink.plink_settings_generate_grid_select(opts);
      
    });
    
    // Media Query 2
     $('#edit-mq2-grid-width').change(function(){

       var opts = {
          gwidth : $('#edit-mq2-grid-width').val()
          ,grids : plink.grid_columns.val()
        }
          
      opts.field = $("#edit-mq2-secondary-width");
      plink.plink_settings_generate_grid_select(opts);
      opts.field = $("#edit-mq2-tertiary-width");
      plink.plink_settings_generate_grid_select(opts);
    });
    
    // Media Query 3
    $('#edit-mq3-grid-width').change(function(){

       var opts = {
          gwidth : $('#edit-mq3-grid-width').val()
          ,grids : plink.grid_columns.val()
        }
        
      opts.field = $("#edit-mq3-secondary-width");
      plink.plink_settings_generate_grid_select(opts);
      opts.field = $("#edit-mq3-tertiary-width");
      plink.plink_settings_generate_grid_select(opts);
    });
    
    
    
    // Grid columns selector
    plink.grid_columns.change(function(){
      plink.grid_width.change();
      $('#edit-mq1-grid-width').change();
      $('#edit-mq2-grid-width').change();
      $('#edit-mq3-grid-width').change();
    });
    
    // Grid units selector
    plink.grid_unit.change(function(){
      plink.grid_width.change();
    });
    
    // Grid enabled/disabled
    plink.grid_enabled.change(function(){
            
      if($(this).attr('checked')) {
        plink.grid_width.change();
      } else {
        plink.plink_settings_destroy_grid_selects();
      }
      
    });
  
  
    // Initialization
    if(plink.grid_enabled.val()) {
      plink.grid_width.change();
      $('#edit-mq1-grid-width').change();
      $('#edit-mq2-grid-width').change();
      $('#edit-mq3-grid-width').change();
    }
    
  
  })(jQuery);  
  }
  
  /*
    Replace a text field with a select element and generate grid options
  */
  ,plink_settings_generate_grid_select: function(settings) {
  (function ($) {  
    
    var defaults = {field:null, gwidth:960, grids:12, default_value: 4};
    var options = $.extend(defaults, settings);
    if(plink.grid_unit.val() == "%") { options.gwidth = 96; }
    var unit_val = options.gwidth / options.grids;
  
    if(options.field == null) { throw "Invalid field passed to generate grid select"; }
        
    // Check if the field has a current value and process it
    var cval = options.field.val();
        
    if(cval) { 
      if(cval > Math.ceil(options.grids)) {
        options.default_value = cval / unit_val; 
      } else {
        options.default_value = cval; 
      } 
    }    
                    
    // parent container
    var container = options.field.parents('.form-item:first');
    
    // hide suffix if there is one
    container.find(".field-suffix").hide();
    
    // current value of the field
    var current_value = (options.field.val() > 0) ? options.field.val() : options.default_value;
    
    // Create select element
    var select = $("<select />");
    
    // add the current fields properties
    select.addClass(options.field.attr("class"));
    select.addClass('grid-options-select');
    select.attr('id',options.field.attr('id'));
    select.attr('name', options.field.attr("name"));
    
    // remove the current field
    options.field.remove();
    
    // Create options
    var i = 0;
    while(i < options.grids) {
      var opt = $("<option />");
      opt.attr('value', i);
      if(i == options.default_value) {
        opt.attr("selected",'selected');
      } 
      opt.text(i + " grids [" + Math.ceil(unit_val * i) + plink.grid_unit.val() + "]");
      
      opt.appendTo(select);                  
      i++;
    }
    
    // Add the select option into the dom
    container.append(select);
    
  })(jQuery);  
  }
  
  /*
    Handle the disabling of grid systems
  */
  ,plink_settings_destroy_grid_selects: function() {
  (function ($) {
  
    var unit_val = Math.ceil( plink.grid_width.val() / plink.grid_columns.val() );
    var selects = $(".grid-options-select");
    
    $.each(selects,function(k,v){
          
      var elem = selects.eq(k);
      var container = elem.parents('.form-item:first');
      elem.removeClass('grid-options-select');
      
      var txt = $('<input />').attr('type', 'textfield');
      txt.attr("id", elem.attr('id'));
      txt.attr("name", elem.attr("name"));
      txt.addClass(elem.attr("class"));
      
      
      txt.attr('value', elem.val() * unit_val );
      
      elem.remove();
      
      container.append(txt);
      
    });
  
  })(jQuery);  
  }
  
  /*
    Add events to the secondary and tertiary region selects to adjust for preview
  */
  
  ,plink_settings_region_width_preview_events: function() {
  (function ($) {
  
    // MAIN
    $("#edit-main-secondary-width, #edit-main-tertiary-width").live("change", function(){
      var thumbs = $("#edit-main-layout");
      plink.plink_settings_region_width_preview_event_handlers($("#edit-main-secondary-width"), $("#edit-main-tertiary-width"), thumbs, true);
    });
    
    // Media query 1
    $("#edit-mq1-secondary-width, #edit-mq1-tertiary-width").live("change", function(){
      var thumbs = $("#edit-responsive-layouts-mq1");
      plink.plink_settings_region_width_preview_event_handlers($("#edit-mq1-secondary-width"), $("#edit-mq1-tertiary-width"), thumbs, false);
    });
    
    // Media query 2
    $("#edit-mq2-secondary-width, #edit-mq2-tertiary-width").live("change", function(){
      var thumbs = $("#edit-responsive-layouts-mq2");
      plink.plink_settings_region_width_preview_event_handlers($("#edit-mq2-secondary-width"), $("#edit-mq2-tertiary-width"), thumbs, false);
    });
    
    // Media query 3
    $("#edit-mq3-secondary-width, #edit-mq3-tertiary-width").live("change", function(){
      var thumbs = $("#edit-responsive-layouts-mq3");
      plink.plink_settings_region_width_preview_event_handlers($("#edit-mq3-secondary-width"), $("#edit-mq3-tertiary-width"), thumbs, false);
    });
    
  
  })(jQuery);  
  }
  
  /*
    Secondary and Teritary region width select handlers
    Adjusts the thumbnails preview (and the master layout preview for master content layouts) 
  */
  ,plink_settings_region_width_preview_event_handlers: function(secondary, tertiary, selector, master) {
  (function ($) {
        
    // tertiary secondary value
    var sec = secondary.val();
    var ter = tertiary.val();
    var unit_val = plink.grid_width.val() / plink.grid_columns.val();
    
    // if grid system enabled we need to convert grids to pixels
    if(plink.grid_enabled.val()) {
      sec *= unit_val;
      ter *= unit_val;
    }
        
    // convert widths into percentages
    sec = Math.ceil((sec / plink.grid_width.val()) * 100);
    ter = Math.ceil((ter / plink.grid_width.val()) * 100);
    b = 100; // base
        
    selector.find(".option1 .prim").css({width: (b - sec - ter) + "%"});
    selector.find(".option1 .sec").css({width: sec + "%"});
    selector.find(".option1 .ter").css({width: ter + '%'});

    selector.find(".option2 .prim").css({width: (b - sec - ter) + "%"});
    selector.find(".option2 .sec").css({width: sec + "%"});
    selector.find(".option2 .ter").css({width: ter + '%'});

    selector.find(".option3 .prim").css({width: (b - sec - ter) + "%"});
    selector.find(".option3 .sec").css({width: sec + "%"});
    selector.find(".option3 .ter").css({width: ter + '%'});

    selector.find(".option4 .prim").css({width: (b - sec - ter) + "%"});
    selector.find(".option4 .sec").css({width: sec + "%"});
    selector.find(".option4 .ter").css({width: ter + '%'});

    selector.find(".option5 .prim").css({width: (b - sec - ter) + "%"});
    selector.find(".option5 .sec").css({width: sec + "%"});
    selector.find(".option5 .ter").css({width: ter + '%'});

    selector.find(".option6 .prim").css({width: (b - sec - ter) + "%"});
    selector.find(".option6 .sec").css({width: sec + "%"});
    selector.find(".option6 .ter").css({width: ter + '%'});

    selector.find(".option7 .prim").css({width: b - sec + "%"});
    selector.find(".option7 .sec").css({width: sec + "%" });
    selector.find(".option7 .ter").css({width: '100%'});

    selector.find(".option8 .prim").css({width: b - sec + "%"});
    selector.find(".option8 .sec").css({width: sec + "%"});
    selector.find(".option8 .ter").css({width: '100%'});

    selector.find(".option9 .prim").css({width: "100%"});
    selector.find(".option9 .sec").css({width: sec + "%"});
    selector.find(".option9 .ter").css({width: ter + '%'});

    selector.find(".option10 .prim").css({width: "100%"});
    selector.find(".option10 .sec").css({width: sec + "%"});
    selector.find(".option10 .ter").css({width: ter + '%'});

    selector.find(".option11 .prim").css({width: (b - sec) + '%'});
    selector.find(".option11 .sec").css({width: sec + '%'});
    selector.find(".option11 .ter").css({width: (b - sec) + '%'});

    selector.find(".option12 .prim").css({width: (b - sec) + '%'});
    selector.find(".option12 .sec").css({width: sec + "%"});
    selector.find(".option12 .ter").css({width: (b - sec) + '%'});

    // selector.find(".option13 .prim").css({width: (b - sec) + '%'});
    // selector.find(".option13 .sec").css({width: sec + "%"});
    // selector.find(".option13 .ter").css({width: (b - sec) + '%'});

    // selector.find(".option14 .prim").css({width: (b - sec) + '%'});
    // selector.find(".option14 .sec").css({width: sec + "%"});
    // selector.find(".option14 .ter").css({width: (b - sec) + '%'});
    
    
    // If this is the master layout then update the master preview
    if(master) {
      plink.plink_settings_update_master_preview(selector.find('.active'));
    }
    
  })(jQuery);      
  }
  
  /* 
    Adds the events for the handles that check the checked or unchecked full width regions
  */
  ,full_width_region_events: function() {
    (function ($) {
    
      // fill in the blanks
      var full_width_inputs = $('.form-item-full-width-regions input');

      $.each(full_width_inputs,function(i,v){
        plink.update_full_width_region(full_width_inputs.eq(i));
      });

      // On change
      $('.form-item-full-width-regions input').change(function() {    
        plink.update_full_width_region($(this));
      });
      
    })(jQuery);      
  }
  
  /* 
    Handles the checked or unchecked full width regions
  */
  ,update_full_width_region : function(regionObj) {
    (function ($) {      
      if(regionObj.attr('checked') == true){
        $(".page-layout-preview ." + regionObj.val()).css('width','100%');
      }else {
        $(".page-layout-preview ." + regionObj.val()).css('width','60%');
      }
    })(jQuery);      
  }
  
  /* Adds event handling to changes to the custom grid settings for thumbnail previews */
  ,plink_settings_custom_grid_thumbnail_preview_event_handlers: function(context) {
    (function ($) {      
      
      var gw = context.find(".grid-width");
      var gc = context.find(".grid-columns");
      var gg = context.find(".grid-gutter");
    
      var handler = function() { 
        plink.process_custom_grid_sidebar_select(context.find(".secondary-width"), gw.val(), gc.val(), gg.val(), context);
        plink.process_custom_grid_sidebar_select(context.find(".tertiary-width"), gw.val(), gc.val(), gg.val(), context);
        plink.process_custom_grid_change_for_thumbnail_preview(context); 
        
        if(context.attr("id") == "edit-master") {
          plink.plink_settings_update_master_preview($("#edit-main-layout .active"));          
        }
        
      };

      // Add keyup event handling to grid settings
      gw.keyup(handler);
      gc.keyup(handler);
      gg.keyup(handler);
    
      // Change the secondary and tertiary text inputs into selects
      plink.process_custom_grid_sidebar_select(context.find(".secondary-width"), gw.val(), gc.val(), gg.val(), context);
      plink.process_custom_grid_sidebar_select(context.find(".tertiary-width"), gw.val(), gc.val(), gg.val(), context);
      
      // call handler initialy
      plink.process_custom_grid_change_for_thumbnail_preview(context); 
      
    })(jQuery);
  }
  
  /* Processing callback for when the values change */
  ,process_custom_grid_change_for_thumbnail_preview: function(context) {
    (function ($) {      
    
      var width = context.find(".grid-width").val();
      var columns = context.find(".grid-columns").val();
      var gutter = context.find(".grid-gutter").val();
      var secondary = context.find(".secondary-width").val();
      var tertiary = context.find(".tertiary-width").val();
        
      // convert widths into percentages
      sec = Math.ceil((secondary / columns) * 100);
      ter = Math.ceil((tertiary / columns) * 100);
      b = 100; // base
        
      context.find(".option1 .prim").css({width: (b - sec - ter) + "%"});
      context.find(".option1 .sec").css({width: sec + "%"});
      context.find(".option1 .ter").css({width: ter + '%'});

      context.find(".option2 .prim").css({width: (b - sec - ter) + "%"});
      context.find(".option2 .sec").css({width: sec + "%"});
      context.find(".option2 .ter").css({width: ter + '%'});

      context.find(".option3 .prim").css({width: (b - sec - ter) + "%"});
      context.find(".option3 .sec").css({width: sec + "%"});
      context.find(".option3 .ter").css({width: ter + '%'});

      context.find(".option4 .prim").css({width: (b - sec - ter) + "%"});
      context.find(".option4 .sec").css({width: sec + "%"});
      context.find(".option4 .ter").css({width: ter + '%'});

      context.find(".option5 .prim").css({width: (b - sec - ter) + "%"});
      context.find(".option5 .sec").css({width: sec + "%"});
      context.find(".option5 .ter").css({width: ter + '%'});

      context.find(".option6 .prim").css({width: (b - sec - ter) + "%"});
      context.find(".option6 .sec").css({width: sec + "%"});
      context.find(".option6 .ter").css({width: ter + '%'});

      context.find(".option7 .prim").css({width: b - sec + "%"});
      context.find(".option7 .sec").css({width: sec + "%" });
      context.find(".option7 .ter").css({width: '100%'});

      context.find(".option8 .prim").css({width: b - sec + "%"});
      context.find(".option8 .sec").css({width: sec + "%"});
      context.find(".option8 .ter").css({width: '100%'});

      context.find(".option9 .prim").css({width: "100%"});
      context.find(".option9 .sec").css({width: sec + "%"});
      context.find(".option9 .ter").css({width: ter + '%'});

      context.find(".option10 .prim").css({width: "100%"});
      context.find(".option10 .sec").css({width: sec + "%"});
      context.find(".option10 .ter").css({width: ter + '%'});

      context.find(".option11 .prim").css({width: (b - sec) + '%'});
      context.find(".option11 .sec").css({width: sec + '%'});
      context.find(".option11 .ter").css({width: (b - sec) + '%'});

      context.find(".option12 .prim").css({width: (b - sec) + '%'});
      context.find(".option12 .sec").css({width: sec + "%"});
      context.find(".option12 .ter").css({width: (b - sec) + '%'});

    })(jQuery);
  }
  
  /* Change text inputs into smart select options */
  ,process_custom_grid_sidebar_select : function(field, width, cols, gutter, context) {
    (function ($) {      
    
      var curval = field.val();
      var unit = (width) / cols;
      var select = $("<select />")
      select.attr("name", field.attr('name'));
      select.attr("id", field.attr('id'));
      select.attr('class',field.attr("class"));
      select.attr('rel', field.attr("rel"));
      select.css('width', '100%');
      
      var parent = field.parent();
      var options = '';
      var i = 0;
    
      while(i <= cols) {
        var val = unit * i;
        var selected = '';        
        if(i == curval) { selected = " selected=\"selected\""; }
        options += "<option value=\"" + i + "\""+selected+">Grid " + i + " [" + Math.round(val) + "px]</option>";
        i++;
      }
  
      select.append(options);
  
      select.change(function() {
        plink.process_custom_grid_change_for_thumbnail_preview(context); 
        if(context.attr("id") == "edit-master") {
          plink.plink_settings_update_master_preview($("#edit-main-layout .active"));          
        }
      });
      
  
      field.remove();
      parent.append(select);
      
      
    
    })(jQuery);
  }
  
  
  
  
  
  
  
  
}