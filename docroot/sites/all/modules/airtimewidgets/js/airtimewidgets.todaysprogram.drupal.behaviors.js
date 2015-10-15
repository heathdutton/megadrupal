/**
 * Defines the Airtime Widget Block
 */
(function ($) {
  Drupal.behaviors.todaysprogram = {
    attach: function(context, settings) {

      var airtimewidgets_todaysprogram_consolelog = Drupal.settings.airtimewidgets.airtimewidgets_todaysprogram_consolelog;
      var airtimewidgets_todaysprogram_serverurl = Drupal.settings.airtimewidgets.airtimewidgets_todaysprogram_serverurl;
      var airtimewidgets_todaysprogram_claim = Drupal.settings.airtimewidgets.airtimewidgets_todaysprogram_claim;
      var airtimewidgets_todaysprogram_interval = Number(Drupal.settings.airtimewidgets.airtimewidgets_todaysprogram_interval);
      var airtimewidgets_todaysprogram_showlimit = Number(Drupal.settings.airtimewidgets.airtimewidgets_todaysprogram_showlimit);

      if(airtimewidgets_todaysprogram_consolelog == "TRUE") {
        console.log('Init Airtimewidget Todays Program');
        console.log('Claim:' + airtimewidgets_todaysprogram_claim);
        console.log('Interval:' + airtimewidgets_todaysprogram_interval);
        console.log('Show Limit:' + airtimewidgets_todaysprogram_showlimit);
      }

      $("#onAirToday").airtimeShowSchedule({
        sourceDomain: airtimewidgets_todaysprogram_serverurl,
        text: {onAirToday:airtimewidgets_todaysprogram_claim},
        updatePeriod: airtimewidgets_todaysprogram_interval,
        showLimit: airtimewidgets_todaysprogram_showlimit,
      });
    }
  }
})(jQuery);
