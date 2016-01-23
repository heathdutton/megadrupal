(function ($) {
  Drupal.behaviors.ghostery = {
    // Initialize properties
    protocol: 'http',
    basepath: '',
    pid: 0,
    ocid: 0,
    ghost: {},
    icon: {},
    link: {},

    /**
     * Attach Drupal behavior
     */
    attach: function (context, settings) {
      var ghostery = this;

      // Initialize properties and validate settings.
      if (!ghostery.init(context, settings)) {
        return;
      }

      // Set the icon source path
      ghostery.icon.attr('src', ghostery.basepath + 'icon1.png');

      // Bind click event to link
      ghostery.link.on('click', function(event) {
        var f = this;
        event.preventDefault();

        function d(i, l) {
          var j = document.getElementsByTagName("head")[0] || document.documentElement,
            h = false,
            g = document.createElement("script");

          function k() {
            g.onload = g.onreadystatechange = null;
            j.removeChild(g);
            l();
          }

          g.src = i;

          g.onreadystatechange = function() {
            if (!h && (this.readyState == "loaded" || this.readyState == "complete")) {
              h = true;
              k();
            }
          };

          g.onload = k;
          j.insertBefore(g, j.firstChild)
        }

        this.onclick = "return false";

        d(ghostery.basepath + "pub1.js", function() {
          BAPW.i(f, {
            pid: ghostery.pid,
            ocid: ghostery.ocid
          }, false);
        });
      });

      ghostery.ghost = new Image();
      ghostery.ghost.src = ghostery.protocol + "://l.betrad.com/pub/p.gif?pid=" + ghostery.pid + "&ocid=" + ghostery.ocid + "&ii=1&r=" + Math.random();
    },

    /**
     * Initialize properties
     */
    init: function(context, settings) {
      this.protocol = document.location.protocol == "https:" ? "https" : "http";
      this.basepath = this.protocol == "https" ? "https://info.evidon.com/c/betrad/pub/" : "http://cdn.betrad.com/pub/";
      this.icon = $('#_bapw-icon');
      this.link = $('#_bapw-link');

      // If the necessary IDs are not set, hide the link and kick out.
      if (typeof Drupal.settings.ghostery.pid === 'undefined'
        || isNaN(parseInt(Drupal.settings.ghostery.pid))
        || typeof Drupal.settings.ghostery.ocid === 'undefined'
        || isNaN(parseInt(Drupal.settings.ghostery.ocid))) {
        this.link.hide();
        console.log('Ghostery pid and/or ocid are not properly set.');
        return false;
      }
      else {
        this.pid = parseInt(Drupal.settings.ghostery.pid);
        this.ocid = parseInt(Drupal.settings.ghostery.ocid);
      }

      // If there are no ghostery links, kick out.
      if (this.link.length === 0) {
        return false;
      }

      return this;
    }
  };
})(jQuery);
