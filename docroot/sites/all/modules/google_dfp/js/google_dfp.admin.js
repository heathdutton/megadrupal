/**
 * @file
 * Provides js for tabs and admin page
 */


(function ($) {
  /**
   * JS related to the tabs.
   */
  Drupal.behaviors.googleDfpTabs = {
    attach: function (context) {
      $('#google-dfp-tabs', context)
        .once('.tabs-processed', function(){
          $(this).tabs();
        })
    }
  };

  /**
   * Js related to enabled plugins
   */
  Drupal.googleDfp = {};
  Drupal.googleDfp.pluginStatus = function(id, context) {
    $('#' + id + ' input.form-checkbox', context).once(id + '-status', function () {
      var $checkbox = $(this);
      // Retrieve the tabledrag row belonging to this filter.
      var $row = $('#' + $checkbox.attr('id').replace(/-enabled$/, '-weight'), context).closest('tr');
      // Retrieve the vertical tab belonging to this filter.
      var tab = $('#' + $checkbox.attr('id').replace(/-enabled$/, '-settings'), context).data('verticalTab');

      // Bind click handler to this checkbox to conditionally show and hide the
      // plugin's tableDrag row and vertical tab pane.
      $checkbox.bind('click.googleDfpPluginUpdate', function () {
        if ($checkbox.is(':checked')) {
          $row.show();
          if (tab) {
            tab.tabShow().updateSummary();
          }
        }
        else {
          $row.hide();
          if (tab) {
            tab.tabHide().updateSummary();
          }
        }
        // Restripe table after toggling visibility of table row.
        var tableId = $row.parents('table').attr('id');
        Drupal.tableDrag[tableId].restripeTable();
      });

      // Attach summary for configurable plugins (only for screen-readers).
      if (tab) {
        tab.fieldset.drupalSetSummary(function (tabContext) {
          return $checkbox.is(':checked') ? Drupal.t('Enabled') : Drupal.t('Disabled');
        });
      }

      // Trigger our bound click handler to update elements to initial state.
      $checkbox.triggerHandler('click.googleDfpPluginUpdate');
    });
  }

  /**
   * Js related to enabled plugins.
   */
  Drupal.behaviors.googleDfpPluginStatus = {
    attach: function (context, settings) {
      Drupal.googleDfp.pluginStatus('tiers-status-wrapper', context);
      Drupal.googleDfp.pluginStatus('keywords-status-wrapper', context);
    }
  };

})(jQuery);

