/**
 * @file
 * A jQuery plugin for responsive ads.
 */
(function($){
  $.fn.googleDfp = function(options, breakpoints) {

    // Define defaults and collate options
    var opts = $.extend({
      screenSize: 'all',
      placement: 'none'
    }, options);

    this.placeAds = function (screenSize) {

      // Define the ad block
      var obj = this;

      // Check screensize by using a Modernizr media query.
      var deviceSize = '', breakpointName, breakpoint;
      for (breakpointName in breakpoints) {
        if (breakpoints.hasOwnProperty(breakpointName)) {
          breakpoint = breakpoints[breakpointName];
          if (Modernizr.mq(breakpoint.breakpoint) == true) {
            deviceSize = breakpointName;
            break;
          }
        }
      }

      // Check device placement based on screen size and device size
      var placeAd = false;
      if (opts.screenSize.search('_all') != '-1') {
        placeAd = true;
      }
      else if (deviceSize && screenSize.search(deviceSize) != '-1') {
        placeAd = true;
      }

      // Place the ad
      if (placeAd == true) {

        // Use regular old javascript to add the script tag in
        script = document.createElement("script");
        script.type = "text/javascript";
        script.text = 'googletag.cmd.push(function() { googletag.display("' + obj.attr('id') + '"); });';

        id = obj.attr('id');
        document.getElementById(id).appendChild(script);
        return true;

      }

      return false;

    };

    // Call the function
    return this.placeAds(opts.screenSize);

  };
})(jQuery);
