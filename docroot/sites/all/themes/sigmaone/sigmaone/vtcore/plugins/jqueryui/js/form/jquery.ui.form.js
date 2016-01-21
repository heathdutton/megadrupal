(function($) {
  Drupal.behaviors.jqueryUIFormInit = { 
      attach: function(context) {
        var opts = {};
        // Fieldset is attached using modified collapse.js
        
        // Buttons
        $('input.form-submit').once('submit').Fhover();
        $('a.button').once('a.button').Fhover();
        $('input.form-button').once('fbutton').Fhover();
        $('input.form-image_button').once('ibutton').Fhover();
              
        // Textfield
        $('input.form-text').once('textfield').input();
        
        // Textarea
        $('textarea').once('textarea').input();
        
        // Select box
        // IE Can't handle this
        if (!$.browser.msie) {
          $('select').once('select').input();
        }
        
        // Vertical tabs
        $('.vertical-tabs').once('vtabs').verticaltabs();         
        
        // Password confirm
        $('.form-type-password-confirm').once('password').password();
        
        // Checkbox
        // Not stable, so it is removed for now
        // $('input.form-checkbox').checkbox();
        
        // Radio
        // Not stable, so it is removed for now
        // $('input.form-radio').radiobutton();
     }
   };
  
  /**
   * Altering autocomplete to play nice with jQuery UI.
   */
  if (Drupal.jsAC) {
    Drupal.jsAC.prototype.populatePopup = function () {
      var $input = $(this.input);
      var position = $input.position();
      // Show popup.
      if (this.popup) {
        $(this.popup).remove();
      }
      this.selected = false;
      this.popup = $('<div id="autocomplete"></div>')[0];
      this.popup.owner = this;
      $(this.popup).css({
        top: parseInt(position.top + this.input.offsetHeight, 10) + 'px',
        left: parseInt(position.left, 10) + 'px',
        width: $input.innerWidth() + 'px',
        display: 'none'
      }).addClass('ui-corner-all ui-widget-content');
   
      $input.before(this.popup);
  
      // Do search.
      this.db.owner = this;
      this.db.search(this.input.value);
    };
  }
  
  $.fn.input = function (options) {
    var defaults = {
        hoverClass: 'ui-state-hover',
        focusClass: 'ui-state-focus',
        activeClass: 'ui-state-active',
        disabledClass: 'ui-state-disabled',
        baseClass: 'ui-widget-content ui-corner-all'
    };
    
    options = $.extend(defaults, options);
    
    return this.each(function() {
      var self = $(this);
      
      // Check activer state
      options.disabled = self.is(':disabled');
      
      if (options.disabled == true) {
        self.addClass(options.disabledClass);
        return;
      }
      // Make all fonts the same non-bold
      self.css('font-weight', 'normal');
      
      // Register hover function
      self.hover(function() {
        $(this).addClass(options.hoverClass);
      }, function() {
        $(this).removeClass(options.hoverClass);
      });

      self.click(function() {
        $(this).addClass(options.activeClass);
        $(this).removeClass(options.hoverClass);
      });
      
      self.focusout(function() {
        $(this).removeClass(options.activeClass);
      });
      
      self.focus(function() {
        $(this).addClass(options.focusClass);
      });
      
      self.blur(function() {
        $(this).removeClass(options.focusClass);
      });
      
      self.mousedown(function() {
        $(this).addClass(options.activeClass);
      });
      
      self.mouseup(function() {
        $(this).removeClass(options.activeClass);
      });
    });
  };

  /**
   * Plugin for changing class according
   * to jQueryUI specification
   */
  $.fn.Fhover = function (options) {
    var defaults = {
        hoverClass: 'ui-state-hover'
    };
    
    options = $.extend(defaults, options);
    
    return this.each(function() {
      $(this).hover(function() {
        $(this)
        .toggleClass(options.hoverClass);
      }, function() {
        $(this)
        .toggleClass(options.hoverClass);
      });
    });
  };
  
  
  /**
   * Plugin for formatting drupal password fields
   */
  $.fn.password = function (options) {
    var defaults = {
        suggestionClass: 'ui-corner-all ui-state-error ui-widget-content',
        indicatorClass: 'ui-widget ui-widget-content ui-corner-all',
        indicatorContentClass: 'ui-progressbar-value ui-widget-header ui-corner-all'
    };

    options = $.extend(defaults, options);

    return this.each(function(context) {
      var self = $(this);
      
      self.find('.password-suggestions')
          .removeClass('description')
          .addClass(options.suggestionClass);
      
      self.find('.password-indicator')
          .addClass(options.indicatorClass)
          .children('.indicator')
          .addClass(options.indicatorContentClass);
    });
  };
  
  /**
   * Plugin to format vertical tabs according to jQueryUI format
   */
  $.fn.verticaltabs = function (options) {
    var defaults = {
        wrapperClass: 'ui-widget-content ui-corner-all',
        hoverClass: 'ui-state-hover',
        tabClass: 'ui-widget-header ui-no-border-top',
        activeTabClass: 'ui-widget-content ui-no-border-right'
    };
    
    options = $.extend(defaults, options);
    
    return this.each(function() {
      var self = $(this),
          tabList = self.find('.vertical-tab-button');
        
      // Initialization
      self.addClass(options.wrapperClass)
          .find('.vertical-tab-button:not(.selected)').addClass(options.tabClass)
          .find('.selected').addClass(options.activeTabClass);
      
      self.find('.vertical-tab-button.last').addClass('ui-corner-bl');
      self.find('.vertical-tab-button.first').addClass('ui-corner-tl');
      self.find('.vertical-tabs-list').addClass('ui-corner-left');
      
      // Remove first legend and .fieldset-wrapper
      self.children('.vertical-tabs-panes')
          .children('fieldset')
          .children('legend')
          .hide()
          .next()
          .removeClass('ui-widget-content');
      
      // Register click function
      self.find('.vertical-tab-button a').click(function(e) {         
        $(this).parent()
               .removeClass(options.tabClass)
               .addClass(options.activeTabClass);
        
        // Resetting all other element
        self
          .find('.vertical-tab-button:not(.selected)')
          .addClass(options.tabClass)
          .removeClass(options.activeTabClass);
    
        e.preventDefault(); 
      });
      
      
      // Register hover function
      self.find('.vertical-tab-button').hover(function() {
        $(this).addClass(options.hoverClass);
      }, function() {
        $(this).removeClass(options.hoverClass);
      });

    });
  };
  
  // Fix for field ui select element disabled state
  // We are "altering" the jQuery fn here
  var _fieldUIPopulateOptions = $.fn.fieldUIPopulateOptions;
  $.fn.fieldUIPopulateOptions = function(options, selected) {
    
    // Applying default field fn function first
    _fieldUIPopulateOptions.apply(this, arguments);
    
    return this.each(function() {
      if ($(this).attr('disabled') == false) {
        $(this).removeClass('ui-state-disabled');
      }
      else {
        $(this).addClass('ui-state-disabled');
      }
    });
  };
  
})( jQuery );
