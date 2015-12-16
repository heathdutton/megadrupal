(function ($, Drupal, window, document, undefined) {

Drupal.PageWatchers = Drupal.PageWatchers || {};

Drupal.PageWatchers.query = function() {
  if ($('.pagewatchers--live-count').length > 0) {
    $.ajax({
      url: Drupal.settings.basePath + 'ajax/pagewatchers/count',
      data: {url: Drupal.settings.PageWatchers.url},
      success: function(data) {
        $('.pagewatchers--live-count').html(data.count);
        window.setTimeout(Drupal.PageWatchers.query, Drupal.settings.PageWatchers.updateInterval);
      }
    });
  }
}

Drupal.behaviors.PageWatchers = {
  attach: Drupal.PageWatchers.query
};

})(jQuery, Drupal, this, this.document);