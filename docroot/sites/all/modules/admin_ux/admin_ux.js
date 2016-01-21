(function ($) {

Drupal.behaviors.adminUXTestOverview = {
  attach: function (context) {
    $('.simpletest-test-label > label', context).once('admin-ux', function () {
      var $label = $(this);
      $label.click(function () {
        // @todo Use .prop() in jQuery 1.6+.
        $('#' + $label.attr('for')).attr('checked', true);
        $(this.form).submit();
      });
    });
  }
};

})(jQuery);
