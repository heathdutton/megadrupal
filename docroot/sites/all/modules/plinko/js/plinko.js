Drupal.behaviors.plinko = {
  attach: function(context, settings) {
    (function ($) {
    
      // Plinko store configuration options
      plinko.configuration_setup();
    
      // Attach hide/show functionality
      plinko.toggle_toolbar();
    
      // Resize expanded toolbar on window resize
      $(window).resize(function() {
        plinko.set_expanded_toolbar_height();
      });
    
    
    })(jQuery);
  }
};

//  ////////////////////////////////////////////////////////////////////////////////////////////////////////

var plinko = {
  
  /**
  * Creates and saves some information about the current theme
  **/
  
  configuration_setup : function() {
    (function ($) {
      $("body").data('orig-padding', $("body").css('padding'));
    })(jQuery);    
  }
  
  /**
  * Toggles the expanded / Collapsed view of the toolbar
  */
  ,toggle_toolbar : function() {
    (function ($) {
    
      $(".plinko-plus, .plinko-minus").click(function(e){
        plinko.set_expanded_toolbar_height();
        $('#plinko-toolbar').toggleClass('plinko-closed plinko-open');
      
        if($('#plinko-toolbar').hasClass('plinko-closed')) {
          $("#container").css('opacity', 1);          
        }
        
      });
      
    })(jQuery);
  }
  
  /**
  * Sizes expanded toolbar height
  **/
  
  ,set_expanded_toolbar_height : function() {
    (function ($) {
      var h = Math.round($(window).height() * 0.85);
      $(".plinko-expanded").css('height', h + "px");      
    })(jQuery);
  }
  
  ,submit_form : function() {
    
  }
  
} // end plinko