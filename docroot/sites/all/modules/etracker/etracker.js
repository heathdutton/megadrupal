(function ($) {

  $(document).ready(function() {

    // Attach mousedown, keyup, touchstart events to document only and catch
    // clicks on all elements.
    $(document.body).bind("mousedown keyup touchstart", function(event) {

      // Catch the closest surrounding link of a clicked element.
      $(event.target).closest("a,area").each(function() {

        if (Drupal.settings.etracker.track.mailto && $(this).is("a[href^='mailto:'],area[href^='mailto:']")) {
          // Mailto link clicked.
          ET_Event.click('Email:%20'+this.href.substring(7), 'Email');
        }
        else if (Drupal.settings.etracker.track.external && $(this).is("a[href^='http:'],a[href^='https:'],area[href^='http:'],area[href^='https:']") && (this.href.indexOf('://' + location.host) < 0)) {
          // External link clicked.
          ET_Event.link('OK,External link:%20'+this.href, '');
        }
        else if (Drupal.settings.etracker.track.download && (new RegExp("\\.("+Drupal.settings.etracker.settings.download_extensions+")(\\?|$)")).test(this.href)) {
          // Download link clicked.
          ET_Event.download(this.href, 'Download');
        }
      });

    });

    var i;

    // Trigger events for each message being displayed.
    if (Drupal.settings.etracker.messages.length > 0) {
      for (i in Drupal.settings.etracker.messages) {
        ET_Event.eventStart('Message', Drupal.settings.etracker.messages[i].text, Drupal.settings.etracker.messages[i].type, '');
      }
    }

    // Trigger events for internal search terms.
    if (Drupal.settings.etracker.search_terms != '') {
      if (Drupal.settings.etracker.number_of_search_results == undefined) {
        Drupal.settings.etracker.number_of_search_results = 0;
      }
      ET_Event.eventStart('Internal Search', Drupal.settings.etracker.search_terms, 'Search terms', '');
      ET_Event.eventStart('Internal Search', Drupal.settings.etracker.number_of_search_results, 'Number of results', '');
    }

  });

})(jQuery);
