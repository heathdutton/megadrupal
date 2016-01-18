var EJS = EJS || {};
EJS.attach = EJS.attach || [];

(function ($) {

Drupal.behaviors.attach = {
  attach: function (context, settings) {
    $(".attach-node-poll:not(.attach-processed)", context).each(function() {
      $t = $(this);
      $t
        .load(EJS.attach[$t.attr('id')], function() {
          if (!$t.find('form').length) {
            return;
          }
          $t.find('form').attr('action', $t.find('form').attr('action').replace(/__FAKED__/, encodeURIComponent(location.pathname.substring(Drupal.settings.basePath.length))));
        })
        .addClass('attach-processed');
    });
  }
};

})(jQuery);

