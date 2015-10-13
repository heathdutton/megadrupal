(function ($) {
  Drupal.behaviors.jqueryUITableInit = { 
    attach: function() {
      // Intialize table
      $('table').once('table').table();
   }
  };
  
  $.fn.table = function (options) {
   var defaults = {
       table: 'ui-table ui-widget-content',
       colgroup: 'ui-table-colgroup',
       caption: 'ui-table-caption',
       empty: 'ui-table-empty',
       header: 'ui-table-header ui-state-default',
       body: 'ui-table-body',
       row: 'ui-table-row ui-widget-content',
       rowOdd: 'ui-table-row-odd',
       rowEven: 'ui-table-row-even',
       hover: 'ui-state-hover'
    };
    options = $.extend(defaults, options);
        
    return this.each(function () {
      self = $(this);
      // Only add class if Drupal PHP hasn't process it.
      if ($(this).hasClass('ui-table') == false) {
        self.addClass(options.table);
        self.find('colgroup').addClass(options.colgroup);
        self.find('caption').addClass(options.caption);
        self.find('thead tr').addClass(options.header);
        self.find('tbody').addClass(options.body);
        self.find('tbody tr').addClass(options.row);
        self.find('tbody tr:odd').addClass(options.rowOdd);
        self.find('tbody tr:even').addClass(options.rowEven);
      }
      
      // Add hover bling bling
      self.find('tr').hover(function (event) {
        $(this).addClass(options.hover);
      }, function() {
        $(this).removeClass(options.hover);
      });
    });
  };
})(jQuery);
