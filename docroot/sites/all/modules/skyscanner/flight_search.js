(function ($) {

Drupal.behaviors.skyscannerFlightSearch = {

  attach: function (context, settings) {
  
    var searchPanel = new skyscanner.snippets.SearchPanelControl();

    if (settings.skyscanner.flightSearch.currency != "") {
      searchPanel.setCurrency(settings.skyscanner.flightSearch.currency);
    }

    if (settings.skyscanner.flightSearch.departure != "") {
      searchPanel.setDeparture(settings.skyscanner.flightSearch.departure,
        (settings.skyscanner.flightSearch.lock == "departure"));
    }

    if (settings.skyscanner.flightSearch.destination != "") {
      searchPanel.setDestination(settings.skyscanner.flightSearch.destination,
        (settings.skyscanner.flightSearch.lock == "destination"));
    }
    
    if (settings.skyscanner.flightSearch.departing != "") {
      searchPanel.setODate(settings.skyscanner.flightSearch.departing);
    }

    if (settings.skyscanner.flightSearch.returning != "") {
      searchPanel.setIDate(settings.skyscanner.flightSearch.returning);
    }

    searchPanel.setShape(settings.skyscanner.flightSearch.shape);

    if (settings.skyscanner.flightSearch.culture != "") {
      searchPanel.setCulture(settings.skyscanner.flightSearch.culture);
    }

    if (settings.skyscanner.flightSearch.new_window == false) {
      searchPanel.setSearchTargetWindow(false);
    }

    searchPanel.draw($("#skyscanner-flight-search").get(0));
  }
};

})(jQuery);
