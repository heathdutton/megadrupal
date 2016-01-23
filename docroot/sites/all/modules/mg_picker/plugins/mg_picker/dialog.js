/**
 * @file
 *
 *
 *
 * @author Kálmán Hosszu - hosszu.kalman@gmail.com
 */

(function ($) {

  /**
   * Cancel button click
   */
  Drupal.behaviors.mgPickerCloseDialog = {
    attach: function (context, settings) {
      $('#mg-picker-cancel', context).click(function () {
        var dialog = Drupal.settings.wysiwyg;
        var target = window.opener || window.parent;
        target.Drupal.wysiwyg.instances[dialog.instance].closeDialog(window);
        return false;
      });
    }
  };

  /**
   * Select file item
   */
  Drupal.behaviors.mgPickerSelectItem = {
    attach: function (context, settings) {
      $('.mg-picker-file-item', context).click(function () {
        $('.mg-picker-file-item', context).each(function() {
          $(this).removeClass('selected');
        });

        $(this).addClass('selected');

        return false;
      });
    }
  };

  /**
   * Insert the pattern
   */
  Drupal.behaviors.mgPickerInsertItem = {
    attach: function (context, settings) {
      $('#mg-picker-gallery-search-form #edit-submit', context).click(function () {
        // No gallery selected
        if (typeof(Drupal.settings.mgPicker) == "undefined") {
          alert(Drupal.t('Please select a gallery!'));
          return false;
        }

        // Default pattern
        var pattern = '[mg_picker:' + Drupal.settings.mgPicker.galleryNid;

        var target = window.opener || window.parent;
        var dialog = Drupal.settings.wysiwyg;
        // Check selected item
        var selected_item = $('.mg-picker-file-item.selected', $('#gallery-images'));

        // Add selected item's fid
        if (typeof(selected_item.attr('id')) != "undefined") {
          var class_array = selected_item.attr('id').split('-');
          pattern += ' fid:' + class_array.pop();
        }

        // Pattern closure
        pattern += ']';

        // Insert pattern into currently attached editor.
        target.Drupal.wysiwyg.instances[dialog.instance].insert(pattern);
        // Close this dialog.
        target.Drupal.wysiwyg.instances[dialog.instance].closeDialog(window);

        return false;
      });
    }
  };

})(jQuery);