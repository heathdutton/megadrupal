(function($) {
  Drupal.shareSelection = Drupal.shareSelection || {};
  Drupal.shareSelection.selectedText = null;
  Drupal.shareSelection.dialogOpen = false;

  Drupal.shareSelection.getSelection = function() {
    if (window.getSelection) {
      var sel = window.getSelection();
      if (sel.getRangeAt && sel.rangeCount) {
        return sel.getRangeAt(0);
      }
    }
    else if (document.selection && document.selection.createRange) {
      return document.selection.createRange();
    }
    return null;
  }

  Drupal.shareSelection.restoreSelection = function(range) {
    if (range) {
      if (window.getSelection) {
        var sel = window.getSelection();
        sel.removeAllRanges();
        sel.addRange(range);
      }
      else if (document.selection && range.select) {
        range.select();
      }
    }
  }

  Drupal.behaviors.shareSelection = {
    attach : function(context, settings) {
      $('.share-selection-button', context).each(function(index) {
        $(this).mousedown(function(e) {
          // Avoid selection object disappears when click on button.
          Drupal.shareSelection.restoreSelection(Drupal.shareSelection.selectedText);
          // Hiding share buttons.
          setTimeout(function() {
            $('.share-selection-wrapper').css('top', -9999);
            $('.share-selection-wrapper').css('left', -9999);
          }, 1000);
        });
      });
      $('body', context).once('share-selection', function() {
        $('#ss-dialog-wrapper').on('dialogclose', function(event, ui) {
          Drupal.shareSelection.dialogOpen = false;
        });
        $('#ss-dialog-wrapper').on('dialogopen', function(event, ui) {
          Drupal.shareSelection.dialogOpen = true;
        });
        $('body').mouseup(function(e) {
          if (!Drupal.shareSelection.dialogOpen) {
            // Save selection on mouse-up.
            Drupal.shareSelection.selectedText = Drupal.shareSelection.getSelection();
            // Check selection text length.
            var isEmpty = Drupal.shareSelection.selectedText.toString().length === 0;
            // Set sharing wrapper position.
            if (isEmpty) {
              $('.share-selection-wrapper').css('top', -9999);
              $('.share-selection-wrapper').css('left', -9999);
            }
            else {
              $('.share-selection-wrapper').position({
                of: e,
                my: 'left top',
                at: 'center',
                collision: 'fit'
              });
              $('body').trigger('shareSelectionShow');
            }
          };
        });
      });
    }
  };
})(jQuery);
