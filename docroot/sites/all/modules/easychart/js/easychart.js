;(function($) {

  Drupal.easychart = Drupal.easychart || {};

  // Function that renders the chart preview.
  Drupal.easychart.preview = function(widget) {
    var config = $('.easychart-config', widget).val();
    var csv = $('.easychart-csv', widget).val();
    var csv_url = $('.easychart-csv-url', widget).val();
    var options = '';

    if (config != '') {
      if (csv_url != '') {
        $.get(csv_url, function(data) {
          csv = $.trim(data);
          options = _preprocessHighchartsData(config, csv);
          $('.easychart-preview', widget).highcharts(options);
        });
      }
      else {
        options = _preprocessHighchartsData(config, csv);
        $('.easychart-preview', widget).highcharts(options);
      }
    }
  };

  // Function that decides which buttons to show.
  Drupal.easychart.buttons = function(widget) {
    var config = $('.easychart-config', widget).val();
    if (config != '') {
      $('.easychart-configure-link', widget).text(Drupal.t('Configure chart'));
      $('.easychart-delete-link', widget).removeClass('element-hidden');
    }
  };


  Drupal.behaviors.easychart = {
    attach: function() {

      // Open the popup and call the Easychart plugin.
      $('.easychart-configure-link').once('easyChart').click(function(e){
        e.preventDefault();
        var $widget = $(this).parents('.easychart-wrapper');

        // Create the Easychart plugin options based on the admin configuration.
        var options = {};
        options.csvData = $('.easychart-csv', $widget).val();
        options.csvDataUrl = $('.easychart-csv-url', $widget).val();
        options.storedConfig = $('.easychart-stored', $widget).val();

        if (Drupal.settings.easychart.highchart_ui_options != '') {
          options.guiConfig = Drupal.settings.easychart.highchart_ui_options;
        }

        // Add some translations.
        options.lang = {
          contextButtonTitle: Drupal.t('Chart context menu'),
          downloadJPEG: Drupal.t('Download JPEG image'),
          downloadPDF: Drupal.t('Download PDF document'),
          downloadPNG: Drupal.t('Download PNG image'),
          downloadSVG: Drupal.t('Download SVG vector image'),
          drillUpText: Drupal.t('Back to {series.name}'),
          loading: Drupal.t('Loading...'),
          printChart: Drupal.t('Print chart'),
          resetZoom: Drupal.t('Reset zoom'),
          resetZoomTitle: Drupal.t('Reset zoom level 1:1')
        }

        var easychart = $('.easychart-popup-content', $widget).easychart(options);

        // Support for the overlay module.
        if ($('#overlay').length > 0) {
          var top = parseInt($('body').css('marginTop')) + 10;
          $('.easychart-popup', $widget).css({'top' : top})
        }
        $('.easychart-popup', $widget).show();

        // Close the popup, and update the preview.
        $('.easychart-bar a.close', $widget).click(function(e) {
          e.preventDefault();

          // Hide the popup.
          $('.easychart-popup', $widget).hide();
          $('.easychart-popup-content', $widget).html();

          // Get CSV and configuration data.
          $('.easychart-csv-url', $widget).val(easychart.easychart('getCsvUrl'));
          $('.easychart-csv', $widget).val(easychart.easychart('getCsvData'));
          $('.easychart-stored', $widget).val(easychart.easychart('getStoredValues'));
          $('.easychart-config', $widget).val(easychart.easychart('getChartOptions'));

          // Destroy the Easychart object.
          easychart.easychart('destroy');

          // Update the chart preview
          Drupal.easychart.preview($widget);

          // Update the add/configure/delete links.
          Drupal.easychart.buttons($widget);

          // unbind this click again.
          $(this).unbind('click');
        })

        // Cancel and close the popup.
        $('.easychart-bar a.cancel', $widget).click(function(e) {
          e.preventDefault();

          // Hide the popup.
          $('.easychart-popup', $widget).hide();
          $('.easychart-popup-content', $widget).html();

          // Destroy the Easychart object.
          easychart.easychart('destroy');

          // unbind this click again.
          $(this).unbind('click');
        })

        // Close and cancel when clicking ESC.
        /*$(document).keyup(function(e) {
          if (e.keyCode == 27) {
            $('.easychart-bar a.cancel').click();
          }
        });*/

      });

      // Delete a chart configuration.
      $('.easychart-delete-link').click(function(e){
        e.preventDefault();
        var $widget = $(this).parents('.easychart-wrapper');

        $('.easychart-csv-url', $widget).val('');
        $('.easychart-csv', $widget).val('');
        $('.easychart-stored', $widget).val('');
        $('.easychart-config', $widget).val('');
        $('.easychart-preview', $widget).html('');

        $('.easychart-configure-link', $widget).text(Drupal.t('Add chart'));
        $(this).addClass('element-hidden');
      });


      // Add some logic to the node edit form.
      if ($('.field-widget-easychart').length > 0) {

        $('.field-widget-easychart .easychart-wrapper').each(function(i, widget) {
          // Show the chart preview.
          Drupal.easychart.preview(widget);

          // Update the buttons.
          Drupal.easychart.buttons(widget);
        })
      }


    }
  }

})(jQuery);