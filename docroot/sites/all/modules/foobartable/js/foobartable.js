/**
 * @file
 * Foobartable sorting helpers.
 */
(function($) {

    var foobartableHelper = {
        getTableId: function (table) {
            var classes = table.attr("class").split(" ");
            for (var i in classes) {
                var cl = classes[i];
                if (cl.indexOf("foobartable-id-") === 0) {
                    return cl.substring("foobartable-id-".length);
                }
            }

            return "";
        },
        saveSortingOrder: function (id, order) {
            var sortings = foobartableHelper.loadSortingOrders();
            if (sortings === null) {
                sortings = {};
            }
            sortings[id] = order;
            $.cookie("foobarsorting", JSON.stringify(sortings), {path: window.location.pathname + ""});
        },
        loadSortingOrders: function () {
            var sortingString = $.cookie("foobarsorting");
            return sortingString === null ? JSON.parse(sortingString) : null;
        },
        loadSortingOrder: function (id) {
            var sortings = foobartableHelper.loadSortingOrders();
            if (sortings === null || !sortings.hasOwnProperty(id)) {
                return null;
            }
            var parts = sortings[id].split(",");
            return [[parts[0], parts[1]]];
        },
        getDefaultSortingOrder: function (tableId, defaultOrder) {
            var savedOrder = foobartableHelper.loadSortingOrder(tableId);
            return savedOrder !== null ? savedOrder : defaultOrder;
        }
    };
    
    Drupal.behaviors.foobartableInitSorting = {
        attach: function(context, settings) {
            if ($("table.foobartable-sorting-enabled", context).length === 0) {
                return;
            }
            
            $.tablesorter.defaults.widgets = ["zebra"];

            $("table.foobartable-sorting-enabled:not(.foobartable-processed)", context).each(function () {
                var table = $(this);
                var id = foobartableHelper.getTableId(table);
                var headers = {};
                for(var i in settings.foobartableSorters[id]) {
                    if (settings.foobartableSorters[id].hasOwnProperty(i)) {
                        headers[i] = { sorter: settings.foobartableSorters[id][i] };
                    }
                }
                table.tablesorter({
                    headers: headers,
                    sortList: foobartableHelper.getDefaultSortingOrder(id, settings.foobartableDefaultSortOrder[id]),
                });
                table.bind("sortEnd", function (sorter) {
                    var tableId = foobartableHelper.getTableId($(sorter.target));
                    foobartableHelper.saveSortingOrder(tableId, sorter.target.config.sortList);
                });
                table.addClass("foobartable-processed");
            })
        }
    };

})(jQuery);