/**
 * Google Chart Tools javascript
 *
 */

(function($) {
  Drupal.behaviors.datavizGooChart = {
    attach: function(context, settings) {
      // Need to check this or the views admin page crashes
      if (settings.Dataviz) {
        // Loop on the charts in the settings.
        for (var chartId in settings.Dataviz.chart) {
          // Filter by context
          if ($('#'+chartId, context).length) {
            // Data table.
            var data = null;
            var data = new google.visualization.DataTable();
            
            data.addColumn('string', 'Label');
            for (var col in settings.Dataviz.chart[chartId].columns) {
              data.addColumn('number', settings.Dataviz.chart[chartId].columns[col]);
            }
            for (var i in settings.Dataviz.chart[chartId].header) {
              var row = new Array();
              // Adding the rows.
              // The points of the column for each row.
              for (var j in settings.Dataviz.chart[chartId].rows) {
                row[j] = parseInt(settings.Dataviz.chart[chartId].rows[j][i]);
              }
              row.unshift(settings.Dataviz.chart[chartId].header[i]);
              data.addRows([row])
            };
            var options = settings.Dataviz.chart[chartId].options;
            var chart = new Object;
            var element = document.getElementById(settings.Dataviz.chart[chartId].containerId);
            if (element) {
              chart[settings.Dataviz.chart[chartId]] = new google.visualization[settings.Dataviz.chart[chartId].chartType](element);
              chart[settings.Dataviz.chart[chartId]].draw(data, options);
            }
          }
        }
      }
    }
  };
})(jQuery);

