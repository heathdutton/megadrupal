(function ($) {
  // The following line will prevent a JavaScript error if this file is included and vertical tabs are disabled.
  Drupal.verticalTabs = Drupal.verticalTabs || {};

  Drupal.verticalTabs.node_level_blocks = function() {
    if ($('#edit-node_level_blocks-enabled').size()) {
      var result;

      if ($('#edit-node_level_blocks-enabled').attr('checked')) {
        result = Drupal.t('Enabled');

        if ($('#edit-node_level_blocks-regions').val().length > 0) {
          result += '<br />' + Drupal.t('Regions: %regions', {'%regions': $('#edit-node_level_blocks-regions').val().join(', ')});
        }
      }
      else {
        result = Drupal.t('Disabled');
      }
      return result;
    }
    else {
      return '';
    }
  }

  // onDomReady: override Drupal.tableDrag.prototype.row.prototype.isValidSwap to add swap-validation.
  if (Drupal.tableDrag) {
    var _isValidSwap = Drupal.tableDrag.prototype.row.prototype.isValidSwap;
    Drupal.tableDrag.prototype.row.prototype.isValidSwap = function(tr) {
      // Do standard tableDrag validation.
      var valid = _isValidSwap.apply(this, arguments);
      if (!valid) {
        return false;
      }

      // One of us?
      var $select = $('.form-select', this.element),
          loner = $select[0].options.length <= 1;
      if (loner) {
        // Can't swap with non-draggables, since that would mean a boundary.
        if (!$(tr).is('.draggable')) {
          return false;
        }

        // Can't skip over any rows, since that might be a boundary.
        var diff = tr.rowIndex - this.element.rowIndex;
        if (Math.abs(diff) > 1) {
          return false;
        }
      }

      return true;
    };
  }
})(jQuery);
