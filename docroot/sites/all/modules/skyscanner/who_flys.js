(function ($) {

Drupal.behaviors.skyscannerWhoFlys = {

  attach: function (context, settings) {
  
    var whoFlys = new skyscanner.snippets.WhoFliesControl();
    
    if (settings.skyscanner.whoFlys.departure != "") {
      whoFlys.setDeparture(settings.skyscanner.whoFlys.departure,
        (settings.skyscanner.whoFlys.lock == "departure"));
    }

    if (settings.skyscanner.whoFlys.destination != "") {
      whoFlys.setDestination(settings.skyscanner.whoFlys.destination,
        (settings.skyscanner.whoFlys.lock == "destination"));
    }

    whoFlys.setShape(settings.skyscanner.whoFlys.shape);

    if (settings.skyscanner.whoFlys.culture != "") {
      whoFlys.setCulture(settings.skyscanner.whoFlys.culture);
    }

    if (settings.skyscanner.whoFlys.new_window == false) {
      whoFlys.setSearchTargetWindow(false);
    }

    whoFlys.draw($("#skyscanner-who-flys").get(0));
  }
};

})(jQuery);
