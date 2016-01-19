(function($) {
  Drupal.behaviors.table_chart = {
    attach: function(context, settings) {
      var count = 0;
      $('.table-chart').each(function(){
        count++;
        $(this).once(function(){
          var wrapper = $(this);
          var table = wrapper.find('table');

          // Chart settings
          var options = {};
          options.element = 'morris-chart-' + count;
          options.colors = table.data('morris-colors') ? table.data('morris-colors').split(',') : null;
          options.stacked = table.data('morris-stacked') ? true == table.data('morris-stacked') : false;
          options.resize = table.data('morris-resize') ? true == table.data('morris-resize') : false;
          options.type = table.data('morris-type') ? table.data('morris-type') : 'line';
          options.lineWidth = table.data('morris-lineWidth') ? table.data('morris-lineWidth') : null;
          options.pointSize = table.data('morris-pointSize') ? table.data('morris-pointSize') : null;
          options.pointFillColors = table.data('morris-pointFillColors') ? table.data('morris-pointFillColors') : null;
          options.pointStrokeColors = table.data('morris-pointStrokeColors') ? table.data('morris-pointStrokeColors') : null;
          options.ymax = table.data('morris-ymax') ? table.data('morris-ymax') : null;
          options.ymin = table.data('morris-ymin') ? table.data('morris-ymin') : null;
          options.smooth = table.data('morris-smooth') ? true == table.data('morris-smooth') : true;
          options.hideHover = table.data('morris-hideHover') ? table.data('morris-hideHover') : false;
          options.hoverCallback = table.data('morris-hoverCallback') ? table.data('morris-hoverCallback') : null;
          options.parseTime = table.data('morris-parseTime') ? true == table.data('morris-parseTime') : true;
          options.postUnits = table.data('morris-postUnits') ? table.data('morris-postUnits') : null;
          options.preUnits = table.data('morris-preUnits') ? table.data('morris-preUnits') : null;
          options.dateFormat = table.data('morris-dateFormat') ? table.data('morris-dateFormat') : null;
          options.xLabels = table.data('morris-xLabels') ? table.data('xLabels') : null;
          options.xLabelFormat = table.data('morris-xLabelFormat') ? table.data('morris-xLabelFormat') : null;
          options.yLabelFormat = table.data('morris-yLabelFormat') ? table.data('morris-yLabelFormat') : null;
          options.goals = table.data('morris-goals') ? table.data('morris-goals').split(',') : null;
          options.goalStrokeWidth = table.data('morris-goalStrokeWidth') ? table.data('morris-goalStrokeWidth') : null;
          options.goalLineColors = table.data('morris-goalLineColors') ? table.data('morris-goalLineColors').split(',') : null;
          options.events = table.data('morris-events') ? table.data('morris-events') : null;
          options.eventStrokeWidth = table.data('morris-eventStrokeWidth') ? table.data('morris-eventStrokeWidth') : null;
          options.eventLineColors = table.data('morris-eventLineColors') ? table.data('morris-eventLineColors').split(',') : null;
          options.continuousLine = table.data('morris-continuousLine') ? table.data('morris-continuousLine') : null;
          options.axes = table.data('morris-axes') ? true == table.data('morris-axes') : true;
          options.grid = table.data('morris-grid') ? true == table.data('morris-grid') : true;
          options.gridTextColor = table.data('morris-gridTextColor') ? table.data('morris-gridTextColor') : null;
          options.gridTextSize = table.data('morris-gridTextSize') ? parseInt(table.data('morris-gridTextSize'), 10) : null;
          options.gridTextFamily = table.data('morris-gridTextFamily') ? table.data('morris-gridTextFamily') : null;
          options.gridTextWeight = table.data('morris-gridTextWeight') ? table.data('morris-gridTextWeight') : null;
          options.fillOpacity = table.data('morris-fillOpacity') ? parseFloat(table.data('morris-fillOpacity')) : null;
          options.behaveLikeLine = table.data('morris-behaveLikeLine') ? true == table.data('morris-behaveLikeLine') : null;
          options.xkey = table.data('morris-xkey') ? table.data('morris-xkey') : null;
          options.ykeys = table.data('morris-ykeys') ? table.data('morris-ykeys').split(',') : [];
          options.labels = table.data('morris-labels') ? table.data('morris-labels').split(',') : null;

          // Data settings

          // Ignoring the first column (i.e. 0) has some odd behavior. Zero (0)
          // being misinterpreted as the emtpy array value. So we treat this
          // case separately.
          if (table.data('tabletojson-ignoreColumns') == 0) {
            options.ignoreColumns = [0];
          }
          else {
            options.ignoreColumns = table.data('tabletojson-ignoreColumns') ? String(table.data('tabletojson-ignoreColumns')).split(',') : [];
          }
          // Same issue as above
          if (table.data('tabletojson-onlyColumns') == 0) {
            options.onlyColumns = [0];
          }
          else {
            options.onlyColumns = table.data('tabletojson-onlyColumns') ? table.data('tabletojson-onlyColumns').split(',') : null;
          }
          options.ignoreHiddenRows = table.data('tabletojson-ignoreHiddenRows') ? true == table.data('tabletojson-ignoreHiddenRows') : true;
          options.headings = table.data('tabletojson-headings') ? table.data('tabletojson-headings').split(',') : null;
          options.keys = [];
          // Data preperation

          // Ensure all the values are true integers and not string numbers
          options.ignoreColumns = options.ignoreColumns.map(function (x) {
              return parseInt(x, 10);
          });

          // Read the table data with the given configuration
          options.table_data = table.tableToJSON({
            ignoreColumns: options.ignoreColumns,
            onlyColumns: options.onlyColumns,
            ignoreHiddenRows: options.ignoreHiddenRows,
            headings: options.headings
          });

          // Get the list of keys
          for (var i in options.table_data) {
            for (var j in options.table_data[i]) {
              options.keys.push(j);
            }
            break;
          }

          // Prepare display
          table.addClass('element-invisible');
          wrapper.append('<div id="' + options.element + '" class="morris-chart" style="height: 250px;"></div> <a href="#" class="button toggle-table" title="'+ Drupal.t("Toggle Data Table") +'">' + Drupal.t("Show data") + '</a>');

          // Draw the chart with the given options
          drawChart(options);

          wrapper.find('.button.toggle-table').click(function(event){
            table.toggleClass('element-invisible');
            $(this).toggleClass('down');
            if (table.hasClass('element-invisible')) {
              $(this).text(Drupal.t('Show Data'));
            }
            else {
              $(this).text(Drupal.t('Hide Data'));
            }
            event.preventDefault();
          });
        });
      });
    }
  };

  /**
   * Generate the chart with the given defaults
   */
  function drawChart(options) {
    // Set global settings
    var settings = {
      element: options.element,
      data: options.table_data,
      resize: options.resize
    };

    // Set per-type settings
    switch (options.type) {
      case 'bar':
        // Add optional settings
        if (null !== options.colors && options.colors != undefined) {
          settings.barColors = options.colors;
        }
        if (options.stacked) {
            settings.stacked = options.stacked;
        }
        else {
          settings.stacked = settings.stacked;
        }
        if (null !== options.hideHover) {
          settings.hideHover = options.hideHover;
        }
        if (null !== options.hoverCallback) {
          settings.hoverCallback = options.hoverCallback;
        }
        if (null !== options.axes) {
          settings.axes = options.axes;
        }
        if (null !== options.grid) {
          settings.grid = options.grid;
        }
        if (null !== options.gridTextColor) {
          settings.gridTextColor = options.gridTextColor;
        }
        if (null !== options.gridTextSize) {
          settings.gridTextSize = options.gridTextSize;
        }
        if (null !== options.gridTextFamily) {
          settings.gridTextFamily = options.gridTextFamily;
        }
        if (null !== options.gridTextWeight) {
          settings.gridTextWeight = options.gridTextWeight;
        }
        if (null !== options.hideHover) {
          settings.hideHover = options.hideHover;
        }

        // Add required settings
        if (null != options.xkey) {
          settings.xkey = options.xkey;
        }
        else {
          settings.xkey = options.keys.shift();
        }

        if (options.ykeys.length > 0) {
          settings.ykeys = options.ykeys;
        }
        else {
          settings.ykeys = options.keys;
        }

        if (null != options.labels) {
          settings.labels = options.labels;
        }
        else {
          settings.labels = settings.ykeys;
        }

        new Morris.Bar(settings);
        break;
      case 'donut':
        if (null !== options.colors && options.colors != undefined) {
          settings.colors = options.colors;
        }
        if (null !== options.formatter) {
          settings.formatter = options.formatter;
        }
        new Morris.Donut(settings);

        break;
      case 'area':
        // Add optional settings
        if (null !== options.behaveLikeLine) {
          settings.behaveLikeLine = options.behaveLikeLine;
        }

        // Area and Line share all the same configuration except the one option
        // So we fall into the default case and check at the end which type to
        // generate.
      case 'line':
      default:
        // Add optional settings

        if (null !== options.colors && options.colors != undefined) {
          settings.lineColors = options.colors;
        }

        // Add required settings
        if (null != options.xkey) {
          settings.xkey = options.xkey;
        }
        else {
          settings.xkey = options.keys.shift();
        }

        if (options.ykeys.length > 0) {
          settings.ykeys = options.ykeys;
        }
        else {
          settings.ykeys = options.keys;
        }

        if (null != options.labels) {
          settings.labels = options.labels;
        }
        else {
          settings.labels = settings.ykeys;
        }

        // @todo adjust line settings
        if (options.type == 'area') {
          new Morris.Area(settings);
        }
        else {
          new Morris.Line(settings);
        }
    }
  }
}(jQuery));