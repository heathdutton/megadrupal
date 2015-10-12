(function($){
Drupal.behaviors.TodoFilter = {
  attach: function (context, settings) {
    $(context).find('.todo-check').click(function() {
      var node = $(this).parents('div.node');
      $.post(Drupal.settings.basePath + 'todo/toggle/' + $(this).attr('rel'));
      $(this).parent().toggleClass('checked');
    });
  }
};
})(jQuery);
