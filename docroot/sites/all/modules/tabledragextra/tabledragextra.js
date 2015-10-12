(function ($) {

  Drupal.behaviors.tableDragExtra = {
    attach: function (context, settings) {
      for (var base in settings.tableDrag) {
        $('#' + base, context).once('tabledragextra', function() {
          var tableDrag = Drupal.tableDrag[base];

          var table = this;

          // Add icon links to each row to move it to the start or end.
          $('> tr.draggable, > tbody > tr.draggable', table).each(function() {
            var row = this;
            $(row).find('.tabledrag-handle')
              .after($('<a href="#" class="tabledragextra-move tabledragextra-move-end" title="' + Drupal.t('Move to end') + '"><div class="tabledragextra-button">&nbsp;</div></a>')
                .click(function(event) {
                  $(this).blur();

                  // Is there a next row?
                  var nextRow = row;
                  do {
                    nextRow = $(nextRow).next('tr').get(0);
                  } while (nextRow && $(nextRow).is(':hidden'));

                  // If not, we're the last row already, so nothing to do.
                  if (nextRow) {
                    tableDrag.rowObject = new tableDrag.row(row, 'mouse', tableDrag.indentEnabled, tableDrag.maxDepth, true);
                    var lastRow = $(table).find('> tr.draggable, > tbody > tr.draggable').filter(':last');
                    while (lastRow && $(lastRow).is(':hidden')) {
                      lastRow = $(lastRow).prev('tr').get(0);
                    }
                    if (lastRow) {
                      tableDrag.rowObject.swap('after', lastRow);
                      tableDrag.rowObject.indent(0);
                    }
                    tableDrag.restripeTable();
                    tableDrag.dropRow(event, tableDrag);
                  }
                  return false;
                })
              )
              .after($('<a href="#" class="tabledragextra-move tabledragextra-move-start" title="' + Drupal.t('Move to start') + '"><div class="tabledragextra-button">&nbsp;</div></a>')
                .click(function(event) {
                  $(this).blur();
                  // Is there a previous row?
                  var prevRow = row;
                  do {
                    prevRow = $(prevRow).prev('tr').get(0);
                  } while (prevRow && $(prevRow).is(':hidden'));

                  // If not, we're the first row already, so nothing to do.
                  if (prevRow) {
                    tableDrag.rowObject = new tableDrag.row(row, 'mouse', tableDrag.indentEnabled, tableDrag.maxDepth, true);
                    var firstRow = $(table).find('> tr.draggable, > tbody > tr.draggable').filter(':first');
                    while (firstRow && $(firstRow).is(':hidden')) {
                      firstRow = $(firstRow).next('tr').get(0);
                    }
                    if (firstRow) {
                      tableDrag.rowObject.swap('before', firstRow);
                      tableDrag.rowObject.indent(0);
                    }
                    tableDrag.restripeTable();
                    tableDrag.dropRow(event, tableDrag);
                  }
                  return false;
                })
              )
          });

          // Add "shuffle" link before the table.
          $(table).parent().find('a.tabledrag-toggle-weight').after($('<a href="#" class="tabledragextra-shuffle">' + Drupal.t('Shuffle') + '</a>')
            .attr('title', Drupal.t('Re-order rows at random.'))
            .click(function(event) {
              var length = $('> tr.draggable, > tbody > tr.draggable', table).length;
              for (var slot = 0; slot < length - 1; ++slot) {
                // Select a row from further down to place in this slot.
                var selection = slot + Math.floor(Math.random() * (length - slot));
                if (selection != slot) {
                  var rows = $('> tr.draggable, > tbody > tr.draggable', table);
                  var selectedRow = rows.eq(selection).get(0);
                  var slotRow = rows.eq(slot);
                  tableDrag.rowObject = new tableDrag.row(selectedRow, 'mouse', tableDrag.indentEnabled, tableDrag.maxDepth, true);
                  tableDrag.rowObject.swap('before', slotRow);
                  tableDrag.rowObject.indent(0);
                  tableDrag.dropRow(event, tableDrag);
                }
              }
              tableDrag.restripeTable();
              return false;
            })
          );
        });
      }
    }
  };

})(jQuery);
