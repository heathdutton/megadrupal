/**
 * @file
 * Scripts for the Webform Charts module.
 */
(function ($) {
  "use strict";

  Drupal.behaviors.webformCharts = {};
  Drupal.behaviors.webformCharts.attach = function(context, settings) {
    // Change options based on the chart type selected.
    var $form = $('#webform-charts-edit-chart');
    $form.find('.form-radios.chart-type-radios').once('charts-axis-inverted-processed', function() {

      // Manually attach collapsible fieldsets first.
      if (Drupal.behaviors.collapse) {
        Drupal.behaviors.collapse.attach(context, settings);
      }

      var xAxisLabel = $('fieldset.chart-xaxis .fieldset-title').html();
      var yAxisLabel = $('fieldset.chart-yaxis .fieldset-title').html();

      $(this).find('input:radio').change(function() {
        if ($(this).is(':checked')) {
          // Flip X/Y axis fieldset labels for inverted chart types.
          if ($(this).attr('data-axis-inverted')) {
            $('fieldset.chart-xaxis .fieldset-title').html(yAxisLabel);
            $('fieldset.chart-yaxis .fieldset-title').html(xAxisLabel);
            $('.form-item-xaxis-labels-rotation').hide();
            $('.form-item-yaxis-labels-rotation').show();
          }
          else {
            $('fieldset.chart-xaxis .fieldset-title').html(xAxisLabel);
            $('fieldset.chart-yaxis .fieldset-title').html(yAxisLabel);
            $('.form-item-xaxis-labels-rotation').show();
            $('.form-item-yaxis-labels-rotation').hide();
          }

          // Show the stacking option for stacking-enabled chart types.
          var $stacking = $('.form-item-stacking');
          if ($(this).attr('data-stacking')) {
            $stacking.show();
          }
          else {
            if ($stacking.find('input').attr('checked')) {
              $stacking.find('input').attr('checked', false).trigger('change');
            }
            $stacking.hide();
          }

          // Hide axis options for single-series chart types.
          if ($(this).attr('data-axis-single')) {
            $('fieldset.chart-xaxis').hide();
            $('fieldset.chart-yaxis').hide();
            if ($stacking.length === 0) {
              $('.webform-charts-colors').find('tr, td').show();
            }
          }
          else {
            $('fieldset.chart-xaxis').show();
            $('fieldset.chart-yaxis').show();
            // If on a bar/column chart without stacking, there's only one color
            // allowed for the whole chart, so hide all other options.
            if ($stacking.length === 0) {
              $('.webform-charts-colors').find('tr:not(:first), td:not(:first)').hide();
            }
          }
        }
      });

      // Set the initial values.
      $(this).find('input:radio:checked').triggerHandler('change');
    });
  };
})(jQuery);