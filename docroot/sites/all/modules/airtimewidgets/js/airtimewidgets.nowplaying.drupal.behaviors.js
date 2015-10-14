/**
 * Defines the Airtime Widget Block
 */
(function ($) {
  Drupal.behaviors.nowplaying = {
    attach: function(context, settings) {

      var airtimewidgets_nowplaying_consolelog = Drupal.settings.airtimewidgets.airtimewidgets_nowplaying_consolelog;
      var airtimewidgets_nowplaying_serverurl = Drupal.settings.airtimewidgets.airtimewidgets_nowplaying_serverurl;
      var airtimewidgets_nowplaying_claim = Drupal.settings.airtimewidgets.airtimewidgets_nowplaying_claim;
      var airtimewidgets_nowplaying_offlinetext = Drupal.settings.airtimewidgets.airtimewidgets_nowplaying_offlinetext;
      var airtimewidgets_nowplaying_currentlabel = Drupal.settings.airtimewidgets.airtimewidgets_nowplaying_currentlabel;
      var airtimewidgets_nowplaying_nextlabel = Drupal.settings.airtimewidgets.airtimewidgets_nowplaying_nextlabel;
      var airtimewidgets_nowplaying_interval = Number(Drupal.settings.airtimewidgets.airtimewidgets_nowplaying_interval);

      if(airtimewidgets_nowplaying_consolelog == "TRUE") {
        console.log('Init Airtimewidget Now Playing');
        console.log('Server:' + airtimewidgets_nowplaying_serverurl);
        console.log('Claim:' + airtimewidgets_nowplaying_claim);
        console.log('Offline Text:' + airtimewidgets_nowplaying_offlinetext);
        console.log('Current:' + airtimewidgets_nowplaying_currentlabel);
        console.log('Next:' + airtimewidgets_nowplaying_nextlabel);
        console.log('Interval:' + airtimewidgets_nowplaying_interval);
      }

      $("#headerLiveHolder").airtimeLiveInfo({
        sourceDomain: airtimewidgets_nowplaying_serverurl,
        text: {onAirNow: airtimewidgets_nowplaying_claim, offline: airtimewidgets_nowplaying_offlinetext, current: airtimewidgets_nowplaying_currentlabel, next: airtimewidgets_nowplaying_nextlabel},
        updatePeriod: airtimewidgets_nowplaying_interval
      });
    }
  }
})(jQuery);
