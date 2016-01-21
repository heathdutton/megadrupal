/**
 * @file
 * Behaviours for the Baidu Analytics module to integrated with its JS API.
 *
 * @link http://tongji.baidu.com/open/api/more?p=guide_overview
 */

(function ($) {

$(document).ready(function() {

  // Expression to check for absolute internal links.
  var isInternal = new RegExp("^(https?):\/\/" + window.location.host, "i");

  // Attach onclick event to document only and catch clicks on all elements.
  $(document.body).click(function(event) {
    // Catch the closest surrounding link of a clicked element.
    $(event.target).closest("a,area").each(function() {

      var hm = Drupal.settings.baidu_analytics;
      // Expression to check for special links like gotwo.module /go/* links.
      var isInternalSpecial = new RegExp("(\/go\/.*)$", "i");
      // Expression to check for download links.
      var isDownload = new RegExp("\\.(" + hm.trackDownloadExtensions + ")$", "i");

      // Is the clicked URL internal?
      if (isInternal.test(this.href)) {
        // Skip 'click' tracking, if custom tracking events are bound.
        if ($(this).is('.colorbox')) {
          // Do nothing here. The custom event will handle all tracking.
        }
        // Is download tracking activated and the file extension configured for download tracking?
        else if (hm.trackDownload && isDownload.test(this.href)) {
          // Download link clicked.
          var extension = isDownload.exec(this.href);
          _hmt.push(["_trackEvent", "Downloads", extension[1].toUpperCase(), "Downloads Link: " + this.href.replace(isInternal, '')]);
        }
        else if (isInternalSpecial.test(this.href)) {
          // Keep the internal URL for Baidu Analytics website overlay intact.
          _hmt.push(["_trackPageview", this.href.replace(isInternal, '')]);
        }
      }
      else {
        if (hm.trackMailto && $(this).is("a[href^='mailto:'],area[href^='mailto:']")) {
          // Mailto link clicked.
          _hmt.push(["_trackEvent", "Mails", "Click", "Mails: " + this.href.substring(7)]);
        }
        else if (hm.trackOutbound && this.href.match(/^\w+:\/\//i)) {
          _hmt.push(["_trackEvent", "Outbound links", "Click", "Outbound links: " + this.href]);
        }
      }
    });
  });

  // Colorbox: This event triggers when the transition has completed and the
  // newly loaded content has been revealed.
  $(document).bind("cbox_complete", function() {
    var href = $.colorbox.element().attr("href");
    if (href) {
      _hmt.push(["_trackPageview", href.replace(isInternal, '')]);
    }
  });

});

})(jQuery);
