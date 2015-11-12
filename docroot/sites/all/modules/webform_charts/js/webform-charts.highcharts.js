/**
 * @file
 * Scripts for the Webform Charts module.
 */
(function ($) {
  "use strict";

  Drupal.behaviors.webformChartsHighcharts = {};
  Drupal.behaviors.webformChartsHighcharts.attach = function(context, settings) {
    var $form = $('#webform-charts-edit-chart');

    // Update the preview when using Highcharts.
    var $preview = $form.find('.charts-highchart');
    if ($preview) {
      var seriesKeys = [];
      var highchart = $preview.highcharts();
      var singleAxis = $form.find('input[type="name"]').attr('data-axis-single');
      for (var n = 0; n < highchart.series.length; n++) {
        seriesKeys.push(n);
      }
      $form.bind('change', function(e) {
        var chartOptions;
        var animation = true;
        highchart = $preview.highcharts();

        switch (e.target.name) {
          case 'type':
            chartOptions = { chart: { type: e.target.value } };
            // Ensure the vertical axis always has non-rotated labels, per the
            // matching requirement in charts module.
            if (e.target.attributes['data-axis-inverted'] && e.target.attributes['data-axis-inverted'].value) {
              chartOptions['xAxis'] = [{ labels: { rotation: 0, align: 'right' } }];
              chartOptions['yAxis'] = [{ align: highchart.options.yAxis[0].rotation ? 'left' : 'center' }];
            }
            else {
              chartOptions['xAxis'] = [{ align: highchart.options.xAxis[0].rotation ? 'left' : 'center' }];
              chartOptions['yAxis'] = [{ labels: { rotation: 0, align: 'right' } }];
            }
            if (e.target.attributes['data-axis-single'] && e.target.attributes['data-axis-single'].value) {
              singleAxis = true;
            }
            else {
              singleAxis = false;
            }
            break;
          case 'legend':
            chartOptions = { legend: { enabled: e.target.checked } };
            break;
          case 'tooltips':
            chartOptions = { tooltip: { enabled: e.target.checked } };
            animation = false;
            break;
          case 'data_labels':
            chartOptions = { plotOptions: { series: { dataLabels: { enabled: e.target.checked } } } };
            animation = false;
            break;
          case 'stacking':
            chartOptions = { plotOptions: { series: { stacking: e.target.checked } } };
            // Reverse the series so first values are stacked on the bottom.
            highchart.options.series = highchart.options.series.reverse();
            seriesKeys = seriesKeys.reverse();
            break;
          case 'xaxis_title':
            highchart.xAxis[0].setTitle({ text: e.target.value });
            animation = false;
            break;
          case 'yaxis_title':
            highchart.yAxis[0].setTitle({ text: e.target.value });
            animation = false;
            break;
          case 'xaxis_labels_rotation':
            var rotation = parseFloat(e.target.value);
            chartOptions = { xAxis: [{ labels: { rotation: rotation, align: rotation ? 'left' : 'center' } }] };
            animation = false;
            break;
          case 'yaxis_labels_rotation':
            var rotation = parseFloat(e.target.value);
            chartOptions = { yAxis: [{ labels: { rotation: rotation, align: rotation ? 'left' : 'center' } }] };
            animation = false;
            break;
          case 'yaxis_min':
            highchart.yAxis[0].setExtremes(e.target.value.length ? parseFloat(e.target.value) : null);
            break;
          case 'yaxis_max':
            highchart.yAxis[0].setExtremes(null, e.target.value.length ? parseFloat(e.target.value) : null);
            break;
          // Default try to catch color changes.
          default:
            var colorRegex = /colors\[(\d+)\]/;
            if (e.target.name && e.target.name.match(colorRegex)) {
              var colorId = e.target.name.replace(colorRegex, "$1");
              highchart.options.colors[colorId] = e.target.value;
              chartOptions = { colors: highchart.options.colors };
              animation = false;
            }
        }
        if (chartOptions) {
          var newOptions = highchart.options;
          $.extend(true, newOptions, chartOptions);
          newOptions.plotOptions.series.animation = animation;
          highchart.destroy();
          $preview.highcharts(newOptions);
        }
    });

    /**
     * Utility to change the color of a series without redrawing the chart.
     *
     * @todo: Not used currently. Remove if current approach of redrawing on
     * color changes has acceptable performance.
     */
    var updateColor = function(chart, seriesIndex, color) {
      // Make the seriesIndex point at the correct series regardless of the
      // reversed status by checking seriesKeys.
      seriesIndex = seriesKeys[seriesIndex];
      var series = chart.series[seriesIndex];

      // Colorize the series.
      series.color = color;

      // Update the options array for future refreshes.
      chart.options.series[seriesIndex].color = color;
      // Colorize the legend to match the series.
      if (chart.legend && chart.legend.enabled) {
        chart.legend.colorizeItem(series, series.visible);
      }
      // Update each data point for scatter/line charts.
      $.each(series.data, function(i, point) {
        point.graphic.attr({
          fill: color
        });
      });
    };
  }
};

})(jQuery);