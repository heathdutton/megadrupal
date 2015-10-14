(function ($) {
  $.fn.equalheightrow = function (options) {
   var defaults = {
      rowClass: 'rows',
      children: false,
      useHeight: false
   };
   options = $.extend(defaults, options);
    
   return this.each(function () {
    var self = $(this);
    
    if (options.children == false) {
      item = self.children();
    }
    else {
      item = self.find(options.children);
    }
    
    // Break if target has no children
    if (item.length == 0) {
      return false;
    }
    
    // function for modifying the catalog grid mode  
    var itemWidth = item.width(), parentWidth = self.width();     
    // estimate how many item can fit a single row
    var itemCanFit = Math.floor(parentWidth / itemWidth);
     
    // mark the row
    var i = 1, key = itemCanFit, b = 0;
    item.each(function(){
      if ((b * itemCanFit) == item.index($(this))) {
        $(this).addClass(options.rowClass + '-row').css('clear', 'both');
        b++;
      }
    });
     
    // calculate the height and find the tallest
    var itemRow = $('.' + options.rowClass + '-row');
     
    itemRow.each(function(){
      rowIndex = item.index($(this));
      row = item.slice(rowIndex, itemCanFit + rowIndex);
     
      // reset the row height and assign the first row element height
      rowHeight = row.eq(0).height();
       
      if (options.useHeight == true) {
        row.each(function() {
          if (rowHeight < $(this).height()) {
            rowHeight = $(this).height();
          }
        }).height(rowHeight);
      } 
      else {
        row.each(function() {
          if (rowHeight < $(this).height()) {
            rowHeight = $(this).height();
          }
        }).css('min-height', rowHeight);
      } 
    });
         
    });
  };
})(jQuery);
