(function ($) {

/**
 * Allow / disallow moving <front> menu links
 */
Drupal.behaviors.restrictedHomeLink = {
  attach: function (context, settings) {
    // tableDrag is required and we should be on the menu overview admin page.
    if (typeof Drupal.tableDrag == 'undefined' || typeof Drupal.tableDrag['menu-overview'] == 'undefined') {
      return;
    }

    // Get the menu overview tableDrag object.
    var tableDrag = Drupal.tableDrag['menu-overview'];

    // Save current positions so we can unswap if needed
    tableDrag.onDrag = function() {
      $('#menu-overview tbody tr').each(function(index, element) {
        $(element).data({'prevPos': index});
      });
    };

    // Add a handler to prevent moving <front> items
    tableDrag.onDrop = function() {
      var frontMenuLinkMoved = false;

      // Detect if this drag-n-drop moved a <front> item
      $('#menu-overview tbody tr').each(function(index, element) {
        var prevPos = $(element).data('prevPos');
        var newPos = index;
        if ($(element).hasClass('front-menu-link') && (prevPos != newPos)) {
          frontMenuLinkMoved = true;
          return false;
        }
      });

      // If a <front> item has been moved, unswap
      if (frontMenuLinkMoved) {
        alert(Drupal.t('Menu links pointing at the front page cannot be moved.'));
        var newPos  = $('#menu-overview tbody tr').index(this.rowObject.element);
        var prevPos = $(this.rowObject.element).data('prevPos');
        var prevRow = $('#menu-overview tbody tr').get(prevPos);
        this.rowObject.swap((newPos < prevPos) ? 'after' : 'before', prevRow);
      }

    };
  }
};

})(jQuery);
