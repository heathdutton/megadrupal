(function ($) {

Drupal.behaviors.paginator3000 = {};
Drupal.behaviors.paginator3000.attach = function(context, settings) {
  var href = decodeURIComponent(window.location.href);
  var pages = href.match(/page=[0-9,]+/);
  var page_present = false;
  if (pages !== null) {
    pages = pages[0].substr(5).split(',');
    page_present = true;
  }
  else {
    pages = [];
    var max_element = 0;
    $.each(Drupal.settings.paginator3000, function(index, element) {
      if (element.element > max_element) {
        max_element = element.element;
      }
    });
    for (var i = 0; i <= max_element; i++) {
      pages[i] = 0;
    }
  }

  $.each(Drupal.settings.paginator3000, function(index, element) {
    var new_pages = pages.slice();
    new_pages[element.element] = '%number%';
    new_pages = new_pages.join(',');

    var new_href;
    if (page_present) {
      new_href = href.replace(/page=[0-9,]+/, 'page=' + new_pages);
    }
    else {
      if (window.location.search) {
        new_href = href + '&page=' + new_pages;
      }
      else {
        new_href = href + '?page=' + new_pages;
      }
    }

    var $pager = $('#' + element.id, context);
    $pager.addClass('paginator').paginator({
      pagesTotal: element.total,
      pagesSpan: element.quantity,
      pageCurrent: element.current,
      baseUrl: new_href,
      buildCounter: function(page) {return page - 1;},
      pageScroll: 3,
      clickHandler: null,
      //clickHandler: function(page) {console.log(page); return false;},
      returnOrder: (element.returnorder === '0' ? false : true),
      lang: {
        next: element.tags[3],
        last: element.tags[4],
        prior: element.tags[1],
        first: element.tags[0],
        arrowRight: String.fromCharCode(8594),
        arrowLeft: String.fromCharCode(8592)
      },
      events: {
        keyboard: true,
        scroll: false
      }
    });
    //console.log($pager.find('a'));
  });
};

})(jQuery);
