/**
 * @file
 * Views Blocksit breakpoint management and setup JS.
 */

(function($) {
  "use strict";
  /**
   * Run the Blocksit library setup when required.
   */
  Drupal.behaviors.setup_blocksit = {
    attach: function (context, settings) {
      var grids = settings.views_blocksit;

      $.each(grids, function(grid_name, grid_data) {

        var brm = new BlocksitResponsiveManager(grid_data);
        brm.calculate_cols();

        $(grid_data.selector, context).once(function() {
          // Only bind these once to avoid memory leaks.
          $(window).bind("resize.views_blocksit load.views_blocksit", function() {
            brm.calculate_cols();
          });
        });
      });
    }
  };

  /**
   * Manage the Views Blocksit responsive and break points.
   *
   * @param grid_data
   *  The grid_data from settings.views_blocksit passed in by PHP.
   */
  function BlocksitResponsiveManager(grid_data) {
    this.grid_data = grid_data;
    this.current_cols = 0;
  }

  /**
   * Calculate the number of columns that should be shown, based on
   * the container width, not the window width.
   */
  BlocksitResponsiveManager.prototype.calculate_cols = function() {
    var breakPoints = this.grid_data.breakPoints,
      new_width = $(this.grid_data.selector).width(),
      set_cols = this.grid_data.numOfCol, i = 0;

    if (breakPoints) {
      // Loop through all of the break points.
      // This assumes that the breakpoints will be in order of HIGHEST to LOWEST.
      for (i; i < breakPoints.length; i++) {
        var parts = breakPoints[i].split(",");

        // Check if the container meets any of the breakpoints.
        if (new_width <= parts[0]) {
          set_cols = parts[1];
        }
      }
    }

    // Passing a string to offsetY causes a strange height bug.
    if (parseInt(this.grid_data.offsetY) >= 0) {
      $(this.grid_data.selector + " .views-blocksit-grid").BlocksIt({
        numOfCol: set_cols,
        offsetX: parseInt(this.grid_data.offsetX),
        offsetY: parseInt(this.grid_data.offsetY),
        blockElement: '.blocksit-block'
      });
    } else {
      $(this.grid_data.selector + " .views-blocksit-grid").BlocksIt({
        numOfCol: set_cols,
        offsetX: parseInt(this.grid_data.offsetX),
        blockElement: '.blocksit-block'
      });
    }

    if (set_cols !== this.current_cols) {
      this.current_cols = set_cols;
    }
  };

})(jQuery);
