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
  }

  function hideRows ($table, $toggle) {
    if ($toggle) {
      toggleState = !toggleState;
    }
    var $rows = $table.find('.hidden-count, .hidden-extension, .hidden-property').closest('tr').toggle(toggleState);
    // TODO: Drupal.org Specific!
    var count = $rows.not('.pift-test-info').length;
    var linkText;
    if (toggleState) {
      linkText = Drupal.formatPlural(count, 'Hide 1 file', 'Hide @count files');
    }
    else {
      linkText = Drupal.formatPlural(count, 'Show 1 more file', 'Show @count more files');
    }
    $toggleLink.html(linkText);
    return count;
  }

  Drupal.behaviors.extendedFileFieldTable = {
    attach: function (context) {
      var $table = $(context).find('.field-name-field-issue-files table[id]').filter(':first').once('extended-file-field');
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
        var $table = $(context).find('.field-name-field-issue-files table[id]').removeOnce('extended-file-field');
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

})(jQuery, Drupal);
