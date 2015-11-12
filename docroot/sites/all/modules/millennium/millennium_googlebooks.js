
Drupal.behaviors.millenniumGbooks = {
  attach: function(context) {
    if (!Drupal.settings.millennium.gbooks) {
      return;
    }
    var ids = '';
    jQuery.each(Drupal.settings.millennium.gbooks, function(nid, isbns) {
      if (ids) ids += ",";
      ids += isbns;
    });
    // callback=xxx tells the Gbooks API what function to call.
    var url = "http://books.google.com/books?jscmd=viewapi&bibkeys=" + ids + "&callback=millenniumGbooks.processGBSBookInfo";
    jQuery.getScript(url, function() {} );
  }
}

millenniumGbooks = {
  // Function that recieves and processes book information from the callback fired
  //   by the books API call above.
  processGBSBookInfo: function(booksInfo) {
    var bookInfo;
    var tmpImage = new Image();
    var destImgElement;
    var isbn;

    for (isbn in booksInfo) {
      bookInfo = booksInfo[isbn];
      if (bookInfo) {
        var link = '';
        if (bookInfo.preview == "partial") {
          link = '<a href="' + bookInfo.preview_url + '">' + Drupal.t('Preview this item in Google Books') + '</a>';
        }
        if (bookInfo.preview == "full") {
          link = '<a href="' + bookInfo.preview_url + '">' + Drupal.t('View this item in Google Books') + '</a>';
        }
        if (link) {
          // Add the link to the div
          jQuery("#gbooks-link-" + isbn).html(link);
          // If there's a div ready for an embedded widget, add it
          if (jQuery('#gbooks-widget-' + isbn).length) {
            this.embedBookViewer(isbn);
          }
        }

        // If settings allow overriding the cover image, do it
        if (Drupal.settings.millennium.gbooks_replace_covers) {
          // Find image
          destImgElement = jQuery("img.isbn-" + isbn);
          if (destImgElement.length == 1) {
            // If the current cover image's width <10, replace it with the google books image
            tmpImage.src = destImgElement.attr("src");
            if (tmpImage.width <10) {
              destImgElement.attr("src", bookInfo.thumbnail_url);
            }
          }
        }
      }
    }
  },
  // Object that will hold the embedded google books viewer
  viewer: null,
  // Embed the actual Google Books Reader
  embedBookViewer: function(isbn) {
    // Resize the div
    jQuery('#gbooks-widget-' + isbn).css('height', '400px').addClass("loading");
    this.viewer = new google.books.DefaultViewer(
      document.getElementById('gbooks-widget-' + isbn)
      );
    this.viewer.load(
      //'ISBN:' + Drupal.settings.millennium.gbooksViewerISBN,
      'ISBN:' + isbn,
      this.notfound,
      this.success
    );
  },
  // Callback when book not found
  notfound: function() {
    jQuery('#millennium_googlebookswidget').hide();
  },
  // Callback when book found
  success: function() {
    if (!this.viewer) {
      return;
    }
    jQuery('#gbooks-widget-' + isbn).removeClass("loading");
    this.viewer.zoomOut();
    this.viewer.zoomOut();
    this.viewer.zoomOut();
  }
}