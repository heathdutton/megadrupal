(function($) {

  /**
   * Run the SwipeView library setup when required.
   */
  Drupal.behaviors.setup_slideshows = {
    attach : function(context, settings) {

      var total_slide_panes = 3;

      // Iterate over each view that uses the SwipeView style.
      var views = settings.views_swipeview || {};

      $.each(views, function(view_name, view_data) {

        // The view can be inside the context or the context itself.
        var $view_wrapper = $(context).find(view_data.selector);
        if ($view_wrapper.length == 0 && $(context).is(view_data.selector)) {
          var $view_wrapper = $(context);
        }

        // Only setup a slideshow if the views are found in the context.
        if ($view_wrapper.length == 0) {
          return;
        }

        // Check if swipeview is supported by the current browser.
        if (!window.addEventListener) {
          $view_wrapper.hide();
          return;
        }

        // Get some elements of the slideshow.
        var $views_content = $view_wrapper.find('.swipeview-slides');
        var $rows = $view_wrapper.find('.views-row');
        var $controls = $view_wrapper.find('.swipeview-controls');

        // Empty the contents of the view, making way for the swipeview.
        $views_content.empty();

        var swipeview_settings = {
          'numberOfPages' : $rows.length,
          'hastyPageFlip' : view_data.hasty_page_flip,
          'loop' : view_data.loop,
          'snapThreshold' : view_data.snap_threshold
        };
        var swipeview = new SwipeView($views_content.get(0), swipeview_settings);

        // Attach swipeview to the DOM element for access/use by other scripts.
        $view_wrapper.data('swipeview', swipeview);

        // Attach each of the views rows to the slideshow.
        for (var i = 0; i < total_slide_panes; i++) {
          // The "masterPages" are populated in order of LTR off-screen
          swipeview.masterPages[i].appendChild($rows.get(i - 1));
        }
        post_render_operations();

        // Ensure content is shuffled into place when the panes scroll.
        swipeview.onFlip(function() {
          for (var pane_index = 0; pane_index < total_slide_panes; pane_index ++) {
            var pane = swipeview.masterPages[pane_index];
            var $pane = $(pane);
            var upcoming = pane.dataset.upcomingPageIndex;
            if (upcoming != pane.dataset.pageIndex) {
              $pane.empty().append($rows.get(upcoming));
            }
          }
          post_render_operations();
          Drupal.attachBehaviors($('.swipeview-slides').get(), settings);
        });

        $(window).load(post_render_operations);
        $(window).resize(recalculate_slide_height)

        // Functions to execute after a rendering/switching has taken place.
        function post_render_operations() {
          recalculate_slide_height();
          update_slide_active_states();
        }
        // Recalculate the non-automatic height of the wrapper container.
        function recalculate_slide_height() {
          $views_content.height('auto');
          var $middle_pane_content = $(swipeview.masterPages[1]).find('.views-row');
          $views_content.height($middle_pane_content.height());
        }
        // Add active and inactive classes to slides.
        function update_slide_active_states() {
          var $pages = $(swipeview.masterPages);
          $pages.removeClass('inactive');
          $pages.filter(':not(.swipeview-active)').addClass('inactive');
        }

        // Enable slide navigation.
        $controls.find('.navigation-link').click(function() {
          swipeview[$(this).data('navigation-type')]();
        });
        // Enable the slide pager.
        $controls.find('.pager-item').click(function() {
          swipeview.goToPage($(this).data('navigate-to-page'));
        });

      });
    }
  };

})(jQuery);
