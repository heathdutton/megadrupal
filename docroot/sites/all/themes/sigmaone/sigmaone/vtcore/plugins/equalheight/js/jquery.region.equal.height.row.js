(function ($) {
  $.fn.regionEqualHeight = function () {    
    return this.each(function () {
      var maxHeight = 0,
          items = $(this).find('.region');  
      
      items.each(function() {
        var self = $(this),
            selfheight = self.height();
        
        // Start height recording
        if (self.hasClass('newrow') == true) {
          maxHeight = selfheight,
          rowIndex = items.index($(this)),
          count = 0;
        }
        
        // Add the element to count for slice.
        count++;
        if (typeof maxHeight == 'undefined' || selfheight > maxHeight) {
          maxHeight = selfheight;
        }
        
        // Applying the recorded height to current row.
        if (self.hasClass('lastrow')) {
          items.slice(rowIndex, count).css('min-height', maxHeight);
        }
      });
    });
 };
})(jQuery);
