(function ($) {
  /**
   * Move a block in the blocks table from one region to another via select list.
   *
   * This behavior is dependent on the tableDrag behavior, since it uses the
   * objects initialized in that behavior to update the row.
   */
  Drupal.behaviors.knockoutDrag = {
    attach: function (context, settings) {
      // tableDrag is required and we should be on the blocks admin page.
      if (typeof Drupal.tableDrag == 'undefined' || typeof Drupal.tableDrag.knockout == 'undefined') {
        return;
      }

      var table = $('table#blocks');
      var tableDrag = Drupal.tableDrag.knockout; // Get the blocks tableDrag object.

      // Add the behavior to each seed select list.
      $('select.knockout-seed', context).once('knockout-seed', function () {
        $(this).change(function (event) {
          // Make our new row and select field.
          var row = $(this).parents('tr:first');
          var select = $(this);
          tableDrag.rowObject = new tableDrag.row(row);

          // Manually update weights and restripe.
          tableDrag.updateFields(row.get(0));
          tableDrag.rowObject.changed = true;

          tableDrag.restripeTable();
          tableDrag.rowObject.markChanged();

          // Remove focus from selectbox.
          select.get(0).blur();
        });
      });
    }
  };


})(jQuery);
