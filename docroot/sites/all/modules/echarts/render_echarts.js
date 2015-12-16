/**
 * Echarts javascript
 *
 */

(function($) {
  Drupal.behaviors.echarts = {
    attach: function(context, settings) {
      require.config({
        paths: {
            echarts: Drupal.settings.echarts_lib_path + '/lib'
        }
      });
    
      require(
        Drupal.settings.echarts_types,
        function (ec) {
          for (var chartId in settings.echart) {
            var coption = new Object(); 
            for(var key in settings.echart[chartId].options){ 
              coption[key] = settings.echart[chartId].options[key];
            }
            var elementChart = ec.init(document.getElementById(settings.echart[chartId].containerId));
            elementChart.setOption(coption);
          }
        }
      );  
    }
  };
})(jQuery);

