(function($) {
  // Private methods.
  var tools = {
    /**
     * Returns primary views maintenance table.
     *
     * @param item
     *   Primary table or any of its child nodes.
     */
    getTable: function(item) {
      var tableSelector = 'table.views-mnt-views-table';

      if (item.is(tableSelector)) {
        return item;
      }
      else {
        var table = item.parents(tableSelector);
        if (table.length) {
          return table;
        }
        else {
          return false;
        }
      }
    },
    
    /**
     * Returns table-level persistent variables.
     *
     * @param item
     *   Primary table or any of its child nodes.
     */
    getVars: function(item) {
      var table = tools.getTable(item);
      if (table) {
        return table.data('viewsMaintenance');
      }
      else {
        return false;
      }
    },
    
    /**
     * Returns row-level persistent variables.
     *
     * @param item
     *   View row, displays row or any of their child nodes.
     */
    getRowVars: function(item) {
      var tr = tools.getViewRow(item);
      if (tr) {
        return tr.data('viewsMaintenance');
      }
      else {
        return false;
      }
    },

    /**
     * Saves variables object to item.
     *
     * @param item
     *   jQuery element to save variables to.
     * @param v
     *   Object containing variables.
     */
    setVars: function(item, v) {
      item.data('viewsMaintenance', v);
    },

    /**
     * Switches toggleAll link to collapse all views on click.
     */
    switchToCollapseAll: function(v) {
      v.toggleAll
        .text('[--]')
        .attr('title', Drupal.t('Click to collapse all views'))
        .unbind('click.viewsMaintenance')
        .bind('click.viewsMaintenance', function() {
          $(this).viewsMaintenance('collapseAll');
        });
    },

    /**
     * Switches toggleAll link to expand all views on click.
     */
    switchToExpandAll: function(v) {
      v.toggleAll
        .text('[++]')
        .attr('title', Drupal.t('Click to expand all views'))
        .unbind('click.viewsMaintenance')
        .bind('click.viewsMaintenance', function() {
          $(this).viewsMaintenance('expandAll');
        });
    },

    /**
     * Handles changes in number of collapsed rows.
     * 
     * Updates number of collapsed rows in persistent variables and switches
     * toggleAll link according to changes.
     * 
     * @param v
     *   Table-level persistent variables.
     * @param collapsedDiff
     *   Number of collapsed rows if positive, number of expanded rows if
     *   negative.
     */
    changeCollapsedNumber: function(v, collapsedDiff) {
      if (v.collapsed == 0) {
        if (collapsedDiff > 0) {
          // All rows were expanded and now few of them became collapsed.
          // Toggle all link should be "expand all" if any of them are collapsed.
          tools.switchToExpandAll(v);
        }
        else {
          // Collapsed number can't be less than zero, just exit.
          return;
        }
      }

      v.collapsed += collapsedDiff;
      if (v.collapsed < 0) {
        v.collapsed = 0;
      }

      if (v.collapsed == 0) {
        // All rows became expanded. Toggle all link should be "collapse all".
        tools.switchToCollapseAll(v);
      }
    },

    /**
     * Returns displays row for view row passed.
     */
    getDisplaysRow: function(viewRow) {
      return viewRow.next('tr.views-mnt-view-displays');
    },

    /**
     * Returns view row.
     *
     * @param item
     *   View row, displays row or any of their child nodes.
     *
     * @return
     *   View row or false if row can't be found.
     */
    getViewRow: function(item) {
      var tr;
      // Find TR from its children.
      if (!item.is('tr')) {
        tr = item.parents('tr:first');
        if (tr.length == 0) {
          return false;
        }
      }
      else {
        tr = item;
      }

      // We're sure it is TR, now find view row if we have displays row.
      if (tr.is('.views-mnt-view-displays')) {
        tr = tr.prev('tr.views-mnt-view-info');
      }

      // Final check to be sure we return correct tag.
      if (tr.is('.views-mnt-view-info')) {
        return tr;
      }
      else {
        return false;
      }
    }
  };
  
  // Public methods.
  var methods = {
    /**
     * Initializes primary views table, should be called only once for table.
     */
    init: function() {
      // Persistent variables.
      var v = {
        table: this,
        collapsed: 0
      };

      // Create "Expand/Collapse All" button, event handlers will be attached
      // later from addRows() by collapsing first row on init.
      v.toggleAll = v.table
        .find('th.views-mnt-view-toggle-all')
        .html('<a class="views-mnt-displays-toggle-all" href="javascript:;"></a>')
        .find('a.views-mnt-displays-toggle-all');

      // Save persistent variables.
      tools.setVars(this, v);

      // Init rows by refreshing table.
      this.viewsMaintenance('addRows');
    },

    // Table-level public methods.
    table: {
      /**
       * Initializes primary views table rows not yet processed.
       */
      addRows: function(v) {
        v.table
          .find('tr.views-mnt-view-info:not(.views-mnt-processed)')
          .addClass('views-mnt-processed')
          .each(function() {
            var viewRow = $(this);

            // Row persistent variables.
            var rv = {
              // Row with displays info.
              displaysRow: tools.getDisplaysRow(viewRow),

              // Collapse/expand link.
              toggleLink: viewRow
                .find('td.views-mnt-view-toggle')
                .html('<a class="views-mnt-displays-toggle" href="javascript:;"></a>')
                .find('a.views-mnt-displays-toggle'),

              // Clickable cells in view row.
              toggleCells: viewRow
                .find('td:not(.views-mnt-view-links)')
                .css({
                  cursor: 'pointer'
                })
            };

            // Save row-level variables to view row.
            tools.setVars(viewRow, rv);

            // All rows are initially collapsed.
            viewRow.viewsMaintenance('collapseView');
          });
      },
      
      /**
       * Collapses all rows.
       */
      collapseAll: function(v) {
        v.table
          .find('tr.views-mnt-view-displays:visible')
          .each(function() {
            $(this).viewsMaintenance('collapseView')
          });
      },

      /**
       * Expands all rows.
       */
      expandAll: function(v) {
        v.table
          .find('tr.views-mnt-view-displays:hidden')
          .each(function() {
            $(this).viewsMaintenance('expandView')
          });
      }
    },

    // Row-level public methods.
    row: {
      /**
       * Collapses displays row for single view.
       */
      collapseView: function(v, rv) {
        rv.toggleLink.text('[+]');
        rv.displaysRow.hide();
        rv.toggleCells
          .attr('title', Drupal.t('Click to expand displays info'))
          .unbind('click.viewsMaintenance')
          .bind('click.viewsMaintenance', function() {
            $(this).viewsMaintenance('expandView');
          });
        tools.changeCollapsedNumber(v, 1);
      },

      /**
       * Expands displays row for single view.
       */
      expandView: function(v, rv) {
        rv.toggleLink.text('[-]');
        rv.displaysRow.show();
        rv.toggleCells
          .attr('title', Drupal.t('Click to collapse displays info'))
          .unbind('click.viewsMaintenance')
          .bind('click.viewsMaintenance', function() {
            $(this).viewsMaintenance('collapseView');
          });
        tools.changeCollapsedNumber(v, -1);
      }
    }
  };

  /**
   * Primary function, which calls init, table-level or row-level methods.
   *
   * If first argument is a string and function with that name exists
   */
  $.fn.viewsMaintenance = function() {
    if (arguments.length > 0 && typeof arguments[0] == 'string') {
      // Function name is passed as first string argument.
      var func = arguments[0];
      if (methods.table[func] || methods.row[func]) {
        // Get table-level persistent variables.
        var v = tools.getVars(this);
        if (v) {
          // Row-level method.
          if (methods.row[func]) {
            // Get row-level persistent variables.
            var rv = tools.getRowVars(this);
            if (rv) {
              methods.row[func].call(this, v, rv);
            }
          }
          // Table-level method.
          else {
            methods.table[func].call(this, v);
          }
        }

        // Fallback for situation when persistent variables can't be loaded and
        // exit point for successful calls to table-level and row-level methods.
        // Just return current object to keep the function chainable.
        return this;
      }
    }

    // Fallback to init method if function not exists or no arguments were
    // passed.
    methods.init.call(this);
    return this;
  };
})(jQuery);

/**
 * Behavior for views maintenance primary table.
 */
Drupal.behaviors.viewsMaintenance = {
  attach: function (context) {
    jQuery('table.views-mnt-views-table:not(.views-mnt-processed)', context)
      .addClass('views-mnt-processed')
      .viewsMaintenance();
  }
};
