(function ($) {

/**
 * Move a node in the nodequeues table from one queue to another via select list.
 *
 * This behavior is dependent on the tableDrag behavior, since it uses the
 * objects initialized in that behavior to update the row.
 */
Drupal.behaviors.nodequeueSuggestionsDrag = {
  attach: function (context, settings) {
    // tableDrag is required and we should be on the nodequeue suggestions admin page.
    if (typeof Drupal.tableDrag == 'undefined' || typeof Drupal.tableDrag.nodequeue_suggestions_dragdrop == 'undefined') {
      return;
    }

    var table = $('table#nodequeue_suggestions_dragdrop');
    // Get the nodequeues tableDrag object.
    var tableDrag = Drupal.tableDrag.nodequeue_suggestions_dragdrop;
    // Add a handler for when a row is swapped, update empty regions.
    tableDrag.row.prototype.onSwap = function (swappedRow) {
      checkEmptyRegions(table, this);
    };

    // Add a handler so when a row is dropped, update fields dropped into new regions.
    tableDrag.onDrop = function () {
      dragObject = this;
      // Use "region-message" row instead of "region" row because
      // "region-{region_name}-message" is less prone to regexp match errors.
      var regionRow = $(dragObject.rowObject.element).prevAll('tr.region-message').get(0);
      var regionName = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
      var regionField = $('select.subqueue-select', dragObject.rowObject.element);
      // Check whether the newly picked region is available for this node.
      if ($('option[value=' + regionName + ']', regionField).length === 0) {
        // If not, alert the user and keep the node in its old region setting.
        alert(Drupal.t('The node cannot be placed in this queue.'));
        // Simulate that there was a selected element change, so the row is put
        // back to from where the user tried to drag it.
        regionField.change();
      }
      else if ($(dragObject.rowObject.element).prev('tr').is('.region-message')) {
        var weightField = $('select.node-position', dragObject.rowObject.element);
        var oldRegionName = weightField[0].className.replace(/([^ ]+[ ]+)*subqueue-([^ ]+)([ ]+[^ ]+)*/, '$2');

        if (!regionField.is('.subqueue-' + regionName)) {
          regionField.removeClass('subqueue-' + oldRegionName).addClass('subqueue-' + regionName);
          weightField.removeClass('node-position-' + oldRegionName).addClass('node-position-' + regionName);
          regionField.val(regionName);
        }
      }
      table.find('td.position').each(function(i) {
        $(this).html(i + 1);
      });
    };

    var checkEmptyRegions = function (table, rowObject) {
      $('tr.region-message', table).each(function () {
        // If the dragged row is in this region, but above the message row, swap it down one space.
        if ($(this).prev('tr').get(0) == rowObject.element) {
          // Prevent a recursion problem when using the keyboard to move rows up.
          if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
            rowObject.swap('after', this);
          }
        }
        // This region has become empty.
        if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').length === 0) {
          $(this).removeClass('region-populated').addClass('region-empty');
        }
        // This region has become populated.
        else if ($(this).is('.region-empty')) {
          $(this).removeClass('region-empty').addClass('region-populated');
        }
      });
    };
  }
};

})(jQuery);
