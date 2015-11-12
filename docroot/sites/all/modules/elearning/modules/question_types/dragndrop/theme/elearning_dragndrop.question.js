/**
 * @file
 *
 * Implements Jquery UI drag and drop functionality.
 */
 (function($) {
  Drupal.behaviors.elearning_dragndrop = {
    attach: function(context, settings) {

      $('.elearning-question-elearning-dragndrop', context).each(function(questionIndex, questionElement) {

        $(questionElement).find(".elearning-dragndrop-origin").draggable({
          revert: 'invalid'
        });

        $(questionElement).find('.elearning-dragndrop-origin-wrapper', context).droppable({
          over: function(event, ui) {
            $(this).addClass('hovering');
          },
          out: function(event, ui) {
            $(this).removeClass('hovering');
          },
          drop: function(event, ui) {

            var origin_index = $(ui.draggable).index();

            $(ui.draggable).removeClass('dropped-element').css({
              top: "0px",
              left: "0px"
            });
            $(questionElement).find('input').each(function(inputIndex, inputElement) {
              if (this.value == origin_index) {
                $(inputElement).val('');
                $(questionElement).find('.elearning-dragndrop-destination').eq(inputIndex).removeClass('dropped');
              }
            });
            $(questionElement).find('.elearning-dragndrop-origin-wrapper').removeClass('empty');

            $(this).removeClass('hovering');
          }
        });


        $(questionElement).find('.elearning-dragndrop-destination', context).droppable({
          over: function(event, ui) {
            $(this).addClass('hovering');
          },
          out: function(event, ui) {
            $(this).removeClass('hovering');
          },
          drop: function(event, ui) {

            var destination_index = $(this).index();
            var origin_index = $(ui.draggable).index();

            $(ui.draggable).addClass('dropped-element');

            occupierIndex = $(questionElement).find('input').eq(destination_index).val();

            if (occupierIndex !== '' && occupierIndex !== origin_index) {
              $(questionElement).find('.elearning-dragndrop-origin').eq(occupierIndex).removeClass('dropped-element').css({
                top: 0,
                left: 0,
              });
            }

            $(questionElement).find('input').each(function(inputIndex, inputValue) {
              if ($(inputValue).val() == origin_index) {
                $(inputValue).val('');
                $(questionElement).find('.elearning-dragndrop-destination').eq(inputIndex).removeClass('dropped');
              }
            });

            if ($(questionElement).find('.elearning-dragndrop-origin:not(.dropped-element)').length === 0) {
              $(questionElement).find('.elearning-dragndrop-origin-wrapper').addClass('empty');
            } else {
              $(questionElement).find('.elearning-dragndrop-origin-wrapper').removeClass('empty');
            }

            $(questionElement).find('.form-type-textfield').eq(destination_index).find('input').val(origin_index);
            $(questionElement).find('.elearning-dragndrop-destination').eq(destination_index).addClass('dropped');

            $(ui.draggable).css({
              top: 0,
              left: 0,
            }).css({
              top: $(this).offset().top + $(this).height() / 2 - $(ui.draggable).offset().top - $(ui.draggable).height() / 2,
              left: $(this).offset().left + $(this).width() / 2 - $(ui.draggable).offset().left - $(ui.draggable).width() / 2,
            });
            $(this).removeClass('hovering');
          },
        });
});

}
};
})(jQuery);
