(function ($, Drupal) {

  "use strict";

  var toggleState = false; // By default hide the rows for hidden files.
  var $toggleLink;

  function prepareTable ($table) {
    $toggleLink = $('<a href="#" class="extended-file-field-table-toggle-row"></a>');
    $toggleLink
      .attr('title', Drupal.t('Show or hide rows with hidden files.'))
      .bind('click.extendedFileFieldTable', function (e) {
        hideRows($table, true);
        e.preventDefault();
      });
    // Add a toggle after the table to allow users to show or hide hidden files.
    $table.after(
      $('<div class="extended-file-field-table-toggle-row-wrapper more-link" />').prepend($toggleLink)
    );
    $table.find('.file-display').change(function(e) {
      if (toggleState) {
        $toggleLink[hideRows($table, false) ? 'show' : 'hide']();
      }
    });
  }

  function hideRows ($table, $toggle) {
    var tableDragObject = Drupal.tableDrag[$table.attr('id')];
    if ($toggle) {
      toggleState = !toggleState;
    }
    var $rows = $table.find('.file-display:not(:checked), .hidden-count').closest('tr').toggle(toggleState);
    var count = $rows.length;
    var linkText;
    if (toggleState) {
      linkText = Drupal.formatPlural(count, 'Hide 1 file', 'Hide @count files');
    }
    else {
      linkText = Drupal.formatPlural(count, 'Show 1 more file', 'Show @count more files');
    }
    tableDragObject.restripeTable();
    $toggleLink.html(linkText);
    return count;
  }

  Drupal.behaviors.extendedFileFieldTable = {
    attach: function (context) {
      var $table = $(context).find('.field-widget-file-generic table[id]').once('extended-file-field');
      if ($table.length) {
        prepareTable($table);
        if (!hideRows($table, false)) {
          $toggleLink.hide();
          // If there are no hidden files at attach time default to show files.
          toggleState = true;
        }
      }
    },
    detach: function (context, settings, trigger) {
      if (trigger === 'unload') {
        var $table = $(context).find('.field-widget-file-generic table[id]').removeOnce('extended-file-field');
        if ($table.length) {
          // Force display of hidden rows.
          toggleState = true;
          // We need to select the parent because the link is outside the table.
          $table.parent().find('.extended-file-field-table-toggle-row').click()
            .unbind('click.extendedFileFieldTable').remove();
        }
      }
    }
  };

  Drupal.tableDrag.prototype.onDrop = function () {
    if (Drupal.settings.extendedFileField.reverseDisplay == true) {
      $('#edit-field-issue-files-und-table').find('select.form-select[name*="[_weight]"]').each(function() {
        $(this).val(function(i, oldVal) {
          return oldVal * -1;
        });
      });
    }
  };

})(jQuery, Drupal);
