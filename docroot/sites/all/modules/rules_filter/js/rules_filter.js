/**
 * @file
 * Contains the javascript for the Rules filter.
 */

(function ($) {

  Drupal.behaviors.rulesFilter = {
    attach: function (context) {
      var $tbody = $('.rules-overview-table tbody', context).addClass('list');
      var $filter_form = $('.rules-filter-form', context);
      var $tabs = $('#rules-filter-tabs', context);

      // Apply list plugin for Rules table.
      // Search will work for .rules-element-content and .search-field fields.
      var rulesList = new List('rules-filter-rules', {
        valueNames: ['rules-element-content', 'search-field']
      });

      // Adds behaviour for tabs links.
      $tabs.find("a").bind("click", function() {

        // Select active tab.
        $tabs.find("a").removeClass('selected');
        $(this).addClass('selected');

        // Execute filter.
        var tab_val = $(this).attr('rel');
        filterExecute(tab_val);
      });

      // Adds behaviour for enabled/disabled checkboxes.
      $filter_form.find('.rule-enabled, .rule-disabled').bind("click", function() {

        // Execute filter.
        var tab_val = window.location.hash.replace('#', '');
        filterExecute(tab_val);
      });

      // Execute filter by default.
      var tab_val = window.location.hash.replace('#', '');
      if (!tab_val) {
        tab_val = 'all';
      }
      $tabs.find("a[rel='" + tab_val + "']").addClass('selected');
      filterExecute(tab_val);

      /**
       * Filter rows by params.
       *
       * @param tab_val
       *   Only rows with given tab_val will be shown.
       */
      function filterExecute(tab_val) {
        // Skip executing for empty table.
        if ($tbody.parent().hasClass('empty')) {
          return;
        }

        // Check enabled/disabled checkboxes.
        var enabled_checked = $filter_form.find('.rule-enabled').is(':checked');
        var disabled_checked = $filter_form.find('.rule-disabled').is(':checked');

        // Execute plugin filter.
        rulesList.filter(function(item) {
          var $item = $(item.elm);

          // Define variable only if enabled or disabled checkboxes checked.
          var is_enabled_or_disabled_checked = true;
          if ((enabled_checked && !disabled_checked) || (!enabled_checked && disabled_checked)) {
            is_enabled_or_disabled_checked = enabled_checked ? $item.hasClass('enabled') : $item.hasClass('disabled');
          }

          // Show row only if it has needed classes.
          if ($item.hasClass(tab_val) && is_enabled_or_disabled_checked) {
            return true;
          }
          return false;
        });

        // Restripe a table.
        $tbody.find('tr').removeClass('odd even')
          .filter(':even').addClass('odd').end()
          .filter(':odd').addClass('even');
      }
    }
  };

})(jQuery);
