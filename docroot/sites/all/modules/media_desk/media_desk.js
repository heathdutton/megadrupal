/**
 * @file
 * Javascript file that performs the image path manipulations.
 */
(function ($) {
  // Attaching to Drupal behaviors
  Drupal.behaviors.media_desk_js = {
    attach: function (context, settings) {
      // Draggable
      $('.field-widget-media-desk-selector .media-thumbnail img').draggable({
        'containment' : 'document',
        'snap' : '.field-widget-media-desk-image .preview',
        'helper' : 'clone',
        'opacity' : '0.5'
      });

      // Droppable
      $('.field-widget-media-desk-image .preview').droppable({
        drop: handleImageDrop,
        hoverClass: 'ui-state-active',
        tolerance: 'touch'
      });
      
      // Handling image drop event
      function handleImageDrop(event, ui) {
        var mediaEl = $(ui.draggable).closest('.media-widget');
        var thumbnail = $(ui.draggable).closest('.preview');
        $(this).html(thumbnail.html());
        $('.ui-draggable-dragging').remove();

        // Find field name to replace in edit URL.
        var fieldName = $(this).parent().find('.field_identifier').attr('id');

        // Set fid of image
        $(this).parent().find('.fid').val(mediaEl.find('.fid').val());
        
        // Unbind edit button and bind it with the new click behavior.
        var editButton = $(this).parent().find('.edit');
        var editHref = mediaEl.find('.edit').attr('href');
        editHref = editHref.replace('field_image_selector', fieldName);
        editButton.attr('href', editHref);
        editButton.unbind('click');
        editButton.removeClass('ctools-use-modal-processed');
        editButton.css('display', 'inline-block');
        Drupal.attachBehaviors(editButton.parent(), Drupal.settings);

        $(this).parent().find('.remove').css('display', 'inline-block');
      }
    }
  };
}(jQuery));
