(function($) {
Drupal.behaviors.commerceReports = {
  attach: function (context) {
    $('.commerce-reports-dashboard-block .operations a.switchSection').click(function( e ) {
      e.preventDefault();

      var block = $(this).parents('.commerce-reports-dashboard-block');
      var selectedSection = $(this).attr('data-section');

      var currentHeight = block.height();
      var currentWidth = block.width();

      var currentlyActive = block.find('.commerce-reports-dashboard-section:visible');
      var nextActive = block.find('.commerce-reports-dashboard-section[data-section=\'' + selectedSection + '\']');

      if ((currentlyActive && nextActive) && (currentlyActive[0] !=nextActive[0])) {
        var chartContainer = nextActive.find('.charts-google');

        if (chartContainer) {
          var chart = chartContainer.attr("id");

          /**
           * Attempted to replicate JavaScript that melded with Visualization to resize the charts on tab switch.
           * Charts needs a JavaScript function exposed to allow other modules to rebuild charts.
           */
          //var chartConfig = chartContainer.data('chart');
          //var chartType = chartConfig.visualization;
          //var chartDataTable = chartConfig.data;
          //var chartOptions = chartConfig.options;

          if (chart !== undefined) {
            //Charts does not provide a way to rebuild a chart.
           }
         }

        nextActive.show();
        nextActive.height('auto');

        currentlyActive.hide();
       }

      block.height(currentHeight);
      block.width(currentWidth);

      block.find('.operations .switchSection').removeClass('activated');
      block.find('.operations .switchSection[data-section="' + selectedSection + '"]').addClass('activated');

    });
  }
}
})(jQuery);
