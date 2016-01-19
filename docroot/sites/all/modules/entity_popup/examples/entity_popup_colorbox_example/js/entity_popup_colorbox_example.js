(function($, Drupal) {
  Drupal.behaviors.entityPopupColorboxExample = {
    attach: function(context) {
      $('a.entity-popup-colorbox-example', context).once('entity-popup-colorbox-example', function () {
          var $self = $(this);
          var href = $self.attr('href')
          // Create an element so we can parse our a URL no matter if its internal or external.
          var parse = document.createElement('a');
          parse.href = href;
          $self.colorbox({
            href: '/entity-popup' + parse.pathname
          });
      });
    }
  };
})(jQuery, Drupal);
