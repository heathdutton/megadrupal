/**
 * @file
 * JavaScript functions for XC Account module
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
(function ($) {

  var XCSearch = {};
  $(document).ready(function() {
    $('.select-all-action').click(function() {
      $('#xc-account-bookmarked-items-form input[type=checkbox]').each(function() {
        if (!this.checked) {
          this.checked = 'checked';
        }
      });
    });

    $('.select-none-action').click(function() {
      $('#xc-account-bookmarked-items-form input[type=checkbox]').each(function() {
        if (this.checked) {
          this.checked = false;
        }
      });
    });

    $('#email_action').click(function() {
      document.location = Drupal.settings.mail_url + XCSearch.getSelectedItems().join(',')
        + '?' + Drupal.settings.destination;
    });

    $('#print_action').click(function() {
      document.location = Drupal.settings.print_url + XCSearch.getSelectedItems().join(',');
    });

    $('.ui-dialog-buttonpane button:first').attr('id', 'cancel-button').html(Drupal.t('Cancel'));
    $('.ui-dialog-buttonpane button:last').attr('id', 'yes-button').html(Drupal.t('Yes'));

    $('#remove-bookmark').click(function() {
      var selectedItems = [];
      $('#xc-account-bookmarked-items-form input[type=checkbox]').each(function() {
        if (this.checked) {
          selectedItems.push(this.name);
        }
      });

      var form = $('#xc-account-bookmarked-items-form');
      var count = 0;
      var total = parseInt(selectedItems.length);

      for (i in selectedItems) {
        var id  = $('input[name=items'+selectedItems[i]+'nid]', form).val() + '|' + $('input[name=items'+selectedItems[i]+'sid]', form).val();
        var options = {'id': id};
        $.getJSON(Drupal.settings.xc_search.remove_bookmark_item_url, options, function(data, textStatus, XMLHttpRequest) {
        });
      }
      setTimeout(function () {
        location.reload();
      }, 1000);
    });
  });

  XCSearch.changeButtons = function() {
    alert('changeButtons');
    $('.ui-dialog-buttonpane button:first').html('Delete all items');
    $('.ui-dialog-buttonpane button:last').html(Drupal.t('Cancel'));
  },

  /**
   * Get the selected items in a search result list
   */
  XCSearch.getSelectedItems = function() {
    var selectedItems = [];
    $('#xc-account-bookmarked-items-form input[type=checkbox]').each(function() {
      if (this.checked) {
        selectedItems.push(this.value);
      }
    });
    return selectedItems;
  }

})(jQuery);