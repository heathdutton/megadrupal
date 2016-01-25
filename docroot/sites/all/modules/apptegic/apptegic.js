/**
 * Set up the Apptegic namespace.
 */
var _aaq = _aaq || [];

/* $Id$ */
(function($, window, document) {

Drupal.behaviors.apptegic = {
  attach: function (context, settings) {
    // Initialize the Apptegic account information.
    $("body").once('apptegic', function() {
      var apptegicAccount = settings.apptegic["account"];
      var apptegicBaseURL = document.location.protocol + "//" + apptegicAccount + ".apptegic.com/";

      for (var key in settings.apptegic["actions"] || {}) {
        var actionset = settings.apptegic["actions"][key];
        var apptegicAction = actionset.action || false;

        // Send the parameters over.
        for (var key in actionset.params || {}) {
          var value = actionset.params[key];
          window._aaq.push([key, value]);
        }

        // Address the custom variables.
        for (var key in actionset.custom || {}) {
          var value = actionset.custom[key];
          window._aaq.push(['setCustomVariable', key, value, apptegicAction]);
        }

        // Process the OnMessage settings.
        if (settings.apptegic["onmessage"] || false) {
          if (settings.apptegic["encrypteduserid"] || false) {
            _aaq.push(['setEncryptedUser', settings.apptegic["encrypteduserid"]]);
          }
          _aaq.push(['setTwoWayEnabled', true]);
        }

        window._aaq.push(['trackAction', apptegicAction]);
      }

      var d=document, g=d.createElement('script'), s=d.getElementsByTagName('script')[0];
      g.type='text/javascript';
      g.defer=true;
      g.async=true;
      if (settings.apptegic["onmessage"] || false) {
        g.src = apptegicBaseURL + 'scripts/apptegic-tw.js';
      }
      else {
        g.src = apptegicBaseURL + 'scripts/apptegic.js';
      }
      s.parentNode.insertBefore(g,s);
    });
  }
};

})(jQuery, window, document);
