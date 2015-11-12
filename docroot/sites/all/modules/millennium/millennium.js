
// Fire startup event
Drupal.behaviors.millennium = {
  attach: function () {
    Drupal.millennium.init();
  }
};

// Define millennium object that does the actual work
Drupal.millennium = {
  init: function() {
    Drupal.millennium.updateNext();
  },
  /**
   * updateNext():
   *   Looks for first div element with class "load" and updates it depending on
   *   its id attribute (DIVs should already have an id="nid-XXXX" attribute,
   *   where XXXX is the node's id)
   */
  updateNext: function() {
    // Get the first div element with the classes we are looking for
    var div = jQuery("div.millennium.holdings.load").get(0);
    if (! div) {
      return;
    }
    var id = div.id;
    if (! id || id.substr(0,4) != "nid-" ) {
      return;
    }

    // Get the node id from the id attribute
    var nid = id.substr(4); //nid-123456 --> 123456

    /**
     * URL to get information from. If you go here in your browser you get plain HTML
     * depending on the nid argument. This URL is defined in the module under millennium_menu()
     */
    var url = "?q=millennium_ajax&nid=" + nid + "&locale=" + Drupal.settings.millennium.locale + "&page=" + Drupal.settings.millennium.page;

    // Show 'loading' message.
    jQuery(div).html(Drupal.t('Loading...'));
    // "unmark" it so millenniumUpdateNext() skips this element the next time around
    jQuery(div).removeClass("load");
    // Show throbber
    jQuery(div).addClass("loading");

    // Activate the ajax information fetch from "url". When done function(data) will get executed.
    jQuery.get(url, function(data) {
        jQuery(div).removeClass("loading");
        jQuery(div).html(data); // Replace 'loading' message with fetched HTML
        Drupal.millennium.updateNext(); // Start over for another element
      }
    );
  }
}

