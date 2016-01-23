// Run LazyLoad Ad when document loaded.
(function($) { 
  $(document).ready(function() {
    $(Drupal.settings.lazyloadad).lazyLoadAd();  
  });
})(jQuery); 