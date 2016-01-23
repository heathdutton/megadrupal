/**
 * Defines the Airtime Widget Block
 */
(function ($) {
  Drupal.behaviors.showandtrack = {
    attach: function(context, settings) {

      var airtimewidgets_showandtrack_consolelog = Drupal.settings.airtimewidgets.airtimewidgets_showandtrack_consolelog;
      var airtimewidgets_showandtrack_serverurl = Drupal.settings.airtimewidgets.airtimewidgets_showandtrack_serverurl;
      var airtimewidgets_showandtrack_interval = Number(Drupal.settings.airtimewidgets.airtimewidgets_showandtrack_interval);
      var airtimewidgets_showandtrack_claim = Drupal.settings.airtimewidgets.airtimewidgets_showandtrack_claim;
      var airtimewidgets_showandtrack_offlinetext = Drupal.settings.airtimewidgets.airtimewidgets_showandtrack_offlinetext;
      var airtimewidgets_showandtrack_currentlabel = Drupal.settings.airtimewidgets.airtimewidgets_showandtrack_currentlabel;
      var airtimewidgets_showandtrack_nextlabel = Drupal.settings.airtimewidgets.airtimewidgets_showandtrack_nextlabel;

      if(airtimewidgets_showandtrack_consolelog == "TRUE") {
        console.log('Init Airtimewidget Show And Track');
        console.log('Server:' + airtimewidgets_showandtrack_serverurl);
        console.log('Claim:' + airtimewidgets_showandtrack_claim);
        console.log('Offline Text:' + airtimewidgets_showandtrack_offlinetext);
        console.log('Current:' + airtimewidgets_showandtrack_currentlabel);
        console.log('Next:' + airtimewidgets_showandtrack_nextlabel);
        console.log('Interval:' + airtimewidgets_showandtrack_interval);
      }

      $("#headerLiveTrackHolder").airtimeLiveTrackInfo({
        sourceDomain: airtimewidgets_showandtrack_serverurl,
        text: {onAirNow: airtimewidgets_showandtrack_claim, offline: airtimewidgets_showandtrack_offlinetext, current: airtimewidgets_showandtrack_currentlabel, next: airtimewidgets_showandtrack_nextlabel},
        updatePeriod: airtimewidgets_showandtrack_interval,
       });
    }
  }
})(jQuery);
