/**
 * Defines the Airtime Widget Block
 */
(function ($) {
  Drupal.behaviors.weeklyprogram = {
    attach: function(context, settings) {

      var airtimewidgets_weeklyprogram_consolelog = Drupal.settings.airtimewidgets.airtimewidgets_weeklyprogram_consolelog;
      var airtimewidgets_weeklyprogram_active = Drupal.settings.airtimewidgets.airtimewidgets_weeklyprogram_active;
      var airtimewidgets_weeklyprogram_serverurl = Drupal.settings.airtimewidgets.airtimewidgets_weeklyprogram_serverurl;
      var airtimewidgets_weeklyprogram_interval = Number(Drupal.settings.airtimewidgets.airtimewidgets_weeklyprogram_interval);

      if(airtimewidgets_weeklyprogram_consolelog == "TRUE") {
        console.log('Init Airtimewidget Weekly Program');
        console.log('Server:' + airtimewidgets_weeklyprogram_serverurl);
        console.log('Interval:' + airtimewidgets_weeklyprogram_interval);
      }

      $("#scheduleTabs").airtimeWeekSchedule({
        sourceDomain:airtimewidgets_weeklyprogram_serverurl,
        dowText:{monday:"Monday", tuesday:"Tuesday", wednesday:"Wednesday", thursday:"Thursday", friday:"Friday", saturday:"Saturday", sunday:"Sunday"},
        miscText:{time:"Time", programName:"Program Name", details:"Details", readMore:"Read More"},
        updatePeriod: airtimewidgets_weeklyprogram_interval,
      });
      var d = new Date().getDay();
      $("#scheduleTabs").tabs({selected: d === 0 ? 6 : d - 1, fx: { opacity: 'toggle' }});
    }
  }
})(jQuery);
