(function($) {
  Drupal.behaviors.jqueryUIMiscInit = { 
      attach: function() {
        // Initialize pager
        $('.pager-item, .pager-next, .pager-last, .pager-first, .pager-previous, .pager-current').once('pager').UIhover();
        
        // Intialize actions
        if ($('.action-links li').hasClass('ui-state-default') == false) {
          $('.action-links li')
            .once('action')
            .addClass('ui-state-default ui-corner-all')
            .UIhover()
            .prepend('<span class="ui-icon ui-icon-circle-plus"></span>');
        }
        
        var Tabs = $('#tabs-content');    
        if (Tabs.length != 0) {
          Tabs.once('tabs').addClass('ui-widget-content ui-helper-reset').UIhover();
        }
     }
  };
  
  $.fn.UIhover = function (options) {
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
})( jQuery );
