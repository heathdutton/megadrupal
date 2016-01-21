/**
 * @file
 * Ajax refresh on views at page load
 */
(function($) {

  Drupal.behaviors.views_lazy_load = {
    attach: function(context, settings) {
      $.each(settings.views_lazy_load, function(i, dom_id) {
        var selector = '.view-dom-id-' + dom_id,
            dom_key = 'views_dom_id:' + dom_id;

        // For each view on the page we disable the lazy loading and then cause
        // an AJAX refresh which loads the contents.
        $(selector, context).once(function() {
          Drupal.views.instances[dom_key].refreshViewAjax.submit['views_lazy_load_disabled'] = true;
          $(selector).trigger('RefreshView');
        });
      });
    }
  };

})(jQuery);
