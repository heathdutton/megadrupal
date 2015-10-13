(function( $ ) {
  /**
   * @file 
   * Collection of jQuery plugin extension to process region area plugin
   * Most of this plugin will assume the element processed is the 
   * dragged element unless otherwise notified.
   */
  
  // Plugin for change element column class
  // when user change the size via form
  $.fn.elementResize = function() {
    var value = this.val();
    
    this.parents('.element-wrapper').eq(0).attr('class',
        function(i, c) {
      return c.replace(/\bcol\S+/g, '');
    }).addClass('col-' + value);   
    
    return this;
  };

  // Plugin to toggle Clearfix and clear class when
  // user changes it from the form
  $.fn.elementClearfix = function() {
    if (this.attr('checked') != true) {
      this.parents('.element-wrapper').eq(0).removeClass('clearfix');
    }
    else {
      this.parents('.element-wrapper').eq(0).addClass('clearfix');    
    }
    
    return this;
  };

  // Plugin to toggle New Row and clear class when
  // user changes it from the form
  $.fn.elementNewrow = function() {
    if (this.attr('checked') != true) {
      this.parents('.element-wrapper').eq(0).removeClass('newrow');
    }
    else {
      this.parents('.element-wrapper').eq(0).addClass('newrow');    
    }
    
    return this;
  };

  // Plugin to toggle New Row and clear class when
  // user changes it from the form
  $.fn.elementLastrow = function() {
    if (this.attr('checked') != true) {
      this.parents('.element-wrapper').eq(0).removeClass('lastrow');
    }
    else {
      this.parents('.element-wrapper').eq(0).addClass('lastrow');    
    }
    
    return this;
  };
  
  // Plugin to do the updating process when an element
  // is dragged around. Currently it will change
  // the Parent value and the enabled or disabled value
  $.fn.elementMoveUpdate = function() {
    var parentID = this.parent().attr('id');
    var element =  $(this).find('.element-enabled');
    
    // Process parentID
    var modes = parentID.split('--');
    
    var element =  $(this).children('.region-configuration').find('.element-parent');
    
    // Moved to final target parent
    // element can be moved to disabled or
    // enabled region
    if (modes[0] == 'target') {
      mode = false;
      if (modes[1] == 'enabled') {
        mode = true;
      }
    
      if (modes[2] == 'area') {
        var element =  $(this).children('.region-configuration').find('.element-enabled');
      }
      
      if (element.hasClass('form-checkbox')) {
        element.attr('checked', mode);
      }
      
      if (element.hasClass('form-select')) {
        element.val(modes[2]);
      }
    };
    
    // Moved to other target parent
    if (modes[0] != 'target') {
      element.val(modes[1]);
    }
    
    return this;
  };
  
  
  // Plugin to reorder the weight when element is moved around
  $.fn.elementMoveUpdateOrder = function() {
    var Parent = this.parent(),
        items = this.parent().children('.element-wrapper');
    
    items.each(function() {
      var order = $('#' + Parent.attr('id') + ' > .element-wrapper').index($(this));
      $(this).find('.element-weight').eq(0).val(order);
    });
    
    return this;
  };
  
  
  // Plugin to hide or show the empty messages
  $.fn.elementToggleEmpty = function(minimum) {
    var Parent = this.parent(),
        count = Parent.children('.element-wrapper').length;
    
    if (count > minimum) {
      Parent.children('.emptyslot').hide();
    }
    else {
      Parent.children('.emptyslot').show();
    }
    
    return this;
  };
  
  // Function to enable the toggle button for showing the 
  // configuration forms
  $.fn.elementAttachToggle = function() {
    this.find('.region-title:not(h3)')
        .once()
        .prepend('<span class="region-open-config region-button ui-icon ui-icon-pencil" />')
        .find('span.region-open-config')
        .click(function() {
          $(this).parent()
                 .next('div.region-configuration', this)
                 .each(function() {
                    var stopper = false;
                    if ($(this).hasClass('open')) {
                      $(this).removeClass('open').slideUp();
                      stopper = true;
                    }
                    
                    // close every open box first
                    $('div.region-configuration').slideUp().removeClass('open');
                    
                    if (stopper != true) {
                      $(this).addClass('open').slideDown();
                    }
          });
        });   
     
    return this;
  };
  
  // Plugin to auto close configuration form when element is dragged
  $.fn.elementCloseConfig = function() {
    this.parent().find('.region-configuration').slideUp().removeClass('open');
    
    return this;
  };
  
  $.fn.elementResizePlaceholder = function() {
    var column = this.find('.element-column').eq(0).val();
      $('.ui-state-highlight')
        .addClass('col-' + column)
        .height(this.outerHeight())
        .css('marginRight', '1%');
    
    return this;
  };
  
})( jQuery );  