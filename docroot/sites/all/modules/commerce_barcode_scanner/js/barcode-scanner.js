(function($) {

  Drupal.settings.commerce_barcode_scanner_vk = {
    'ENTER' : 13
  };

  /**
   * Allow a barcode scanner to be triggered when looking at the website.
   */
  Drupal.behaviors.detect_barcode_scan = {
    /**
     * Trigger when the page loads.
     */
    attach : function(context) {
      if (context != document) {
        return;
      }
      $(window).load(this.start_barcode_detection);
      this.respond_to_scan();
    },

    /**
     * When a successful scan is triggered
     */
    respond_to_scan : function() {
      $(document).bind('barcode_scan', function(e, barcode) {
        var redirect_url = Drupal.settings.commerce_barcode_scanner.trigger_path;
        var connecting_query = redirect_url.indexOf('?q=') == -1 ? '?' : '&';
        // Trigger our barcode scanner event.
        window.location = redirect_url + '/' + barcode + connecting_query + 'destination=' + window.location.pathname;
      });
    },

    /**
     * Detect barcode scans.
     */
    start_barcode_detection : function() {

      var settings = Drupal.settings.commerce_barcode_scanner;
      var vk = Drupal.settings.commerce_barcode_scanner_vk;

      var last_keypress = 0;
      var current_keypress = 0;
      var last_char = '';
      var barcode = [];

      // Check if we have a barcode in the pipeline.
      function check_barcode_buffer() {
        if (barcode.length > settings.min_sku_length) {
          $(document).trigger('barcode_scan', [barcode.join('')]);
          barcode = [];
        }
      }

      // Whenever a key is pushed.
      $(document).bind('keydown', function(e) {
        // Log the time of the keypress.
        current_keypress = Date.now();
        var char = String.fromCharCode(e.keyCode);
        // If the scanner pushed enter, check the buffer.
        if (e.keyCode == vk.ENTER) {
          check_barcode_buffer();
          // Otherwise, check if our keypress was fast enough to be a scanner.
        }
        else if (current_keypress - last_keypress < settings.key_interval) {
          if (barcode.length == 0) {
            barcode.push(last_char);
          }
          barcode.push(char);
        }
        // Keep track of the last keypress.
        last_keypress = current_keypress;
        last_char = char;
      });
    }
  };
})(jQuery);
