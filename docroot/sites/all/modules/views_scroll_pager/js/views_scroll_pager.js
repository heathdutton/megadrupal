(function ($) {
  Drupal.behaviors.viewsScrollPager = {
    attach:function (context, settings) {
      $('.views-scroll-pager').once(function () {

        // Remember view container.
        var view_container = $(this);

        // Array with information about pagers: current page, total pages.
        var pager_settings = Drupal.settings.views_scroll_pager;

        if (!$.isEmptyObject(pager_settings)) {

          // Check pagers for existing in DOM.
          $.each(pager_settings, function(key, value) {

            // Compare View container with current key.
            if (view_container.hasClass(key)) {
              var pane = view_container.find('.pager-container');
              pane.jScrollPane({horizontalDragMinWidth: 10});
              var api = pane.data('jsp');

              // Data for calculating scroll focus.
              var totalPages = value.totalPages;
              var currentElement = view_container.find('ul li.pager-current');
              var container_width = view_container.width();
              var element_width = currentElement.outerWidth();
              var capacity = Math.floor(container_width / element_width);

              // Scroll to element.
              api.scrollToElement(currentElement, true);
              // Additional scroll bar position fix.
              if (totalPages - currentElement.index() > capacity / 2) {
                 api.scrollByX(-1 * container_width / 2 + element_width);
              }

            }
          });

        }
      });
    }
  };

})(jQuery);
