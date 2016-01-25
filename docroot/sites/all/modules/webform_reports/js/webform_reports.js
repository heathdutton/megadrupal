
jQuery(document).ready(function() {
   var data = new Array();   
   for(var i=0; i<Drupal.settings.series.length; i++) {     
     data[i] = new Array();
     data[i][0] = Drupal.settings.series[i].date;
     data[i][1] = parseInt(Drupal.settings.series[i].count);
   }   
  jQuery.jqplot.config.enablePlugins = true;
  jQuery.jsDate.config.defaultCentury = 2000;
  
  jQuery.jqplot("chartdiv", [data], {
    series: [{
      renderer: jQuery.jqplot.BarRenderer
    }],
    seriesDefaults: {
      showMarker: true,
      pointLabels: {
        show: true
      },
      rendererOptions: {
        barMargin: 35
      }
    },
    axes: {
      xaxis: {
        renderer: jQuery.jqplot.DateAxisRenderer,
        tickOptions:{formatString:'%b %#d'},
        tickInterval:'4 days',
        min: Drupal.settings.min_date     
      }
    }
  });
});