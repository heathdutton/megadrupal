(function($) {
  Drupal.behaviors.stickyTableColumns = {
    attach: function(context) {

      function createStickyColumns(table, scrollContainer) {
        var $stickyTable = table.clone();
        $stickyTable.find('th, td').not('.sticky-column').remove();
        $stickyTable.addClass('sticky-columns-table');
        var width = 0;
        table.find('tr').has('.sticky-column').eq(0).find('.sticky-column').each(function () {
          width += $(this).width();
        });
        $stickyTable.width(width);

        // Add copied table to the page.
        $stickyTable.insertAfter(scrollContainer);

        // Show copied table if scroll is used.
        var tableZeroLeft = table.offset().left;
        scrollContainer.scroll(function() {
          if (table.offset().left < tableZeroLeft) {
            $stickyTable.show();
          }
          else {
            $stickyTable.hide();
          }
        });
      }

      // Initialize sticky columns.
      $('table.sticky-columns', context).each(function() {
        $(this).wrap('<div class="sticky-columns-container" style="position:relative;"><div class="scroll-container" style="overflow:auto;">');
        var scrollContainer = $(this).parent();
        createStickyColumns($(this), scrollContainer);
      });
    }
  }
})(jQuery);
