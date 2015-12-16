/**
 * @file
 * Javascript behaviors for the monitoring module.
 */

(function($) {

  /**
   * Behavior that adds and controls the toggle link on the overview page.
   */
  Drupal.behaviors.monitoringOverviewToggle = {
    attach: function(context) {
      // Check if there are any criticals, warnings or unknowns.
      if (Drupal.settings.monitoring_error_sensors > 0) {
        // Inject toggle link into DOM.
        $('<a class="monitoring-sensor-toggle" href="#">' + Drupal.t('Show OK sensors') + '</a>')
            .appendTo('.monitoring-overview-summary', context)
            .toggle(
            function () {
              $(this).text(Drupal.t('Hide OK sensors'));
              $('#monitoring-sensors-overview tr.monitoring-ok, #monitoring-sensors-overview tr.sensor-category-ok', context).fadeIn();
            },
            function () {
              $(this).text(Drupal.t('Show OK sensors'));
              $('#monitoring-sensors-overview tr.monitoring-ok, #monitoring-sensors-overview tr.sensor-category-ok', context).fadeOut();
            }
        );
        // Hide OK sensors by default.
        $('#monitoring-sensors-overview tr.monitoring-ok, #monitoring-sensors-overview tr.sensor-category-ok', context).hide();
      }
    }
  }

})(jQuery);

