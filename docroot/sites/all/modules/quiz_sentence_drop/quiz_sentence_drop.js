/**
 * @file
 * Javascript functions for the quiz sentence drop question type.
 * 
 */

var qsdAnswerCount = 0;
var dragCount = 0;

(function($) {
  Drupal.behaviors.quiz_sentence_drop = {
    attach: function(context) {
      $(".draggable").draggable({
        revert: 'invalid',
        snap: true,
        stop: function(event, ui) {}
      });
      $(".droppable").droppable({
        tolerance: 'touch',
        greedy: false,
        refreshPositions: true,
        drop: function(event, ui) {
          var placeholderId = this.id;
          var placeholderIdArray = placeholderId.split('_');
          var placeholderFid = placeholderIdArray[1];

          var imageId = ui.draggable.attr("id");
          var imageDraggedArray = imageId.split('_');
          var imageFid = imageDraggedArray[1];

          if(placeholderFid == imageFid) {
            qsdAnswerCount++;
          }
          dragCount++;

          $("#" + imageId).draggable("option", "disabled", true);
          $('#' + placeholderId).droppable("option", "disabled", true);
          $('#dropCount').val(dragCount);
          $('#qsdAnswerCount').val(qsdAnswerCount);
        }
      });

      $("#btnReset").click(function() {
        $(".draggable").animate({
          "left": '',
          "top": ''
        });
        qsdAnswerCount = 0;
        dragCount = 0;

        $('#dropCount').val(dragCount);
        $('#qsdAnswerCount').val(qsdAnswerCount);

        $("li[id^='dragged_from_']").each(function(){
          var id = $(this).attr("id");
          var idArray = id.split('_');
          $('#placeholder_' + idArray[2]).droppable("option", "disabled", false);
          $('#from_' + idArray[2]).draggable("option", "disabled", false);
        });

        return false;
      });
      $(".ui-draggable").data("left", $(".ui-draggable").position().left).data("top", $(".ui-draggable").position().top);
    }
  };
})(jQuery);
