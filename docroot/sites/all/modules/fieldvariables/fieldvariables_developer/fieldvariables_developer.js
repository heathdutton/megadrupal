(function($) {

Drupal.behaviors.fieldvariables_developer = {
  attach: function (context, settings) {
    $('.fieldvariable a').click(function(e) {
      $(this).next().slideToggle('medium');
      e.preventDefault();
    });
  }
};

})(jQuery);
