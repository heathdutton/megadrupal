(function ($) {
  $(document).ready(function() {
    $.ajax({
      type: "POST",
      cache: false,
      url: Drupal.settings.taxonomy_term_statistics.url,
      data: Drupal.settings.taxonomy_term_statistics.data
    });
  });
})(jQuery);
