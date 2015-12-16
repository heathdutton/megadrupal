/*
 * Add drag/drop and sort functionality to the block manager admin page
 */

(function($){
  function addDraggable($object) {
    $object.once('block_manager_make_draggable').draggable({
      appendTo: '#block-manager-manage-blocks',
      connectToSortable: '#managed_blocks .inner',
      distance: 1,
      // helper: original has two issues - it ignored appendTo, leaving the element
      // in its container (making it seem to drag behind the box; and it sets the
      // wrong position in the sortable if the object is dragged into, out of, then
      // back into the sortable.
      helper: function(){
        return this;
      },
      revert: 'invalid',
      scroll: false,
      stop: function(event, ui) {
        $('#available_blocks .block').attr('style', '');
      },
      zIndex: 1000
    });
  }

  Drupal.behaviors.block_manager = {
    attach: function(context, settings){
      var $sortObject = $('#managed_blocks .inner', context);
      $sortObject.once('block_manager_make_sortable').sortable({
        placeholder: 'block placeholder',
        scroll: false,
        stop: function(event, ui) {
          ui.item.attr('style', '');
        },
        update: function(event, ui) {
          //Set this block to be managed
          $('input.manage', this).val(1);

          //Update all the weights in the 'managed blocks' section
          var $weight = 0;
          $('#managed_blocks .block').each(function(){
            $('input.weight', this).val($weight++);
          });
        }
      });

      var $dragObject = $('#available_blocks .inner .block', context);
      addDraggable($dragObject);

      // Remove button.
      $('#managed_blocks', context).delegate('.remove', 'click', function(){
        // Move the block back to the available blocks section.
        $block = $(this).closest('.block');
        $('#available_blocks .inner').prepend($block);

        // Make sure that the block is still draggable.
        //$dragObject.draggable('destroy');
        //$dragObject = $('#available_blocks .inner .block');
        //addDraggable($dragObject);

        $('input.manage', $block).val(0);

        // We don't have to udpate the weights, because the managed blocks will
        // still be rendered in order as they are.
        return false;
      });

      $('#available_blocks', context).delegate('.remove', 'click', function(){
        return false;
      });
    }
  };
}(jQuery));
