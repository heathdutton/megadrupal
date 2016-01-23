(function( $ ) {
  /**
   * Hijack ajax for sane admin theme
   */
  Drupal.behaviors.regionAreaAjax = {
    attach: function(context, settings) {
      if ($('#edit-region').length != 0) {
        Drupal.ajax['edit-layout-process'].beforeSerialize = function (element, options) {
          Drupal.settings.ajaxPageState.theme = Drupal.settings.adminTheme.theme;
          Drupal.settings.ajaxPageState.theme_token = Drupal.settings.adminTheme.theme_token;

          // Allow detaching behaviors to update field values before collecting them.
          // This is only needed when field values are added to the POST data, so only
          // when there is a form such that this.form.ajaxSubmit() is used instead of
          // $.ajax(). When there is no form and $.ajax() is used, beforeSerialize()
          // isn't called, but don't rely on that: explicitly check this.form.
          if (this.form) {
            var settings = this.settings || Drupal.settings;
            Drupal.detachBehaviors(this.form, settings, 'serialize');
          }
    
          // Prevent duplicate HTML ids in the returned markup.
          // @see drupal_html_id()
          options.data['ajax_html_ids[]'] = [];
          $('[id]').each(function () {
            options.data['ajax_html_ids[]'].push(this.id);
          });
    
          // Allow Drupal to return new JavaScript and CSS files to load without
          // returning the ones already loaded.
          // @see ajax_base_page_theme()
          // @see drupal_get_css()
          // @see drupal_get_js()
          options.data['ajax_page_state[theme]'] = Drupal.settings.ajaxPageState.theme;
          options.data['ajax_page_state[theme_token]'] = Drupal.settings.ajaxPageState.theme_token;
          for (var key in Drupal.settings.ajaxPageState.css) {
            options.data['ajax_page_state[css][' + key + ']'] = 1;
          }
          for (var key in Drupal.settings.ajaxPageState.js) {
            options.data['ajax_page_state[js][' + key + ']'] = 1;
          }   
        };
      }
    }
  };
  
  /**
   * Main jQuery script to process the region area plugin manager form
   * Utilizing Drupal.behaviors for attaching the functions because
   * the region area plugin manager form will be fetched using ajax.
   */  
  Drupal.behaviors.regionArea = { 
    attach: function(context, settings) {
      
      var area = $('#regionarea-workarea');
      
      // Hide auto calculated items
      area.find('.element-weight').parent().hide();
      area.find('.element-enabled').parent().hide();
      area.find('.element-parent').parent().hide();
      
      // Change class when column change
      area.find('.element-column').change(function() {       
        $(this).elementResize();
      });
      
      // Add or remove class when user change value
      area.find('.element-clear').click(function() {
        $(this).elementClearfix();
      });
      
      // Add or remove class when user change value
      area.find('.element-newrow').click(function() {
        $(this).elementNewrow();
      });

      // Add or remove class when user change value
      area.find('.element-lastrow').click(function() {
        $(this).elementLastrow();
      });
      
      // Attach configuration form display control
      area.find('.element-wrapper').each(function() {
        $(this).elementAttachToggle()
               .elementToggleEmpty(0)
               .elementMoveUpdateOrder();
      });  
      
      area.find('.layout-parent')
          .not('#target--enabled--area')
          .find('h3.region-title')
          .once('slide-toggle')
          .click(function() {
        $(this).parent().toggleClass('status-close');
      });
      
      // Process the area draggable region
      area.find('div.drag-area-target').sortable({
        handler: 'clone',
        //connectWith: '.drag-area-target',
        items: '> div.drag-area-source',
        placeholder: 'ui-state-highlight',
        update: dragUpdate,
        start: dragStart,
        stop: dragStop,
        scroll: true,
        // revert: true,
        cursorAt: {top:10, left: 100},
        tolerance: 'pointer',
        cursor: 'move'
      }).sortable( "option", "connectWith", '.drag-area-target'); 
      
      // Process the region draggable region
      area.find('div.drag-region-target').sortable({
        handler: 'clone',
        //connectWith: '.drag-region-target',
        items: '> .drag-region-source',
        placeholder: 'ui-state-highlight',
        update: dragUpdate,
        start: dragStart,
        stop: dragStop,
        scroll: true,
        // revert: true,
        cursorAt: {top:10, left: 100},
        tolerance: 'pointer',
        cursor: 'move'
      }).sortable( "option", "connectWith", '.drag-region-target'); 
      
      // Process the block draggable region
      area.find('div.drag-block-target').sortable({
        handler: 'clone',
        //connectWith: '.drag-block-target',
        items: '> .drag-block-source',
        placeholder: 'ui-state-highlight',
        update: dragUpdate,
        start: dragStart,
        stop: dragStop,
        scroll: true,
        // revert: true,
        cursorAt: {top:10, left: 100},
        tolerance: 'pointer',
        cursor: 'move'
      }).sortable( "option", "connectWith", '.drag-block-target'); 
         
      // Function when user is updating the element
      function dragUpdate(event, ui) {
        var item = $(ui.item),
            previousParent = $(ui.sender);
        
        item.elementMoveUpdate();
        item.elementMoveUpdateOrder();
        item.elementToggleEmpty(0);
        
        if (previousParent.length != 0) {
          previousParent.children('.element-wrapper').elementMoveUpdateOrder();
        }
      }
      
      // Function when drag stop
      function dragStop(event, ui) {
        var item = $(ui.item);
        
       
        item.elementToggleEmpty(0);
        $(document).unbind('mousemove');
      }
      // Function when user is starting dragging
      function dragStart(event, ui) {
        var item = $(ui.item);

        item.elementResizePlaceholder();
        item.elementCloseConfig()
            .elementToggleEmpty(1);   
        
        var yOffset = 10,
            xOffset = 10;
        
        // Force ui helper to use mouse position
        $(document).mousemove(function(e) {
          $(ui.helper).offset({top: e.pageY - yOffset , left: e.pageX - xOffset});
        });
        
      }
    }
  };
  
})( jQuery );
