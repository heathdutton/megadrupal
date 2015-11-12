(function($) {
  $skipLink = $('#skip-link');
  if ($skipLink.length) {
    $skipLink.append(Drupal.settings.highContrast.link);
  }
})(jQuery);
