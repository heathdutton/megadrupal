/**
 * @file
 * Jquery for ordering elements and copying position to form.
 */

(function($) {
  Drupal.behaviors.elearning_order_question = {
    attach: function(context, settings) {

      $('.entity-elearning-exercise .elearning-order-question-sort-wrapper', context).sortable({
        items: "> .elearning-order-question-sort-item",
        revert: true,
        stop: function(event, ui) {
          var id = $(this).parents('.elearning-question-order-question').attr('id');
          var inputValue = [];
          $(this).find('.elearning-order-question-sort-item', context).each(function(index) {
            var value = $(this).attr('name');
            inputValue.push($(this).attr('name'));
            // Append the trigger change so it can be used by other modules.
            $(this).parents('.elearning-question-elearning-order-question', context).find('.form-text', context).eq(index).val(value).trigger('change');
          });
        }
      });
      $(".entity-elearning-exercise .order-question-sort-wrapper").disableSelection();
    }
  };
})(jQuery);
